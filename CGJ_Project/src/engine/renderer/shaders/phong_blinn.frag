#version 430


const uint MAX_MESHES = 15;

const uint MAX_LIGHTS = 32;
const uint LIGHT_TYPE_DIRECTIONAL = 1;
const uint LIGHT_TYPE_POINT = 2;
const uint LIGHT_TYPE_SPOT = 3;

const uint MAX_TEXTURES = 2;
const uint TEXTURE_MODE_MODULATE_DIFFUSE = 1;
const uint TEXTURE_MODE_REPLACE_DIFFUSE = 2;

const uint FOG_TYPE_LINEAR = 1;
const uint FOG_TYPE_EXP = 2;
const uint FOG_TYPE_EXP2 = 3;




struct MaterialData {
	vec4 diffuse[MAX_MESHES];
	vec4 ambient[MAX_MESHES];
	vec4 specular[MAX_MESHES];
	vec4 emissive[MAX_MESHES];
	float shininess[MAX_MESHES];
};


struct TextureData {
	uint nTextures[MAX_MESHES];
	uint mode[MAX_MESHES];
	sampler2D maps[MAX_MESHES * MAX_TEXTURES];
};


struct LightingData {
	uint nLights;
	uint type[MAX_LIGHTS];
	vec4 position[MAX_LIGHTS];
	vec4 direction[MAX_LIGHTS];
	float intensity[MAX_LIGHTS];
	float cutOff[MAX_LIGHTS];

	float ambientCoefficient;
	float diffuseCoefficient;
	float specularCoefficient;
	float darkTextureCoefficient;
};


struct FogData {
	uint mode;
	vec4 color;
	float density;
	float startDistance;
	float endDistance;
	bool isActive;
};


struct FragLightingData {
	vec4 ambient;
	vec4 specular;
	vec4 diffuse;

	float diffuseIntensity;
	float specularIntensity;
};




uniform MaterialData materialData;
uniform TextureData textureData;
uniform LightingData lightingData;
uniform FogData fogData;


in Data {
	vec4 position;
	vec3 normal;
	vec3 eye;
	vec3 eyeDir;
	vec2 textureCoords;
	uint meshIndex;
} dataIn;


out vec4 colorOut;




FragLightingData processDirectionalLight(FragLightingData fragLighting, uint index, vec3 normal, vec3 eye) {
	vec3 direction = normalize(-lightingData.direction[index].xyz);
	float attenuation = lightingData.intensity[index];

	float diffuseIntensity = max(dot(normal, direction), 0.0);
	if (diffuseIntensity > 0.0) {
		vec3 h = (direction + eye) / 2.0;
		float specularIntensity = pow(max(dot(normal, h), 0.0), materialData.shininess[dataIn.meshIndex]);
		fragLighting.diffuseIntensity += diffuseIntensity * attenuation;
		fragLighting.specularIntensity += specularIntensity * attenuation;
	}

	return fragLighting;
}


FragLightingData processPointLight(FragLightingData fragLighting, uint index, vec3 normal, vec3 eye) {
	vec3 lightDir = vec3(lightingData.position[index].xyz - dataIn.position.xyz);
	float distance = length(lightDir);
	float attenuation = lightingData.intensity[index] / (1.0 + 0.1 * distance + 0.01 * distance * distance);
	lightDir = normalize(lightDir);

	float diffuseIntensity = max(dot(normal, lightDir), 0.0);
	if (diffuseIntensity > 0.0) {
		vec3 h = (lightDir + eye) / 2.0;
		float specularIntensity = pow(max(dot(normal, h), 0.0), materialData.shininess[dataIn.meshIndex]);
		fragLighting.diffuseIntensity += diffuseIntensity * attenuation;
		fragLighting.specularIntensity += specularIntensity * attenuation;
	}

	return fragLighting;
}


