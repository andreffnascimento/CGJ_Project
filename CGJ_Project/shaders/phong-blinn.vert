#version 430

uniform mat4 m_pvm;
uniform mat4 m_viewModel;
uniform mat3 m_normal;

uniform vec4 l_pos;

in vec4 position;
in vec4 normal;    //por causa do gerador de geometria

out Data {
	vec3 normal;
	vec3 eye;
	vec3 eyeDir;
	vec3 lightDir;
	vec4 pos;
} DataOut;

void main () {

	DataOut.pos = m_viewModel * position;

	DataOut.normal = normalize(m_normal * normal.xyz);
	DataOut.lightDir = vec3(l_pos - DataOut.pos);
	DataOut.eye = vec3(-DataOut.pos);
	DataOut.eyeDir = -vec3(m_viewModel * position);

	gl_Position = m_pvm * position;	
}