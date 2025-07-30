#version 300 es
precision mediump float;
out vec4 FragColor;
in vec2 v_uv;

const int MAX_LIGHTS = 10;

uniform int lightCount;
uniform vec2 lightPositions[MAX_LIGHTS];
uniform vec2 screenSize;

uniform float radius;
uniform float softness;

uniform float baseDarkness; 

void main() {
   vec2 fragCoord = v_uv * screenSize;
    float finalLight = 0.0;

    for (int i = 0; i < lightCount; ++i) {
        float d = length(fragCoord - lightPositions[i]);
        float light = clamp(1.0 - (d - radius) / softness, 0.0, 1.0);
        light = light * light;
        finalLight = max(finalLight, light);
    }

    float darkness = clamp(baseDarkness - finalLight, 0.0, 1.0);
    FragColor = vec4(0.0, 0.0, 0.0, darkness);
}