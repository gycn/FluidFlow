#version 330

in vec2 coords;

uniform mat4 projection;
uniform vec2 screenSize;

uniform sampler2D particle_depths;

out float smoothed_depth;

void main() {
  float depth = texture(particle_depths, coords).r;
  if (depth == 0.0f) {
    smoothed_depth = 0;
  } else {
    const float dt = 0.00055f;
    const float dzt = 1000.0f;

    vec2 dx = vec2(1.0f / screenSize.x, 0.0f);
    vec2 dy = vec2(0.0f, 1.0f / screenSize.y);

    //First Order Finite Differences
		float dzdxp = texture(particle_depths, coords + dx).r;
		float dzdxn = texture(particle_depths, coords - dx).r;
		float dzdx = 0.5f * (dzdxp - dzdxn);
		dzdx = (dzdxp == 0.0f || dzdxn == 0.0f) ? 0.0f : dzdx;

		float dzdyp = texture(particle_depths, coords + dy).r;
		float dzdyn = texture(particle_depths, coords - dy).r;
		float dzdy = 0.5f * (dzdyp - dzdyn);
		dzdy = (dzdyp == 0.0f || dzdyn == 0.0f) ? 0.0f : dzdy;

		
		//Second Order Finite Differencess
		float dzdx2 = dzdxp + dzdxn - 2.0f * depth;
		float dzdy2 = dzdyp + dzdyn - 2.0f * depth;

		// Second order finite differences, alternating variables
		float dzdxpdyp = texture(particle_depths, coords + dx + dy).r;
		float dzdxndyn = texture(particle_depths, coords - dx - dy).r;
		float dzdxpdyn = texture(particle_depths, coords + dx - dy).r;
		float dzdxndyp = texture(particle_depths, coords - dx + dy).r;
		float dzdxdy = (dzdxpdyp + dzdxndyn - dzdxpdyn - dzdxndyp) / 4.0f;

    float Cx = 1.0f / screenSize.x / -projection[0][0];
    float Cy = 1.0f / screenSize.y / -projection[1][1]; 
		
	  float dDdx = Cy * Cy * 2.0f * dzdx * dzdx2 + Cx * Cx * 2.0f * dzdy * dzdxdy 
      + Cx * Cx * Cy * Cy * 2.0f * depth * dzdx;
	  float dDdy = Cy * Cy * 2.0f * dzdx * dzdxdy + Cx * Cx * 2.0f * dzdy * dzdy2 
      + Cx * Cx * Cy * Cy * 2.0f * depth * dzdy;

    float D = Cy * Cy * dzdx * dzdx + Cx * Cx * dzdy * dzdy + Cx * Cx * Cy * Cy * depth * depth;

    float Ex = dzdx * dDdx / 2.0f - dzdx2 * D;
    float Ey = dzdy * dDdy / 2.0f - dzdy2 * D;

    float h = (Cy * Ex + Cx * Ey) / (2.0f * pow(D, 1.5f));

    smoothed_depth = depth + h * dt * (1.0f + (abs(dzdx) + abs(dzdy)) * dzt);
  }
}
