#ifndef BUFFER_H_
#define BUFFER_H_		

#include <glad.h>

struct VertexBuffer {
  GLuint VBO, EBO;
};

struct DepthBuffer {
  GLuint textures[2];
  GLuint frame_buffers[2];
  GLuint render_buffer;
  int width;
  int height;
};

struct ScreenQuadBuffer {
	GLuint VBO, EBO;	
};

struct ThicknessBuffer {
  GLuint texture;
  GLuint frame_buffer;
  GLuint render_buffer;
  int width, height;
};

GLfloat screen_quad_vertices[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f
	};

GLuint screen_quad_indices[] = {0, 1, 3, 1, 2, 3};

#endif
