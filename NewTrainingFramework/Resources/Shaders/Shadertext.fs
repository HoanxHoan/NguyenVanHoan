#version 300 es
precision mediump float;
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    float alpha = texture(text, TexCoords).r;
    color = vec4(textColor, alpha);
}
