#ifndef FLUID_SIMULATION_H_
#define FLUID_SIMULATION_H_

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <cmath>
#include <OpenCL/cl.hpp>

class FluidSimulation {
  public:
  void checkErr(const char * name) {
    if (err != CL_SUCCESS) {
      std::cerr << "ERROR: " << name  << " (" << err << ")" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

	// Particle Initialization
	void init_test_particles() {
    vertices = (GLfloat*) malloc((num_vertices + 256) * sizeof(GLfloat));
    indices = (GLuint*) malloc((num_points +  64)* sizeof(GLuint));
	  float center = (float) (num_per_side - 1.0f) * spacing / 2.0f;
	  int ind = 0;
    float offset = 0.01;
    float xoff = 0.01f;
	  for(int y=0; y < num_per_side - 4; y++)
	  {
	      for(int x=0; x < num_per_side; x++)
	      {
	          for(int z=0; z < num_per_side; z++)
	          {   
	            vertices[ind] = (float) x * spacing - center + xoff;
	            ind++;
	            vertices[ind] = (float) y * spacing - center;
	            ind++;
	            vertices[ind] = (float) z * spacing - center + offset;
	            ind++;
              vertices[ind] = 1.0f;
              ind++;
	            printf("%f, %f, %f\n", vertices[ind - 4], vertices[ind - 3], vertices[ind - 2]);
	          }   
       }   
	  }   
    
	  center = (float) (7.0f) * spacing / 2.0f;
	  for(int x=0; x < 16; x++)
	  {
	      for(int y=0; y < 16; y++)
	      {
	          for(int z=0; z < 16; z++)
	          {   
	            vertices[ind] = (float) x * spacing - center;
	            ind++;
	            vertices[ind] = (float) y * spacing - center + 3.0f;
	            ind++;
	            vertices[ind] = (float) z * spacing - center;
	            ind++;
              vertices[ind] = 1.0f;
              ind++;
	            printf("%f, %f, %f\n", vertices[ind - 4], vertices[ind - 3], vertices[ind - 2]);
	          }   
	      }   
	  }   
	 for (int i = 0; i < num_per_side * num_per_side * num_per_side; i++) {
	   indices[i] = i;
	 }
	
	}

  void step();
  void sort();
  void init_CL(GLuint vertex_VBO);

  //Particle parameters
  int num_per_side = 32;
  int num_points = 32768;
  int num_vertices = 131072;
  float spacing;
  float radius = 0.05f;

  float mass = 1.0f;
  float h = 0.2f;
  float density = 700.0f;

  //Neighbor grid parameters
  float x_start = -5.0f, y_start = -5.0f, z_start = -5.0f;
 int cells_per_side = 50;
  float grid_width = 0.20f;
  int num_grids;

  //CL stuff
	cl_int err;
	cl::Context context;
  cl::CommandQueue queue;
  cl::Event event;
  
  
  //GL Buffers for particle positions
	GLfloat * vertices;
  GLuint * indices;	

	//CL Buffers for particle data  
  cl::BufferGL cl_positions;
  cl::Buffer cl_new_positions[2];

  cl::Buffer cl_velocities;
  cl::Buffer cl_new_velocities;

  cl::Buffer cl_lambdas;

  cl::Buffer cl_grid_starts;

  //CL objects
  std::vector<cl::Device> devices;
  cl::Program program;
  cl::Program sort_program;

  //CL kernels
  cl::Kernel step_kernel;
  cl::Kernel density_lambda_kernel;
  cl::Kernel lambda_pos_kernel;
  cl::Kernel update_kernel;
  cl::Kernel collision_kernel;
  cl::Kernel hash_kernel;
  cl::Kernel grid_kernel;
  cl::Kernel velocity_kernel;

  //Sorting
  cl::Kernel hist_kernel;
  cl::Kernel scan_kernel;
  cl::Kernel sum_kernel;
  cl::Kernel offset_kernel;
  cl::Kernel reorder_kernel;

  cl::Buffer cl_hist;
  cl::Buffer cl_sums;
  cl::Buffer cl_offsets;
  cl::Buffer cl_offset_sums;

  cl::Buffer cl_hashes[2];
  cl::Buffer cl_indices[2];

  cl::LocalSpaceArg local_hist;
  cl::LocalSpaceArg local_scan_sums;
  cl::LocalSpaceArg local_reorder_sums;

  int num_blocks;
  int threads_per_block = 256;
  int num_per_thread = 4;
  int total_threads;
  int splits = 128;

  int radix_size = 4;
  int sort_passes = 16;
  int radix_bits = 2;
  cl_uint radix_mask = 0b11;

  cl_int * negative_ones;
  cl_uint * inds;

  void init_params(int nps, float s, float gw, 
                   float xstrt, float ystrt, float zstrt,
                   int gps, int npc) {
    spacing = radius * 2.0f;

    printf("%f\n", mass);
    
    num_grids = cells_per_side * cells_per_side * cells_per_side;
    
    num_blocks = num_points / num_per_thread / threads_per_block;
    total_threads = num_blocks * threads_per_block;
  }

};


#endif
