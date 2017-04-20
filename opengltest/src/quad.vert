#version 330

in vec3 pos;
out vec2 coords;

void main() {
  coords = (pos.xy + vec2(1.0f, 1.0f)) / 2.0f;
  gl_Position = vec4(pos, 1.0f);
}

