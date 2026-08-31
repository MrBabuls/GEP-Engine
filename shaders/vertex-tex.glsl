#version 460 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float layer;
uniform vec2 uvOffset;
uniform vec2 uvScale;
uniform bool flipX;


void main()
{
    gl_Position = projection * view * model * vec4(aPos, layer, 1.0);
    vec2 uv = aTexCoord;

    if (flipX)
        uv.x = 1.0 - uv.x;

    TexCoord = uv * uvScale + uvOffset;
}