#version 330

// Particle depth fragment shader

// Parameters from the vertex shader
in vec3 eyespacePos;
in float eyespaceRadius;
in float grid_assignment;

// Uniforms
uniform mat4 modelview;
uniform mat4 projection;
uniform vec2 screenSize;

// Textures
uniform sampler2D terrainTexture;

// Output
out vec4 vFragColor;

void main() {
	vec3 normal;

	// See where we are inside the point sprite
	normal.xy = (gl_PointCoord * 2.0f) - vec2(1.0);
	float dist = dot(normal.xy, normal.xy);
	
	// Outside sphere? Discard.
	if(dist > 1.0f) {
		discard;
	}

	// Set up rest of normal
	normal.z = sqrt(1.0f - dist);
	normal.y = -normal.y;
	normal = normalize(normal);

  // calculate lighting
  float diffuse = max(0.0f, dot(vec3(0, 0, 1.0f), normal));

  int grid = int(grid_assignment);

  //for testing neighbors
  //vFragColor = vec4(grid / 27000.0f, (grid % 900) / 900.0f, (grid % 30) / 30.0f, 1.0f) * diffuse;
  vFragColor = vec4(1.0f, 0, 0, 1.0f) * diffuse;

	// Calculate fragment position in eye space, project to find depth
	vec4 fragPos = vec4(eyespacePos + normal * eyespaceRadius, 1.0);
	vec4 clipspacePos = projection * fragPos;

	// Set up output
	float far = gl_DepthRange.far; 
	float near = gl_DepthRange.near;
	float deviceDepth = clipspacePos.z / clipspacePos.w;
	float fragDepth = (((far - near) * deviceDepth) + near +far) / 2.0;
	gl_FragDepth = fragDepth;

	//if(fragDepth > texture(terrainTexture, gl_FragCoord.xy / screenSize).w) {
	//	discard;
	//}
	//particleDepth = clipspacePos.z;
}
