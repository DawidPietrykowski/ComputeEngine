#pragma once
#include <vector>

#include "GLUtils.h"

class UBO {
private:
	unsigned int m_id = 0;
	int m_binding = 0;
	size_t m_last_size = 0;
public:

	UBO(int bind);
	UBO();
	UBO& operator=(UBO&& other);
	UBO(UBO&& other);
	UBO(const UBO&) = delete;
	UBO& operator=(const UBO&) = delete;
	~UBO();
	int GetId();
	void Delete();
	void Bind();

	template<typename T>
	void Set(T* shader_data) {
		size_t size = sizeof(*shader_data);

		m_last_size = size;

		Bind();
		GLCall(glBufferData(GL_UNIFORM_BUFFER, size, size > 0 ? shader_data : nullptr, GL_DYNAMIC_DRAW));

		GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, m_binding, m_id));
	}
	template<typename T>
	void Set(std::vector<T>* shader_data) {
		size_t size = sizeof(T) * shader_data->size();

		m_last_size = size;

		Bind();
		GLCall(glBufferData(GL_UNIFORM_BUFFER, size, size > 0 ? shader_data->data() : nullptr, GL_DYNAMIC_DRAW));

		GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, m_binding, m_id));
	}


	template<typename T>
	void Update(T* shader_data) {
		size_t size = sizeof(*shader_data);

		Bind();
		if (size == m_last_size) {
			GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, size, shader_data));
		}
		else {
			m_last_size = size;
			GLCall(glBufferData(GL_UNIFORM_BUFFER, size, shader_data, GL_DYNAMIC_DRAW));
			GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, m_binding, m_id));
		}
	}
	template<typename T>
	void Update(std::vector<T>* shader_data) {
		size_t size = sizeof(T) * shader_data->size();

		Bind();
		if (size == m_last_size) {
			GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, size, size > 0 ? shader_data->data() : nullptr));
		}
		else {
			m_last_size = size;
			GLCall(glBufferData(GL_UNIFORM_BUFFER, size, size > 0 ? shader_data->data() : nullptr, GL_DYNAMIC_DRAW));
			GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, m_binding, m_id));
		}
	}
};