#version 430


const uint MAX_LIGHTS = 32;


struct VertexData {
	vec4 position;
	vec3 normal;
	vec3 eye;
	vec3 eyeDir;
};

struct MaterialData {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	float shininess;
	int texCount;
};

struct LightingData {
	uint nLights;
	uint lightTypes[MAX_LIGHTS];
	vec4 lightPositions[MAX_LIGHTS];
	vec4 lightDirections[MAX_LIGHTS];
	float lightIntensities[MAX_LIGHTS];
	float lightCutOffs[MAX_LIGHTS];
};

struct FragLightingData {
	vec4 specular;
	vec4 diffuse;
};




uniform MaterialData material;
uniform LightingData lighting;


in Data {
	vec4 position;
	vec3 normal;
	vec3 eye;
	vec3 eyeDir;
} dataIn;


out vec4 colorOut;




FragLightingData processDirectionalLight(FragLightingData fragLighting, uint index, vec3 normal, vec3 eye) {
	return fragLighting;
}


FragLightingData processPointLight(FragLightingData fragLighting, uint index, vec3 normal, vec3 eye) {
	vec3 lightDir = vec3(lighting.lightPositions[index].xyz - dataIn.position.xyz);
	float distance = length(lightDir);
	float attenuation = lighting.lightIntensities[index] / (1.0 + 0.9 * distance + 0.09 * distance * distance);
	lightDir = normalize(lightDir);

	float diffuseIntensity = max(dot(normal, lightDir), 0);
	if (diffuseIntensity > 0.0) {
		vec3 h = (lightDir + eye) / 2.0;
		float specularIntensity = max(pow(dot(normal, h), material.shininess), 0.0f);
		fragLighting.diffuse += material.diffuse * diffuseIntensity * attenuation;
		fragLighting.specular += material.specular * specularIntensity * attenuation;
	}

	return fragLighting;
}


FragLightingData processSpotLight(FragLightingData fragLighting, uint index, vec3 normal, vec3 eye) {
	return fragLighting;
}




void main() {

	colorOut = vec4(0.0);

	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);

	vec3 normal = normalize(dataIn.normal);
	vec3 eye = normalize(dataIn.eye);


	FragLightingData fragLighting;
	fragLighting.diffuse = vec4(0.0);
	fragLighting.specular = vec4(0.0);

	for (int i = 0; i < lighting.nLights; i++) {
		switch(lighting.lightTypes[i])
		{
		case 1:		// directional light
			fragLighting = processDirectionalLight(fragLighting, i, normal, eye);
			break;

		case 2:		// point light
			fragLighting = processPointLight(fragLighting, i, normal, eye);
			break;

		case 3:		// spot light
			fragLighting = processSpotLight(fragLighting, i, normal, eye);
			break;
		}
	}

	colorOut = material.ambient + fragLighting.diffuse + fragLighting.specular;
}