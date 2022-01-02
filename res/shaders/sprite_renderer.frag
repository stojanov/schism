#version 400

in vec2 fTexCord;

uniform sampler2D sprite;

out vec4 FragColor;

void main()
{
    vec4 clr = texture(sprite, fTexCord);

    if (clr.a <= 0.0) {
        discard;
    }

    FragColor = texture(sprite, fTexCord);
}