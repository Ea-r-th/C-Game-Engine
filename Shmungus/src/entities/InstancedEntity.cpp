#include <sepch.h>
#include "InstancedEntity.h"
#include "ModelTools.h"
#include "TextureTools.h"


//Initialize static variables
std::vector<EntitySpecificVertexDataInfo> CubeEntity::instancedAttributeInfo = CubeEntity::setInstancedAttribInfo();
std::vector<EntitySpecificVertexDataInfo> DefaultEntity::instancedAttributeInfo = DefaultEntity::setInstancedAttribInfo();


InstancedEntity::InstancedEntity(vec3 position, vec3 rotation){}


void InstancedEntity::declareVertexData(GLuint localAttributeNumber, InstancedAttributeInformation& attribInfo, float data) {
	auto currentAttribInfo = attribInfo[localAttributeNumber];
	vertexData.get()[currentAttribInfo.localFloatOffset] = data;
}

void InstancedEntity::declareVertexData(GLuint localAttributeNumber, InstancedAttributeInformation& attribInfo, vec2 data) {
	auto currentAttribInfo = attribInfo[localAttributeNumber];
	vertexData.get()[currentAttribInfo.localFloatOffset] = data.x;
	vertexData.get()[currentAttribInfo.localFloatOffset + 1] = data.y;
}

void InstancedEntity::declareVertexData(GLuint localAttributeNumber, InstancedAttributeInformation& attribInfo, vec3 data) {
	auto currentAttribInfo = attribInfo[localAttributeNumber];
	vertexData.get()[currentAttribInfo.localFloatOffset] = data.x;
	vertexData.get()[currentAttribInfo.localFloatOffset + 1] = data.y;
	vertexData.get()[currentAttribInfo.localFloatOffset + 2] = data.z;
}

void InstancedEntity::declareVertexData(GLuint localAttributeNumber, InstancedAttributeInformation& attribInfo, vec4 data) {
	auto currentAttribInfo = attribInfo[localAttributeNumber];
	vertexData.get()[currentAttribInfo.localFloatOffset] = data.x;
	vertexData.get()[currentAttribInfo.localFloatOffset + 1] = data.y;
	vertexData.get()[currentAttribInfo.localFloatOffset + 2] = data.z;
	vertexData.get()[currentAttribInfo.localFloatOffset + 3] = data.w;
}

void InstancedEntity::instantiateVertexData(InstancedAttributeInformation& vertexDataInfo) {

	GLuint size = 0;
	for (EntitySpecificVertexDataInfo& info : vertexDataInfo) {
		size += info.size;
	}
	vertexData = std::make_unique<float[]>(size);
}

//Functions for all entity types

inline float* InstancedEntity::getVertexData(GLuint attributeNumber, InstancedAttributeInformation& instancedAttributeInfo){
	return &vertexData.get()[instancedAttributeInfo[attributeNumber].localFloatOffset];
}

void DefaultEntity::setPosition(vec3 newPosition){
	declareVertexData(0, instancedAttributeInfo, newPosition);
}

void DefaultEntity::setRotation(float newRotation){
	declareVertexData(1, instancedAttributeInfo, newRotation);
}

void DefaultEntity::setTextureID(float newTextureID){
	declareVertexData(2, instancedAttributeInfo, newTextureID);
}

vec3 DefaultEntity::getPosition(){
	float* positions = getVertexData(0, instancedAttributeInfo);
	return vec3(positions[0],positions[1],positions[2]);
}

float DefaultEntity::getRotation(){
	return getVertexData(1, instancedAttributeInfo)[0]; //Index 0 because it will spit out an array, and since your float is only one element long, you want the first one
}

float DefaultEntity::getTextureID() {
	return getVertexData(2, instancedAttributeInfo)[0];
}

//DefaultEntity is a base implementation of InstancedEntity containing basic vertex attributes that all entities should have.
//All subclasses will still have to manually declare the instanced attributes in the setInstancedAttribInfo method, however they will have
//access to all of the getters and setters for that data without needing to redefine it.

DefaultEntity::DefaultEntity(vec3 position, vec3 rotation) : InstancedEntity(position,rotation){

	instantiateVertexData(instancedAttributeInfo);

	setPosition(position);
	setRotation(rotation.z);
	setTextureID(0);
}

std::vector<EntitySpecificVertexDataInfo> DefaultEntity::setInstancedAttribInfo() {
	std::vector<EntitySpecificVertexDataInfo> vertexDataInfo;
	GLuint sizeCounter = 0;

	declareInstanceAttribute(vertexDataInfo, sizeCounter, 0, "position", 3);
	declareInstanceAttribute(vertexDataInfo, sizeCounter, 1, "rotation", 1);
	declareInstanceAttribute(vertexDataInfo, sizeCounter, 2, "textureID", 1);

	return vertexDataInfo;
}