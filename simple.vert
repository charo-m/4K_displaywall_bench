#version 440 core

in vec3 a_position;
in vec2 a_texcoord;

uniform mat4 u_modelView;
uniform mat4 u_projection;

out vec2 v_texcoord;
out vec2 v_bound;

void main () {
  // flip vertical texture coords instead of pixels
  v_texcoord.x = a_texcoord.x;
  v_texcoord.y = 1.0 - a_texcoord.y;

  v_bound = (u_modelView * vec4(a_position.xyz, 1.0)).xy;

  gl_Position = u_projection * u_modelView * vec4(a_position.xyz, 1.0);
}
