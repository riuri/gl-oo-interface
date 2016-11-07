#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_color;

out vec4 f_color;

uniform mat4 MVP;

void main()
{
  // Transform and project input vertex using model-view-projection matrix.
  gl_Position = MVP * vec4(v_position, 1.0f);  // P*V*M * vpos.

  // Compute the vertex color (into f_color) to be interpolated.
  f_color = vec4(v_color, 1.0);
}