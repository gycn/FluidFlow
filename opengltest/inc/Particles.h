#ifndef PARTICLES_H
#define PARTICLES_H
#include <glm/glm.hpp>

class Particles {
public:
    Particles();
private:
    struct Particle
    {   
        glm::vec3 p;
        glm::vec3 v;
    };  
    
    std::vector<Particle> particles;
};

#endif
