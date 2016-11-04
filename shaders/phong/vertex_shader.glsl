#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec3 v_uv;

out vec4 f_position;  // Fragment position in camera coordinates.
out vec4 f_normal;    // Fragment normal in camera coordinates.
out vec4 f_uv;        // Fragment uv coordinates.

uniform mat4 M;  // Model matrix.
uniform mat4 V;  // View  matrix.
uniform mat4 P;  // Projection matrix.
uniform mat4 N;  // Normal matrix N = (VM)^-t.

void main()
{
  // Compute the vertex position in world coordinates.
  f_position = V * (M * vec4(v_position, 1.0f));
  f_position = f_position/f_position.w;

  // Then project f_position onto screen and store into gl_Position.
  gl_Position = P * f_position;

  // Transform the vertex normal vector.
  f_normal = N * vec4(v_normal, 1.0f);

  // Pass uv coordinates to be interpolated.
  f_uv = vec4(v_uv, 1.0f);
}