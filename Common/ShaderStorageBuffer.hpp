#pragma once
#include <glad\glad.h>
#include <iostream> 

template <class T>
class ShaderStorageBuffer
{
public:
	ShaderStorageBuffer(size_t size);  // size in number of elements
	~ShaderStorageBuffer();

	void bind();
	void unbind();

	T *map(GLbitfield access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	void unmap();

	GLuint getBuffer() { return m_buffer; }
	size_t getSize() const { return m_size; }

	void dump();

private:
	static const GLenum target = GL_SHADER_STORAGE_BUFFER;

	size_t m_size;

	GLuint m_buffer;
};

template <class T>
ShaderStorageBuffer<T>::ShaderStorageBuffer(size_t size) :
	m_size(size)
{
	glGenBuffers(1, &m_buffer);

	bind();
	glBufferData(target, m_size * sizeof(T), 0, GL_STATIC_DRAW);
	unbind();
}

template <class T>
ShaderStorageBuffer<T>::~ShaderStorageBuffer()
{
	glDeleteBuffers(1, &m_buffer);
}

template <class T>
void ShaderStorageBuffer<T>::bind()
{
	glBindBuffer(target, m_buffer);
}

template <class T>
void ShaderStorageBuffer<T>::unbind()
{
	glBindBuffer(target, 0);
}

template <class T>
T *ShaderStorageBuffer<T>::map(GLbitfield access)
{
	bind();
	return (T *) glMapBufferRange(target, 0, m_size * sizeof(T), access);
}

template <class T>
void ShaderStorageBuffer<T>::unmap()
{
	bind();
	glUnmapBuffer(target);
}

template <class T>
void ShaderStorageBuffer<T>::dump()
{
	T *data = map(GL_MAP_READ_BIT);
	for(size_t i = 0; i<m_size; i++)
	{
		std::cout << i << ": " << data[i] << std::endl;
	}
	unmap();
}