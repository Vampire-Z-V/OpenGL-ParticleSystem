#include "ParticleSystem.h"
#include <time.h>


ParticleSystem::ParticleSystem(ShaderProgram &computeShader, ShaderProgram &renderProgram, ParticleController *controller, unsigned int particleNum)
	: m_ComputeShaderProgram(computeShader)
	, m_RenderProgram(renderProgram)
	, m_pParticleController(controller)
	, m_ParticleNum(particleNum)
{
	m_pSSBO = new ShaderStorageBuffer<Particle>(particleNum);
	reset();

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_pSSBO->getBuffer());
	// position
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(2*sizeof(glm::vec4)));
	glEnableVertexAttribArray(1);
	//// lifetime
	//glVertexAttribPointer(2, 1, GL_INT, GL_FALSE, sizeof(Particle), (void*)(3 * sizeof(glm::vec4)));
	//glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}


ParticleSystem::~ParticleSystem()
{
	delete m_pSSBO;
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &m_VAO);
}


void ParticleSystem::reset()
{
	Particle* p = m_pSSBO->map();

	m_pParticleController->init(p);

	m_pSSBO->unmap();
}

void ParticleSystem::update(float dt)
{
	m_ComputeShaderProgram.use();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_pSSBO->getBuffer());
	m_ComputeShaderProgram.setUniform("numParticles", m_ParticleNum);
	m_ComputeShaderProgram.setUniform("time_seed", (float)time(0));
	m_ComputeShaderProgram.setUniform("dt", dt);

	glDispatchCompute(GLuint(m_ParticleNum / WORK_GROUP_SIZE + 1), 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glUseProgram(0);
}

void ParticleSystem::render(glm::mat4x4 &MVP)
{
	m_RenderProgram.use();
	m_RenderProgram.setUniform("MVP", MVP);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_POINTS, 0, m_ParticleNum);
	glBindVertexArray(0);
}