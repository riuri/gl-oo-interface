#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_color;

out vec4 f_position;
out vec4 f_color;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{

  // Compute the vertex position in world coordinates.
  f_position = V * (M * vec4(v_position, 1.0f));
  f_position = f_position/f_position.w;

  // Then project it onto screen and store into gl_Position.
  gl_Position = P * f_position;

  // Compute the vertex color (into f_color).
  f_color = vec4(v_color, 1.0); 
}