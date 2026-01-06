
#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h>

#include <string>

class Shader {
public:
	GLuint ID;

	Shader() = default;

	Shader(const std::string& vertexPath, const std::string& fragmentPath);

	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	Shader(Shader&& other) noexcept;
	Shader& operator=(Shader&& other) noexcept;

	~Shader();

	void use() const;

	void setBool(const std::string& name, bool value) const;
	
	void setInt(const std::string& name, int value) const;
	
	void setFloat(const std::string& name, float value) const;

private:
	void checkCompileError(GLuint shader, const std::string& type);
};

#endif // !SHADER_H
