#include <iostream>
#include <math.h>
#include <cl.hpp>

#include <glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "buffers.h"
#include "shaders.h"
#include "GLShader.hpp"
#include "camera.h"
// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
bool pressed = false;
int num_per_side = 10;
float spacing = 0.1f;
int smoothing_iterations = 40; 
cl_int cl_err;

GLfloat vertices[8000];
GLuint vertex_indices[8000];

GLFWwindow* window;
Camera camera(glm::vec3(0.0f,1.0f, 2.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1.0f, 0), WIDTH, HEIGHT);
GLuint VAO;

ParticleShader particle_shader;
DepthShader depth_shader;
SmoothingShader smoothing_shader;
ThicknessShader thickness_shader;
LiquidShader liquid_shader;

VertexBuffer vertex_buffer;
DepthBuffer depth_buffer;
ScreenQuadBuffer screen_quad_buffer;
ThicknessBuffer thickness_buffer;

void init();
void draw();
void update_mouse_events();

//Callback declarations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);

// Particle Initialization
void init_test_particles() {
  float center = (float) (num_per_side - 1.0f) * spacing / 2.0f;
  float d = 0.05;
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
            //printf("%f, %f, %f\n", vertices[ind - 3], vertices[ind - 2], vertices[ind - 1]);
          }   
      }   
  }   
 
 for (int i = 0; i < num_per_side * num_per_side * num_per_side; i++) {
   vertex_indices[i] = i;
 }

}

inline void checkCLErr(cl_int err, const char * name) {
  if (err != CL_SUCCESS) {
    std::cerr << "ERROR: " << name  << " (" << err << ")" << std::endl;
    exit(EXIT_FAILURE);
  }
}

int main()
{
  std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
  // Init GLFW
  glfwInit();
  // Set all the required options for GLFW
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  // Create a GLFWwindow object that we can use for GLFW's functions
  window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);    
  if (window == nullptr)
  {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
  }
  glfwMakeContextCurrent(window);

	//callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	
	//OpenGL extension loader	
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
  
  //Init OpenCL
  cl_int err;
  std::vector< cl::Platform > platformList;
  cl::Platform::get(&platformList);
  checkCLErr(platformList.size()!=0 ? CL_SUCCESS : -1, "cl::Platform::get");

  std::cerr << "Platform number is: " << platformList.size() << std::endl;std::string platformVendor;
  platformList[0].getInfo((cl_platform_info)CL_PLATFORM_VENDOR, &platformVendor);
  std::cerr << "Platform is by: " << platformVendor << "\n";

  cl_context_properties cprops[3] = {CL_CONTEXT_PLATFORM, (cl_context_properties)(platformList[0])(), 0};
  
  cl::Context context(CL_DEVICE_TYPE_CPU, cprops, NULL, NULL, &err);
  checkCLErr(err, "Context::Context()"); 
  
  //CL test
  std::string hw("Hello World\n");

  char * outH = new char[hw.length()+1];
  cl::Buffer outCL(context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, hw.length()+1, outH, &err);
  checkCLErr(err, "Buffer::Buffer()");


  cl::vector<cl::Device> devices;
  devices = context.getInfo<CL_CONTEXT_DEVICES>();
  checkErr(devices.size() > 0 ? CL_SUCCESS : -1, "devices.size() > 0");

  //Init OpenGL objects 
  init();

  glClearDepth(1.0f);
  glEnable( GL_PROGRAM_POINT_SIZE );

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    update_mouse_events();
    draw(); 
  }

  // Terminate GLFW, clearing any resources allocated by GLFW.
  glfwTerminate();
  return 0;
}

