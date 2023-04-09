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
    GLFWwindow* window = glfwCreateWindow(950, 950, "test", NULL, NULL);

    // Set the current time of GLFW to 0
    glfwSetTime(0);

    // Define the vertices of a square
    GLfloat squareVertices[] =
    { // COORDINATES       / COLORS              / TexCoord      / NORMALS             / TANGENTS            / BITANGENTS //
    -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 0.0f,     0.0f, 0.0f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 1.0f, // Lower left corner
    -0.5f, 0.5f, 0.0f,     0.0f, 0.0f, 0.0f,     0.0f, 1.0f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 1.0f, // Upper left corner
    0.5f, 0.5f, 0.0f,      0.0f, 0.0f, 0.0f,     1.0f, 1.0f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 1.0f, // Upper right corner
    0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 0.0f,     1.0f, 0.0f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 1.0f, // Lower right corner
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
    unsigned char* bytes = stbi_load("Rubik.jpg", &widthTx, &heightTx, &numCol, 0);

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

    // Link VBO data to VAO attributes
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 17 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 17 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 17 * sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 17 * sizeof(float), (void*)(8 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 4, 3, GL_FLOAT, 17 * sizeof(float), (void*)(11 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 5, 3, GL_FLOAT, 17 * sizeof(float), (void*)(14 * sizeof(float)));

    // Unbind VAO, VBO, and EBO
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Get uniform locations from shader program
    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
    GLuint pScale = glGetUniformLocation(shaderProgram.ID, "parallaxScale");
    GLuint texD = glGetUniformLocation(shaderProgram.ID, "texDiffuse");
    GLuint texH = glGetUniformLocation(shaderProgram.ID, "texHeight");
    GLuint texN = glGetUniformLocation(shaderProgram.ID, "texNormal");

    // Activate shader program and set texture uniforms
    shaderProgram.Activate();
    glUniform1i(texD, 0);
    glUniform1i(texH, 0);
    glUniform1i(texN, 0);

    while (!glfwWindowShouldClose(window))
    {
        // Binds the texture to be used
        glBindTexture(GL_TEXTURE_2D, texture);

        // Clears the color buffer with a black color
        glClearColor(0.0f, 0.0f, 0.0f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // Calculates the scale factor for parallax effect based on time
        float pS = sin(glfwGetTime()) * 1.0f + 3.0f;

        // Activates the shader program
        shaderProgram.Activate();

        // Sets the value of the "scale" uniform variable to 0.5
        glUniform1f(uniID, 0.5f);

        // Sets the value of the "parallaxScale" uniform variable to pS
        glUniform1f(pScale, pS);

        // Binds the vertex array object
        VAO1.Bind();

        // Draws the elements of the vertex array object using the shader program
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swaps the front and back buffers of the window
        glfwSwapBuffers(window);

        // Processes events from the window
        glfwPollEvents();
    }

    // Deletes the vertex array object, vertex buffer object, and element buffer object
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();

    // Deletes the shader program
    shaderProgram.Delete();

    // Sets the viewport to the entire window size
    glViewport(0, 0, 950, 950);

    // Swaps the front and back buffers of the window
    glfwSwapBuffers(window);

    // Destroys the window and terminates GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    // Exits the program with a success status code
    return 0;

}