#include "PumpEmitter.h"
//Cheap random number generator, will generate
//numbers between 0.0 and 1.0 to 2 DP
#define RAND() ((rand()%101)/100.0f)



PumpEmitter::PumpEmitter() : ParticleEmitter()
{
}


PumpEmitter::~PumpEmitter()
{
}

Particle * PumpEmitter::GetFreeParticle()
{
	Particle * p = NULL;

	//If we have a spare particle on the freelist, pop it off!
	if (freeList.size() > 0) {
		p = freeList.back();
		freeList.pop_back();
	}
	else {
		//no spare particles, so we need a new one
		p = new Particle();
	}

	//Now we have to reset its values - if it was popped off the
	//free list, it'll still have the values of its 'previous life'

	//p->colour = Vector4(RAND(), RAND(), RAND(), 1.0);
	p->colour = Vector4(0.8, 0.4, 0.5, 1.0);
	p->direction = initialDirection;
	//p->direction.x += ((RAND() - RAND()) * particleVariance);
	//p->direction.y += ((RAND() - RAND()) * particleVariance);
	//p->direction.z += ((RAND() - RAND()) * particleVariance);
	p->direction.x += ((RAND() - RAND()) * particleVariance);
	p->direction.y += (1.0f * particleVariance);
	p->direction.z += ((RAND() - RAND()) * particleVariance);

	p->direction.Normalise();	//Keep its direction normalised!
	p->position.ToZero();

	return p;	//return the new particle :-)
}
