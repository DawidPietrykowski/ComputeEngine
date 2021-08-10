#include "SSBO.h"
#include "GLUtils.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

SSBO::SSBO(int bind) :
    m_id(0), m_binding(bind), m_last_size(0) 
{
    GLCall(glGenBuffers(1, &m_id));
}

SSBO::SSBO() :
    m_id(0), m_binding(0), m_last_size(0)
{
}

SSBO::SSBO(unsigned int i, int bind) :
    m_id(i), m_binding(bind), m_last_size(0)
{}

SSBO::~SSBO() {
    Delete();
}

int SSBO::GetId()
{
    return m_id;
}

SSBO::SSBO(SSBO&& other) :
    m_id(other.m_id), m_binding(other.m_binding), m_last_size(other.m_last_size)
{
    other.m_id = 0;
}

SSBO& SSBO::operator=(SSBO&& other)
{
    if (this != &other)
    {
        Delete();
        std::swap(m_id, other.m_id);
        m_binding = other.m_binding;
        m_last_size = other.m_last_size;
    }
    return *this;
}

void SSBO::Delete() {
    if (m_id != 0) {
        GLCall(glDeleteBuffers(1, &m_id));
    }
    m_id = 0;
}

void SSBO::Bind() {
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id));
}