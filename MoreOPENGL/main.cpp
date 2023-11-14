#include <GL/glew.h>
#define GL_SILENCE_DEPRECATION
#include <OpenGL/OpenGL.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Shader.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEGIHT 600
#define LOG(x) std::cout << x <<  '\n'

using namespace glm;

bool wireframeMode = false;
bool isKeyHeld = false;




static void frameBufferResizeCallBack( GLFWwindow* window, int w ,int h )
{
    glViewport(0, 0, w, h);
}

static void getInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)== GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        if(!isKeyHeld)
        {
            isKeyHeld = true;
            if(wireframeMode)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                wireframeMode = false;
            }
            else{
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                wireframeMode = true;
            }
            
        }
    }
    else{
        isKeyHeld = false;
    }
        
    
    
}

static void updateTriangle(GLFWwindow* window)
{
    
}

static void update(GLFWwindow* window)
{
    getInput(window);
    updateTriangle(window);
    
}

int main(void)
{

    GLFWwindow* window;



    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEGIHT, "OPENGL", NULL, NULL);


    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwSetWindowOpacity(window, 1.f);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    glewInit();
    /* Create a windowed mode window and its OpenGL context */
    
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEGIHT);
    glfwSetFramebufferSizeCallback(window, frameBufferResizeCallBack);
    
   
    // ------------------------------------------------------------------
    float vertices[] = {
        // POSITIONS             COLORS
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // top right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom right
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // bottom left
    };
    
    float* colors[] = {
        &vertices[3], &vertices[4], &vertices[5],
        &vertices[9], &vertices[10], &vertices[11],
        &vertices[15], &vertices[16], &vertices[17]
    };
    
    
    
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    // POSITIONS
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*) 0);
    glEnableVertexAttribArray(0);
    
    // COLORS
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*) (3* sizeof(float)));
    glEnableVertexAttribArray(1);
    
    
    
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindVertexArray(VAO);
    
    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    
    
    
    // unsigned int shader = createShader(vshader, fshader);
    // glUseProgram(shader);
    Shader ourShader("/MoreOPENGL/Shaders/Basic.shader");
    
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // update
        update(window);
        
        
        // render
        // ------
        glClearColor(0, 0, 0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // updating the color
         
        float timeVal = glfwGetTime();
        float PI = 3.14159265359;
        
        float red = sin(timeVal);
        float green = sin(timeVal + 0.33f * 2.0 * PI);
        float blue = sin(timeVal + 0.66 * 2.0 * PI);
        
        ourShader.setVec4f("rainbowColor", red, green, blue, 1.0);
        
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        ourShader.use();
        
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VAO);
    
    glfwTerminate();
    return 0;
}
