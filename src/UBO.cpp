#include "UBO.h"
#include "GLUtils.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

UBO::UBO(int bind) :
    m_id(0), m_binding(bind), m_last_size(0)
{
    GLCall(glGenBuffers(1, &m_id));
}

UBO::UBO() :
    m_id(0), m_binding(0), m_last_size(0)
{
}

UBO::~UBO() {
    Delete();
}

int UBO::GetId()
{
    return m_id;
}

UBO::UBO(UBO&& other) :
    m_id(other.m_id), m_binding(other.m_binding), m_last_size(other.m_last_size)
{
    other.m_id = 0;
}

UBO& UBO::operator=(UBO&& other)
{
    if (this != &other)
    {
        Delete();
        std::swap(m_id,other.m_id);
        m_binding = other.m_binding;
        m_last_size = other.m_last_size;
    }
    return *this;
}

void UBO::Delete() {
    if (m_id != 0) {
        GLCall(glDeleteBuffers(1, &m_id));
    }
    m_id = 0;
}

void UBO::Bind() {
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_id));
}


