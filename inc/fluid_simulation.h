#ifndef FLUID_SIMULATION_H_
#define FLUID_SIMULATION_H_

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
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
  int num_per_side;
  int num_points;
  int num_vertices;
  float spacing;
  float radius;

  float mass;
  float h = 4.0f;
  float density = 1.0f;

  //Neighbor grid parameters
  int num_grids_per_side;
  float x_start, y_start, z_start;
  int cells_per_side;
  int num_per_cell;
  float grid_width;
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
  int threads_per_block = 2;
  int num_per_thread = 2;
  int total_threads;
  int splits = 8;

  int radix_size = 4;
  int sort_passes = 16;
  int radix_bits = 2;
  cl_uint radix_mask = 0b11;

  cl_int * negative_ones;

  void init_params(int nps, float s, float gw, 
                   float xstrt, float ystrt, float zstrt,
                   int gps, int npc) {
    num_per_side = nps;
    spacing = s;
    radius = s/2.0f;
    mass = density * 4.0f * radius * radius * radius * 3.141592 / 3.0f;
    num_points = num_per_side * num_per_side * num_per_side;
    num_vertices = 4 * num_points;

    printf("%f\n", mass);

    x_start = xstrt;
    y_start = ystrt;
    z_start = zstrt;
    
    cells_per_side = gps;
    num_per_cell = npc;
  
    grid_width = gw;
    vertices = (GLfloat*) malloc(num_vertices * sizeof(GLfloat));
    indices = (GLuint*) malloc(num_points * sizeof(GLuint));
    num_grids = cells_per_side * cells_per_side * cells_per_side;
    
    num_blocks = num_points / num_per_thread / threads_per_block;
    total_threads = num_blocks * threads_per_block;
  }

};


#endif
