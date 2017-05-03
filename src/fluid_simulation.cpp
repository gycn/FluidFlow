#include "fluid_simulation.h"
#include <OpenCL/cl.h>
#include <OpenCL/cl.hpp>
#include <OpenCL/cl_gl.h>
#include <OpenGL/CGLCurrent.h>
#include <OpenGL/CGLDevice.h>
#include <OpenCL/cl_gl_ext.h>

#include "glad.h"
void FluidSimulation::step() {
  //Apply gravity & update based on current velocity
  err = step_kernel.setArg(0, cl_positions);
  checkErr("Kernel::setArg()");
  err = step_kernel.setArg(1, cl_new_positions[0]);
  checkErr("Kernel::setArg()");
  err = step_kernel.setArg(2, cl_velocities);
  checkErr("Kernel::setArg()");
  err = step_kernel.setArg(3, cl_new_velocities);
  checkErr("Kernel::setArg()");
  err = step_kernel.setArg(4, num_points);
  checkErr("Kernel::setArg()");

  err = queue.enqueueNDRangeKernel(step_kernel, 
      cl::NullRange, cl::NDRange(num_points), cl::NDRange(1, 1),
      NULL, &event);
  checkErr("ComamndQueue::enqueueNDRangeKernel()");  
  event.wait();
  
  err = hash_kernel.setArg(0, cl_positions);
  checkErr("Kernel::setArg()");
  err = hash_kernel.setArg(1, cl_hashes[0]);
  checkErr("Kernel::setArg()");
  err = hash_kernel.setArg(2, num_points);
  checkErr("Kernel::setArg()");
  err = hash_kernel.setArg(3, cells_per_side);
  checkErr("Kernel::setArg()");
  err = hash_kernel.setArg(4, grid_width);
  checkErr("Kernel::setArg()");
  err = hash_kernel.setArg(5, x_start);
  checkErr("Kernel::setArg()");
  err = hash_kernel.setArg(6, y_start);
  checkErr("Kernel::setArg()");
  err = hash_kernel.setArg(7, z_start);
  checkErr("Kernel::setArg()");

  err = queue.enqueueNDRangeKernel(hash_kernel, 
      cl::NullRange, cl::NDRange(num_points), cl::NDRange(1, 1),
      NULL, &event);
  checkErr("ComamndQueue::enqueueNDRangeKernel()");  
  event.wait();
  
  sort();

  queue.enqueueWriteBuffer(cl_grid_starts, true, 0, sizeof(cl_int) * num_grids, negative_ones);
  
  err = grid_kernel.setArg(0, cl_hashes[0]);
  checkErr("Kernel::setArg()");
  err = grid_kernel.setArg(1, cl_grid_starts);
  checkErr("Kernel::setArg()");
  err = grid_kernel.setArg(2, num_points);
  checkErr("Kernel::setArg()");

  err = queue.enqueueNDRangeKernel(grid_kernel, 
      cl::NullRange, cl::NDRange(num_points), cl::NDRange(1, 1),
      NULL, &event);
  checkErr("ComamndQueue::enqueueNDRangeKernel()");  
  event.wait();
  //cl_uint start_read[num_grids];
  //queue.enqueueReadBuffer(cl_grid_starts, true, 0, sizeof(cl_uint) * num_grids, start_read);
  //for (int i = 0; i < num_grids; i++) printf("%d ", start_read[i]);
  //printf("\n");

  int ind = 0;
  for (int i = 0; i < 2; i++) {
    //Calculate lambdas
    err = density_lambda_kernel.setArg(0, cl_new_positions[ind]); 
    checkErr("Kernel::setArg()");
    err = density_lambda_kernel.setArg(1, cl_grid_starts); 
    checkErr("Kernel::setArg()");
    err = density_lambda_kernel.setArg(2, cl_indices[0]);
    checkErr("Kernel::setArg()");
    err = density_lambda_kernel.setArg(3, cl_hashes[0]);
    checkErr("Kernel::setArg()");
    err = density_lambda_kernel.setArg(4, num_points);
    checkErr("Kernel::setArg()");
    err = density_lambda_kernel.setArg(5, cells_per_side);
    checkErr("Kernel::setArg()");
    err = density_lambda_kernel.setArg(6, num_grids);
    checkErr("Kernel::setArg()");
    err = density_lambda_kernel.setArg(7, mass);
    checkErr("Kernel::setArg()");
    err = density_lambda_kernel.setArg(8, density);
    checkErr("Kernel::setArg()");
    err = density_lambda_kernel.setArg(9, h);
    checkErr("Kernel::setArg()");
    err = density_lambda_kernel.setArg(10, cl_lambdas);
    checkErr("kernel::setArg()");

    err = queue.enqueueNDRangeKernel(density_lambda_kernel, 
        cl::NullRange, cl::NDRange(num_points), cl::NDRange(1, 1),
        NULL, &event);
    checkErr("ComamndQueue::enqueueNDRangeKernel()");  
    event.wait();

  //* 
    //Set new positions
    err = lambda_pos_kernel.setArg(0, cl_new_positions[ind]); 
    checkErr("Kernel::setArg()");
    err = lambda_pos_kernel.setArg(1, cl_lambdas); 
    checkErr("Kernel::setArg()");
    err = lambda_pos_kernel.setArg(2, cl_grid_starts);
    checkErr("Kernel::setArg()");
    err = lambda_pos_kernel.setArg(3, cl_indices[0]);
    checkErr("Kernel::setArg()");
    err = lambda_pos_kernel.setArg(4, cl_hashes[0]);
    checkErr("Kernel::setArg()");
    err = lambda_pos_kernel.setArg(5, cells_per_side);
    checkErr("Kernel::setArg()");
    err = lambda_pos_kernel.setArg(6, num_grids);
    checkErr("Kernel::setArg()");
    err = lambda_pos_kernel.setArg(7, num_points);
    checkErr("Kernel::setArg()");
    err = lambda_pos_kernel.setArg(8, h);
    checkErr("Kernel::setArg()");
    err = lambda_pos_kernel.setArg(9, cl_new_positions[1 - ind]);
    checkErr("kernel::setArg()");

    err = queue.enqueueNDRangeKernel(lambda_pos_kernel, 
        cl::NullRange, cl::NDRange(num_points), cl::NDRange(1, 1),
        NULL, &event);
    checkErr("ComamndQueue::enqueueNDRangeKernel()");  
    event.wait();
    ind = 1 - ind;
  /*
    //adjust for collisions
    err = collision_kernel.setArg(0, cl_new_positions[ind]); 
    checkErr("Kernel::setArg()");
    err = collision_kernel.setArg(1, cl_grid_starts); 
    checkErr("Kernel::setArg()");
    err = collision_kernel.setArg(2, cl_indices[0]);
    checkErr("Kernel::setArg()");
    err = collision_kernel.setArg(3, cl_hashes[0]);
    checkErr("Kernel::setArg()");
    err = collision_kernel.setArg(4, cells_per_side);
    checkErr("Kernel::setArg()");
    err = collision_kernel.setArg(5, num_grids);
    checkErr("Kernel::setArg()");
    err = collision_kernel.setArg(6, num_points);
    checkErr("Kernel::setArg()");
    err = collision_kernel.setArg(7, h);
    checkErr("Kernel::setArg()");
    err = collision_kernel.setArg(8, spacing);
    checkErr("Kernel::setArg()");
    err = collision_kernel.setArg(9, cl_new_positions[1 - ind]);
    checkErr("kernel::setArg()");

    err = queue.enqueueNDRangeKernel(collision_kernel, 
        cl::NullRange, cl::NDRange(num_points), cl::NDRange(1, 1),
        NULL, &event);
    checkErr("ComamndQueue::enqueueNDRangeKernel()");  
    event.wait();
    ind = 1 - ind;
  //*/
  }
  //Update previous particle values to new values
  err = update_kernel.setArg(0, cl_positions);
  checkErr("Kernel::setArg()");
  err = update_kernel.setArg(1, cl_new_positions[ind]);
  checkErr("Kernel::setArg()");
  err = update_kernel.setArg(2, cl_velocities);
  checkErr("Kernel::setArg()");
  err = update_kernel.setArg(3, cl_new_velocities);
  checkErr("Kernel::setArg()");
  err = update_kernel.setArg(4, num_points);
  checkErr("Kernel::setArg()");

  err = queue.enqueueNDRangeKernel(update_kernel, 
      cl::NullRange, cl::NDRange(num_points), cl::NDRange(1, 1),
      NULL, &event);
  checkErr("ComamndQueue::enqueueNDRangeKernel()");  
  event.wait();
}


