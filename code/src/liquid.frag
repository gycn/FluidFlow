#version 330

in vec2 coords;

uniform sampler2D thickness;
uniform sampler2D surface;
uniform sampler2D background;

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
  
  float root = pow(1.0f - n1n22 * sinthetai2, 0.5f);

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
// Width of one pixel
	vec2 dx = vec2(1.0f / screenSize.x, 0.0f);
	vec2 dy = vec2(0.0f, 1.0f / screenSize.y);

	// Central z
	float zc =  texture(surface, pos).r;

	// Derivatives of z
	// For shading, one-sided only-the-one-that-works version
	float zdxp = texture(surface, pos + dx).r - zc;
	float zdxn = zc - texture(surface, pos - dx).r;
  float zdx = (abs(zdxp) < abs(zdxn)) ? zdxp : zdxn; 

	float zdyp = texture(surface, pos + dy).r - zc;
	float zdyn = zc - texture(surface, pos - dy).r;
  float zdy = (abs(zdyp) < abs(zdyn)) ? zdyp : zdyn; 

	// Projection inversion
	float cx = 2.0f / (screenSize.x * -projection[0][0]);
	float cy = 2.0f / (screenSize.y * -projection[1][1]);

	// Screenspace coordinates
	float sx = floor(pos.x * (screenSize.x - 1.0f));
	float sy = floor(pos.y * (screenSize.y - 1.0f));
	float wx = (screenSize.x - 2.0f * sx) / (screenSize.x * projection[0][0]);
	float wy = (screenSize.y - 2.0f * sy) / (screenSize.y * projection[1][1]);

	// Eyespace position derivatives
	vec3 pdx = normalize(vec3(cx * zc + wx * zdx, wy * zdx, zdx));
	vec3 pdy = normalize(vec3(wx * zdy, cy * zc + wy * zdy, zdy));

	return normalize(cross(pdx, pdy));
}

void main() {
  float depth = texture(surface, coords).r;
  float Wx = (2.0f * coords.x - 1.0f) * (-projection[0][0]);
  float Wy = (2.0f * coords.y - 1.0f) * (-projection[1][1]);
  vec3 eyespacePos = vec3(Wx, Wy, 1.0f);
  eyespacePos *= depth;
  eyespacePos = normalize(eyespacePos);

	vec3 n = normal(coords);
	
  vec3 worldCameraVec = (inverse(modelview) * vec4(-eyespacePos, 1.0f)).xyz;
  vec3 worldnormal = (mat3(inverse(modelview)) * n);
  worldnormal = normalize(worldnormal);
	worldnormal.xz = - worldnormal.xz;
  worldCameraVec = normalize(worldCameraVec);
	worldCameraVec.xz = -worldCameraVec.xz;

  float normalcameraangle = dot(worldnormal, worldCameraVec);

  vec3 h = 0.5f * (worldCameraVec + lightdir);
  
  float normalhangle = dot(h, worldnormal);

  float thickness = max(texture(thickness, coords).r * 0.5f, 1.0f) * 0.5f;
  
  normalcameraangle = max(smoothstep(0.95, 1.0, abs(normalcameraangle)), 0.05) * 1.5f + 0.2f;
  
  float offset = 1.0f - fresnel(normalcameraangle, 1.0f, 1.5f); 
  vec3 colorxyz =a(fluid_color, texture(background, vec2(1.0f) - coords).xyz, thickness) * offset + highlight_color * pow(normalhangle, 2.0f) + vec3(0.1f); 
  if (depth == 0) colorxyz = texture(background, vec2(1.0f) - coords).xyz;
 
  color = vec4(colorxyz, 1.0f);
}

