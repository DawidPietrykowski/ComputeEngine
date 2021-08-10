#include "Shader.h"
#include "GLUtils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

std::string vert_source = 
"#version 330 core\n"
"layout(location = 0) in vec4 position;\n"
"layout(location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"
"void main(){\n"
"gl_Position = position;\n"
"TexCoord = aTexCoord;\n"
"}";

std::string frag_source_tonemapped =
"#version 330 core\n"
"layout(location = 0) out vec4 color;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D img_output;\n"
"void main(){\n"
"const float gamma = 2.2;\n"
"vec3 hdrColor = texture(img_output, TexCoord).xyz;\n"
"vec3 mapped = vec3(1.0) - exp(-hdrColor * 1.0);\n"
"mapped = pow(mapped, vec3(1.0 / gamma));\n"
"color = vec4(mapped, 1.0);\n"
"}";

std::string frag_source =
"#version 330 core\n"
"layout(location = 0) out vec4 color;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D img_output;\n"
"void main(){\n"
"color = texture(img_output, TexCoord);\n"
"}";

Shader::Shader(std::string path, unsigned int type) {
    std::string source;

    LoadShader(path, source);

    m_id = CompileShader(source, type);
}

Shader::Shader(std::string path, unsigned int type, bool tonemapping)
{
    std::string source;

    LoadShader(path, source);

    m_id = CompileShader(source, type);
}

Shader::Shader(std::string path) {
    std::string source;

    LoadShader(path, source);

    m_id = CompileShader(source, GL_COMPUTE_SHADER);
}

//Shader::Shader(unsigned int type) {
//    switch (type) {
//    case GL_VERTEX_SHADER:
//        m_id = CompileShader(vert_source, type);
//        break;
//    case GL_FRAGMENT_SHADER:
//        m_id = CompileShader(frag_source, type);
//        break;
//    default:
//        __debugbreak();
//        break;
//    }
//}
Shader::Shader(unsigned int type, bool tonemapped) {
    switch (type) {
    case GL_VERTEX_SHADER:
        m_id = CompileShader(vert_source, type);
        break;
    case GL_FRAGMENT_SHADER:
        if(!tonemapped)
            m_id = CompileShader(frag_source, type);
        else
            m_id = CompileShader(frag_source_tonemapped, type);
        break;
    default:
        __debugbreak();
        break;
    }
}

Shader::~Shader() {
    Delete();
}

Shader::Shader(Shader&& other) :
    m_id(other.m_id)
{
    other.m_id = 0;
}

Shader& Shader::operator=(Shader&& other)
{
    if (this != &other)
    {
        Delete();
        std::swap(m_id, other.m_id);
    }
    return *this;
}

void Shader::Delete() {
    if (m_id != 0) {
        GLCall(glDeleteShader(m_id));
    }
    m_id = 0;
}

int Shader::CompileShader(std::string _source, unsigned int type){
    unsigned int id = glCreateShader(type);
    const char* src = _source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    m_source = _source;
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        m_compilation_success = false;
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        if (type == GL_COMPUTE_SHADER) {
            LOG("Compilation failed of compute shader");
            LOG(message);
        }
        GLCall(glDeleteShader(id));
        return 0;
    }
    else {
        m_compilation_success = true;
        if (type == GL_COMPUTE_SHADER)
            LOG("Compilation successfull of compute shader");
    }

    return id;
}

void Shader::LoadShader(std::string name, std::string& Shader){
    std::ifstream v(name);
    std::stringstream buffer;
    buffer << v.rdbuf();
    Shader = buffer.str();
}

int Shader::GetId() {
    return m_id;
}

bool Shader::Compiled()
{
    return m_compilation_success;
}
