#include "vector3D.h"
#include "matrix3x3.h"

#include "CGL.h"
#include "fluid.h"

namespace CGL {


struct Particle {
    /**
     * Used in initialization.
     */
    Vector<*ParticleConstraint> constraintP; ///constraints
    float p[3];///The initial positon of the particle.
    float v[3]; ///The initial velocity of a paritcle.
    int  phase;/// The phase, to determine what fluid this belongs to.
    float mass;///The initial mass of a paritcle.
    float density;/// The initial density of a paritcle.
    float w;/// The initial W of a paritcle
    float grad_w; ///The initial gradient of a paritcle.
    
    /**
     * Constructor.
     * Create a Particle instance with given parameters.
     * \param p position of the particle
     * \param v velocity of the particle
     * \param phase of the particle
     * \param mass of the particle
     * \param density of the particle
     * \param w of the particle
     * \param grad_w of the particle
     * \param constraints of the particle
     */
    Particle(const float pi[3], float vi[3], int p, float m, float d, float wi,  float grad_wi, Vector<*ParticleConstraint> constr)
    {
        p[3] = pi[3];
        v[3] = vi[3];
        phase = p;
        mass = m;
        density = d;
        w = wi;
        grad_w = grad_wi;
        constraintP = constr
       
    }
    /**
     * Takes in the neighbors as well are the forces
     * deletes all contraints 
     * applies the movement to the structure
     * then rebuilds the structure
     */
    void update(Vector<Particle>neighbors, Vector<vector3D> forces){};
    
    /**
     * Calculates the pressure of the structure 
     *
     */
    
    vector3D viscosity( Vector<Particle> neighbors) {};
    /**
     * Calculates the viscosity of the structure
     *
     */
    vector3D viscosity( Vector<Particle> neighbors) {};

    
    
};//struct particle
}// namespace CGL
    
