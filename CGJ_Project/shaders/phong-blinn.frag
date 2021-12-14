#version 430

out vec4 colorOut;

struct Materials {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	float shininess;
	int texCount;
};

struct LightProperties {
	int isEnabled;
	int lightType;
	vec4 position;
	vec4 lightDir;
	vec4 coneDirection;
	float spotCutOff;
};

const int numLights = 7;
uniform LightProperties Lights[numLights];

uniform Materials mat;

in Data {
	vec3 normal;
	vec3 eye;
	vec3 eyeDir;
	vec3 lightDir;
	vec4 pos;
} DataIn;

void main() {

	vec3 l_dir, s_dir;
	float intensity;

	vec4 spec = vec4(0.0);

	vec3 n = normalize(DataIn.normal);
	vec3 e = normalize(DataIn.eye);
	colorOut = vec4(0, 0, 0, 0);


	int i;
	for(i = 0; i < numLights; i++) {
		if(Lights[i].isEnabled == 0){
			continue;
		}
		
		if(Lights[i].lightType == 1) { // Direct light		
			l_dir = normalize(vec3(Lights[i].lightDir));
			e = normalize(vec3(DataIn.eyeDir));
		}
		else if(Lights[i].lightType == 2) { // Point light
			e = normalize(vec3(DataIn.eye));	
			l_dir = normalize(vec3(Lights[i].position - DataIn.pos));
		}
		else if(Lights[i].lightType == 3) { // Spot light
			l_dir = normalize(vec3(Lights[i].position - DataIn.pos));
			s_dir = normalize(vec3(-Lights[i].coneDirection));  
			e = normalize(vec3(DataIn.eye));	

			// inside the cone?
			if(!(dot(s_dir, l_dir) > Lights[i].spotCutOff)) {
				continue;
			}
		}
		
		intensity = max(dot(n, l_dir), 0.0);

		if (intensity > 0.0) {

			vec3 h = normalize(l_dir + e);
			float intSpec = max(dot(h,n), 0.0);
			spec = mat.specular * pow(intSpec, mat.shininess);	
			colorOut += max(intensity * mat.diffuse + spec, mat.ambient);
		}

	}

	colorOut = vec4(vec3(max(colorOut, mat.ambient)),mat.diffuse.a);
	
}