#define DT 0.0083f
#define GRAVITY ((float4){0.0f, -9.81f, 0.0f, 0.0f})
#define PI 3.141592f
#define EPSILON 400.0f
#define DAMPING 0.4f
#define BOUND_X 1.55f
#define BOUND_Y 1.55f
#define BOUND_Z 1.55f

float poly6(float4 pos, float h) {
  float len2 = pos[0] * pos[0] + pos[1] * pos[1] + pos[2] * pos[2];
  float h2 = h * h;
  if (len2 <= h2) {
    return 315.0f * pow(h2 - len2, 3.0f) / (64.0f * PI * pow(h, 9)); 
  }
  return 0;
}

__kernel void timestep(const __global float4 * positions, __global float4 * new_positions, 
                       const __global float4 * velocity, __global float4 * new_velocity,
                       const int num_particles) {
  int gid =  get_global_id(0);
  if (gid < num_particles) {
    float4 p = positions[gid];
    float4 v = velocity[gid];

    v += DT * GRAVITY;
    p += DT * v;
    new_positions[gid] = p;
    new_velocity[gid] = v;
  }
}

__kernel void update(__global float4 * positions, const __global float4 * new_positions, 
                     __global float4 * velocity,  const __global float4 * new_velocity,
                     const int num_particles) {  
  int gid =  get_global_id(0);
  if (gid < num_particles) {
    velocity[gid] = (new_positions[gid] - positions[gid]) / DT * 0.99f;
    float4 pos = new_positions[gid];
    
    if (pos[0] > BOUND_X) {
      pos[0] = BOUND_X - 0.001f;
      velocity[gid][0] = -DAMPING * velocity[gid][0];
    }
    if (pos[0] < -BOUND_X) {
      pos[0] = -BOUND_X + 0.001f;
      velocity[gid][0] = -DAMPING * velocity[gid][0];
    }
    
    if (pos[1] < -BOUND_Y) {
      pos[1] = -BOUND_Y + 0.001f;
      velocity[gid][1] = 0;//-DAMPING * velocity[gid][1];
    }
    
    if (pos[2] > BOUND_Z) {
      pos[2] = BOUND_Z - 0.001f;
      velocity[gid][2] = -DAMPING * velocity[gid][2];
    }
    if (pos[2] < -BOUND_Z) {
      pos[2] = -BOUND_Z + 0.001f;
      velocity[gid][2] = -DAMPING * velocity[gid][2];
    }

    positions[gid] = pos;
    barrier(CLK_GLOBAL_MEM_FENCE);

  }
}

