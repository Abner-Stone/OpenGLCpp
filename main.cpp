#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

GLfloat vertices[] =
{
    -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, /* Lower Left Corner */
    0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, /* Lower Right Corner */
    0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, /* Upper Middle Corner */
    -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, /* Inner Left */
    0.5 / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, /* Inner Right */
    0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f /* Inner Down */

};

GLuint indices[] =
{
    0, 3, 5, /* Lower Left Triangle */
    3, 2, 4, /* Lower Right Triangle */
    5, 4, 1 /* Upper Triangle */
};

int main()
{
    /* Initiliaze GLFW library */
	glfwInit();

    /* Tells GLFW what version of OpenGL (3.3) */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    /* Tells GLFW to use the CORE profile */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    

    GLFWwindow* window;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "Hello World", NULL, NULL);
    /* Checks if window creation was succesful */
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Put the window in the current context */
    glfwMakeContextCurrent(window);

    /* Load GLAD for it to be configured */
    gladLoadGL();

    /* Specifyt the viewport */
    glViewport(0, 0, 800, 800);

    Shader shaderProgram("default.vert", "default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkVBO(VBO1, 0);
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    /* Main render loop */
    while (!glfwWindowShouldClose(window)) 
    {
        /* Set the background color */
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        /* Specify the shader program and bind VAO */
        shaderProgram.Activate();
        VAO1.Bind();
        /* Draw using the GL_TRIANGLES primitive */
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        /* Swap the front and back buffer */
        glfwSwapBuffers(window);
        /* Take care of GLFW events */
        glfwPollEvents();
    }

    /* Delete all of the objects */
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();

    /* Terminates and deletes window */
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}