void init_shaders() {
	glm::vec2 screensize; 
  screensize.x = depth_buffer.width;
  screensize.y = depth_buffer.height;

  glm::vec3 lightdir(0.0f, -1.0f, 0.0f);
  glm::vec3 fluidcolor(0.01f, 0.2f, 0.8f);
  glm::vec3 highlightcolor(.01f, .01f, .01f);

  //Point Sprite shader
  particle_shader.shader_program = LoadShader("../src/particle_positioning.vert", "../src/point_sprite_sphere.frag");
	particle_shader.view_location = glGetUniformLocation(particle_shader.shader_program, "modelview");
	particle_shader.projection_location = glGetUniformLocation(particle_shader.shader_program, "projection");
	particle_shader.screen_size_location = glGetUniformLocation(particle_shader.shader_program, "screenSize");
  particle_shader.pos_location = glGetAttribLocation(particle_shader.shader_program, "pos");

	glUseProgram(particle_shader.shader_program);

  glUniformMatrix4fv(particle_shader.view_location, 1, GL_FALSE, glm::value_ptr(camera.look_at));
	glUniformMatrix4fv(particle_shader.projection_location, 1, GL_FALSE, glm::value_ptr(camera.prsp));
  glUniform2fv(particle_shader.screen_size_location, 1, glm::value_ptr(screensize));
  
  //Depth Shader
  depth_shader.shader_program = LoadShader("../src/particle_positioning.vert", "../src/particle_depth.frag");
	depth_shader.view_location = glGetUniformLocation(depth_shader.shader_program, "modelview");
	depth_shader.projection_location = glGetUniformLocation(depth_shader.shader_program, "projection");
	depth_shader.screen_size_location = glGetUniformLocation(depth_shader.shader_program, "screenSize");
  depth_shader.pos_location = glGetAttribLocation(depth_shader.shader_program, "pos");

	glUseProgram(depth_shader.shader_program);

  glUniformMatrix4fv(depth_shader.view_location, 1, GL_FALSE, glm::value_ptr(camera.look_at));
	glUniformMatrix4fv(depth_shader.projection_location, 1, GL_FALSE, glm::value_ptr(camera.prsp));
  glUniform2fv(depth_shader.screen_size_location, 1, glm::value_ptr(screensize));

  glBindFragDataLocation(depth_shader.shader_program, 0, "particleDepth");

  //Smoothing Shader
  smoothing_shader.shader_program = LoadShader("../src/quad.vert", "../src/curvature_smoothing.frag");
	smoothing_shader.projection_location = glGetUniformLocation(smoothing_shader.shader_program, "projection");
	smoothing_shader.screen_size_location = glGetUniformLocation(smoothing_shader.shader_program, "screenSize");
  smoothing_shader.particle_depths_location = glGetUniformLocation(smoothing_shader.shader_program, "particle_depths");
  smoothing_shader.pos_location = glGetAttribLocation(smoothing_shader.shader_program, "pos");

	glUseProgram(smoothing_shader.shader_program);

	glUniformMatrix4fv(smoothing_shader.projection_location, 1, GL_FALSE, glm::value_ptr(camera.prsp));
  glUniform2fv(smoothing_shader.screen_size_location, 1, glm::value_ptr(screensize));

  glBindFragDataLocation(smoothing_shader.shader_program, 0, "smoothedDepth");

  //Thickness Shader
  thickness_shader.shader_program = LoadShader("../src/particle_positioning.vert", "../src/thickness.frag");
	thickness_shader.view_location = glGetUniformLocation(thickness_shader.shader_program, "modelview");
	thickness_shader.projection_location = glGetUniformLocation(thickness_shader.shader_program, "projection");
	thickness_shader.screen_size_location = glGetUniformLocation(thickness_shader.shader_program, "screenSize");
  thickness_shader.pos_location = glGetAttribLocation(thickness_shader.shader_program, "pos");

	glUseProgram(thickness_shader.shader_program);

	glUniformMatrix4fv(thickness_shader.projection_location, 1, GL_FALSE, glm::value_ptr(camera.prsp));
  glUniform2fv(thickness_shader.screen_size_location, 1, glm::value_ptr(screensize));

  glBindFragDataLocation(thickness_shader.shader_program, 0, "particle_width");

  //Liquid Shader
  liquid_shader.shader_program = LoadShader("../src/quad.vert", "../src/liquid.frag");
	liquid_shader.view_location = glGetUniformLocation(liquid_shader.shader_program, "modelview");
	liquid_shader.projection_location = glGetUniformLocation(liquid_shader.shader_program, "projection");
	liquid_shader.screen_size_location = glGetUniformLocation(liquid_shader.shader_program, "screenSize");
  liquid_shader.particle_depths_location = glGetUniformLocation(liquid_shader.shader_program, "surface");
  liquid_shader.thickness_location = glGetUniformLocation(liquid_shader.shader_program, "thickness");
  liquid_shader.fluid_color_location = glGetUniformLocation(liquid_shader.shader_program, "fluid_color");
  liquid_shader.highlight_color_location = glGetUniformLocation(liquid_shader.shader_program, "highlight_color");
  liquid_shader.lightdir_location = glGetUniformLocation(liquid_shader.shader_program, "lightdir");

	glUseProgram(liquid_shader.shader_program);

  glUniformMatrix4fv(depth_shader.view_location, 1, GL_FALSE, glm::value_ptr(camera.look_at));
	glUniformMatrix4fv(liquid_shader.projection_location, 1, GL_FALSE, glm::value_ptr(camera.prsp));
  glUniform3fv(liquid_shader.fluid_color_location, 1, glm::value_ptr(fluidcolor));
  glUniform3fv(liquid_shader.highlight_color_location, 1, glm::value_ptr(highlightcolor));
  glUniform3fv(liquid_shader.lightdir_location, 1, glm::value_ptr(lightdir));
  glUniform2fv(liquid_shader.screen_size_location, 1, glm::value_ptr(screensize));
}

