#version 330 core
in vec3 position; // the position variable has attribute position 0
in vec3 normal;  

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;

out vec4 vertexColor; // specify a color output to the fragment shader

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    vertexColor = vec4(color, 1.0 + 0.0001*normal.x); // set the output variable to a dark-red color
}