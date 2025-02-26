#version 460 core
layout (location = 0) in vec4 vertex;

uniform mat4 u_Model;
uniform mat4 u_Ortho;

void main()
{
    gl_Position = u_Ortho * u_Model * vec4(vertex.xy, 0.0, 1.0);
}