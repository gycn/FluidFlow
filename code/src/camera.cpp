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
  if (phi > PI) {
    phi -= PI;
  } else if (phi < - PI) {
    phi += PI;
  }
  glm::mat4 rotx = glm::rotate(theta, glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 roty = glm::rotate(phi, glm::vec3(1.0f, 0.0f, 0.0f));
  look_at = glm::lookAt(position, target, up) * rotx * roty;
}

