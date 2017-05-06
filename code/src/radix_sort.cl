__kernel void histogram(const __global uint * hashes, __global uint * hist, __local uint * local_hist, 
                        const uint radix_size, const uint mask, const uint shift, 
                        const uint num_per_thread, const int num_points) {
  int local_id = get_local_id(0);
  int global_id = get_global_id(0);
  int group_id = get_group_id(0);
  int group_size = get_local_size(0);
  
  int local_start = local_id * radix_size;
  int local_end = local_start + radix_size;

  for (int i = local_start; i < local_end; i++) {
    local_hist[i] = 0;
  }

  barrier(CLK_LOCAL_MEM_FENCE);

  int global_arr_start = num_per_thread * global_id;

  for (int i = global_arr_start; i < global_arr_start + num_per_thread; i++) {
    int radix = (mask & hashes[i]) >> shift;
    local_hist[local_start + radix]++;
  }

  barrier(CLK_LOCAL_MEM_FENCE);
  
  int num_threads = get_global_size(0);

  for (int i = 0; i < radix_size; i++) {
    hist[num_threads * i + global_id] = local_hist[local_start + i]; 
  }

  barrier(CLK_GLOBAL_MEM_FENCE);
}

__kernel void sum(const __global uint * hist, __global uint * sums, const int len) {
  int s = len / get_global_size(0);
  int num = get_global_id(0);
  int start = s * num;
  uint total = 0;
  for (int i = start; i < start + s; i++) {
    total += hist[i];
  }
  sums[num] = total;
}

__kernel void scan(const __global uint * hist, __global uint * sums, __local uint * local_sums, 
                   const __global uint * offsets, const char offset, const int total_len) {
  int global_id = get_global_id(0);
  int local_id = get_local_id(0);
  int size  = get_local_size(0);
  int group_num = get_group_id(0);

  int len = total_len / get_num_groups(0);
  int start = len * group_num;
  local_sums[local_id] = hist[local_id + start];

  barrier(CLK_GLOBAL_MEM_FENCE);

  int mult = 1;
  for(int i = len >> 1; i >= 1; i>>=1) {
    if (local_id < i) {
      int high = 2 * mult * (local_id + 1) - 1;
      int low = high - mult;
      local_sums[high] += local_sums[low];
    }
    mult <<= 1;
    barrier(CLK_GLOBAL_MEM_FENCE);
  }
  
  if (local_id == 0) {
    local_sums[len - 1] = 0; 
  }
  
  mult >>= 1;

  for(int i = 1; i < len; i <<= 1) {
    if (local_id < i) {
      int high = 2 * mult * (local_id + 1) - 1;
      int low = high - mult;

      uint temp = local_sums[low];
      local_sums[low] = local_sums[high];
      local_sums[high] += temp;
    }
    barrier(CLK_GLOBAL_MEM_FENCE);
    mult >>= 1;
  }
  
  if (offset) 
    local_sums[local_id] += offsets[group_num];

  sums[local_id + start] = local_sums[local_id];
}

__kernel void reorder(const __global uint * sums, __local uint * local_sums,
                      const __global uint * hashes, __global uint * sorted_hashes,
                      const __global uint * vals, __global uint * sorted_vals, 
                      const uint radix_size, const uint mask, const uint shift, 
                      const uint num_per_thread, const int num_points) {
  
  int local_id = get_local_id(0);
  int global_id = get_global_id(0);
  int group_id = get_group_id(0);
  int group_size = get_local_size(0);

  int local_start = local_id * radix_size;
  int num_threads = get_global_size(0);

  for (int i = 0; i < radix_size; i++) {
    local_sums[local_start + i] = sums[num_threads * i + global_id]; 
  }
  
  barrier(CLK_LOCAL_MEM_FENCE);

  int global_arr_start = num_per_thread * global_id;

  for (int i = global_arr_start; i < global_arr_start + num_per_thread; i++) {
    uint radix = (mask & hashes[i]) >> shift;
    uint new_pos = local_sums[local_start + radix];
    local_sums[local_start + radix]++;
    
    sorted_hashes[new_pos] = hashes[i];
    sorted_vals[new_pos] = vals[i];
  }
}
