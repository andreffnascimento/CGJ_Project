#version 430


const uint MAX_LIGHTS = 32;
const uint LIGHT_TYPE_DIRECTIONAL = 1;
const uint LIGHT_TYPE_POINT = 2;
const uint LIGHT_TYPE_SPOT = 3;

const uint MAX_2D_TEXTURES = 30;
const uint MAX_CUBE_MAP_TEXTURES = 2;
const uint MAX_TEXTURES_PER_MESH = 2;
const uint TEXTURE_MODE_NONE = 0;
const uint TEXTURE_MODE_MODULATE_DIFFUSE = 1;
const uint TEXTURE_MODE_REPLACE_DIFFUSE = 2;

const uint FOG_TYPE_LINEAR = 1;
const uint FOG_TYPE_EXP = 2;
const uint FOG_TYPE_EXP2 = 3;

const float NORMAL_BLEND_AMOUNT = 0.5;

const uint RENDER_MODE_MESH = 1;
const uint RENDER_MODE_IMAGE = 2;
const uint RENDER_MODE_PARTICLE = 3;
const uint RENDER_MODE_SKYBOX = 4;
const uint RENDER_MODE_LENS_FLARE = 5;
const uint RENDER_MODE_SHADOWS = 7;




struct MaterialData {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emissive;
	float shininess;
};


struct TextureData {
	uint mode;
	uint nColorMaps;
	uint nNormalMaps;
	uint colorMapIds[MAX_TEXTURES_PER_MESH];
	uint normalMapIds[MAX_TEXTURES_PER_MESH];
	sampler2D maps[MAX_2D_TEXTURES];
	samplerCube cubeMaps[MAX_CUBE_MAP_TEXTURES];
	bool bumpActive;
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


struct LensFlareData {
	vec4 color;
	uint colorMapId;
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
uniform LensFlareData lensFlareData;
uniform uint renderMode;


in Data {
	vec4 position;
	vec3 normal;
	vec3 eye;
	vec3 eyeDir;
	vec2 textureCoords;
	vec3 skyboxTextureCoords;
	vec4 particleColor;
} dataIn;


out vec4 colorOut;




FragLightingData processDirectionalLight(FragLightingData fragLighting, uint index, vec3 normal, vec3 eye) {
	vec3 direction = normalize(-lightingData.direction[index].xyz);
	float attenuation = lightingData.intensity[index];

	float diffuseIntensity = max(dot(normal, direction), 0.0);
	if (diffuseIntensity > 0.0) {
		vec3 h = (direction + eye) / 2.0;
		float specularIntensity = pow(max(dot(normal, h), 0.0), materialData.shininess);
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
		float specularIntensity = pow(max(dot(normal, h), 0.0), materialData.shininess);
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
			float specularIntensity = pow(max(dot(normal, h), 0.0), materialData.shininess);
			fragLighting.diffuseIntensity += diffuseIntensity * attenuation;
			fragLighting.specularIntensity += specularIntensity * attenuation;
		}
	}