__kernel void update_velocities(__global float4 * new_velocities, const __global float4 * positions,
                                const __global int * grid_starts, const __global uint * sorted_indices, 
                                const __global uint * sorted_hashes, const int num_particles, 
                                const int cells_per_side, const int num_grids, const float cell_width, 
                                const float x_start, const float y_start, float z_start,
                                const float h) {
  int gid = get_global_id(0);
  if (gid < num_particles) {
    float4 pos = positions[gid];
    float4 v = new_velocities[gid];
    int grid_x = max(min(cells_per_side, (int) ((pos[0] - x_start) / cell_width)), 0);
    int grid_y = max(min(cells_per_side, (int) ((pos[1] - y_start) / cell_width)), 0);
    int grid_z = max(min(cells_per_side, (int) ((pos[2] - z_start) / cell_width)), 0);
    float4 dv = (float4) (0, 0, 0, 0);
    for (int z = -1; z <= 1; z++) {
      for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
          int cell_x = grid_x + x;
          int cell_y = grid_y + y;
          int cell_z = grid_z + z;
          if (cell_x >= 0 && cell_y >= 0 && cell_z >= 0 && 
              cell_x < cells_per_side && cell_y < cells_per_side && cell_z < cells_per_side) {
            int grid_num = get_cell_ind(cell_x, cell_y, cell_z, cells_per_side);
            int grid_start = grid_starts[grid_num];
            if (grid_start >= 0) {
              int i = grid_start;
              uint hash = sorted_hashes[i];
              while((i == 0) || (i < num_particles && sorted_hashes[i] == hash)) {
                int n_ind = sorted_indices[i];
                float4 n = positions[n_ind];
                if (n_ind != gid) {
                  float4 diff = pos - n;
                  dv += (v - new_velocities[n_ind]) * poly6(diff, h);
                }
              }
            }
          }
        }
      }
    }
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
  if (len < h && len > 0) {
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
                                const float h, __global float * lambdas, const float cell_width,
                                const float x_start, const float y_start, const float z_start) {
  int gid = get_global_id(0);
  if (gid < num_particles) {
    float4 pos = new_positions[gid]; 
    
    float Cf = 0.0f;
    float denom = 0.0f;
    float4 sum_grads = (float4) (0, 0, 0, 0);

    int grid_x = max(min(cells_per_side, (int) ((pos[0] - x_start) / cell_width)), 0);
    int grid_y = max(min(cells_per_side, (int) ((pos[1] - y_start) / cell_width)), 0);
    int grid_z = max(min(cells_per_side, (int) ((pos[2] - z_start) / cell_width)), 0);
    for (int z = -1; z <= 1; z++) {
      for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
          int cell_x = grid_x + x;
          int cell_y = grid_y + y;
          int cell_z = grid_z + z;
          if (cell_x >= 0 && cell_y >= 0 && cell_z >= 0 && 
              cell_x < cells_per_side && cell_y < cells_per_side && cell_z < cells_per_side) {
            int grid_num = get_cell_ind(cell_x, cell_y, cell_z, cells_per_side);
            int grid_start = grid_starts[grid_num];
            if (grid_start >= 0) {
              int i = grid_start;
              uint hash = sorted_hashes[i];
              while((i == 0) || (i < num_particles && sorted_hashes[i] == hash)) {
                int n_ind = sorted_indices[i];
                float4 n = new_positions[n_ind];
                if (n_ind != gid) {
                  float4 diff = pos - n;

                  Cf += poly6(diff, h);

                  float4 vec = grad_spiky(diff, h) * mass / rest_density;
                  sum_grads += vec;
                  denom += vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
                }
                i++;
              }
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

__kernel void apply_lambdas(const __global float4 * new_positions, const __global float * lambdas,
                            const __global int * grid_starts, const __global uint * sorted_indices,
                            const __global uint * sorted_hashes, const int cells_per_side, 
                            const int num_grids, const int num_particles, 
                            const float h, __global float4 * final_positions, const float cell_width,
                                const float x_start, const float y_start, const float z_start, const float rest_density) {
  int gid = get_global_id(0);
  if (gid < num_particles) {
    float4 pos = new_positions[gid];
    float4 dp = (float4) (0, 0, 0, 0);
    float l = lambdas[gid];

    int grid_x = max(min(cells_per_side, (int) ((pos[0] - x_start) / cell_width)), 0);
    int grid_y = max(min(cells_per_side, (int) ((pos[1] - y_start) / cell_width)), 0);
    int grid_z = max(min(cells_per_side, (int) ((pos[2] - z_start) / cell_width)), 0);
    
    for (int z = -1; z <= 1; z++) {
      for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
          int cell_x = grid_x + x;
          int cell_y = grid_y + y;
          int cell_z = grid_z + z;
          if (cell_x >= 0 && cell_y >= 0 && cell_z >= 0 && 
              cell_x < cells_per_side && cell_y < cells_per_side && cell_z < cells_per_side) {
            int grid_num = get_cell_ind(cell_x, cell_y, cell_z, cells_per_side);
            int grid_start = grid_starts[grid_num];
            if (grid_start >= 0) {
              int i = grid_start;
              uint hash = sorted_hashes[i];
              while((i == 0) || (i < num_particles && sorted_hashes[i] == hash)) {
                int n_ind = sorted_indices[i];
                float4 n = new_positions[n_ind];
                if (n_ind != gid) {
                  float n_l = lambdas[n_ind];
                  float4 diff = pos - n;
                  
                  float q = 0.1 * h; 
                  float s_corr = poly6(diff, h) / poly6corr(q, h);
                  
                  s_corr = pow(s_corr, 4.0f);

                  dp += (l + n_l - 0.005f * s_corr) * grad_spiky(diff, h);
                }
                i++;
              }
            }
          }
        }
      }
    }
    
    float4 final_pos = pos + dp / rest_density;
    //printf("%f %f %f\n", final_pos[0], final_pos[1], final_pos[2]);
    final_pos[3] = 1.0f;
    final_positions[gid] = final_pos;
  }
}

__kernel void fix_collisions(__global float4 * new_positions,
                                        const __global int * grid_starts, const __global uint * sorted_indices,
                                        const __global uint * sorted_hashes, const int cells_per_side, 
                                        const int num_grids, const int num_particles, 
                                        const float h, const float spacing, __global float4 * final_positions, const float cell_width, 
                                const float x_start, const float y_start, const float z_start) {
  int gid = get_global_id(0);
  if (gid < num_particles) {
    float4 pos = new_positions[gid];

    final_positions[gid][0] = 0.0f;
    final_positions[gid][1] = 0.0f;
    final_positions[gid][2] = 0.0f;

    barrier(CLK_GLOBAL_MEM_FENCE);
    
    int num_collisions = 0;
    int grid_x = max(min(cells_per_side, (int) ((pos[0] - x_start) / cell_width)), 0);
    int grid_y = max(min(cells_per_side, (int) ((pos[1] - y_start) / cell_width)), 0);
    int grid_z = max(min(cells_per_side, (int) ((pos[2] - z_start) / cell_width)), 0);
    
    for (int z = -1; z <= 1; z++) {
      for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
          int cell_x = grid_x + x;
          int cell_y = grid_y + y;
          int cell_z = grid_z + z;
          if (cell_x >= 0 && cell_y >= 0 && cell_z >= 0 && 
              cell_x < cells_per_side && cell_y < cells_per_side && cell_z < cells_per_side) {
            int grid_num = get_cell_ind(cell_x, cell_y, cell_z, cells_per_side);
            int grid_start = grid_starts[grid_num];
            if (grid_start >= 0) {
              int i = grid_start;
                uint hash = sorted_hashes[i];
                while((i == 0) || (i < num_particles && sorted_hashes[i] == hash)) {
                int n_ind = sorted_indices[i];
                float4 n = new_positions[n_ind];
                if (n_ind != gid) {
                  float4 diff = n - pos;
                  float len = sqrt(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]);
                  float4 dp = (float4) (0, 0, 0, 0);
                  if (len < spacing && len != 0) {
                    num_collisions++;
                    float4 dp = (spacing - len) * 0.5f * diff / len;
                    final_positions[gid] -= dp;
                    //final_positions[n_ind][0] += dp[0] * n[0];
                    //final_positions[n_ind][1] += dp[1] * n[1];
                    //final_positions[n_ind][2] += dp[2] * n[2];
                  } else if (len == 0) {
                    num_collisions++;
                    printf("zero\n");
                    dp[1] += 0.5f * spacing;
                    new_positions[n_ind][1] -= 0.5f * spacing; 
                  }
                }
                i++; 
              }
            }
          }
        }
      }
    }
    final_positions[gid] += pos;
    final_positions[gid][3] = 1.0f;
  }
  
}