void FluidSimulation::init_CL(GLuint vertex_VBO) {
  std::vector< cl::Platform > platformList;
  cl::Platform::get(&platformList);
  err = platformList.size()!=0 ? CL_SUCCESS : -1;
  checkErr("cl::Platform::get");
  
  std::cerr << "Platform number is: " << platformList.size() << std::endl;std::string platformVendor;
  platformList[0].getInfo((cl_platform_info)CL_PLATFORM_VENDOR, &platformVendor);
  std::cerr << "Platform is by: " << platformVendor << "\n";
  
  CGLContextObj     CGLGetCurrentContext(void);
  CGLShareGroupObj  CGLGetShareGroup(CGLContextObj);
  
  CGLContextObj     kCGLContext     = CGLGetCurrentContext();
  CGLShareGroupObj  kCGLShareGroup  = CGLGetShareGroup(kCGLContext);
  
  cl_context_properties properties[] =
  {
    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
    (cl_context_properties) kCGLShareGroup,
    0
  };
  
  context = cl::Context(CL_DEVICE_TYPE_GPU, properties, NULL, NULL, &err);
  checkErr( "Context::Context()");
  
  devices = context.getInfo<CL_CONTEXT_DEVICES>();
  err = devices.size() > 0 ? CL_SUCCESS : -1;
  checkErr("devices.size() > 0");

  std::ifstream file("../src/simulation.cl");
  err = file.is_open() ? CL_SUCCESS:-1;
  checkErr("simulation.cl");

  std::string prog(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
  cl::Program::Sources source(1, std::make_pair(prog.c_str(), prog.length()+1));
  program = cl::Program(context, source);
  err = program.build(devices,"");
  checkErr("Program::build()");

  std::ifstream sort_file("../src/radix_sort.cl");
  err = sort_file.is_open() ? CL_SUCCESS:-1;
  checkErr("simulation.cl");

  std::string sort_prog(std::istreambuf_iterator<char>(sort_file), (std::istreambuf_iterator<char>()));
  cl::Program::Sources sort_source(1, std::make_pair(sort_prog.c_str(), sort_prog.length()+1));
  sort_program = cl::Program(context, sort_source);
  err = sort_program.build(devices,"");
  checkErr("Program::build()");

  queue = cl::CommandQueue(context, devices[0], 0, &err); 
  checkErr("CommandQueue::CommandQueue()");

  //Kernel initializations
  step_kernel = cl::Kernel(program, "timestep", &err);
  checkErr("Kernel::Kernel() step_kernel");
  
  density_lambda_kernel = cl::Kernel(program, "calculate_density_lambdas", &err);
  checkErr("Kernel::Kernel() density_lambda_kernel");

  lambda_pos_kernel = cl::Kernel(program, "apply_lambdas", &err);
  checkErr("Kernel::Kernel() lambda_pos_kernel");

  collision_kernel = cl::Kernel(program, "fix_collisions", &err);
  checkErr("Kernel::Kernel() collision_kernel");

  update_kernel = cl::Kernel(program, "update", &err);
  checkErr("Kernel::Kernel() update_kernel");
  
  hash_kernel = cl::Kernel(program, "calculate_hashes", &err);
  checkErr("Kernel::Kernel() hash_kernel");

  grid_kernel = cl::Kernel(program, "find_grid_starts", &err);
  checkErr("Kernel::Kernel() grid_kernel");

  //Radix sort kernels
  hist_kernel = cl::Kernel(sort_program, "histogram", &err);
  checkErr("Kernel::Kernel() hist_kernel");

  scan_kernel = cl::Kernel(sort_program, "scan", &err);
  checkErr("Kernel::Kernel() scan_kernel");

  sum_kernel = cl::Kernel(sort_program, "sum", &err);
  checkErr("Kernel::Kernel() sum_kernel");

  reorder_kernel = cl::Kernel(sort_program, "reorder", &err);
  checkErr("Kernel::Kernel() reorder_kernel");


  //Buffer initializations
  cl_positions = cl::BufferGL(context, CL_MEM_READ_WRITE, vertex_VBO, &err);
  checkErr("cl::BufferGL cl_positions");

  float* zeros = (float*) malloc(sizeof(float) * num_vertices);
  for (int i = 0; i < num_vertices; i++) {
    zeros[i] = 0.0f;
  }

  cl_new_positions[0] = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 
    sizeof(float) * num_vertices, zeros, &err); 
  checkErr("cl::Buffer cl_new_positions");

  cl_new_positions[1] = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 
    sizeof(float) * num_vertices, zeros, &err); 
  checkErr("cl::Buffer cl_new_positions");

  cl_velocities = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 
    sizeof(float) * num_vertices, zeros, &err); 
  checkErr("cl::Buffer cl_velocities");

  cl_new_velocities = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 
    sizeof(float) * num_vertices, zeros, &err); 
  checkErr("cl::Buffer cl_new_velocities");
  
  free(zeros);

  cl_lambdas = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(float) * num_points, NULL, &err);
  checkErr("cl::Buffer cl_lambdas");
  
  cl_grid_starts = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(cl_int) * num_grids, NULL, &err);
  checkErr("cl::Buffer cl_grid_starts");

  //Sorting
  cl_hashes[0] = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(cl_uint) * num_points, NULL, &err);
  checkErr("cl::Buffer cl_hashes");

  cl_hashes[1] = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(cl_uint) * num_points, NULL, &err);
  checkErr("cl::Buffer cl_hashes");

  cl_hist = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(cl_uint) * total_threads * radix_size, NULL, &err);
  checkErr("cl::Buffer cl_hist");

  cl_sums = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(cl_uint) * total_threads * radix_size, NULL, &err);
  checkErr("cl::Buffer cl_sums");

  cl_offsets = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(cl_uint) * splits, NULL, &err);

  cl_offset_sums =  cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(cl_uint) * splits, NULL, &err);

  cl_uint * inds = (cl_uint *) malloc(sizeof(cl_uint) * num_points);
  for (cl_uint i = 0; i < num_points; i++) {
    inds[i] = i;
  }

  cl_indices[0] = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 
    sizeof(cl_uint) * num_points, inds, &err);
  checkErr("cl::Buffer cl_indices");
  
  cl_indices[1] = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 
    sizeof(cl_uint) * num_points, inds, &err);
  checkErr("cl::Buffer cl_indices");

  local_hist = cl::Local(sizeof(cl_uint) * threads_per_block * radix_size);
  local_reorder_sums = cl::Local(sizeof(cl_uint) * threads_per_block * radix_size);

  free(inds);

  negative_ones = (cl_int *) malloc(sizeof(cl_int) * num_grids);
  for (int i = 0; i < num_grids; i++) negative_ones[i] = -1;
}

