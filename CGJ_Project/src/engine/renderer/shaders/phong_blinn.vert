#version 430


const uint MAX_INSTANCES = 50;

const uint MAX_FLARE_ELEMENTS = 10;

const uint RENDER_MODE_MESH = 1;
const uint RENDER_MODE_IMAGE = 2;
const uint RENDER_MODE_PARTICLE = 3;
const uint RENDER_MODE_SKYBOX = 4;
const uint RENDER_MODE_LENS_FLARE = 5;




struct InstanceData {
	mat4 pvmMatrix[MAX_INSTANCES];
	mat4 vmMatrix[MAX_INSTANCES];
	mat3 normalMatrix[MAX_INSTANCES];
	vec4 particleColor[MAX_INSTANCES];
};




uniform InstanceData instanceData; 
uniform mat4 viewMatrix;
uniform mat4 skyboxModelMatrix;
uniform uint renderMode;


in vec4 position;
in vec4 normal;
in vec4 textureCoords;
in vec4 tangent;


out Data {
	vec4 position;
	vec3 normal;
	vec3 eye;
	vec3 eyeDir;
	vec2 textureCoords;
	vec3 skyboxTextureCoords;
	vec3 reflected;
	vec4 particleColor;
} dataOut;




void main () {

	dataOut.position = instanceData.vmMatrix[gl_InstanceID] * position;
	dataOut.normal = normalize(instanceData.normalMatrix[gl_InstanceID] * normal.xyz);
	dataOut.eye = vec3(-dataOut.position);
	dataOut.eyeDir = -vec3(instanceData.vmMatrix[gl_InstanceID] * position);
	dataOut.textureCoords = textureCoords.st;
	dataOut.particleColor = instanceData.particleColor[gl_InstanceID];
	dataOut.reflected = vec3(transpose(viewMatrix) * vec4(vec3(reflect(-dataOut.eye, dataOut.normal)), 0.0));
	
	if (renderMode == RENDER_MODE_SKYBOX) {
		dataOut.skyboxTextureCoords = vec3(skyboxModelMatrix * position);
		dataOut.skyboxTextureCoords.x = -dataOut.skyboxTextureCoords.x;
	}

	gl_Position = instanceData.pvmMatrix[gl_InstanceID] * position;
}