#version 430

uniform mat4 m_pvm;
uniform mat4 m_viewModel;
uniform mat3 m_normal;


uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float shininess;

uniform vec4 l_pos;

in vec4 position;
in vec4 normal;

out Data {
	vec3 normal;
	vec3 eye;
	vec3 lightDir;
} DataOut;

out vec4 vColor;

void main () {

	vec3 v = vec3(m_viewModel * position);
	vec3 N = normalize(m_normal * vec3(normal));
	
	vec3 L = normalize(l_pos.xyz - v);
	vec3 E = normalize(-v);
	vec3 R = normalize(-reflect(L, N));

	vec4 iAmb = ambient;
	vec4 iDiff = diffuse * max(dot(N, L), 0.0);
	vec4 iSpec = specular * pow(max(dot(R, E), 0.0), 0.3 * shininess);

	vColor = vec4(iAmb + iDiff + iSpec);
	gl_Position = m_pvm * position;
}