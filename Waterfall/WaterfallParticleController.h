#ifndef WATERFALLPARTICLECONTROLLER_H
#define WATERFALLPARTICLECONTROLLER_H

#define TOP  2.f
#define BOTTOM  -4.f
#define CLIFF  2.f
#define LEDGE  -2.f
#define GRAV  -32.f

#ifdef __cplusplus

#include "ParticleController.h"
#include "Rand.h"

class WaterfallParticleController :
	public ParticleController
{
public:
	WaterfallParticleController(unsigned int particleNum, unsigned int onceInitNum = 0)
		: ParticleController(particleNum, onceInitNum) {}

	void init(Particle *p)
	{
		for (int i = 0; i < m_ParticleNum; i++)
		{
			Particle *particle = p + i;
			particle->position = glm::vec4(tweak(.5f), TOP + tweak(.1f), 0.f, 1.f);
			particle->velocity = glm::vec4(3.f + tweak(.3f), 0.f, 0.f, 0.f);
			particle->color = glm::vec4(.05f, .32f, .72f, 1.f);
			particle->lifetime = 2 + i % m_OnceInitNum;
		}
	}
};

#else
void init(uint i, float dt)
{
	vec2 s = p[i].position.xy * dt;
	p[i].position = vec4(tweak(.5f, s), TOP + tweak(.1f, s * dt), 0.f, 1.f);
	p[i].velocity = vec4(3.f + tweak(.3f, s), 0.f, 0.f, 0.f);
	p[i].lifetime = 2;
}

void update(uint i, float dt)
{
	vec3 pos = p[i].position.xyz;
	vec3 vel = p[i].velocity.xyz;
	int lifetime = p[i].lifetime;

	if (lifetime > 2)
	{
		p[i].lifetime--;
		return;
	}

	vec2 s = pos.xy  * dt;

	if (pos.y < BOTTOM)
	{
		init(i, dt);
		return;
	}
	else
	{	// is the particle still on top of the cliff?
		if (pos.x < CLIFF)
			pos.x += vel.x * dt;
		else
		{   
			// is the particle still above the ledge?
			if (pos.y > LEDGE)
			{
				pos.x += vel.x * dt;						// update horiz position
				vel.y += GRAV * dt * (1. - tweak(.1, s));   // update vert velocity
				pos.y += vel.y * dt;						// update vert position
			}
			else
			{   // particle has now hit the ledge; note this with the flag
				if (lifetime > 1)
				{
					lifetime = 0;					// set "past ledge" flag
					vel.x += tweak(.2, s);     // update horiz velocity
					vel.y *= -0.5 + tweak(.2, s * dt);   // reverse direction of vert velocity
				}
				pos.x += vel.x * dt; // update horiz position
				vel.y += GRAV * dt;  // update vert velocity
				pos.y += vel.y * dt; // update vert position
			}
		}
	}

	p[i].position = vec4(pos, 1.0f);
	p[i].velocity = vec4(vel, 0.0f);
	p[i].lifetime = lifetime;
}

#endif // __cplusplus
#endif // !WATERFALLPARTICLECONTROLLER_H


