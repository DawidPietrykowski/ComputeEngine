#include <Windows.h>

#include "ComputeEngine.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#pragma warning(disable : 4996)
#include <stb_image_write.h>
#include <cstdlib>
#include <chrono> 
#include <string>
#include <vector>
#include <iostream>
#include <math.h>
#include <array>
#include <cmath>
#include <vector>
#include <string>
#include <fileapi.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Texture.h"
#include "Program.h"
#include "Shader.h"
#include "Timer.h"

ComputeEngine* m_instance;

ComputeEngine::~ComputeEngine() {
    m_TextureDisplay.~Program();
    m_instance = nullptr;
    glfwTerminate();
}

ComputeEngine::ComputeEngine(int w, int h, std::string name, bool focus, bool tonemapping)
{
    m_instance = this;

    m_width = w;
    m_height = h;

    m_window = InitWindow(m_width, m_height, name, focus);
    LOG("GLFW initiated");
    
    GLCall(glEnable(GL_DEBUG_OUTPUT));
    glewExperimental = GL_FALSE;

    glfwGetCursorPos(m_window, &m_xpos, &m_ypos);

    Shader vertexShadrer(GL_VERTEX_SHADER);
    Shader fragmentShader(GL_FRAGMENT_SHADER, tonemapping);

    m_TextureDisplay = InitQuad(vertexShadrer, fragmentShader);
    LOG("Texture display shader created");

    for(int i = 0; i < 348; i++)
        m_keys[i] = GLFW_RELEASE;
}

GLFWwindow* ComputeEngine::InitWindow(int width, int height, std::string name, bool focus) {

    GLFWwindow* window;

    if (!glfwInit())
        LOG("Failed to initialize glfw");

    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        LOG("Failed to initialize window");
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, this->KeyCallback);
    glfwSetMouseButtonCallback(window, this->MouseButtonCallback);
    glfwSetFramebufferSizeCallback(window, this->FramebufferSizeCallback);
    if (focus) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    glfwSetWindowSizeLimits(window, 200, 200, m_fullscreen_width, m_fullscreen_height);

    m_input = focus;

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        LOG("Error: " << glewGetErrorString(err));
    }

    LOG("Using GL " << glGetString(GL_VERSION));
    LOG("Using GLEW " << glewGetString(GLEW_VERSION));

    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primary);
    m_fullscreen_width = mode->width;
    m_fullscreen_height = mode->height;

    return window;
}

Program ComputeEngine::InitQuad(Shader& vertShader, Shader& fragShader) {

    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    float quad_vao[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, // bottom left corner
        -1.0f,  1.0f, 0.0f, 1.0f, // top left corner
         1.0f, -1.0f, 1.0f, 0.0f, // bottom right corner
         1.0f,  1.0f, 1.0f, 1.0f // top right corner
    };
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, quad_vao, GL_STATIC_DRAW));
    GLuint vao = 0;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2)));

    Program quad_program(vertShader, fragShader);
    return quad_program;
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::string sev = "";
    if (severity == GL_DEBUG_SEVERITY_LOW)
        sev = "low";
    else if (severity == GL_DEBUG_SEVERITY_HIGH)
        sev = "high";
    else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
        return;
    else if (severity == GL_DEBUG_SEVERITY_MEDIUM)
        sev = "medium";

    fprintf(stderr, "GL CALLBACK: %s type = %x, severity = %s, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, sev.c_str(), message);

}

void ComputeEngine::FramebufferSizeCallback(GLFWwindow* window, int w, int h) {
    if(w != 0 && h != 0)
        (*m_instance).UpdateSize(w, h);
}

void ComputeEngine::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (*m_instance).m_keys[key] = action;
}

void ComputeEngine::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    (*m_instance).m_mouse_buttons[button] = action;
}

void ComputeEngine::PollEvents() {
    ResetKeys();

    glfwPollEvents();

    for (int i = 0; i < 348; i++) {
        m_keys[i] = (m_keys[i] == GLFW_PRESS && m_prev_keys[i] == GLFW_PRESS) ? GLFW_REPEAT : m_keys[i];
    }

    for (int i = 0; i < 8; i++) {
        m_mouse_buttons[i] = (m_mouse_buttons[i] == GLFW_PRESS && m_prev_mouse_buttons[i] == GLFW_PRESS) ? GLFW_REPEAT : m_mouse_buttons[i];
    }

    double x_p, y_p;
    glfwGetCursorPos(m_window, &x_p, &y_p);

    m_dx = (float)(m_xpos - x_p) * m_input;
    m_dy = (float)(m_ypos - y_p) * m_input;
    m_xpos = x_p;
    m_ypos = y_p;
}

void ComputeEngine::ResetKeys() {
    memcpy(m_prev_keys, m_keys, sizeof(int) * 348);
    memcpy(m_prev_mouse_buttons, m_mouse_buttons, sizeof(int) * 8);
}