FragLightingData processSpotLight(FragLightingData fragLighting, uint index, vec3 normal, vec3 eye) {
	vec3 direction = normalize(-lightingData.direction[index].xyz);

	vec3 lightDir = vec3(lightingData.position[index].xyz - dataIn.position.xyz);
	float distance = length(lightDir);
	float attenuation = lightingData.intensity[index] / (1.0 + 0.1 * distance + 0.01 * distance * distance);
	lightDir = normalize(lightDir);
	
	float diffuseIntensity = max(dot(normal, lightDir), 0.0);
	if (diffuseIntensity > 0.0) {
		float lightAngleDot = dot(direction, lightDir);
		if (lightAngleDot > lightingData.cutOff[index]) {
			vec3 h = (lightDir + eye) / 2.0;
			float specularIntensity = pow(max(dot(normal, h), 0.0), materialData.shininess[dataIn.meshIndex]);
			fragLighting.diffuseIntensity += diffuseIntensity * attenuation;
			fragLighting.specularIntensity += specularIntensity * attenuation;
		}
	}

	return fragLighting;
}




vec4 processModulateDiffuseTexture(FragLightingData fragLighting) {
	vec4 texel = vec4(1.0);
	for (int i = 0; i < textureData.nTextures[dataIn.meshIndex]; i++)
		texel *= texture(textureData.maps[i], dataIn.textureCoords);

	return max(fragLighting.diffuse * texel + fragLighting.specular, lightingData.darkTextureCoefficient * texel);
}


vec4 processReplaceDiffuseTexture(FragLightingData fragLighting) {
	vec4 texel = vec4(1.0);
	for (int i = 0; i < textureData.nTextures[dataIn.meshIndex]; i++)
		texel *= texture(textureData.maps[i], dataIn.textureCoords);

	return max(fragLighting.diffuseIntensity * texel + fragLighting.specular, lightingData.darkTextureCoefficient * texel);
}




void main() {
	colorOut = vec4(0.0);
	vec3 normal = normalize(dataIn.normal);
	vec3 eye = normalize(dataIn.eye);


	// process all the lights of the scene
	FragLightingData fragLighting;
	fragLighting.ambient = materialData.ambient[dataIn.meshIndex] * lightingData.ambientCoefficient;
	fragLighting.diffuse = materialData.diffuse[dataIn.meshIndex] * lightingData.diffuseCoefficient;
	fragLighting.specular = materialData.specular[dataIn.meshIndex] * lightingData.specularCoefficient;
	fragLighting.diffuseIntensity = 0.0;
	fragLighting.specularIntensity = 0.0;

	for (int i = 0; i < lightingData.nLights; i++) {
		if (lightingData.type[i] == LIGHT_TYPE_DIRECTIONAL)
			fragLighting = processDirectionalLight(fragLighting, i, normal, eye);

		else if (lightingData.type[i] == LIGHT_TYPE_POINT)
			fragLighting = processPointLight(fragLighting, i, normal, eye);

		else if (lightingData.type[i] == LIGHT_TYPE_SPOT)
			fragLighting = processSpotLight(fragLighting, i, normal, eye);
	}


	// generates the color of the fragment according to the texture mode
	fragLighting.diffuse *= fragLighting.diffuseIntensity;
	fragLighting.specular *= fragLighting.specularIntensity;
	fragLighting.diffuseIntensity *= lightingData.diffuseCoefficient;
	fragLighting.specularIntensity *= lightingData.specularCoefficient;
	vec4 rgbColor = vec4(0.0);
	
	if (textureData.nTextures[dataIn.meshIndex] == 0)
		rgbColor = max(fragLighting.diffuse + fragLighting.specular, fragLighting.ambient);

	else if (textureData.mode[dataIn.meshIndex] == TEXTURE_MODE_MODULATE_DIFFUSE)
		rgbColor = processModulateDiffuseTexture(fragLighting);

	else if (textureData.mode[dataIn.meshIndex] == TEXTURE_MODE_REPLACE_DIFFUSE)
		rgbColor = processReplaceDiffuseTexture(fragLighting);


	// generate the fog color
	float fogAmount = 1.0;
	float fragDistance = sqrt(dot(dataIn.position, dataIn.position));
	if (fogData.isActive) {
		if (fogData.mode == FOG_TYPE_LINEAR)
			fogAmount = clamp((fogData.endDistance - fragDistance) / (fogData.endDistance - fogData.startDistance), 0.0, 1.0);

		else if (fogData.mode == FOG_TYPE_EXP)
			fogAmount = exp(-fogData.density * fragDistance);

		else if (fogData.mode == FOG_TYPE_EXP2)
			fogAmount = pow(exp(-fogData.density * fragDistance), 2);
	}

	colorOut = mix(fogData.color, rgbColor, fogAmount);
}