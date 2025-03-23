#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;
out vec4 finalColor;

uniform float utime;

void main() {
    float pulse = 0.5 + 0.5 * sin(utime);
    finalColor = mix(fragColor, vec4(0.0, 0.0, 1.0, 1.0), pulse);
}