	return fragLighting;
}


FragLightingData processLighting(vec3 eye, vec3 normal) {
	FragLightingData fragLighting;
	fragLighting.ambient = materialData.ambient;
	fragLighting.diffuse = materialData.diffuse;
	fragLighting.specular = materialData.specular;
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
	fragLighting.diffuseIntensity *= lightingData.diffuseCoefficient;
	fragLighting.specularIntensity *= lightingData.specularCoefficient;
	fragLighting.diffuse *= fragLighting.diffuseIntensity;
	fragLighting.specular *= fragLighting.specularIntensity;
	return fragLighting;
}




float processFogAmount() {
	float fragDistance = sqrt(dot(dataIn.position, dataIn.position));
	if (fogData.isActive) {
		if (fogData.mode == FOG_TYPE_LINEAR)
			return clamp((fogData.endDistance - fragDistance) / (fogData.endDistance - fogData.startDistance), 0.0, 1.0);

		else if (fogData.mode == FOG_TYPE_EXP)
			return exp(-fogData.density * fragDistance);

		else if (fogData.mode == FOG_TYPE_EXP2)
			return pow(exp(-fogData.density * fragDistance), 2);
	}
	
	return 1.0;
}




vec3 processNormalMaps()
{
	vec3 normal = normalize(dataIn.normal);
	if(!textureData.bumpActive)
		return normal;

    vec3 currentMap;
	vec3 calculatedNormals = normal;

	// Calculate normal if a normal map is provided
	for (int i = 0; i < textureData.nNormalMaps; i++) {	
		currentMap = texture(textureData.maps[textureData.normalMapIds[i]], dataIn.textureCoords).rgb;
		normal = normalize(2.0 * currentMap - 1.0);
		calculatedNormals = normalize(vec3(normal.xy + calculatedNormals.xy, normal.z));
	}

	return calculatedNormals;
}




vec4 processModulateDiffuseTexture(FragLightingData fragLighting) {
	vec4 texel = vec4(1.0);
	for (int i = 0; i < textureData.nColorMaps; i++)
		texel *= texture(textureData.maps[textureData.colorMapIds[i]], dataIn.textureCoords);
	
	return max(fragLighting.diffuse * texel + fragLighting.specular, lightingData.darkTextureCoefficient * texel);
}


vec4 processReplaceDiffuseTexture(FragLightingData fragLighting) {
	vec4 texel = vec4(1.0);
	for (int i = 0; i < textureData.nColorMaps; i++)
		texel *= texture(textureData.maps[textureData.colorMapIds[i]], dataIn.textureCoords);

	return max(fragLighting.diffuseIntensity * texel + fragLighting.specular, lightingData.darkTextureCoefficient * texel);
}




vec4 renderMesh() {
	vec3 eye = normalize(dataIn.eye);
	vec3 normal = processNormalMaps();
	
	vec4 rgbColor = vec4(0.0);
	float fogAmount = processFogAmount();
	FragLightingData fragLighting = processLighting(eye, normal);
	
	if (textureData.mode == TEXTURE_MODE_NONE)
	{
		rgbColor = max(fragLighting.diffuse + fragLighting.specular, fragLighting.ambient);
		return vec4(mix(fogData.color.rgb, rgbColor.rgb, fogAmount), materialData.diffuse.a);
	}
	
	else if (textureData.mode == TEXTURE_MODE_MODULATE_DIFFUSE)
	{
		rgbColor = processModulateDiffuseTexture(fragLighting);
		return vec4(mix(fogData.color.rgb, rgbColor.rgb, fogAmount), materialData.diffuse.a);
	}
	
	else if (textureData.mode == TEXTURE_MODE_REPLACE_DIFFUSE)
	{
		rgbColor = processReplaceDiffuseTexture(fragLighting);
		return vec4(mix(fogData.color.rgb, rgbColor.rgb, fogAmount), materialData.diffuse.a);
	}
}




vec4 renderImage() {
	if (textureData.nColorMaps == 0)
		return materialData.ambient;

	vec4 texel = vec4(1.0);
	for (int i = 0; i < textureData.nColorMaps; i++)
		texel *= texture(textureData.maps[textureData.colorMapIds[i]], dataIn.textureCoords);

	if (texel.a == 0.0)
		discard;

	return vec4(texel.xyz * (1 - materialData.ambient.a) + materialData.ambient.xyz * materialData.ambient.a, texel.a);
}




vec4 renderParticle() {
	if (textureData.nColorMaps == 0)
		return dataIn.particleColor;

	vec4 texel = vec4(1.0);
	for (int i = 0; i < textureData.nColorMaps; i++)
		texel *= texture(textureData.maps[textureData.colorMapIds[i]], dataIn.textureCoords);

	if (texel.a == 0.0)
		discard;

	return dataIn.particleColor * texel;
}




vec4 renderSkybox() {
	if (textureData.nColorMaps == 0)
		return materialData.ambient;

	vec4 cubeTexel = texture(textureData.cubeMaps[MAX_2D_TEXTURES - textureData.colorMapIds[0]], dataIn.skyboxTextureCoords);
	return vec4(cubeTexel.xyz * (1 - materialData.ambient.a) + materialData.ambient.xyz * materialData.ambient.a, cubeTexel.a);
}




vec4 renderLensFlare() {
	vec4 lensFlareTexel = texture(textureData.maps[lensFlareData.colorMapId], dataIn.textureCoords);
	if (lensFlareTexel.a == 0.0  || lensFlareData.color.a == 0.0)
		discard;

	return lensFlareData.color * lensFlareTexel;
}


vec4 renderShadows() 
{
	vec4 shadowColor = vec4(0.04, 0.04, 0.1, 1.0);

	return shadowColor;
}


void main() {
	if (renderMode == RENDER_MODE_MESH)
		colorOut = renderMesh();
	else if (renderMode == RENDER_MODE_IMAGE)
		colorOut = renderImage();
	else if (renderMode == RENDER_MODE_PARTICLE)
		colorOut = renderParticle();
	else if (renderMode == RENDER_MODE_SKYBOX)
		colorOut = renderSkybox();
	else if (renderMode == RENDER_MODE_LENS_FLARE)
		colorOut = renderLensFlare();
	else if (renderMode == RENDER_MODE_SHADOWS)
		colorOut = renderShadows();
}