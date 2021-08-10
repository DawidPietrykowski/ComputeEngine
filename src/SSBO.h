#pragma once
#include <vector>
#include "GLUtils.h"

class SSBO {
private:
	unsigned int m_id;
	int m_binding;
	int m_last_size;
public:

	SSBO(int bind);
	SSBO();
	SSBO(unsigned int i, int bind);
	SSBO& operator=(SSBO&& other);
	SSBO(SSBO&& other);
	SSBO(const SSBO&) = delete;
	SSBO& operator=(const SSBO&) = delete;
	~SSBO();
	int GetId();
	void Delete();
	void Bind();

	template<typename T>
	void Set(T shader_data) {
		int size = sizeof(shader_data);

		m_last_size = size;

		Bind();
		GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, size, size > 0 ? &shader_data : nullptr, GL_DYNAMIC_DRAW));

		GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_binding, m_id));
	}
	template<typename T>
	void Set(std::vector<T>* shader_data) {
		int size = sizeof(T) * (int)shader_data->size();

		m_last_size = size;

		Bind();
		GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, size, size > 0 ? shader_data->data() : nullptr, GL_DYNAMIC_DRAW));

		GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_binding, m_id));
	}


	template<typename T>
	void Update(T shader_data) {
		int size = sizeof(shader_data);

		Bind();
		if (size == m_last_size) {
			GLCall(glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, &shader_data));
		}
		else {
			m_last_size = size;
			GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, size, &shader_data, GL_DYNAMIC_DRAW));
			GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_binding, m_id));
		}
	}
	template<typename T>
	void Update(std::vector<T>* shader_data) {
		int size = sizeof(T) * shader_data->size();

		Bind();
		if (size == m_last_size) {
			GLCall(glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, size > 0 ? shader_data->data() : nullptr));
		}
		else {
			m_last_size = size;
			GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, size, size > 0 ? shader_data->data() : nullptr, GL_DYNAMIC_DRAW));
			GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_binding, m_id));
		}
	}
};