#pragma once
#include "Shader.h"
#include "Texture.h"

class Program {
private:
	unsigned int m_id = 0;

public:
	Program();
	~Program();
	Program(unsigned int id);
	Program(Shader& vertShader, Shader& fragShader);
	Program(Shader& computeShader);

	Program& operator=(Program&& other);
	Program(Program&& other);
	Program(const Program&) = delete;
	Program& operator=(const Program&) = delete;

	void Delete();

	void Use();
	void DispatchCompute(int width, int height);
	void DispatchCompute(int x);
	void BindTexture(Texture& texture, int slot);
	void BindTexture(Texture* texture, int slot);
	void BindTexture(Texture& texture, int slot, std::string name);
	void BindTextureImage(Texture& texture, int slot);
	unsigned int GetId();

	void SetUniform1f(std::string name, float u);
	void SetUniform2f(std::string name, float u1, float u2);
	void SetUniform3f(std::string name, float u1, float u2, float u3);
	void SetUniform4f(std::string name, float u1, float u2, float u3, float u4);

	void SetUniform1i(std::string name, int u);
	void SetUniform2i(std::string name, int u1, int u2);
	void SetUniform3i(std::string name, int u1, int u2, int u3);
	void SetUniform4i(std::string name, int u1, int u2, int u3, int u4);

	void SetUniform1ui(std::string name, unsigned int u);
	void SetUniform2ui(std::string name, unsigned int u1, unsigned int u2);
	void SetUniform3ui(std::string name, unsigned int u1, unsigned int u2, unsigned int u3);
	void SetUniform4ui(std::string name, unsigned int u1, unsigned int u2, unsigned int u3, unsigned int u4);

	void SetUniform1fv(std::string name, int count, float* u);
	void SetUniform2fv(std::string name, int count, float* u);
	void SetUniform3fv(std::string name, int count, float* u);
	void SetUniform4fv(std::string name, int count, float* u);

	void SetUniform1iv(std::string name, int count, int* u);
	void SetUniform2iv(std::string name, int count, int* u);
	void SetUniform3iv(std::string name, int count, int* u);
	void SetUniform4iv(std::string name, int count, int* u);

	void SetUniform1uiv(std::string name, int count, unsigned int* u);
	void SetUniform2uiv(std::string name, int count, unsigned int* u);
	void SetUniform3uiv(std::string name, int count, unsigned int* u);
	void SetUniform4uiv(std::string name, int count, unsigned int* u);
};