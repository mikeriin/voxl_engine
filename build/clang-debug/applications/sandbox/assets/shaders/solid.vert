#version 460 core


layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;
layout (location = 3) in vec3 in_color;


out VERT_OUT {
  vec3 color;
} frag;


void main() {
  gl_Position = vec4(in_position, 1.0);
  frag.color = in_color;
}