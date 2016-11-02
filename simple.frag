#version 330 core

in vec2 v_texcoord;
in vec2 v_bound;
uniform sampler2D u_texture;
uniform float u_time;

out vec4 frag_color;

const float pi = 3.1415926535897932384626433832795;

void main ()
{
  frag_color.rgb = texture2D(u_texture, v_texcoord).rgb;
  // add time-dependent visual cue to better detect X freeze
  frag_color.rgb *= sin(u_time * pi);
  frag_color.a = 1.0;
}