void init_buffers() {
	//Set viewport
	int width, height;
  glfwGetFramebufferSize(window, &width, &height);  
  glViewport(0, 0, width, height);
  //Vertex Buffer
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &vertex_buffer.VBO);
	glGenBuffers(1, &vertex_buffer.EBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STREAM_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
  
  //Depth Buffer
  depth_buffer.width = width;
  depth_buffer.height = height;
  glGenRenderbuffers(1, &depth_buffer.render_buffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer.render_buffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, depth_buffer.width, depth_buffer.height);
  for (int i = 0; i < 2; i++) {

    glGenTextures(1, &depth_buffer.textures[i]);
    glBindTexture(GL_TEXTURE_2D, depth_buffer.textures[i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, depth_buffer.width, depth_buffer.height, 0, GL_RED, GL_FLOAT, 0);

    glGenFramebuffers(1, &depth_buffer.frame_buffers[i]);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_buffer.frame_buffers[i]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, depth_buffer.textures[i], 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer.render_buffer);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
      printf("could not create framebuffer");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  
  //Thickness Buffer
  thickness_buffer.width = width;
  thickness_buffer.height = height;

  glGenRenderbuffers(1, &thickness_buffer.render_buffer);
  glBindRenderbuffer(GL_RENDERBUFFER, thickness_buffer.render_buffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, thickness_buffer.width, thickness_buffer.height);

  glGenTextures(1, &thickness_buffer.texture);
  glBindTexture(GL_TEXTURE_2D, thickness_buffer.texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, thickness_buffer.width, thickness_buffer.height, 0, GL_RED, GL_FLOAT, 0);

  glGenFramebuffers(1, &thickness_buffer.frame_buffer);
  glBindFramebuffer(GL_FRAMEBUFFER, thickness_buffer.frame_buffer);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, thickness_buffer.texture, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, thickness_buffer.render_buffer);

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
  {
    printf("could not create framebuffer");
  }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //Screen Quad Buffer
  glGenBuffers(1, &screen_quad_buffer.VBO);
  glGenBuffers(1, &screen_quad_buffer.EBO);

  glBindBuffer(GL_ARRAY_BUFFER, screen_quad_buffer.VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(screen_quad_vertices), screen_quad_vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_quad_buffer.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(screen_quad_indices), screen_quad_indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

void init() {
  init_test_particles();
  init_buffers();
  init_shaders();
}

void update_mouse_events() {
  if (pressed) {
    double mx, my;
    glfwGetCursorPos(window, &mx, &my );
    camera.update_position(mx, my);
  }
}

void draw() {
  glBindVertexArray(VAO);
  //Get Surface
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  glUseProgram(depth_shader.shader_program);
  glUniformMatrix4fv(depth_shader.view_location, 1, GL_FALSE, glm::value_ptr(camera.look_at));
 
  //Bind Vertex Buffers
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer.VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
	glVertexAttribPointer(depth_shader.pos_location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);	
	glEnableVertexAttribArray(depth_shader.pos_location);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer.EBO);

  glBindFramebuffer(GL_FRAMEBUFFER, depth_buffer.frame_buffers[0]);
  //Clear framebuffer depth
  GLfloat depth1 = 1.0f;
  glClearBufferfv(GL_DEPTH, 0, &depth1);
  //Clear framebuffer values 
  GLfloat depth0 = 0.0f;
  glClearBufferfv(GL_COLOR, 0, &depth0);

  glDrawElements(GL_POINTS, num_per_side * num_per_side * num_per_side, GL_UNSIGNED_INT, (void *) 0);
  
  //Clearing Parameters
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_DEPTH_TEST);
  //End Surface 

  //Thickness
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE); 
  
  glUseProgram(thickness_shader.shader_program);
  glUniformMatrix4fv(thickness_shader.view_location, 1, GL_FALSE, glm::value_ptr(camera.look_at));

  glBindFramebuffer(GL_FRAMEBUFFER, thickness_buffer.frame_buffer);
  //clear framebuffer values
  GLfloat thick0 = 0.0f;
  glClearBufferfv(GL_COLOR, 0, &thick0);

  GLfloat thick1 = 1.0f;
  glClearBufferfv(GL_DEPTH, 0, &thick1);

  glDrawElements(GL_POINTS, num_per_side * num_per_side * num_per_side, GL_UNSIGNED_INT, (void *) 0);
  
  //Clearing Parameters 
  glDisable(GL_BLEND);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //End Thickness
  
  //Curvature Smoothing
  //*
  glUseProgram(smoothing_shader.shader_program);

  glBindBuffer(GL_ARRAY_BUFFER, screen_quad_buffer.VBO);
  glVertexAttribPointer(smoothing_shader.pos_location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
  glEnableVertexAttribArray(smoothing_shader.pos_location);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_quad_buffer.EBO);

  glActiveTexture(GL_TEXTURE0);
  glUniform1i(smoothing_shader.particle_depths_location, 0);
  int ind = 0;
  for (int i = 0; i < smoothing_iterations; i++) {
    glBindFramebuffer(GL_FRAMEBUFFER, depth_buffer.frame_buffers[1 - ind]);
    glBindTexture(GL_TEXTURE_2D, depth_buffer.textures[ind]);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *) 0);
    ind = 1 - ind;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, depth_buffer.textures[0]);
  glUniform1i(smoothing_shader.particle_depths_location, 0);
  
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *) 0);
  
  //*/

  //Render liquid quad
  //*
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(liquid_shader.shader_program);
  glUniformMatrix4fv(liquid_shader.view_location, 1, GL_FALSE, glm::value_ptr(camera.look_at));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, depth_buffer.textures[ind]);
  glUniform1i(liquid_shader.particle_depths_location, 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, thickness_buffer.texture);
  glUniform1i(liquid_shader.thickness_location, 1);
   
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *) 0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  //*/
  //Sprite Shader
  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //glEnable(GL_DEPTH_TEST);
  //
  //glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //glUseProgram(particle_shader.shader_program);

	//glUniformMatrix4fv(particle_shader.view_location, 1, GL_FALSE, glm::value_ptr(camera.look_at));

  //glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer.VBO);
  //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
	//glVertexAttribPointer(particle_shader.pos_location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);	
	//glEnableVertexAttribArray(particle_shader.pos_location);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer.EBO);

  //glDrawArrays(GL_POINTS,0,num_per_side * num_per_side * num_per_side);

  //glBindVertexArray(0);
  glfwSwapBuffers(window);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    pressed = true;
    glfwGetCursorPos(window, &camera.x, &camera.y );
    
  } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    pressed = false;
  }
}

