// Declare the version of GLSL to use
#version 330 core

// Declare the output variable for the fragment shader
out vec4 FragColor;

// Declare the input variable for the fragment shader, which receives texture coordinates from the vertex shader
in vec2 texCoord;

// Declare a uniform sampler2D variable, which references the texture to be sampled
uniform sampler2D tex0;

// Declare a uniform float variable for time
uniform float time;

// Define a function to interpolate smoothly between two values
float smoothstep(float edge0, float edge1, float x) {
    float t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}

// Define a function to generate Perlin noise at a given position and time
float perlinNoise(vec2 uv, float t) {
    vec2 i = floor(uv + vec2(t, 0.0));
    vec2 f = fract(uv + vec2(t, 0.0) * 0.5f);

    // Smoothly interpolate between the four corners of the grid cell
    float a = dot(texture(tex0, i + vec2(0.0, 0.0)).rgb, vec3(0.25));
    float b = dot(texture(tex0, i + vec2(1.0, 0.0)).rgb, vec3(0.25));
    float c = dot(texture(tex0, i + vec2(0.0, 1.0)).rgb, vec3(0.25));
    float d = dot(texture(tex0, i + vec2(1.0, 1.0)).rgb, vec3(0.25));
    float s = smoothstep(0.0, 1.0, f.x);
    float u = smoothstep(0.0, 1.0, f.y);
    float ab = mix(a, b, s);
    float cd = mix(c, d, s);
    float abc = mix(ab, cd, u);

    // Scale the result to the range [-1, 1]
    return 2.0 * abc - 1.0;
}

void main()
{
    // Create a modified texture coordinate by adding an offset to the original texture coordinates
    vec2 modifiedTexCoord = texCoord;

    // Generate improved Perlin noise at the modified texture coordinate and time, and scale the result
    float noiseValue = perlinNoise(modifiedTexCoord + vec2(time * 0.2, time * 0.3), time) * 0.5f * time;

    // Modify the texture coordinates by adding the Perlin noise value
    modifiedTexCoord += vec2(noiseValue);

    // Sample the texture at the modified texture coordinate and assign the resulting color to a vec4 variable
    vec4 textureColor = texture(tex0, modifiedTexCoord);

    // Assign the texture color to the output variable of the fragment shader
    FragColor = textureColor;
}






