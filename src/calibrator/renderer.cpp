#include "renderer.h"

#include <iostream>

// Both callbacks could be moved into their own namespace and file

void inline framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

Renderer::Renderer(int width, int height)
    : width(width), height(height) {}

Renderer::~Renderer() {
    glfwDestroyWindow(window);
	glfwTerminate();
}

int Renderer::run() {
    // Could be used if another thread is spawned for calibration
    
    /*
    initGlfw();
    while (!glfwWindowShouldClose(window)) {

    }
    */
    return 0;
}

// Draws a single frame with the given data.
// Calling init in required first.
int Renderer::drawFrame(unsigned char *data) {
    if (window == nullptr) return -1;

    glClearColor(0.5f, 0.5f, 0.55f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

    if (data == nullptr) {
        return 1;
    }

    glActiveTexture(GL_TEXTURE0);
	glBindTexture(textureType, id);
    // Send the new data to the same texture
    glTexSubImage2D(textureType, 0, 0, 0, width / 2, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
	glfwPollEvents();

    return glfwWindowShouldClose(window);
}

// Sets all necessary Glfw variables
int Renderer::initGlfw() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	// Doing this makes the the fullscreen switch a lot faster
    // Not using it for now
	// glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	// glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	// glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	// glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	// window = glfwCreateWindow(mode->width, mode->height, "Walking simulator", monitor, nullptr);

    window = glfwCreateWindow(width, height, "Raspiance", nullptr, nullptr);
	
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	// glfwSetWindowUserPointer(window, &camera);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// glfwSetCursorPosCallback(window, Callbacks::mouse_callback);
	// glfwSetScrollCallback(window, Callbacks::scroll_callback);
    glfwSetKeyCallback(window, keyCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		std::cout << "Failed to initialise GLAD" << std::endl;
		return 1;
	}
	return 0;
}

// Sets everything ready for the program to run
int Renderer::init() {
    initGlfw();

    // A fullscreen quad, could use EBO's as well
    vertices = {
        -1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 1.0f, 1.0f,

        1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f
	};

    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Not in use for now, but data has them
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	glGenTextures(1, &id);
	glBindTexture(textureType, id);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(textureType, 0, GL_RGBA8, width / 2, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    // glGenerateMipmap(textureType);

    glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    shader = std::move(Shader("./src/calibrator/shaders/vertex.glsl", "./src/calibrator/shaders/fragment.glsl"));

    shader.use();
    shader.setInt("yuyvTex", 0);
    
    return 0;
}

void Renderer::render() {
}

void Renderer::processInput() {
}

