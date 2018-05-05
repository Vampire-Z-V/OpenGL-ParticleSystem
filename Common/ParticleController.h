#pragma once
#include "Particle.h"

class ParticleController
{
public:
	ParticleController(unsigned int particleNum, unsigned int onceInitNum = 0)
		: m_ParticleNum(particleNum), m_OnceInitNum(onceInitNum)
	{
		if (m_OnceInitNum == 0 || m_OnceInitNum > m_ParticleNum)
			m_OnceInitNum = m_ParticleNum;
	}

	virtual void init(Particle *p) = 0;
	virtual void update(Particle *p) {};

protected:
	unsigned int m_ParticleNum;
	unsigned int m_OnceInitNum;
};

