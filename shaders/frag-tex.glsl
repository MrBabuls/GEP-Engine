#version 460 core
in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 spriteColor;
uniform bool useTexture;

out vec4 FragColor;

void main()
{
    vec4 texColor = vec4(1.0);
    if (useTexture)
        texColor = texture(texture1, TexCoord);

    FragColor = texColor * vec4(spriteColor, 1.0);
}