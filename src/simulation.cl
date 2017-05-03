#define DT 0.04f
#define GRAVITY ((float4){0.0f, -9.81f, 0.0f, 0.0f})
#define PI 3.141592f
#define EPSILON 0.001f
#define DAMPENING 0.85f

__kernel void timestep(const __global float4 * positions, __global float4 * new_positions, 
                       const __global float4 * velocity, __global float4 * new_velocity,
                       const int num_particles) {
  int gid =  get_global_id(0);
  if (gid < num_particles) {
    float4 p = positions[gid];
    float4 v = velocity[gid];

    //v += DT * GRAVITY;
    //p += DT * v;
    new_positions[gid] = p;
    new_velocity[gid] = v;
  }
}

__kernel void update(__global float4 * positions, const __global float4 * new_positions, 
                     __global float4 * velocity,  const __global float4 * new_velocity,
                     const int num_particles) {  
  int gid =  get_global_id(0);
  if (gid < num_particles) {
    velocity[gid] = (new_positions[gid] - positions[gid]) / DT;
    positions[gid] = new_positions[gid];
  }
}

int get_cell_ind(int x, int y, int z, int cells_per_side) {
  int cps2 = cells_per_side * cells_per_side;
  return cps2 * z + cells_per_side * y + x;
}

__kernel void calculate_hashes(const __global float4 * positions, __global uint * hashes,
                               const int num_particles, const int cells_per_side, const float cell_width,
                               const float x_start, const float y_start, const float z_start) {
  int gid = get_global_id(0);
  if (gid < num_particles) {
    float4 pos = positions[gid];
    int x = max(min(cells_per_side, (int) ((pos[0] - x_start) / cell_width)), 0);
    int y = max(min(cells_per_side, (int) ((pos[1] - y_start) / cell_width)), 0);
    int z = max(min(cells_per_side, (int) ((pos[2] - z_start) / cell_width)), 0);

    hashes[gid] = get_cell_ind(x, y, z, cells_per_side);
  }
}

__kernel void find_grid_starts(const __global uint * sorted_hashes, __global int * starts, const int num_particles) {
  int gid = get_global_id(0);
  if (gid < num_particles) {
    if (gid == 0 || sorted_hashes[gid] != sorted_hashes[gid - 1]) {
      starts[sorted_hashes[gid]] = gid;
    }
  }
}

float poly6(float4 pos, float h) {
  float len2 = pos[0] * pos[0] + pos[1] * pos[1] + pos[2] * pos[2];
  float h2 = h * h;
  if (len2 <= h2) {
    return 315.0f * pow(h2 - len2, 3.0f) / (64.0f * PI * pow(h, 9)); 
  }
  return 0;
}

float poly6corr(float r, float h) {
  float h2 = h * h;
  float r2 = r * r;
  if (r2 <= h2) {
    return 315.0f * pow(h2 - r2, 3.0f) / (64.0f * PI * pow(h, 9)); 
  }
  return 0;
}

float4 grad_spiky(float4 pos, float h) {
  float len = sqrt(pos[0] * pos[0] + pos[1] * pos[1] + pos[2] * pos[2]);
  float4 grad = pos;
  if (len < h) {
    grad = -45.0f * (h - len) * (h - len) / (PI * pow(h, 6.0f)) * pos / len;
    grad[3] = 1.0f;
  }
  if (isnan(grad[0])) printf("%f %f %f %f %f\n", len, pos[0], pos[1], pos[2], grad[0]);
  return grad; 
}

__kernel void calculate_density_lambdas(const __global float4 * new_positions, 
                                const __global int * grid_starts, const __global uint * sorted_indices, 
                                const __global uint * sorted_hashes, const int num_particles, 
                                const int cells_per_side, const int num_grids, 
                                const float mass, const float rest_density, 
                                const float h, __global float * lambdas) {
  int gid = get_global_id(0);
  if (gid < num_particles) {
    float4 pos = new_positions[gid]; 
    
    float Cf = 0.0f;
    float denom = 0.0f;
    float4 sum_grads = (float4) (0, 0, 0, 0);
    
    for (int z = -1; z <= 1; z++) {
      for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
          int grid_num = get_cell_ind(x, y, z, cells_per_side);
          uint grid_start = grid_starts[grid_num];
          if (grid_start >= 0) {
            int i = grid_start;
            while((i == 0) || (i < num_particles && sorted_hashes[i] == sorted_hashes[i - 1])) {
              int n_ind = sorted_indices[i];
              if (n_ind >= num_particles) printf("%d\n", n_ind);
              float4 n = new_positions[n_ind];
              if (n_ind != gid) {
                float4 diff = pos - n;

                Cf += poly6(diff, h);

                float4 vec = grad_spiky(diff, h) / rest_density;
                sum_grads += vec;
                denom += vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
              }
              i++;
            }
          }
        }
      }
    }
    Cf *= mass;
    Cf /= rest_density;
    Cf -= 1.0f;

    denom += sum_grads[0] * sum_grads[0] + sum_grads[1] * sum_grads[1] + sum_grads[2] * sum_grads[2];
    
    lambdas[gid] = - Cf / (denom + EPSILON);
  }
}

