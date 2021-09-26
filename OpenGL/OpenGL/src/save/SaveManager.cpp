#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

#include "blocks/AllBlocks.hpp"
#include "SaveManager.hpp"
#include "entity/character/Character.hpp"
#include <SDL2/include/SDL2/SDL_keyboard.h>

std::string SaveManager::chunk_save_path = "save/defaultWorld/chunk";
std::string SaveManager::entity_save_path = "save/defaultWorld/entities";

std::string remainder(std::stringstream& iss) {
	std::string s;
	std::string res;
	iss >> res;
	while (iss >> s) {
		res += " " + s;
	}
	return res;
}

Config SaveManager::getConfig() {
	std::string filePath = "save/config.txt";
	std::ifstream openedFile(filePath);
	if (!openedFile.is_open()) {
		Config config;
		saveConfig(config);
		return config;
	}

	Config config;
	std::string readLine;
	std::string param;
	std::string tmp;

	while (getline(openedFile, readLine)) {
		std::stringstream iss;
		iss.str(readLine);
		iss >> param;
		if (param == "Multiplayer:")
			iss >> std::boolalpha >> config.multiplayer;
		else if (param == "Server_adress:")
			config.SERVER_ADDR = remainder(iss);
		else if (param == "Server_port:")
			iss >> config.SERVER_PORT;
		else if (param == "FoV:")
			iss >> config.fov;
		else if (param == "Forward:")
			config.forward = SDL_GetKeyFromName(remainder(iss).c_str());
		else if (param == "Backward:") 
			config.backward = SDL_GetKeyFromName(remainder(iss).c_str());
		else if (param == "Left:")
			config.left = SDL_GetKeyFromName(remainder(iss).c_str());
		else if (param == "Right:")
			config.right = SDL_GetKeyFromName(remainder(iss).c_str());
		else if (param == "Jump:")
			config.jump = SDL_GetKeyFromName(remainder(iss).c_str());
		else if (param == "Sneak:")
			config.sneak = SDL_GetKeyFromName(remainder(iss).c_str());
		else if (param == "View:")
			config.view = SDL_GetKeyFromName(remainder(iss).c_str());
		else if (param == "Sprint:")
			config.sprint = SDL_GetKeyFromName(remainder(iss).c_str());
		else if (param == "Menu:")
			config.menu = SDL_GetKeyFromName(remainder(iss).c_str());
	}

	config.fov = std::min(180.0f, std::max(0.0f, config.fov));
	//std::cout << SDL_GetKeyName(config.backward) << std::endl;
	saveConfig(config);
	return config;

}


bool SaveManager::saveConfig(const Config& config) {
	std::string filePath = "save/config.txt";
	std::ofstream openedFile(filePath, std::fstream::trunc);
	if (!openedFile) return 0;

	openedFile << "MonCraft v1.1.0" << std::endl;
	openedFile << "	Multiplayer: " << std::boolalpha << config.multiplayer << std::endl;
	openedFile << "	Server_adress: " << config.SERVER_ADDR << std::endl;
	openedFile << "	Server_port: " << std::to_string((int)config.SERVER_PORT) << std::endl;
	openedFile << "	FoV: " << std::to_string(config.fov) << std::endl;
	openedFile << "	Forward: " << SDL_GetKeyName(config.forward) << std::endl;
	openedFile << "	Backward: " << SDL_GetKeyName(config.backward) << std::endl;
	openedFile << "	Left: " << SDL_GetKeyName(config.left) << std::endl;
	openedFile << "	Right: " << SDL_GetKeyName(config.right) << std::endl;
	openedFile << "	Jump: " << SDL_GetKeyName(config.jump) << std::endl;
	openedFile << "	Sneak: " << SDL_GetKeyName(config.sneak) << std::endl;
	openedFile << "	View: " << SDL_GetKeyName(config.view) << std::endl;
	openedFile << "	Sprint: " << SDL_GetKeyName(config.sprint) << std::endl;
	openedFile << "	Menu: " << SDL_GetKeyName(config.menu) << std::endl;

	return true;
}


std::unique_ptr<Chunk> SaveManager::getChunk(glm::ivec3 chunkPos) {
	std::string filePath = chunk_save_path + "/chunk_" + std::to_string(chunkPos.x) + "_" + std::to_string(chunkPos.y) + "_" + std::to_string(chunkPos.z) + ".chunk";
	std::ifstream openedFile(filePath, std::fstream::binary | std::fstream::in);
	if (!openedFile.is_open()) 
		return std::unique_ptr<Chunk>(nullptr);

	////MonCraft version
	//char versionBuffer[3];
	//openedFile.read(versionBuffer, 3);

	//Chunk size
	uint8_t chunkSizeBuffer;
	openedFile.read((char*) &chunkSizeBuffer, 1);

	Chunk* newChunk = new Chunk(chunkPos, chunkSizeBuffer);

	int offset = 0;
	uint8_t buffer[3];
	while (openedFile.read((char*) buffer, 3)) //Block data
	{
		int blockCount = buffer[0] * 256 + buffer[1];
		BlockType block = (BlockType)buffer[2];
		for (int i = 0; i < blockCount; i += 1) {
			int y = (i + offset) / (newChunk->size.x * newChunk->size.x);
			int z = ((i + offset) / newChunk->size.x) % newChunk->size.x;
			int x = (i + offset) % newChunk->size.x;
			newChunk->setBlock(glm::ivec3(x, y, z), AllBlocks::create_static(block));
		}
		offset += blockCount;

	}
	openedFile.close();

	return std::unique_ptr<Chunk>(newChunk);
	
}



