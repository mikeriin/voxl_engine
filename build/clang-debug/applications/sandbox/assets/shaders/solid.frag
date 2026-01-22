#version 460 core


out vec4 FragColor;


in VERT_OUT {
  vec3 color;
} frag;


void main() {
  FragColor = vec4(frag.color, 1.0);
}