float4 reflect(float4 v, float4 n) {
  return -2.0f * dot(v, n) * n + v;
  }


__kernel void apply_lambdas(const __global float4 * new_positions, const __global float * lambdas,
                            const __global int * grid_starts, const __global uint * sorted_indices,
                            const __global uint * sorted_hashes, const int cells_per_side, 
                            const int num_grids, const int num_particles, 
                            const float h, __global float4 * final_positions) { 
  int gid = get_global_id(0);
  if (gid < num_particles) {
    float4 pos = new_positions[gid];
    float4 dp = (float4) (0, 0, 0, 0);
    float l = lambdas[gid];

    for (int z = -1; z <= 1; z++) {
      for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
          int grid_num = get_cell_ind(x, y, z, cells_per_side);
          uint grid_start = grid_starts[grid_num];
          if (grid_start >= 0) {
            int i = grid_start;
            while((i == 0) || (i < num_particles && sorted_hashes[i] == sorted_hashes[i - 1])) {
              int n_ind = sorted_indices[i];
              float4 n = new_positions[n_ind];
          /*
              if (n_ind != gid) {
                float n_l = lambdas[n_ind];
                float4 diff = pos - n;
                
                //float q = 0.1 * h; 
                //float s_corr = poly6(diff, h) / poly6corr(q, h);
                //
                //s_corr = pow(s_corr, 4.0f);

                //dp += (l + n_l - 0.1f * s_corr) * grad_spiky(diff, h);
              }
           */   
            }
          }
        }
      }
    }
    
    float4 final_pos = pos + dp;
    //printf("%f %f %f\n", final_pos[0], final_pos[1], final_pos[2]);
    final_pos[3] = 1.0f;
    final_positions[gid] = final_pos;
  }
}

__kernel void fix_collisions(const __global float4 * new_positions,
                                        const __global int * grid_starts, const __global uint * sorted_indices,
                                        const __global uint * sorted_hashes, const int cells_per_side, 
                                        const int num_grids, const int num_particles, 
                                        const float h, const float spacing, __global float4 * final_positions) { 
  int gid = get_global_id(0);
  if (gid < num_particles) {
    float4 pos = new_positions[gid];
    float4 dp = (float4) (0, 0, 0, 0);
    
    int num_collisions = 0;
    for (int z = -1; z <= 1; z++) {
      for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
          int grid_num = get_cell_ind(x, y, z, cells_per_side);
          uint grid_start = grid_starts[grid_num];

          if (grid_start >= 0) {
            int i = grid_start;
            while((i == 0) || (i < num_particles && sorted_hashes[i] == sorted_hashes[i - 1])) {
              int n_ind = sorted_indices[i];
              float4 n = new_positions[n_ind];
              if (n_ind != gid) {
                float4 diff = n - pos;
                float len = sqrt(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]);
                if (len < spacing && len != 0) {
                  num_collisions++;
                  pos -= (spacing - len) * diff / len;
                } else if (len == 0) {
                  num_collisions++;
                  printf("zero\n");
                  pos[1] += spacing;
                }
              }
              
            }
          }
        }
      }
    }
    if (pos[0] > 12) pos[0] = 12;
    if (pos[0] < -12) pos[0] = -12;
    if (pos[1] > 12) pos[1] = 12;
    if (pos[1] < -12) pos[1] = -12;
    if (pos[2] > 12) pos[2] = 12;
    if (pos[2] < -12) pos[2] = -12;
    pos[3] = 1.0f;
    final_positions[gid] = pos;
  }
  
}
