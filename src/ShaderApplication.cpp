#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Render.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	//glfw initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		std::cin.get();
		glfwTerminate();
		return -1;
	}
	// make the window's context current
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSwapInterval(1);

	//glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		std::cin.get();
		return -1;
	}
	{
		float verticesTR[] = {
			-0.9f, -0.5f, 0.0f,  // left 
			0.0f, -0.5f, 0.0f,  // right
			-0.45f, 0.5f, 0.0f,  // top 

			0.0f, -0.5f, 0.0f,  // left
			0.9f, -0.5f, 0.0f,  // right
			0.45f, 0.5f, 0.0f   // top 
		};

		unsigned int indices[] = {
			0, 1, 2,
			5, 1, 2
		};

		//VAO, VBO, EBO
		VertexArray va;
		VertexBuffer vb(verticesTR, 9 * 2 * sizeof(float));
		IndexBuffer ib(indices, 6);

		VertexBufferLayout layout;
		layout.Push<float>(3);
		va.AddBuffer(vb, layout);

		//shader
		Shader shader("res\\shaders\\Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.5f, 0.3f, 0.8f, 1.0f);
		shader.UnBind();

		va.UnBind();
		vb.UnBind();
		ib.UnBind();

		Render renderer;
		float r = 0.0f;
		float increment = 0.05f;
		//render loop
		while (!glfwWindowShouldClose(window))
		{
			//keyboard input 
			processInput(window);

			renderer.Clear();

			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			renderer.Draw(va, ib, shader);

			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;
			r += increment;		
			//glfw: swap buffers and poll IO events(keys pressed/released, mouse moved etc.)
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		//delete 
		//~
	}
	//glfw: terminate. clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


