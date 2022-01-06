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


struct TextureData {
	int textMode;
	sampler2D texmap0;
	sampler2D texmap1;
	sampler2D texmap2;
};


struct LightingData {
	uint nLights;
	uint lightTypes[MAX_LIGHTS];
	vec4 lightPositions[MAX_LIGHTS];
	vec4 lightDirections[MAX_LIGHTS];
	float lightIntensities[MAX_LIGHTS];
	float lightCutOffs[MAX_LIGHTS];

	float ambientCoefficient;
	float diffuseCoefficient;
	float specularCoefficient;
};


struct FragLightingData {
	vec4 specular;
	vec4 diffuse;
};




uniform MaterialData materialData;
uniform TextureData textureData;
uniform LightingData lightingData;


in Data {
	vec4 position;
	vec3 normal;
	vec3 eye;
	vec3 eyeDir;
} dataIn;


out vec4 colorOut;




FragLightingData processDirectionalLight(FragLightingData fragLighting, uint index, vec3 normal, vec3 eye) {
	vec3 direction = normalize(-lightingData.lightDirections[index].xyz);
	float attenuation = lightingData.lightIntensities[index];

	float diffuseIntensity = max(dot(normal, direction), 0.0);
	if (diffuseIntensity > 0.0) {
		vec3 h = (direction + eye) / 2.0;
		float specularIntensity = pow(max(dot(normal, h), 0.0), materialData.shininess);
		fragLighting.diffuse += materialData.diffuse * diffuseIntensity * attenuation;
		fragLighting.specular += materialData.specular * specularIntensity * attenuation;
	}

	return fragLighting;
}


FragLightingData processPointLight(FragLightingData fragLighting, uint index, vec3 normal, vec3 eye) {
	vec3 lightDir = vec3(lightingData.lightPositions[index].xyz - dataIn.position.xyz);
	float distance = length(lightDir);
	float attenuation = lightingData.lightIntensities[index] / (1.0 + 0.1 * distance + 0.01 * distance * distance);
	lightDir = normalize(lightDir);

	float diffuseIntensity = max(dot(normal, lightDir), 0.0);
	if (diffuseIntensity > 0.0) {
		vec3 h = (lightDir + eye) / 2.0;
		float specularIntensity = pow(max(dot(normal, h), 0.0), materialData.shininess);
		fragLighting.diffuse += materialData.diffuse * diffuseIntensity * attenuation;
		fragLighting.specular += materialData.specular * specularIntensity * attenuation;
	}

	return fragLighting;
}


FragLightingData processSpotLight(FragLightingData fragLighting, uint index, vec3 normal, vec3 eye) {
	vec3 direction = normalize(-lightingData.lightDirections[index].xyz);

	vec3 lightDir = vec3(lightingData.lightPositions[index].xyz - dataIn.position.xyz);
	float distance = length(lightDir);
	float attenuation = lightingData.lightIntensities[index] / (1.0 + 0.1 * distance + 0.01 * distance * distance);
	lightDir = normalize(lightDir);
	
	float diffuseIntensity = max(dot(normal, lightDir), 0.0);
	if (diffuseIntensity > 0.0) {
		float lightAngleDot = dot(direction, lightDir);
		if (lightAngleDot > lightingData.lightCutOffs[index]) {
			vec3 h = (lightDir + eye) / 2.0;
			float specularIntensity = pow(max(dot(normal, h), 0.0), materialData.shininess);
			fragLighting.diffuse += materialData.diffuse * diffuseIntensity * attenuation;
			fragLighting.specular += materialData.specular * specularIntensity * attenuation;
		}
	}

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

	for (int i = 0; i < lightingData.nLights; i++) {
		switch(lightingData.lightTypes[i])
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



	vec4 ambientColor = lightingData.ambientCoefficient * materialData.ambient;
	vec4 lightingDataColor = lightingData.diffuseCoefficient * fragLighting.diffuse + lightingData.specularCoefficient * fragLighting.specular;
	colorOut = max(ambientColor, lightingDataColor);
}