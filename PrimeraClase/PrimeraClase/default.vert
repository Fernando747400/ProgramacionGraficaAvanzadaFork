// Sets the version of the GLSL language to be used
#version 330 core

// Specifies the layout and location of input variables
layout (location = 0) in vec3 aPos;        // Position
layout (location = 1) in vec3 aColor;      // Color
layout (location = 2) in vec2 aTex;        // Texture coordinates
layout (location = 3) in vec3 aNormal;     // Normal vector
layout (location = 4) in vec3 aTangent;    // Tangent vector
layout (location = 5) in vec3 aBitangent;  // Bitangent vector

// Declares output variables to be passed to the fragment shader
out vec3 color;
out vec2 texCoord;
out vec3 T;
out vec3 B;
out vec3 N;

// Declares a uniform variable (same value for all vertices)
uniform float scale;

// Function to calculate the TBN matrix based on the given normal, tangent, and bitangent vectors
mat3 calcTBN(vec3 normal, vec3 tangent, vec3 bitangent)
{
    mat3 TBN = mat3(tangent, bitangent, normal);  // Constructs a matrix from the input vectors
    return TBN;  // Returns the resulting TBN matrix
}

// Main function of the vertex shader
void main()
{
    // Calculates the position of the vertex by scaling the input position vector in all three dimensions
    gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
    // Passes the input color and texture coordinate to the fragment shader
    color = aColor;
    texCoord = aTex;

    // Calculates the TBN matrix based on the input normal, tangent, and bitangent vectors
    mat3 TBN = calcTBN(normalize(aNormal), normalize(aTangent), normalize(aBitangent));
    // Extracts the tangent, bitangent, and normal vectors from the TBN matrix and passes them to the fragment shader
    T = TBN[0];
    B = TBN[1];
    N = TBN[2];
}
