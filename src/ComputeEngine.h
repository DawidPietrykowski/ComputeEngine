#pragma once
#define GLEW_STATIC

#include "UBO.h"
#include "GLUtils.h"
#include "SSBO.h"
#include "Texture.h"
#include "Program.h"
#include "Shader.h"
#include "Timer.h"

class ComputeEngine {
public:
	ComputeEngine(int w, int h, std::string name, bool focus = true, bool tonemapping = false);
	~ComputeEngine();
	
	GLFWwindow* InitWindow(int width, int height, std::string name, bool focus);

	void UpdateFrametime();
	void CloseWindow();
	void SwitchInput();
	void SetInput(bool mode);
	void PollEvents();
	void GetMousePos(double& xpos, double& ypos);
	void GetMouseDelta(double& dx, double& dy);
	void SaveScreen(std::string name);
	void ResetKeys();
	void SetFullscreen(bool fullscreen);
	void SwitchFullScreen();
	void GetWindowSize(int& width, int& height);
	void DrawTexture(Texture& texture);
	void Finish();

	bool IsFullscreen();
	bool GetInput();
	bool ShouldClose();
	bool IsKeyPressed(int key);
	bool IsKeyClicked(int key);
	bool IsMouseButtonPressed(int button);
	bool IsMouseButtonClicked(int button);
	bool FileExists(const std::string& name);

	double GetFrametime();
	double GetAverageFrametime();

	int GetWidth();
	int GetHeight();
	int GetFramecount();

	float GetRand();

	GLFWwindow* GetWindow();

private:
	Program m_TextureDisplay;
	Timer m_timer;
	GLFWwindow* m_window;


	Program InitQuad(Shader& vertShader, Shader& fragShader);

	double m_frametime = 0;
	double m_time_sum = 0;

	int m_keys[348];
	int m_prev_keys[348];
	int m_mouse_buttons[8];
	int m_prev_mouse_buttons[8];

	bool m_fullscreen = false;

	bool m_input = false;

	int m_framecount = 0;
	int m_width, m_height;
	int m_windowed_width, m_windowed_height;

	double m_xpos = 0, m_ypos = 0;
	float m_dx = 0, m_dy = 0;

	int m_fullscreen_width = 1920;
	int m_fullscreen_height = 1080;

	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods); 
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void UpdateSize(int w, int h);
};
