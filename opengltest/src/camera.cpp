#include "camera.h"
inline float clip(const float& n, const float& lower, const float& upper)
{   
  return glm::max(lower, glm::min(n, upper));
}

void Camera::update_position(double mx, double my) {
	int dx = x - mx; 
  int dy = y - my; 
	x = mx;
	y = my;
  if(abs(dx) > abs(dy))
      theta += dx*0.1;
  else
      phi -= dy*0.1;
  if(theta > 2*PI)
      theta -= 2*PI;
  if(theta < -2*PI)
      theta += 2*PI;
  phi = clip(phi, M_PI/12, M_PI*11/12);
	position.x = dist*sin(phi)*cos(theta);
	position.z = dist*sin(phi)*sin(theta);
	position.y = dist*cos(phi); 
  look_at = glm::lookAt(position, target, up);
}

