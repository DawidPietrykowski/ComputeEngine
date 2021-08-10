#include "Program.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GLUtils.h"
#include "Texture.h"
#include "Shader.h"

Program::Program() :
    m_id(0)
{
}

Program::Program(unsigned int _id) :
    m_id(_id)
{
}

Program::Program(Shader& vertShader, Shader& fragShader) {
    unsigned int program;
    GLCall(program = glCreateProgram());

    GLCall(glAttachShader(program, vertShader.GetId()));
    GLCall(glAttachShader(program, fragShader.GetId()));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    m_id = program;
}

Program::~Program() {
    Delete();
}


Program::Program(Program&& other) :
    m_id(other.m_id)
{
    other.m_id = 0;
}

Program& Program::operator=(Program&& other)
{
    if (this != &other)
    {
        Delete();
        std::swap(m_id, other.m_id);
    }
    return *this;
}

Program::Program(Shader& computeShader) {
    unsigned int program;
    GLCall(program = glCreateProgram());

    GLCall(glAttachShader(program, computeShader.GetId()));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    m_id = program;
}

void Program::Delete() {
    if (m_id != 0) {
        GLCall(glDeleteProgram(m_id));
    }
    m_id = 0;
}

void Program::DispatchCompute(int width, int height) {
    Use();

    int up_w = int(ceil((float)width / 8.0f));
    int up_h = int(ceil((float)height / 8.0f));
    GLCall(glDispatchCompute(up_w, up_h, 1));
}

void Program::DispatchCompute(int x) {
    Use();

    GLCall(glDispatchCompute(x, 1, 1));
}

void Program::Use() {
    GLCall(glUseProgram(m_id));
}

void Program::BindTexture(Texture& texture, int slot) {
    Use();

    int location;
    GLCall(location = glGetUniformLocation(m_id, texture.GetName().c_str()));

    ASSERT(location != -1);

    Use();
    GLCall(glUniform1i(location, slot));
    texture.Bind(slot);
}
void Program::BindTexture(Texture* texture, int slot) {
    Use();

    int location;
    GLCall(location = glGetUniformLocation(m_id, texture->GetName().c_str()));

    ASSERT(location != -1);

    Use();
    GLCall(glUniform1i(location, slot));
    texture->Bind(slot);
}

void Program::BindTexture(Texture& texture, int slot, std::string name) {
    Use();

    int location;
    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform1i(location, slot));
    texture.Bind(slot);
}

void Program::BindTextureImage(Texture& texture, int slot) {
    Use();

    int location;
    GLCall(location = glGetUniformLocation(m_id, texture.GetName().c_str()));

    ASSERT(location != -1);

    GLCall(glUniform1i(location, slot));
    texture.BindImage(slot);
}

unsigned int Program::GetId() {
    return m_id;
}

void Program::SetUniform1f(std::string name, float u) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform1f(location, u));
}

void Program::SetUniform2f(std::string name, float u1, float u2) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform2f(location, u1, u2));
}

void Program::SetUniform3f(std::string name, float u1, float u2, float u3) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform3f(location, u1, u2, u3));
}

void Program::SetUniform4f(std::string name, float u1, float u2, float u3, float u4) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform4f(location, u1, u2, u3, u4));
}

void Program::SetUniform1i(std::string name, int u) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform1i(location, u));
}

void Program::SetUniform2i(std::string name, int u1, int u2) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform2i(location, u1, u2));
}

void Program::SetUniform3i(std::string name, int u1, int u2, int u3) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform3i(location, u1, u2, u3));
}

void Program::SetUniform4i(std::string name, int u1, int u2, int u3, int u4) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform4i(location, u1, u2, u3, u4));
}

void Program::SetUniform1ui(std::string name, unsigned int u) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform1ui(location, u));
}

void Program::SetUniform2ui(std::string name, unsigned int u1, unsigned int u2) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform2ui(location, u1, u2));
}

void Program::SetUniform3ui(std::string name, unsigned int u1, unsigned int u2, unsigned int u3) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform3ui(location, u1, u2, u3));
}

void Program::SetUniform4ui(std::string name, unsigned int u1, unsigned int u2, unsigned int u3, unsigned int u4) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform4ui(location, u1, u2, u3, u4));
}

void Program::SetUniform1fv(std::string name, int count, float* u) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform1fv(location, count, u));
}

void Program::SetUniform2fv(std::string name, int count, float* u) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform2fv(location, count, u));
}

void Program::SetUniform3fv(std::string name, int count, float* u) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform3fv(location, count, u));
}

void Program::SetUniform4fv(std::string name, int count, float* u) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform4fv(location, count, u));
}

void Program::SetUniform1iv(std::string name, int count, int* u) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform1iv(location, count, u));
}

void Program::SetUniform2iv(std::string name, int count, int* u) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform2iv(location, count, u));
}

void Program::SetUniform3iv(std::string name, int count, int* u) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform3iv(location, count, u));
}

void Program::SetUniform4iv(std::string name, int count, int* u) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform4iv(location, count, u));
}

void Program::SetUniform1uiv(std::string name, int count, unsigned int* u) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform1uiv(location, count, u));
}

void Program::SetUniform2uiv(std::string name, int count, unsigned int* u) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform2uiv(location, count, u));
}

void Program::SetUniform3uiv(std::string name, int count, unsigned int* u) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform3uiv(location, count, u));
}

void Program::SetUniform4uiv(std::string name, int count, unsigned int* u) {
    Use();

    int location;

    GLCall(location = glGetUniformLocation(m_id, name.c_str()));

    ASSERT(location != -1);

    GLCall(glUniform4uiv(location, count, u));
}