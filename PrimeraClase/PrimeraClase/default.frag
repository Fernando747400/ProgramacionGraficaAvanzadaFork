// Sets the version of the GLSL language to be used
#version 330 core

// Specifies the layout and location of input variables
in vec2 texCoord;  // Texture coordinates
in vec3 T;         // Tangent vector
in vec3 B;         // Bitangent vector
in vec3 N;         // Normal vector

// Declares output variable to be passed to the framebuffer
out vec4 FragColor;

// Declares uniform variables (same value for all fragments)
uniform sampler2D texDiffuse;   // Diffuse texture
uniform sampler2D texNormal;    // Normal map texture
uniform sampler2D texHeight;    // Height map texture
uniform float parallaxScale;    // Parallax mapping scale factor

// Main function of the fragment shader
void main()
{
    // Parallax mapping
    float height = texture(texHeight, texCoord).r;                             // Gets the height value from the height map texture
    vec2 p = parallaxScale * height * normalize(vec2(dot(N, T), dot(N, B)));  // Calculates the parallax displacement based on the height, normal, and tangent vectors
    vec2 newTexCoord = texCoord + p;                                           // Calculates the new texture coordinates based on the displacement

    // Sample textures
    vec3 diffuse = texture(texDiffuse, newTexCoord).rgb;                       // Samples the diffuse texture at the new texture coordinates
    vec3 normal = texture(texNormal, newTexCoord).rgb;                         // Samples the normal map texture at the new texture coordinates
    normal = normalize(normal * 2.0 - 1.0);                                     // Normalizes the normal vector

    // Calculate lighting
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));                             // Defines the direction of the light
    float diffuseFactor = max(dot(normal, lightDir), 1.0);                      // Calculates the diffuse lighting factor based on the normal vector and light direction
    vec3 color = diffuse * diffuseFactor;                                       // Calculates the final color by multiplying the diffuse texture with the diffuse lighting factor

    FragColor = vec4(color, 1.0);                                               // Outputs the final color to the framebuffer
}
