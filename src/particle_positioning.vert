#version 330
in vec3 pos;

out vec3 eyespacePos;
out float eyespaceRadius;

uniform mat4 modelview;
uniform mat4 projection;
uniform vec2 screenSize;

float radius = 0.05f;

void main() {
  vec4 test_point = vec4(pos.xyz, 1.0f) + vec4(0, radius, 0, 0);
  test_point = modelview * test_point;
	vec4 eyespacePos4 = modelview * vec4(pos.xyz, 1.0f);

	eyespacePos = eyespacePos4.xyz;
	eyespaceRadius = length(test_point - eyespacePos4);

	vec4 clipspacePos = projection * eyespacePos4;
  gl_Position = clipspacePos;
  
  test_point = eyespacePos4 + vec4(0, eyespaceRadius, 0, 0);
  test_point = projection * test_point;
  test_point = test_point / test_point.w;
  clipspacePos = clipspacePos / clipspacePos.w;
 
  gl_PointSize = length(test_point - clipspacePos) * screenSize.y;
}
