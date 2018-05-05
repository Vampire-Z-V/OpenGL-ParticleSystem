#ifndef FIREWORKTYPE1PARTICLECONTROLLER_H
#define FIREWORKTYPE1PARTICLECONTROLLER_H

#define _PI  3.1415926535f
#define _2PI 6.2831853072f
#define BOTTOM_R 0.01f
#define LIFETIME_MAX 80
#define GRAV -9

#ifdef __cplusplus

#include "ParticleController.h"
#include "Rand.h"
#include <cmath>

class FireworkParticleController :
	public ParticleController
{
public:
	FireworkParticleController(unsigned int particleNum, unsigned int onceInitNum = 0)
		: ParticleController(particleNum, onceInitNum)
	{
	}

	void init(Particle *p)
	{
		int count = m_ParticleNum / m_OnceInitNum;
		for (int i = 0; i < m_ParticleNum; i++)
		{
			Particle *particle = p + i;
			float rand_pos = frand();
			float rand_vel_x = frand();
			float rand_vel_z = frand();

			particle->position = glm::vec4(
				sin(rand_pos * _2PI) * rand_vel_x * BOTTOM_R,
				.5f * rand_vel_x,
				cos(rand_pos * _2PI) * rand_vel_z * BOTTOM_R,
				1.f
			);

			float r = abs(particle->position.x) + abs(particle->position.y);
			particle->velocity = glm::vec4(
				sin(rand_pos * _2PI) * rand_vel_x,
				2.f + rand_vel_x + r / BOTTOM_R * .05f,
				cos(rand_pos * _2PI) * rand_vel_z,
				0.f
			);
			particle->color = glm::vec4(1.f, frand(), frand(), 1.f);
			particle->lifetime = rand_vel_x * LIFETIME_MAX + i % count * LIFETIME_MAX;
		}
	}
};

#else
void init(uint i, float dt)
{
	vec2 s = p[i].position.xy * dt;
	float rand_pos = frand(s);
	float rand_vel_x = frand(s*dt);
	float rand_vel_z = frand(s*dt*dt);

	p[i].position = vec4(
		sin(rand_pos * _2PI) * rand_vel_x * BOTTOM_R,
		.5f * rand_vel_x,
		cos(rand_pos * _2PI) * rand_vel_z * BOTTOM_R,
		1.f
	);

	float r = abs(p[i].position.x) + abs(p[i].position.y);
	p[i].velocity = vec4(
		sin(rand_pos * _2PI) * rand_vel_x,
		2.f + rand_vel_x + r / BOTTOM_R * .05f,
		cos(rand_pos * _2PI) * rand_vel_z,
		0.f
	);
	p[i].lifetime = int(rand_vel_x * LIFETIME_MAX);
}

void update(uint i, float dt)
{
	vec3 pos = p[i].position.xyz;
	vec3 vel = p[i].velocity.xyz;
	int lifetime = p[i].lifetime;

	if (lifetime > LIFETIME_MAX)
	{
		p[i].lifetime -= LIFETIME_MAX;
		return;
	}

	if (lifetime <= 0 || pos.y < 0)
	{
		init(i, dt);
		return;
	}

	lifetime--;
	vec2 s = pos.xy  * dt;
	vel.y += GRAV * dt * (1. - tweak(.1, s));

	pos.x += vel.x * dt;
	pos.y += vel.y * dt;
	pos.z += vel.z * dt;

	p[i].position = vec4(pos, 1.0f);
	p[i].velocity = vec4(vel, 0.0f);
	p[i].lifetime = lifetime;
}

#endif // __cplusplus


#endif // !FIREWORKTYPE1PARTICLECONTROLLER_H
