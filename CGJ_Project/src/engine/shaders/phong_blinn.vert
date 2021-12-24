#version 430


uniform mat4 m_pvm;
uniform mat4 m_viewModel;
uniform mat3 m_normal;


in vec4 position;
in vec4 normal;
in vec4 textureCoords;
in vec4 tangent;


out Data {
	vec4 position;
	vec3 normal;
	vec3 eye;
	vec3 eyeDir;
} dataOut;




void main () {

	dataOut.position = m_viewModel * position;
	dataOut.normal = normalize(m_normal * normal.xyz);
	dataOut.eye = vec3(-dataOut.position);
	dataOut.eyeDir = -vec3(m_viewModel * position);
	
	gl_Position = m_pvm * position;
}