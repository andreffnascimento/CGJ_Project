#version 430

uniform mat4 m_pvm;
uniform mat4 m_viewModel;
uniform mat3 m_normal;

struct Materials {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform Materials mat;

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

	vec4 iAmb = mat.ambient;
	vec4 iDiff = mat.diffuse * max(dot(N, L), 0.0);
	vec4 iSpec = mat.specular * pow(max(dot(R, E), 0.0), 0.3 * mat.shininess);

	vColor = vec4(iAmb + iDiff + iSpec);
	gl_Position = m_pvm * position;
}