void ComputeEngine::SetFullscreen(bool fullscreen) {
    m_fullscreen = fullscreen;

    if (!m_fullscreen) {
        m_width = m_windowed_width;
        m_height = m_windowed_height;

        glfwSetWindowMonitor(m_window, NULL, 200, 200, m_width, m_height, GLFW_DONT_CARE);

        glViewport(0, 0, m_width, m_height);
    }
    else {
        m_windowed_width = m_width;
        m_windowed_height = m_height;

        m_width = m_fullscreen_width;
        m_height = m_fullscreen_height;

        glfwSetWindowMonitor(m_window, NULL, 0, 0, m_width, m_height, GLFW_DONT_CARE);

        glViewport(0, 0, m_width, m_height);

        m_dx = 0;
        m_dy = 0;
    }
}

void ComputeEngine::SwitchFullScreen() {
    SetFullscreen(!IsFullscreen());
}

void ComputeEngine::GetWindowSize(int& width, int& height) {
    width = m_width;
    height = m_height;
}

bool ComputeEngine::IsFullscreen() {
    return m_fullscreen;
}

void ComputeEngine::SetInput(bool mode) {
    m_input = mode;
    if(!mode)
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void ComputeEngine::UpdateSize(int width, int height) {
    m_width = width;
    m_height = height;
    
    glViewport(0, 0, m_width, m_height);
}

bool ComputeEngine::ShouldClose() {
    return glfwWindowShouldClose(m_window);
}

void ComputeEngine::CloseWindow() {
    glfwSetWindowShouldClose(m_window, GLFW_TRUE);
}

void ComputeEngine::SwitchInput() {
    SetInput(!m_input);
}

void ComputeEngine::DrawTexture(Texture &texture){
    m_TextureDisplay.Use();

    GLCall(glMemoryBarrier(GL_ALL_BARRIER_BITS));


    GLCall(glActiveTexture(GL_TEXTURE0));
    GLuint boundTexture = 0;
    GLCall(glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&boundTexture));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture.GetId()));

    GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));

    GLCall(glBindTexture(GL_TEXTURE_2D, boundTexture));
    GLCall(glfwSwapBuffers(m_window));
}

void ComputeEngine::UpdateFrametime() {
    double ms = m_timer.GetMilis();
    m_frametime = m_framecount > 0 ? ms : 1;
    m_time_sum += m_frametime;

    m_framecount++;
}

void ComputeEngine::SaveScreen(std::string name) {
    bool found = true;
    int i = 0;
    while (found) {
        i++;
        std::string full_name_png = "screenshots\\" + name + std::to_string(i) + ".png";
        std::string full_name_jpg = "screenshots\\" + name + std::to_string(i) + ".jpg";
        found = FileExists(full_name_png) || FileExists(full_name_jpg);
    }

    const char* path = "screenshots";
    CreateDirectoryA(path, NULL);

    name = name + std::to_string(i) + ".png";

    int img_size = 4 * m_width * m_height;

    unsigned char* pixels = new unsigned char[img_size];

    LOG("Saving screenshot: " + name);

    GLCall(glReadBuffer(GL_FRONT));
    GLCall(glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, pixels));

    stbi__flip_vertically_on_write = true;

    stbi_write_png(("screenshots\\" + name).c_str(), m_width, m_height, 4, pixels, m_width * 4);

    delete[] pixels;
};

void ComputeEngine::Finish() {
    GLCall(glFinish());
}

bool ComputeEngine::IsKeyPressed(int key) {
    int state = m_keys[key];
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool ComputeEngine::IsKeyClicked(int key) {
    return m_keys[key] == GLFW_PRESS;
}

bool ComputeEngine::IsMouseButtonPressed(int button) {
    int state = m_mouse_buttons[button];
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool ComputeEngine::IsMouseButtonClicked(int button) {
    return m_prev_mouse_buttons[button] == GLFW_PRESS;
}

void ComputeEngine::GetMousePos(double& xpos, double& ypos) {
    xpos = m_xpos;
    ypos = m_ypos;
}

void ComputeEngine::GetMouseDelta(double& dx, double& dy) {
    dx = m_dx;
    dy = m_dy;
}

bool ComputeEngine::GetInput() {
    return m_input;
}

double ComputeEngine::GetFrametime() {
    return m_frametime;
}

int ComputeEngine::GetFramecount() {
    return m_framecount;
}

double ComputeEngine::GetAverageFrametime() {
    return m_time_sum / m_framecount;
}

int ComputeEngine::GetWidth() {
    return m_width;
}

int ComputeEngine::GetHeight() {
    return m_height;
}

float ComputeEngine::GetRand() {
    return (float)((double)rand() / (RAND_MAX));
}

bool ComputeEngine::FileExists(const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

GLFWwindow* ComputeEngine::GetWindow() {
    return m_window;
}