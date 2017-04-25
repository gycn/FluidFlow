#define DT 0.01f
#define GRAVITY ((float4){0.0f, -9.81f, 0.0f, 0.0f})

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
    positions[gid] = new_positions[gid];
    velocity[gid] = new_velocity[gid];
  }
}

__kernel void radix_calc(const __global uint* hash_vals, const int num_elements,
                    __global int* num_of_each, const int radix) {
  int gid = get_global_id(0);
  
  if (gid < num_elements) {
    int num = (hash_vals[gid] / (int) pow(10.0f, (float)radix)) % 10;
    
    atomic_add(&num_of_each[num], 1);
  }
}

