#ifndef PARTICLE_H
#define PARTICLE_H

#ifndef __cplusplus
#define VEC4 vec4
precision highp float;
#else
#include <glm\glm.hpp>

#define VEC4 glm::vec4

#endif // !__cplusplus

struct Particle
{
	VEC4 position;
	VEC4 velocity;
	VEC4 color;
	int lifetime; 
#ifdef __cplusplus
	float __padding[3];
#endif // __cplusplus
};

#ifndef __cplusplus
layout(std140, binding = 0) buffer particles
{
	Particle p[];
};

uniform uint numParticles;
#endif // !__cplusplus

#define WORK_GROUP_SIZE 128

#endif // !PARTICLE_H
