#version 400 
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCord;

out vec2 fTexCord;

uniform mat4 transform;
uniform mat4 projection;

void main()
{
    gl_Position = projection * transform * vec4(aPos.xy, 0.0, 1.0);
    fTexCord = aTexCord;
}