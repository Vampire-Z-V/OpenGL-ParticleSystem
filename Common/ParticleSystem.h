#pragma once
#include <glad\glad.h>
#include <glm\glm.hpp>

#include "Particle.h"
#include "ParticleController.h"
#include "ShaderStorageBuffer.hpp"
#include "ShaderProgram.h"

class ParticleSystem
{
public:
	explicit ParticleSystem(ShaderProgram &computeShader, ShaderProgram &renderProgram, ParticleController *controller, unsigned int particleNum);
	~ParticleSystem();

	void reset();
	void update(float dt);
	void render(glm::mat4x4 &MVP);

private:
	ParticleController * m_pParticleController;
	ShaderStorageBuffer<Particle> * m_pSSBO;
	ShaderProgram m_ComputeShaderProgram;
	ShaderProgram m_RenderProgram;

	GLuint m_VAO;
	unsigned int m_ParticleNum;

private:
	ParticleSystem(const ParticleSystem& other) = delete;
	ParticleSystem& operator=(const ParticleSystem& other) = delete;
};