bool SaveManager::saveChunk(std::shared_ptr<Chunk> chunk) {

	std::string filePath = chunk_save_path + "/chunk_" + std::to_string(chunk->chunkPos.x) + "_" + std::to_string(chunk->chunkPos.y) + "_" + std::to_string(chunk->chunkPos.z) + ".chunk";
	std::ofstream openedFile(filePath, std::fstream::trunc | std::fstream::binary | std::fstream::out);
	if (!openedFile) return 0;

	////MonCraft version
	//char versionBuffer[3];
	//versionBuffer[0] = 1;
	//versionBuffer[1] = 0;
	//versionBuffer[2] = 2;
	//openedFile.write(versionBuffer, 3);

	//Chunk size
	char chunkSizeBuffer = chunk->size.x;
	openedFile.write(&chunkSizeBuffer, 1);

	int chunkSaveLength = chunk->size.x * chunk->size.y * chunk->size.z;
	int continuous = 1;
	BlockType last = chunk->getBlock(glm::vec3(0))->type;
	for (int i = 1; i < chunkSaveLength; i += 1) {
		int y = i / (chunk->size.x * chunk->size.x);
		int z = (i / chunk->size.x) % chunk->size.x;
		int x = i % chunk->size.x;

		if (chunk->getBlock(glm::vec3(x, y, z))->type == last) {
			continuous += 1;
		}
		else {
			unsigned char buffer[3];
			buffer[0] = continuous / 256;
			buffer[1] = continuous % 256;
			buffer[2] = (uint8_t)last;
			openedFile.write((char*)buffer, 3);
			continuous = 1;
			last = chunk->getBlock(glm::vec3(x, y, z))->type;
		}

	}
	uint8_t buffer[3];
	buffer[0] = continuous / 256;
	buffer[1] = continuous % 256;
	buffer[2] = (uint8_t)last;
	openedFile.write((char*)buffer, 3);

	openedFile.close();
	return 1;
}


std::ofstream& operator<<(std::ofstream& stream, const glm::vec3& vec) {
	stream << vec.x << vec.y << vec.z;
	return stream;
}

std::ofstream& operator<<(std::ofstream& stream, State state) {
	stream << (uint8_t)state;
	return stream;
}

std::ofstream& operator<<(std::ofstream& stream, EntityClass entityClass) {
	stream << (uint8_t)entityClass;
	return stream;
}

std::ofstream& operator<<(std::ofstream& stream, const Node& node) {
	stream << node.loc << node.rot << node.sca;
	return stream;
}

std::ofstream& operator<<(std::ofstream& stream, const Entity& entity) {
	stream << entity.getNode() << entity.getHead() << entity.state;
	return stream;
}

std::ofstream& operator<<(std::ofstream& stream, const Character& character) {
	stream << EntityClass::Character << (const Entity&)character;
	return stream;
}


bool SaveManager::saveEntity(const Entity& entity) {

	std::string filePath = entity_save_path + "/entity_" + std::to_string(entity.getIdentifier()) + ".entity";
	std::ofstream openedFile(filePath, std::fstream::trunc | std::fstream::binary | std::fstream::out);
	if (!openedFile) return 0;

	openedFile << entity;
	openedFile.close();
	return 1;
}

std::ifstream& operator>>(std::ifstream& stream, glm::vec3& vec) {
	stream >> vec.x >> vec.y >> vec.z;
	return stream;
}

std::ifstream& operator>>(std::ifstream& stream, State& state) {
	uint8_t temp;
	stream >> temp;
	state = (State)temp;
	return stream;
}

std::ifstream& operator>>(std::ifstream& stream, EntityClass& entityClass) {
	uint8_t temp;
	stream >> temp;
	entityClass = (EntityClass)temp;
	return stream;
}

std::ifstream& operator>>(std::ifstream& stream, Node& node) {

	glm::vec3 loc, rot, sca;

	stream >> loc >> rot >> sca;
	node.loc = loc;
	node.rot = rot;
	node.sca = sca;
	return stream;
}

std::ifstream& operator>>(std::ifstream& stream, Entity& entity) {
	Node head, node;
	State state;
	stream >> node >> head >> entity.state;
	entity.setNode(node);
	entity.setHead(head);
	return stream;
}

std::unique_ptr<Entity> SaveManager::getEntity(Identifier uid) {
	std::string filePath = entity_save_path + "/entity_" + std::to_string(uid) + ".entity";
	std::ifstream openedFile(filePath, std::fstream::binary | std::fstream::in);
	if (!openedFile.is_open())
		return std::unique_ptr<Entity>(nullptr);

	Entity* entity = nullptr;
	EntityClass entityClass;
	openedFile >> entityClass;
	switch (entityClass) {
	case EntityClass::Character:
		entity = new Character({});
	}
	openedFile >> *entity;

	return std::unique_ptr<Entity>(entity);

}