#version 330

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_color;
layout (location = 3) in vec2 in_uv;

out vec4 v_color;
out vec3 v_normal;
out vec2 v_tex_coord;
out vec4 f_pos;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

//uniform mat4 PV;
//uniform mat4 MV;  // Model-view matrix.
//uniform mat4 MVP; // Model-view-projection matrix.

void main()
{
  // compute the transformed and projected vertex position (into gl_Position)
  gl_Position = P * (V * (M * vec4(in_position, 1.0f)));    // uses P, V, M
  //gl_Position = P * (MV * vec4(in_position, 1.0f));       // uses P, model-view
  //gl_Position = MVP * vec4(in_position, 1.0f);            // uses model-view-proj
  //gl_Position = PV * M * vec4(in_position, 1.0f);         // uses view-proj, M

  // compute the vertex color (into col)
  v_color = vec4(in_color, 1.0);
  v_tex_coord = in_uv;

  // Fragment position computation in camera coordinates.
  f_pos = V * (M * vec4(in_position, 1.0f));
  f_pos = f_pos/f_pos.w;

  // Normal computation in camera coordinates.
  v_normal = (V * (inverse(transpose(M)) * vec4(in_normal, 0.0))).xyz;
}