void FluidSimulation::sort() {
  cl_uint mask = radix_mask;
  int ind = 0;
  cl_uint shift = 0;
  for(int i = 0; i < sort_passes; i++) {
    err = hist_kernel.setArg(0, cl_hashes[ind]);   
    checkErr("Kernel::setArg()");
    err = hist_kernel.setArg(1, cl_hist);   
    checkErr("Kernel::setArg()");
    err = hist_kernel.setArg(2, local_hist);   
    checkErr("Kernel::setArg()");
    err = hist_kernel.setArg(3, radix_size);   
    checkErr("Kernel::setArg()");
    err = hist_kernel.setArg(4, mask);   
    checkErr("Kernel::setArg()");
    err = hist_kernel.setArg(5, shift);   
    checkErr("Kernel::setArg()");
    err = hist_kernel.setArg(6, num_per_thread);   
    checkErr("Kernel::setArg()");
    err = hist_kernel.setArg(7, num_points);   
    checkErr("Kernel::setArg()");

    err = queue.enqueueNDRangeKernel(hist_kernel, 
        cl::NullRange, cl::NDRange(total_threads), cl::NDRange(threads_per_block),
        NULL, &event);
    checkErr("ComamndQueue::enqueueNDRangeKernel()");  
    event.wait();
    //cl_uint hist_read[total_threads * radix_size];
    //queue.enqueueReadBuffer(cl_hist, true, 0, total_threads * radix_size * sizeof(cl_uint), hist_read);
    //for (int i = 0; i < total_threads * radix_size; i++) {
    //  printf("%d ", hist_read[i]);
    //  if ((i + 1) % total_threads == 0) {
    //  //printf("\n");
    //  }
    //}
    //printf("\n");
    //printf("\n");
    //printf("\n");

    err = sum_kernel.setArg(0, cl_hist);
    checkErr("Kernel::setArg()");
    err = sum_kernel.setArg(1, cl_offsets);
    checkErr("Kernel::setArg()");
    err = sum_kernel.setArg(2, total_threads * radix_size);   
    checkErr("Kernel::setArg()");

    err = queue.enqueueNDRangeKernel(sum_kernel, 
        cl::NullRange, cl::NDRange(splits), cl::NDRange(1, 1),
        NULL, &event);
    checkErr("ComamndQueue::enqueueNDRangeKernel()");  
    event.wait();

    //cl_uint hist_read[splits];
    //queue.enqueueReadBuffer(cl_offsets, true, 0, splits * sizeof(cl_uint), hist_read);
    //for (int i = 0; i < splits; i++) {
    //  printf("%d ", hist_read[i]);
    //  if ((i + 1) % total_threads == 0) {
    //  printf("\n");
    //  }
    //}
    //printf("\n");
    //printf("\n");
    //printf("\n");

    err = scan_kernel.setArg(0, cl_offsets);   
    checkErr("Kernel::setArg()");
    err = scan_kernel.setArg(1, cl_offset_sums);   
    checkErr("Kernel::setArg()");
    err = scan_kernel.setArg(2, cl::Local(sizeof(cl_uint) * splits));   
    checkErr("Kernel::setArg()");
    err = scan_kernel.setArg(3, NULL);   
    checkErr("Kernel::setArg()");
    err = scan_kernel.setArg(4, false);   
    checkErr("Kernel::setArg()");
    err = scan_kernel.setArg(5, splits);   
    checkErr("Kernel::setArg()");

    err = queue.enqueueNDRangeKernel(scan_kernel, 
        cl::NullRange, cl::NDRange(splits), cl::NDRange(splits),
        NULL, &event);
    checkErr("ComamndQueue::enqueueNDRangeKernel()");  
    event.wait();

    //cl_uint offset_read[splits];
    //queue.enqueueReadBuffer(cl_offset_sums, true, 0, splits * sizeof(cl_uint), offset_read);
    //for (int i = 0; i < splits; i++) {
    //  printf("%d ", offset_read[i]);
    //  if ((i + 1) % total_threads == 0) {
    //  printf("\n");
    //  }
    //}
    //printf("\n");
    //printf("\n");
    //printf("\n");
    
    err = scan_kernel.setArg(0, cl_hist);   
    checkErr("Kernel::setArg()");
    err = scan_kernel.setArg(1, cl_sums);   
    checkErr("Kernel::setArg()");
    err = scan_kernel.setArg(2, cl::Local(sizeof(cl_uint) * radix_size * total_threads / splits));   
    checkErr("Kernel::setArg()");
    err = scan_kernel.setArg(3, cl_offset_sums);   
    checkErr("Kernel::setArg()");
    err = scan_kernel.setArg(4, true);   
    checkErr("Kernel::setArg()");
    err = scan_kernel.setArg(5, radix_size * total_threads);   
    checkErr("Kernel::setArg()");
    
    err = queue.enqueueNDRangeKernel(scan_kernel, 
        cl::NullRange, cl::NDRange(radix_size * total_threads), cl::NDRange(radix_size * total_threads / splits),
        NULL, &event);
    checkErr("ComamndQueue::enqueueNDRangeKernel()");  
    event.wait();

    //cl_uint sums_read[radix_size * total_threads];
    //queue.enqueueReadBuffer(cl_sums, true, 0, radix_size * total_threads * sizeof(cl_uint), sums_read);
    //for (int i = 0; i < radix_size * total_threads; i++) {
    //  printf("%d ", sums_read[i]);
    //  if ((i + 1) % total_threads == 0) {
    //  //printf("\n");
    //  }
    //}
    //printf("\n");
    //printf("\n");
    //printf("\n");

    err = reorder_kernel.setArg(0, cl_sums);   
    checkErr("Kernel::setArg()");
    err = reorder_kernel.setArg(1, cl::Local(sizeof(cl_uint) * radix_size * threads_per_block));   
    checkErr("Kernel::setArg()");
    err = reorder_kernel.setArg(2, cl_hashes[ind]);   
    checkErr("Kernel::setArg()");
    err = reorder_kernel.setArg(3, cl_hashes[1 - ind]);   
    checkErr("Kernel::setArg()");
    err = reorder_kernel.setArg(4, cl_indices[ind]);   
    checkErr("Kernel::setArg()");
    err = reorder_kernel.setArg(5, cl_indices[1 - ind]);   
    checkErr("Kernel::setArg()");
    err = reorder_kernel.setArg(6, radix_size);   
    checkErr("Kernel::setArg()");
    err = reorder_kernel.setArg(7, mask);   
    checkErr("Kernel::setArg()");
    err = reorder_kernel.setArg(8, shift);   
    checkErr("Kernel::setArg()");
    err = reorder_kernel.setArg(9, num_per_thread);   
    checkErr("Kernel::setArg()");
    err = reorder_kernel.setArg(10, num_points);   
    checkErr("Kernel::setArg()");

    ind = 1 - ind;
    mask <<= radix_bits;
    shift += radix_bits;
    
    err = queue.enqueueNDRangeKernel(reorder_kernel, 
        cl::NullRange, cl::NDRange(total_threads), cl::NDRange(threads_per_block),
        NULL, &event);
    checkErr("ComamndQueue::enqueueNDRangeKernel()");  
    event.wait();

    //cl_uint sums_read[num_points];
    //queue.enqueueReadBuffer(cl_hashes[ind], true, 0, num_points * sizeof(cl_uint), sums_read);
    //for (int i = 0; i < num_points; i++) {
    //  printf("%d ", sums_read[i]);
    //  if ((i + 1) % total_threads == 0) {
    //  //printf("\n");
    //  }
    //}
    //printf("\n");
    //printf("\n");
    //printf("\n");
  }
}
