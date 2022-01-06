#version 430


const uint MAX_INSTANCES = 100;




struct InstanceData {
	mat4 pvmMatrix[MAX_INSTANCES];
	mat4 vmMatrix[MAX_INSTANCES];
	mat3 normalMatrix[MAX_INSTANCES];
	uint meshIndex[MAX_INSTANCES];
};




uniform InstanceData instanceData; 


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
	uint meshIndex;
} dataOut;




void main () {

	dataOut.position = instanceData.vmMatrix[gl_InstanceID] * position;
	dataOut.normal = normalize(instanceData.normalMatrix[gl_InstanceID] * normal.xyz);
	dataOut.eye = vec3(-dataOut.position);
	dataOut.eyeDir = -vec3(instanceData.vmMatrix[gl_InstanceID] * position);
	dataOut.textureCoords = textureCoords.st;
	dataOut.meshIndex = instanceData.meshIndex[gl_InstanceID];
	
	gl_Position = instanceData.pvmMatrix[gl_InstanceID] * position;
}