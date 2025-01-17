#version 330
layout (location = 0) in vec3 _pos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main(){
    gl_Position =  model * vec4(_pos, 1.0);
}