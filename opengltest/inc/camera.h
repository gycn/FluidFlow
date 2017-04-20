#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm.hpp>
#include <math.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/rotate_vector.hpp>
const double PI = std::acos(-1);
class Camera {
  public:
    Camera(glm::vec3 pos, glm::vec3 t, glm::vec3 u, int width, int height): position(pos), up(u), target(t), dist(1.0f) {
      look_at = glm::lookAt(position, target, up); 
      prsp = glm::perspective(45.0f, (float)width/(float)height, 0.1f, 100.0f);
    }
    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 target;

    float theta;
    float phi;
    float dist;
    double x;
    double y;
    
    void update_position(double mx, double my);

    glm::mat4 look_at;
    glm::mat4 prsp;
};

#endif
