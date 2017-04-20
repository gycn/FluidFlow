#ifndef SHADERS_H_
#define SHADERS_H_

#include <glad.h>

struct ParticleShader {
  GLuint shader_program;
  GLint view_location;
  GLint projection_location;
  GLint screen_size_location;
  GLint pos_location;
};

struct DepthShader {
  GLuint shader_program;
  GLint view_location;
  GLint projection_location;
  GLint screen_size_location;
  GLint pos_location;
};

struct SmoothingShader {
  GLuint shader_program;
  GLint projection_location;
  GLint screen_size_location;
  GLint particle_depths_location;
  GLint pos_location;
};

struct ThicknessShader {
  GLuint shader_program;
  GLint view_location;
  GLint projection_location;
  GLint screen_size_location;
  GLint pos_location;
};
#endif
