#pragma once
#define _USE_MATH_DEFINES
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Texture {
private:
	unsigned int m_id;
	std::string m_name;
	bool m_image;
	unsigned char* m_buffer;
	int m_width;
	int m_height;
	int m_bpp;

public:
	Texture(std::string name, std::string path);
	Texture(std::string name, int width, int height);
	void Resize(int width, int height);
	Texture();
	~Texture();
	Texture& operator=(Texture&& other);
	Texture(Texture&& other);
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	void Load(std::string path);
	void Bind(unsigned int slot);
	void BindImage(unsigned int slot);
	void Bind();
	void Unbind();
	void Delete();
	unsigned int GetId();
	int GetWidth();
	int GetHeight();
	bool IsImg();
	std::string GetName();
};