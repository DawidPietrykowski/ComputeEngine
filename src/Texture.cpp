#include "Texture.h"
#include "GLUtils.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


Texture::Texture(std::string name, std::string path) :
    m_width(0), m_height(0), m_name(name), m_image(false),
    m_bpp(0), m_id(0), m_buffer(nullptr)
{
    GLCall(glGenTextures(1, &m_id));

    GLCall(glBindTexture(GL_TEXTURE_2D, m_id));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

    Load(path);
}

Texture::Texture(std::string name, int width, int height) :
    m_width(width), m_height(height), m_name(name), m_image(true),
    m_bpp(0), m_id(0), m_buffer(nullptr)
{
    GLCall(glGenTextures(1, &m_id));

    GLCall(glBindTexture(GL_TEXTURE_2D, m_id));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL));
}

void Texture::Resize(int width, int height) {
    m_width = width;
    m_height = height;


    GLuint boundTexture = 0;
    GLCall(glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&boundTexture));
    Bind();

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL));

    GLCall(glBindTexture(GL_TEXTURE_2D, boundTexture));
}

void Texture::Load(std::string path) {
    stbi_set_flip_vertically_on_load(1);
    std::string extension = path.substr(path.length() - 3, 3);
    if (extension != "hdr") {
        m_buffer = stbi_load(path.c_str(), &m_width, &m_height, &m_bpp, 4);

        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_buffer));
    }
    else {
        m_buffer = (unsigned char*)stbi_loadf(path.c_str(), &m_width, &m_height, &m_bpp, 4);

        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, m_buffer));
    }

    if (m_buffer)
        delete[] m_buffer;
}

void Texture::Bind(unsigned int slot) {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_id));
}

void Texture::BindImage(unsigned int slot) {
    GLCall(glBindImageTexture(slot, m_id, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F));
}

bool Texture::IsImg() {
    return m_image;
}

std::string Texture::GetName()
{
    return m_name;
}

Texture::Texture() :
    m_width(0), m_height(0), m_name(""), m_image(false),
    m_bpp(0), m_id(0), m_buffer(nullptr) {}

Texture::~Texture() {
    Delete();
}

Texture::Texture(Texture&& other) :
    m_width(other.m_width), m_height(other.m_height), m_name(other.m_name), m_image(other.m_image),
    m_bpp(other.m_bpp), m_id(other.m_id), m_buffer(other.m_buffer)
{
    other.m_id = 0;
}

Texture& Texture::operator=(Texture&& other)
{
    if (this != &other)
    {
        Delete();
        std::swap(m_id, other.m_id);
        std::swap(m_width, other.m_width);
        std::swap(m_height, other.m_height);
        std::swap(m_name, other.m_name);
        std::swap(m_image, other.m_image);
        std::swap(m_bpp, other.m_bpp);
        std::swap(m_buffer, other.m_buffer);
    }
    return *this;
}

void Texture::Delete() {
    if (m_id != 0) {
        Unbind();
        GLCall(glDeleteTextures(1, &m_id));
    }
}

void Texture::Bind() {
    GLCall(glBindTexture(GL_TEXTURE_2D, m_id));
}

void Texture::Unbind() {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

unsigned int Texture::GetId() {
    return m_id;
}

int Texture::GetWidth() {
    return m_width;
}

int Texture::GetHeight() {
    return m_height;
}