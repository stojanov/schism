#version 400
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;


uniform vec4 projection;

out vec2 oTexCoord;

void main()
{
    gl_Position = projection * vec4(aPos.xy, 0.0, 1.0);
    oTexCoord = aTexCoord;
}