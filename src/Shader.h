#pragma once
#define _USE_MATH_DEFINES
#include <string>
#include <vector>

class Shader {
public:
	Shader(std::string path);
	Shader(std::string path, unsigned int type);
	Shader(std::string path, unsigned int type, bool tonemapping);
	Shader(unsigned int type, bool tonemapped = false);
	~Shader();

	Shader& operator=(Shader&& other);
	Shader(Shader&& other);
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	void Delete();
	int CompileShader(std::string _source, unsigned int type);
	void LoadShader(std::string name, std::string& Shader);
	int GetId();
	bool Compiled();
private:
	bool m_compilation_success = false;
	unsigned int m_id = 0;
	std::string m_source;
};