#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <stb/stb_image.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

const unsigned int width = 800;
const unsigned int height = 800;

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

int main()
{
    std::cout << "Initiliazing GLFW" << std::endl;
    /* Initiliaze GLFW library */
	glfwInit();

    /* Tells GLFW what version of OpenGL (3.3) */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    /* Tells GLFW to use the CORE profile */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window;

    std::cout << "Creating GLFW window" << std::endl;
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    /* Checks if window creation was succesful */
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Put the window in the current context */
    glfwMakeContextCurrent(window);

    std::cout << "Loading GLAD" << std::endl;
    /* Load GLAD for it to be configured */
    gladLoadGL();

    /* Specifyt the viewport */
    glViewport(0, 0, width, height);

    std::cout << "Creating shader program" << std::endl;
    Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag");
    // Create and bind the VAO
    VAO VAO1;
    VAO1.Bind();

    // Create the VBO and EBO
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    // Link the VAO's
    /* Vertices attrib */
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    /* Color attrib */
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    /* Texture Attrib */
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    /* Unbind all to prevent modifying them */
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Get the uniform location for the scale
    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    // Texture
    Texture brick("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    brick.texUnit(shaderProgram, "tex0", 0);

    // Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    /* Main render loop */
    while (!glfwWindowShouldClose(window)) 
    {
        /* Set the background color */
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /* Specify the shader program */
        shaderProgram.Activate();

        /* Pass data into the uniform */
        glUniform1f(uniID, 1.0f);

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

        /* Bind the texture so it appears in rendering */
        brick.Bind();
        /* Bind the VAO so OpenGL uses it */
        VAO1.Bind();
        /* Draw using the GL_TRIANGLES primitive */
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
        /* Swap the front and back buffer */
        glfwSwapBuffers(window);
        /* Take care of GLFW events */
        glfwPollEvents();
    }

    /* Delete all of the objects */
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    brick.Delete();
    shaderProgram.Delete();

    /* Terminates and deletes window */
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}