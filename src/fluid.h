#include "vector3D.h"
#include "matrix3x3.h"

#include "CGL.h"
#include "Particle.h"
namespace CGL {

struct fluid {
    /**
     * Used in initialization.
     */
    
	Vector<*Particle> memberParticles;
	int phase;
	
    /**
     * Constructor.
     * Create a fluid instance with given parameters.
     * \param memberParticles is a list of all the particles in the fluid
     * \param phase of the particle
     */
    fluid(int p, Vector<*Particle>mb){
        phase = p;
        memberParticles = mb;
    }
};//end struct
    
}//end namespace
