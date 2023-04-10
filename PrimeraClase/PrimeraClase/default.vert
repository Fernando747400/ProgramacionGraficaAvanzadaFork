// Declare the version of GLSL to use
#version 330 core

// Specify the location of the vertex input "aPos"
layout (location = 0) in vec3 aPos;

// Specify the location of the vertex input "aColor"
layout (location = 1) in vec3 aColor;

// Specify the location of the vertex input "aTex"
layout (location = 2) in vec2 aTex;

// Declare a variable "color" to be outputted by the vertex shader
out vec3 color;

// Declare a variable "texCoord" to be outputted by the vertex shader
out vec2 texCoord;

// Declare a uniform variable "scale" of type float
uniform float scale;

// Declare a uniform variable "offset" of type vec2
uniform vec2 offset;

void main()
{
	// Calculate the position of the vertex using the scale factor and the input position "aPos"
	gl_Position = vec4(
	aPos.x + aPos.x * scale,
	aPos.y + aPos.y * scale,
	aPos.z + aPos.z * scale, 1.0);

	// Output the input color "aColor" as "color"
	color = aColor;

	// Add the input texture coordinate "aTex" and the offset "offset" and output the result as "texCoord"
	texCoord = aTex + offset;
}

