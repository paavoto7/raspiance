#ifndef RENDERER_H
#define RENDERER_H

#include "shader.h"

#include <array>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

// Is responsible for rendering the captured image data.
// Sends the raw YUYV data to OpenGL, where fragment shader
// converts it to RGB.
class Renderer {
public:
    const int width;
    const int height;
    
    Renderer(int width, int height);
    ~Renderer();

	int run();
    int drawFrame(unsigned char* data = nullptr);
    int initGlfw();
    int init();

private:
	// float deltaTime{};
	// float lastFrame{};

	GLFWwindow* window = nullptr;
    Shader shader;

    // These are here for now, should add a texture class later
    GLuint VAO, VBO, id;
    const GLenum textureType = GL_TEXTURE_2D;
    std::array<float, 24> vertices;

	void render();
	void processInput();
};


#endif // !RENDERER_H
