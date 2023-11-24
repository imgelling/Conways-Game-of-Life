#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 vUV;
layout (location = 2) in vec4 incolor;

uniform sampler2D tex;
uniform mat4 ortho;

out vec2 UV;
out vec4 Color;

void main()
{
    gl_Position = ortho * vec4(vec3(position, 0.0f), 1.0f);
    UV = vUV;
    Color = vec4(1.0f, 1.0f, 1.0f, 1.0f) * incolor;
}