#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 texCoord;

void main() {
    //gl_Position = vec4(aPos, 1.0);
    gl_Position = proj * view * model * vec4(aPos, 1.0);

    texCoord = aTexCoord;
    texCoord.x = (texCoord.x - 0.5) * 0.46 + 0.5;
}
