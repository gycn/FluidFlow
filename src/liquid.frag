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

out vec4 color;

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

vec3 a(vec3 fluidcolor, vec3 scenecolor, float thickness) {
  float t = exp(-thickness);
  return (1.0f - t) * fluidcolor + t * scenecolor;
}

vec3 normal(vec2 pos) {
	float z = texture(surface, pos).r;
	
  vec2 dx = vec2(1.0f / screenSize.x, 0.0f);
  vec2 dy = vec2(0.0f, 1.0f / screenSize.y);
	
  float invFx = 2.0f / projection[0][0];		
  float invFy = 2.0f / projection[1][1];

  float dzdxp = texture(surface, pos + dx).r;
  float dzdxn = texture(surface, pos - dx).r;
  float dzdx = (dzdxp == 0.0f) ? (dzdxn == 0.0f ? 0.0f : (z - dzdxn)) : (dzdxp - z);

  float dzdyp = texture(surface, pos + dy).r;
  float dzdyn = texture(surface, pos - dy).r;
  float dzdy = (dzdyp == 0.0f) ? (dzdyn == 0.0f ? 0.0f : (z - dzdyn)) : (dzdyp - z);
  
  float invFx2 = invFx * invFx;
  float invFy2 = invFy * invFy;

  vec3 unnormalized = vec3(-invFy * dzdx, -invFx * dzdy, z * invFy * invFx);

  float len = pow(invFy2 * dzdx * dzdx + invFx2 * dzdy * dzdy + z * z * invFx2 * invFy2, 0.5f);

  return unnormalized / len;
}

void main() {
  float depth = texture(surface, coords).r;
  //vec3 eyespacePos(((2.0f * coords.x) - 1.0) / (0.5f * projection[0][0]), ((2.0f * coords.y) - 1.0) / (0.5f * projection[1][1]), 1.0f);
  float Wx = (2.0f * coords.x - 1.0f) / (0.5f * projection[0][0]);
  float Wy = (2.0f * coords.y - 1.0f) / (0.5f * projection[0][0]);
  vec3 eyespacePos = vec3(Wx, Wy, 1.0f);
  eyespacePos *= depth;
  eyespacePos = normalize(eyespacePos);

	vec3 n = normal(coords);
	
  vec3 worldCameraVec = (inverse(modelview) * vec4(-eyespacePos, 1.0f)).xyz;
  vec3 worldnormal = (inverse(modelview) * vec4(n, 1.0f)).xyz;

  float normalcameraangle = dot(worldnormal, worldCameraVec);

  vec3 h = 0.5f * (worldCameraVec + lightdir);
  
  float normalhangle = dot(h, worldnormal);

  float thickness = texture(thickness, coords).r;
  
  vec3 colorxyz =a(fluid_color, vec3(0.05f), thickness) * (1.0f - fresnel(normalcameraangle, 1.0f, 1.5f)) + highlight_color * pow(normalhangle, 3.0f); 
  color = vec4(colorxyz, 1.0f);
  //color = vec4(normalcameraangle, 0, 0, 1.0f);
  //color = vec4(thickness, 0, 0, 1.0f);
}

