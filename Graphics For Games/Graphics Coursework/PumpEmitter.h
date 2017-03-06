#pragma once
#include "../../nclgl/ParticleEmitter.h"

class PumpEmitter : public ParticleEmitter
{
public:
	PumpEmitter();
	virtual ~PumpEmitter();

	virtual Particle* GetFreeParticle();
};

