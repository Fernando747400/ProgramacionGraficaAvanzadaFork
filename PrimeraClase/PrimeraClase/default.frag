// Declare the version of GLSL to use
#version 330 core

// Declare the output variable for the fragment shader
out vec4 FragColor;

// Declare the input variable for the fragment shader, which receives texture coordinates from the vertex shader
in vec2 texCoord;

// Declare a uniform sampler2D variable, which references the texture to be sampled
uniform sampler2D tex0;

void main()
{
    // Create a modified texture coordinate by adding an offset to the original texture coordinates
    vec2 modifiedTexCoord = texCoord;

    // Sample the texture at the modified texture coordinate and assign the resulting color to a vec4 variable
    vec4 textureColor = texture(tex0, modifiedTexCoord);

    // Assign the texture color to the output variable of the fragment shader
    FragColor = textureColor;
}

