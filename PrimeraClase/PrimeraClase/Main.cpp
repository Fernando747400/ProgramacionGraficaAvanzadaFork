#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include<stb/stb_image.h>

int main()
{
    // Initialize GLFW library
    glfwInit();

    // Set GLFW version hints to use OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window object
    GLFWwindow* window = glfwCreateWindow(1000, 1000, "test", NULL, NULL);

    // Set the current time of GLFW to 0
    glfwSetTime(0);

    // Define the vertices of a square
    GLfloat squareVertices[] =
    { //     COORDINATES     /        COLORS      /   TexCoord  //
    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,    0.0f, 0.0f, // Lower left corner
    -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,    0.0f, 1.0f, // Upper left corner
     0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,    1.0f, 1.0f, // Upper right corner
     0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,    1.0f, 0.0f  // Lower right corner
    };

    // Define the indices that make up the square
    GLuint squareIndices[] =
    {
     0, 2, 1,
     0, 3, 2
    };

    // Make the window the current context for OpenGL rendering
    glfwMakeContextCurrent(window);

    // Load OpenGL function pointers with GLAD
    gladLoadGL();

    // Create a new texture
    GLuint texture;
    glGenTextures(1, &texture);

    // Set active texture unit to 0 and bind the created texture to a 2D texture target
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Set texture wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Load texture data from file
    int widthTx, heightTx, numCol;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load("SadCat.jpg", &widthTx, &heightTx, &numCol, 0);

    // Print texture information
    std::cout << widthTx << std::endl;
    std::cout << heightTx << std::endl;
    std::cout << numCol << std::endl;

    // Specify texture data and format
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthTx, heightTx, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);

    // Generate texture mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create shaders
    Shader shaderProgram("default.vert", "default.frag");

    // Create and bind a new Vertex Array Object
    VAO VAO1;
    VAO1.Bind();

    // Create and bind a new Vertex Buffer Object and Element Buffer Object
    VBO VBO1(squareVertices, sizeof(squareVertices));
    EBO EBO1(squareIndices, sizeof(squareIndices));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
    GLuint tex0uni = glGetUniformLocation(shaderProgram.ID, "tex0");
    GLuint timeUni = glGetUniformLocation(shaderProgram.ID, "time");

    shaderProgram.Activate();
    glUniform1i(tex0uni, 0);

    while (!glfwWindowShouldClose(window))
    {
        glBindTexture(GL_TEXTURE_2D, texture);
        float timeValue = glfwGetTime();
        std::cout << timeValue << std::endl;


        glClearColor(0.0f, 0.0f, 0.0f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // Dibujar la geometría con la textura animada con ruido de Perlin aplicado
        shaderProgram.Activate();

        // Set the value of the uniform float for time using glUniform1f or similar
        glUniform1f(timeUni, timeValue);
        glUniform1f(uniID, 0.5f);
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Intercambiar buffers y manejar eventos
        glfwSwapBuffers(window);
        glfwPollEvents();
    }



    // Deletes the vertex array object, vertex buffer object, and element buffer object
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();

    // Deletes the shader program
    shaderProgram.Delete();

    // Sets the viewport to the entire window size
    glViewport(0, 0, 1000, 1000);

    // Swaps the front and back buffers of the window
    glfwSwapBuffers(window);

    // Destroys the window and terminates GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    // Exits the program with a success status code
    return 0;

}