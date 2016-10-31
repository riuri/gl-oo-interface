#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_color;

out vec4 f_color;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
  // compute the transformed and projected vertex position (into gl_Position)
  gl_Position = P * (V * (M * vec4(v_position, 1.0f)));    // uses P, V, M.

  // compute the vertex color (into col)
  f_color = vec4(v_color, 1.0);
}