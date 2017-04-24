#version 330

in float eyespaceRadius;
in vec3 eyespacePos;

uniform mat4 projection;
uniform vec2 screenSize;

out float particle_width;

void main() {
  vec3 normal;

  // See where we are inside the point sprite
  normal.xy = (gl_PointCoord * 2.0f) - vec2(1.0);
  float dist = dot(normal.xy, normal.xy);
  
  // Outside sphere? Discard.
  if(dist > 1.0f) {
    discard;
  }
	
	particle_width = 1.0f - length(normal);

	// Calculate fragment position in eye space, project to find depth
	vec4 fragPos = vec4(eyespacePos + normal * eyespaceRadius / screenSize.y, 1.0);
	vec4 clipspacePos = fragPos * projection;

	// Set up output
	float far = gl_DepthRange.far; 
	float near = gl_DepthRange.near;
	float deviceDepth = clipspacePos.z / clipspacePos.w;
	float fragDepth = (((far - near) * deviceDepth) + near + far) / 2.0;
	gl_FragDepth = fragDepth;

	//if(fragDepth > texture(terrainTexture, gl_FragCoord.xy / screenSize).w) {
	//	discard;
	//}
}
