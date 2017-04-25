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

struct LiquidShader {
  GLuint shader_program;
  GLint view_location;
  GLint projection_location;
  GLint screen_size_location;
  GLint particle_depths_location;
  GLint thickness_location;
  GLint fluid_color_location;
  GLint highlight_color_location;
  GLint lightdir_location;
  GLint background_location;
};
ParticleShader particle_shader;
DepthShader depth_shader;
SmoothingShader smoothing_shader;
ThicknessShader thickness_shader;
LiquidShader liquid_shader;
#endif
