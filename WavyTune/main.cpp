#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders/shader_program.h"
#include "Graphics/draw_buffer.h"
#include "Graphics/draw_data2.h"
#include "Graphics/draw_data3.h"
#include "Graphics/entity.h"

#include "Renderer/concrete_renderer.h"
#include "render_builder.h"

#include "Shaders/vs.glsl.h"
#include "Shaders/vs_test.glsl.h"
#include "Shaders/fs.glsl.h"
#include "Shaders/fs_test.glsl.h"
template<class T, size_t N>
constexpr size_t size(T(&)[N])
{
	return N;
}



// More testing for why things arent working
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <chrono>
#include "Fourier/dft_operations.h"
#include <cmath>


#include "GLAbstractions/vao.h"
#include "GLAbstractions/vbo.h"
#include "GLAbstractions/vertex_attribute.h"

struct Camera
{
	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	glm::mat4 transform;


	glm::vec3 getDirection() const
	{
		return transform * glm::vec4(front, 1);
	}

	glm::vec3 getUp() const
	{
		return transform * glm::vec4(up, 1);
	}

	glm::vec3 getRight() const
	{
		return transform * glm::vec4(right, 1);
	}
};


void resizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// TODO : encapsulate all of this global look at stuff
Camera cam;
glm::vec3 eye;
glm::vec3 pos;
glm::vec3 up;
glm::mat4 lookAt;

enum class AXIS {
	X,
	Y,
	Z
};
glm::vec3 rotateVector(const glm::vec3& vector, const glm::vec3& from, const AXIS& axis, const float& angle)
{
	const glm::vec3 INV_VECTOR{
		-from.x,
		-from.y,
		-from.z
	};

	switch (axis) {
	case AXIS::X:
	{
		const glm::vec3 ROT_AXIS{ 1, 0, 0 };
		const glm::mat4& ROT_MATRIX = glm::rotate(angle, ROT_AXIS);
		glm::vec3 retVal = ROT_MATRIX * glm::vec4{ vector.x + INV_VECTOR.x,
			vector.y + INV_VECTOR.y,
			vector.z + INV_VECTOR.z,
			1 };

		return retVal - INV_VECTOR;
	}
	case AXIS::Y:
	{
		const glm::vec3 ROT_AXIS{ 0, 1, 0 };
		const glm::mat4& ROT_MATRIX = glm::rotate(angle, ROT_AXIS);
		glm::vec3 retVal = ROT_MATRIX * glm::vec4{ vector.x + INV_VECTOR.x,
			vector.y + INV_VECTOR.y,
			vector.z + INV_VECTOR.z,
			1 };
		return retVal - INV_VECTOR;
	}
	case AXIS::Z:
	{
		const glm::vec3 ROT_AXIS{ 0, 0, 1 };
		const glm::mat4& ROT_MATRIX = glm::rotate(angle, ROT_AXIS);
		glm::vec3 retVal = ROT_MATRIX * glm::vec4{ vector.x + INV_VECTOR.x,
			vector.y + INV_VECTOR.y,
			vector.z + INV_VECTOR.z,
			1 };
		return retVal - INV_VECTOR;
	}
	}
}

void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		cam.transform = glm::rotate(glm::radians(10.f), cam.getRight()) * cam.transform;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		cam.transform = glm::rotate(glm::radians(-10.f), cam.getRight()) * cam.transform;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		cam.transform = glm::rotate(glm::radians(10.f), cam.getUp()) * cam.transform;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		cam.transform = glm::rotate(glm::radians(-10.f), cam.getUp()) * cam.transform;
	}

	// Movements
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		cam.pos += cam.getDirection() * 0.5f;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		cam.pos -= cam.getDirection() * 0.5f;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		cam.pos += cam.getRight() * 0.5f;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		cam.pos -= cam.getRight() * 0.5f;
	}
}

unsigned vaoId = 0;
unsigned vertexBufferId = 0;
unsigned normalBufferId = 0;

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
int main(int argc, char** argv)
{
	using namespace std::chrono_literals;
	std::chrono::high_resolution_clock c;

	// Getting a signal from sin, with frequency 16Hz
	const double f = 0.25;
	const double A = 1;
	const double N = 64;
	const double T = 1;
	const double phi = 0;
	Matrix<std::complex<double>> signal(N, 1);
	for (std::size_t i = 0; i < N; ++i)
	{
		double x = 2.0 * MATH_PI * i * f * T + phi;
		signal[i][0] = cos(x);
	}

	// Testing the fourier shit
	std::function<double(const std::complex<double>&)> applier = [](const std::complex<double>& v) -> double
	{
		return std::abs(v);
	};
	

	glewExperimental = GL_TRUE;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);


	GLFWwindow* window = glfwCreateWindow(800, 600, "TestingOut Window", NULL, NULL);
	if (!window) {
		std::cout << "Could not create window. Exiting..." << std::endl;
		glfwTerminate();
		return -1;
	}

	// Create the viewport
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, resizeCallback);
	glfwMakeContextCurrent(window);

	// Create the shader program
	glewInit(); // Initialise all the openGL macros

	glm::mat4 proj = glm::perspective<float>(
		glm::radians(45.0f),
		800 / 600,
		0.01,
		100
		);


	// Callback from commands
	glfwSetKeyCallback(window, keyCallBack);

	cam.pos = { 0,  0, 10 };
	cam.front = { 0,  0, -1 };
	cam.right = { 1,  0,  0 };
	cam.up = { 0,  1,  0 };
	cam.transform = glm::mat4{ 1 };

	eye = { 0, 0, 5 };
	pos = { 0, 0, -1 };
	up = { 0, 1, 0 };

	// Create a bar renderer
	RenderBuilder builder;

	auto bar1 = builder.buildBarRenderer(vs, size(vs), fs, size(fs));
	bar1->send_gpu_data();
	auto bar2 = builder.buildBarRenderer(vs_test, size(vs_test), fs_test, size(fs_test));
	bar2->set_height(2.0f);
	bar2->set_offset(1.0f);
	// bar2->send_gpu_data();
	
	

	// Game loop
	glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
	while (!glfwWindowShouldClose(window)) {
		lookAt = glm::lookAt(
			cam.pos,
			cam.pos + cam.getDirection(),
			cam.getUp()
			);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		bar1->render(proj, lookAt);
		bar2->render(proj, lookAt);

		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}
