#ifndef FLUID_SIMULATION_H_
#define FLUID_SIMULATION_H_

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <cl.hpp>

#define num_per_side 20
#define num_points (num_per_side * num_per_side * num_per_side)
#define num_vertices  (num_points * 4)
#define spacing 1.0f

struct FluidSimulation {
  void checkErr(const char * name) {
    if (err != CL_SUCCESS) {
      std::cerr << "ERROR: " << name  << " (" << err << ")" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

	// Particle Initialization
	void init_test_particles() {
	  float center = (float) (num_per_side - 1.0f) * spacing / 2.0f;
	  int ind = 0;
	  for(int x=0; x < num_per_side; x++)
	  {
	      for(int y=0; y < num_per_side; y++)
	      {
	          for(int z=0; z < num_per_side; z++)
	          {   
	            vertices[ind] = (float) x * spacing - center;
	            ind++;
	            vertices[ind] = (float) y * spacing - center;
	            ind++;
	            vertices[ind] = (float) z * spacing - center;
	            ind++;
              vertices[ind] = 1.0f;
              ind++;
	            //printf("%f, %f, %f\n", fluid_simulation.vertices[ind - 3], fluid_simulation.vertices[ind - 2], fluid_simulation.vertices[ind - 1]);
	          }   
	      }   
	  }   
	 
	 for (int i = 0; i < num_per_side * num_per_side * num_per_side; i++) {
	   indices[i] = i;
	 }
	
	}
  
  void step() {
    //Apply gravity & update based on current velocity
    cl::CommandQueue queue(context, devices[0], 
        0, &err);
    checkErr("CommandQueue::CommandQueue()");
  
    err = step_kernel.setArg(0, cl_positions);
    checkErr("Kernel::setArg()");
    err = step_kernel.setArg(1, cl_new_positions);
    checkErr("Kernel::setArg()");
    err = step_kernel.setArg(2, cl_velocities);
    checkErr("Kernel::setArg()");
    err = step_kernel.setArg(3, cl_new_velocities);
    checkErr("Kernel::setArg()");
    err = step_kernel.setArg(4, num_points);
    checkErr("Kernel::setArg()");
  
    cl::Event event;
    err = queue.enqueueNDRangeKernel(step_kernel, 
        cl::NullRange, cl::NDRange(num_points), cl::NDRange(1, 1),
        NULL, &event);
    checkErr("ComamndQueue::enqueueNDRangeKernel()");  
    event.wait();
    
    //Update previous particle values to new values
    err = update_kernel.setArg(0, cl_positions);
    checkErr("Kernel::setArg()");
    err = update_kernel.setArg(1, cl_new_positions);
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



	cl_int err;
	cl::Context context;
	
	GLfloat vertices[num_vertices];
  GLuint indices[num_points];	

	//CL Buffers for particle data  
  cl::Buffer cl_previous_positions;
  cl::BufferGL cl_positions;
  cl::Buffer cl_new_positions;

  cl::Buffer cl_velocities;
  cl::Buffer cl_new_velocities;
  
  //CL Buffers for neighbors
  cl::Buffer cl_hash_values;
  cl::Buffer cl_grid_starts;
  cl::Buffer cl_sorted_particles;
 
  //CL objects
  std::vector<cl::Device> devices;
  cl::Program program;

  //CL kernels
  cl::Kernel step_kernel;
  cl::Kernel update_kernel;

  //Radix sort
  int num_zeros;
  int num_ones;
};


#endif
