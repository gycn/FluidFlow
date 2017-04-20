#version 330
in vec3 pos;

out vec3 eyespacePos;
out float eyespaceRadius;

uniform mat4 modelview;
uniform mat4 projection;
uniform vec2 screenSize;

void main() {
	vec4 eyespacePos4 = modelview * vec4(pos.xyz, 1.0f);
	eyespacePos = eyespacePos4.xyz;
	eyespaceRadius = 1.0f / (-eyespacePos.z * 4.0f * (1.0f / screenSize.y));
	vec4 clipspacePos = projection * eyespacePos4;
  gl_PointSize = eyespaceRadius * 0.18;
  gl_Position = clipspacePos;
}
