#pragma once

#include <ShmingoCore.h>
#include <typeindex>

#include "RenderPair.h"
#include "Renderer.h"
#include "DefaultShader.h"
#include "UniformBuffer.h"


class MasterRenderer {

public:

	inline static MasterRenderer& get() { return instance; };

	//Meant for initializing the maps, all shaders and renderers should be added to the map in this function
	void init();
	//This function is how you render an object using shaders provided by the engine, custom shaders pass in a custom shader object in a shared pointer
	void submitVertexArray(std::shared_ptr<VertexArray> vertexArray, ShaderType type);
	//Meant for shaders added by modders, which do not have a corresponding ShaderType enum. Pass the object directly into the function
	void submitVertexArray(std::shared_ptr<VertexArray>, std::shared_ptr<ShaderProgram> shader);

	void submitInstancedVertexArray(std::shared_ptr<InstancedVertexArray> vertexArray);

	void update();


	void renderBatch();
	void clearBatch();


	//----------------------------------Related to entity instance attributes--------------------------------------
	std::vector<Shmingo::EntitySpecificInstanceDataInfo> getEntitySpecificInstanceAttributeInfo(Shmingo::EntityType type);
	std::vector<Shmingo::EntitySpecificInstanceDataInfo>& getEntitySpecificInstanceAttributeInfoRef(Shmingo::EntityType type);
	GLuint getEntitySpecificMajorInstanceAttribAmount(Shmingo::EntityType type);
	GLuint getEntitySpecificTotalInstanceAttribAmount(Shmingo::EntityType type);

	void declareEntityInstanceAttributes(Shmingo::EntityType type, std::vector<Shmingo::EntitySpecificInstanceDataInfo> uniformInfo);
	void declareEntitySpecificInstanceAttribAmount(Shmingo::EntityType type, GLuint majorAmount, GLuint totalAmount);

	//----------------------------------Related to entity models--------------------------------------
	void declareEntityModel(Shmingo::EntityType type, std::shared_ptr<Model> model);
	std::shared_ptr<Model> getEntityModel(Shmingo::EntityType type);

	std::shared_ptr<ShaderProgram> getEntityShader(Shmingo::EntityType type); //Get an entity type's shader

private:

	//Used to set which shader program and renderer a particular shader enum corresponds to
	void mapShader(ShaderType type, std::shared_ptr<ShaderProgram> shader);
	//Used to set which shader program and renderer a particular entity type corresponds to
	void mapEntityShader(Shmingo::EntityType type, std::shared_ptr<ShaderProgram> shader);

	//Needs to have type explicitly declared, also declares an amount of texture slots
	void mapInstancedShader(std::type_index entityType, GLuint textureSlotAmount, std::shared_ptr<ShaderProgram> shader);

	//Used to set sampler uniform
	void declareShaderTextureMap(ShaderType type, int textureSlotAmount);

	MasterRenderer() {};

	static MasterRenderer instance;

	std::vector<RenderPair> renderQueue; 

	std::map<ShaderType, std::shared_ptr<ShaderProgram>> shaderMap;
	std::map<std::type_index, std::shared_ptr<ShaderProgram>> instancedShaderMap;

	std::unordered_map<Shmingo::EntityType, std::vector<Shmingo::EntitySpecificVertexDataInfo>> entityVertexAttributeMap;
	std::unordered_map<Shmingo::EntityType, std::list<GLuint>> entitySpecificVertexAttribAmountMap;


	std::unordered_map<Shmingo::EntityType, std::vector<Shmingo::EntitySpecificInstanceDataInfo>> entityUniformMap;
	std::unordered_map<Shmingo::EntityType, std::list<GLuint>> entitySpecificInstanceAttribAmountMap;


	std::unordered_map<Shmingo::EntityType, std::shared_ptr<Model>> entityModelMap;
	std::unordered_map<Shmingo::EntityType, std::shared_ptr<ShaderProgram>> entityShaderMap;
};