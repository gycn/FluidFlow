#version 330

in vec2 coords;

uniform sampler2D thickness;
uniform sampler2D surface;

uniform mat4 projection;
uniform mat4 modelview;

uniform vec3 fluid_color;
uniform vec3 highlight_color;
uniform vec3 lightdir;
uniform vec2 screenSize;

vec3 ks = vec3(0.1f, 0.1f, 0.1f);
vec3 lightdir = vec3(0, 0, -1.0f);

float fresnel(float costhetai, float n1, float n2) {
  float costhetai2 = costhetai * costhetai;
  float sinthetai2 = 1.0f - costhetai2;
  
  float n1n2 = n1 / n2;
  float n1n22 = n1n2 * n1n2;
  
  float root = pow(1.0f - n1n22 * sinthetai2, 0.5);

  float Rs = (n1 * costhetai - n2 * root) / (n1 * costhetai + n2 * root);
  Rs = Rs * Rs;

  float Rp = (n1 * root - n2 * costhetai) / (n1 * root + n2 * costhetai);
  Rp = Rp * Rp;
  
  return 0.5f * (Rs + Rp);
}

float a(vec3 fluidcolor, vec3 scenecolor, float thickness) {
  float t = exp(-thickness);
  return (1.0f - t) * fluidcolor + t * scenecolor;
}

vec3 normal(float z) {

  float Cx = 1.0f / screenSize.x / -projection[0][0];
  float Cy = 1.0f / screenSize.y / -projection[1][1];

  vec2 dx = vec2(1.0f / screenSize.x, 0.0f);
  vec2 dy = vec2(0.0f, 1.0f / screenSize.y);
	
	//First Order Finite Differences
 	float zdxp = texture(particleTexture, pos + dx);
	float zdxn = texture(particleTexture, pos - dx);
	float zdx = (zdxp == 0.0f) ? (zdxn == 0.0f ? 0.0f : (zc - zdxn)) : (zdxp - zc);

	float zdyp = texture(particleTexture, pos + dy);
	float zdyn = texture(particleTexture, pos - dy);
	float zdy = (zdyp == 0.0f) ? (zdyn == 0.0f ? 0.0f : (zc - zdyn)) : (zdyp - zc); 

	float D = Cy * Cy * zdx * zdx + Cx * Cx * zdy * zdy + Cx * Cx * Cy * Cy * z * z;
	
	return vec3(-Cy * zdx, -Cx * zdy, Cx * Cy * z) / pow(D, 0.5f);
}

void main() {
  float depth = texture(surface, coords).r;
  vec3 eyespacePos(((2.0f * coords.x) - 1.0) / (0.5f * projection[0][0]),
                   ((2.0f * coords.y) - 1.0) / (0.5f * projection[1][1]),
                   1.0f);
  eyespacePos *= depth;
 	

	vec3 n = normal(depth);
	
  float normalcameraangle = dot(n, -eyespacePos);
  
  vec3 worldCameraVec = (inverse(modelview) * vec4(-eyespacePos, 1.0f)).xyz;
  vec3 worldnormal = (inverse(modelview) * vec4(normal, 1.0f)).xyz;

  vec3 h = 0.5f * (worldCameraVec + lightdir);
  
  float normalhangle = dot(h, worldnormal);

  float thickness = texture(thickness, coords).r;

  return a(fluid_color, vec3(0, 0, 0), thickness) * (1.0f - fresnel(normalcameraangle, 1.0f, 1.5f)) 
       + highlight_color * pow(normalhangle, 3.0f);
}

