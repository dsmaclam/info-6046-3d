#version 330 core
in vec3 position; // the position variable has attribute position 0
in vec3 normal;  

uniform vec3 color;

out vec4 vertexColor; // specify a color output to the fragment shader

void main()
{
    gl_Position = vec4(position, 1.0); // see how we directly give a vec3 to vec4's constructor
    vertexColor = vec4(color, 1.0 + 0.0001*normal.x); // set the output variable to a dark-red color
}