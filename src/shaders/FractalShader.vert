#version 410 core
layout(location = 0) in vec2 aPos;

out vec2 pos;

void main(){
  gl_Position = vec4(aPos, 0.0, 1.0); // see how we directly give a vec3 to vec4's constructor
  pos = aPos;
}