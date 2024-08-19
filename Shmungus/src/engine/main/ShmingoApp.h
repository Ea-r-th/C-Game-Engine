#pragma once

#include <Engine.h>
#include <typeindex>
#include "Input.h"
#include "ModelManager.h"
#include "World.h"
#include "FontUtil.h"

//This class is a singleton, there will only ever be one Application

namespace Shmingo {

	class ShmingoApp {

	public:

		~ShmingoApp();


		//Public utility functions -------------------------------------------------------------------

		void recalculateTextSpacing(float oldDisplayWidth, float oldDisplayHeight, float newDisplayWidth, float newDisplayHeight);

		//Getters -------------------------------------------------------------------------------------
		//Returns a pointer to the window
		inline Shmingo::Window* getWindow() { return window; };

		//Returns a pointer to the current world object
		inline World* getCurrentWorld() { return currentWorld; };

		//Returns time since last frame
		inline double getDeltaTime() { return deltaTime; }

		//Returns total time elapsed since beginning of application
		inline double getElapsedTime() { return timeElapsed; }
		inline double getLastElapsedTime() { return timeElapsed - deltaTime; }

		//I dont think this is being used
		inline bool getOnTick() { return onTick; }

		//Returns info about the application
		inline std::string getApplicationInfo(Shmingo::ApplicationInfoKey key) { return applicationInfo[key]; }
		inline std::string getApplicationInfo(std::string key) { return applicationInfo[applicationInfoStringToKeyMap[key]]; } //Overload for directly using from strings

		//Returns glyph information of a given character
		inline Shmingo::Character getCharacterFontInfo(GLchar c) { return charMap[c]; }

		//Returns a 8 bit integer corresponding to a color, where the first two bits are for red, the second two bits are for green, and the third two bits are for blue. Provide a color code
		uint8_t getTextColor(char c);

		inline bool isInApplicationInfo(Shmingo::ApplicationInfoKey key) { return applicationInfo.find(key) != applicationInfo.end(); }
		inline bool isInApplicationInfo(std::string key) { return applicationInfoStringToKeyMap.find(key.substr(1)) != applicationInfoStringToKeyMap.end(); }

		inline size_t getInfoSpaceAmount(){ return infoSpaces.size(); } //Returns amount of info spaces to set ID of info space upon creation

		GLuint* getFontTextureArrayIDPtr(std::string font) { return &fontTextureArrayMap[font]; }
		GLuint& getFontTextureArrayID(std::string font) { return fontTextureArrayMap[font]; }


		//Setters -------------------------------------------------------------------------------------
		void setApplicationInfo(Shmingo::ApplicationInfoKey, std::string keyString, std::string value);




		inline static ShmingoApp& get() { return instance; };


		//Interaction with public maps ----------------------------------------------------------------

		void setCurrentWorld(World* world); //Sets the current world object to be used in other parts of the application
		void addInfoSpace(InfoSpace* infoSpace) { infoSpaces.push_back(infoSpace);} //Adds info space pointer to list
		void removeInfoSpace(GLuint offset) { infoSpaces.erase(infoSpaces.begin() + offset); } //Removes info space from list at provided offset

		//Application wide information
		void declareEntityType(std::type_index typeIndex, EntityType type); //Type index not needed anymore but going to keep to make sure all entity type enums are real types
		void declareCharacterFontInfo(GLchar c, Shmingo::Character character) { charMap[c] = character; };

		//Set the font texture array ID
		void setFontTextureArrayID(std::string font, GLuint newID);
		//Get a pointer to the ID of the font texture array

		

		std::vector<Shmingo::EntityType> entityTypes;
		void run();

		//Global getters
		inline int getMinimumUniformBlockOffset() { return minimumUniformBlockOffset; }

	private:

		ShmingoApp(); //Privated constructor

		void init();
		void update();

		Shmingo::Window* window; //Window object
		World* currentWorld; //Reference to current world object

		static ShmingoApp instance;



		//Global variables
		void initGlobalVariables();
		int minimumUniformBlockOffset = 0; //Minimum offset for uniform blocks
		double deltaTime = 0; //Time since last frame
		double timeElapsed = 0; //Time since start of application
		bool onTick = false;

		//Global OpenGL objects

		//Application wide information

		std::unordered_map<Shmingo::ApplicationInfoKey, std::string> applicationInfo;
		std::unordered_map<std::string, Shmingo::ApplicationInfoKey> applicationInfoStringToKeyMap;



		std::unordered_map<GLchar, Shmingo::Character> charMap;

		std::unordered_map<std::string, std::unordered_map<GLchar, Shmingo::Character>> fontMap; //Map of map of character font info
		std::unordered_map<std::string, GLuint> fontTextureArrayMap; //Map of texture array IDs for an entire font


		std::vector<InfoSpace*> infoSpaces; //List of info spaces to update upon window resize
	};
}