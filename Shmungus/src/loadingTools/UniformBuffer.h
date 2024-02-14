#pragma once

#include <ShmingoCore.h>

#include <glm/gtc/type_ptr.hpp>

enum UniformBlock {

	se_MATRICES

};

//This entire class and its functionality is written to maximize efficiency, since uniform uploads are happening multiple times per frame.
//Therefore the implementation of this code is likely going to be very difficult to reverse-engineer, dont break anything please

class UniformBuffer {

public:

	//Initialization function to be called in master renderer
	void init();

	//Independent loader functions for each uniform to avoid needing to look up types in a hashmap multiple times per frame
	void setProjectionMatrix(mat4 projectionMatrix);
	void setViewMatrix(mat4 viewMatrix);
	void setTestFloat(float testFloat);


	static UniformBuffer& get() { return instance; };

	void setAsActive();


private:


	UniformBuffer();

	GLuint uboID = 0;

	//Use this to get the index of the uniform you want to use in your shader
	static UniformBuffer instance;


	//set 1 float
	void setUniform1f(float value, GLint offset, GLuint index);
	//set 1 int
	void setUniform1i(int value, GLint offset, GLuint index);
	//set vector of 2 floats
	void setUniformVec2(vec2 value, GLint offset, GLuint index);
	//set vector of 3 floats
	void setUniformVec3(vec3 value, GLint offset, GLuint index);
	//set vector of 4 floats
	void setUniformVec4(vec4 value, GLint offset, GLuint index);
	//set 3 by 3 matrix
	void setUniformMat3(mat3 value, GLint offset, GLuint index);
	//set 4 by 4 matrix
	void setUniformMat4(mat4 value, GLint offset, GLuint index);

};
