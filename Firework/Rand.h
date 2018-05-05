#ifndef MY_RAND_H
#define MY_RAND_H

#ifdef __cplusplus
#include <stdlib.h>

float frand()
{
	return rand() / (float)RAND_MAX;
}

float sfrand()
{
	return frand() * 2.0f - 1.0f;
}

float tweak(float scale)
{
	return scale * sfrand();
}

#else

float frand(vec2 s)
{
	return fract(sin(dot(s.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float sfrand(vec2 s)
{
	return frand(s) * 2.0f - 1.0f;
}

float tweak(float scale, vec2 s)
{
	return scale * sfrand(s);
}

uniform float time_seed;

#endif // __cplusplus


#endif // !MY_RAND_H
