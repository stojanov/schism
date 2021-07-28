#version 400

in vec2 fTexCord;

uniform sampler2D sprite;

out vec4 FragColor;

void main()
{
    FragColor = texture(sprite, fTexCord);
}