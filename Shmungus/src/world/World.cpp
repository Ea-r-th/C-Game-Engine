#include "sepch.h"
#include "World.h"

#include "MasterRenderer.h"
#include "Renderer.h"
#include "ShmingoApp.h"
#include "ModelTools.h"
#include "TextureTools.h"
#include "MiscTools.h"

World::World() {

}

void World::init(){

	Shmingo::setCurrentWorld(this);

	//Initialize the entity type info map with all entity types
	for (Shmingo::EntityType type : se_application.entityTypes) {
		initializedInstancedVaoMap.insert(std::make_pair(type, false));
	}
}

void World::update(){

	updateEntities();
	submitVertexArrays();
}

void World::updateEntities() {
	if (entityList.size() != 0) {
		se_log("First entity vao offest: " << entityList[0]->getOffsetInVao());
	}
	for (InstancedEntity* entity : entityList) {
		entity->update();
	}
}



//Other functions

void World::submitVertexArrays(){
 	for (auto it = instancedVAOMap.begin(); it != instancedVAOMap.end(); it++) {
		Shmingo::renderInstanced(it->second, se_masterRenderer.getEntityShader(it->first));
	}
}

void World::deleteEntity(Shmingo::EntityType type, GLuint localOffset){
	Shmingo::EntityTypeInfo typeInfo = entityTypeInfoMap.map[type];


	if (typeInfo.amount == 0) {
		se_log("Attempted to delete entity of type " << type << " but no entities of that type exist");
		return;
	}	

	//Condition where we are removing the last entity of a certain type - delete VAO and associated information
	else if (typeInfo.amount == 1) {

		delete entityList[typeInfo.offset + localOffset]; //Delete entity

		entityList.erase(entityList.begin() + typeInfo.offset);
		entityTransformList.erase(entityTransformList.begin() + typeInfo.offset);
		entityTypeInfoMap.erase(type);
		instancedVAOMap.erase(type);
		initializedInstancedVaoMap[type] = false;

	}

	//Condition where we are removing an entity that is not the last entity of a certain type
	else {

		GLuint selectedEntityIndex = typeInfo.offset + localOffset;
		GLuint lastEntityIndex = typeInfo.offset + typeInfo.amount - 1;

		if (selectedEntityIndex != lastEntityIndex) {
			se_log("Selected entity index: " << selectedEntityIndex << "Last index: " << lastEntityIndex);
			se_log("Switching entity at VAO offset " << entityList[selectedEntityIndex]->getOffsetInVao() << " with entity at VAO offset " << entityList[lastEntityIndex]->getOffsetInVao())
				entityList[lastEntityIndex]->setOffsetInVao(entityList[selectedEntityIndex]->getOffsetInVao()); //Set offset in VAO of entity at the end to the offset in VAO of removed entity

			//Removal process involves replacing the entity to be removed with the last entity of the same type, and removing the last entity to reduce shifting of entities and maintain consistency with VAO
			entityList[selectedEntityIndex] = entityList[lastEntityIndex]; //Replace entity with last entity of the same type
			entityTransformList[selectedEntityIndex] = entityTransformList[lastEntityIndex]; //Replace transform with last transform of the same type
		}
		
		else {
			se_log("Deleting most recently added entity of its type");

		}
		delete entityList[lastEntityIndex]; //Delete entity that was moved to the position of the entity to be removed

		entityList.erase(entityList.begin() + lastEntityIndex); //Erase last entity of the same type
		entityTransformList.erase(entityTransformList.begin() + lastEntityIndex); //Erase last transform of the same type

		instancedVAOMap.at(type)->removeInstancedData(localOffset); //Remove instance data from VAO and replace with last instance data

		entityTypeInfoMap.map[type].amount--; //Decrement amount of entities of the given type
	}

	//Update information about all succeeding entity types - happens regardless of whether the entity is the last entity of a certain type
	for (auto it = entityTypeInfoMap.keys.begin() + entityTypeInfoMap.map[type].vertexArrayOffset + 1; it < entityTypeInfoMap.keys.end(); it++) { //Start updating from the next entity type
		entityTypeInfoMap.map[*it].offset--; //Increment offset of all succeeding entity types
	}

	//Update local VAO offset of all succeding entities of the same type
	auto entityOffset = entityTypeInfoMap.map[type].offset;
	for (auto it = entityList.begin() + entityTypeInfoMap.map[type].offset + localOffset; it < entityList.begin() + entityOffset + entityTypeInfoMap.map[type].amount; it++) {
		(*it)->decrementOffsetInVao();
	}
}


void World::addEntity(Shmingo::EntityType type, InstancedEntity* entity){

	//If entity type does not exist in the entity type map, create a new entity type info and VAO
	if (initializedInstancedVaoMap.at(type) == false) {
		//Creates info for new entity type. Sets the offset to the current size of the entity list, amount is 0 because will increment after conditional
		entityTypeInfoMap.insert(type, Shmingo::EntityTypeInfo((GLuint)initializedInstancedVaoMap.size(), (GLuint)entityList.size(), 0)); //Creates information about the new VAO

		instancedVAOMap.insert(std::make_pair(type, std::make_shared<InstancedVertexArray>(type, se_masterRenderer.getEntityModel(type)))); //Create new VAO for entity type
		initializedInstancedVaoMap[type] = true; //Set entity type to initialized
	}


	std::shared_ptr<InstancedVertexArray> entityVAO = instancedVAOMap.at(type); //Get VAO for entity type
	uint16_t emplaceOffset = entityTypeInfoMap.map[type].offset + entityTypeInfoMap.map[type].amount; //Offset to the back of the section of the given type in the entity list - works because amount has not yet been incremented

	//Emplace entity into entity list at the offset of the entity type
	entityList.emplace(entityList.begin() + emplaceOffset, entity); //Inserts entity into entity list at the offset of the entity type
	entityTransformList.emplace(entityTransformList.begin() + emplaceOffset, entity->getTransformComponent()); //Inserts entity transform into entity list at the offset of the entity type

	//Submit entity data to the VAO
	entityVAO->submitInstanceData(entity);

	Shmingo::TransformComponent currentTransform = entityTransformList[entityTypeInfoMap.map[type].offset];


	//Update information about all succeeding entity types
	for(auto it = entityTypeInfoMap.keys.begin() + entityTypeInfoMap.map[type].vertexArrayOffset + 1; it < entityTypeInfoMap.keys.end(); it++){ //Start updating from the next entity type
		entityTypeInfoMap.map[*it].offset++; //Increment offset of all succeeding entity types
	}

	entityTypeInfoMap.map[type].amount++; //Increment amount of entities of the given type

}



void World::cleanUp(){

	for (InstancedEntity* entity : entityList) {
		delete entity;
	}
}