#include "SaveManager.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

#include <blocks/AllBlocks.hpp>



SaveManager::SaveManager(std::string save_path) {
	this->save_path = save_path;
	if (!std::filesystem::exists(save_path))
		std::filesystem::create_directories(save_path);

	//std::cout << "World will be saved at " << std::filesystem::current_path() << " in " save_path << std::endl;

}


std::unique_ptr<Chunk> SaveManager::getChunk(glm::ivec3 chunkPos) {
	std::string filePath = save_path + "/chunk_" + std::to_string(chunkPos.x) + "_" + std::to_string(chunkPos.y) + "_" + std::to_string(chunkPos.z) + ".chunk";
	std::ifstream openedFile(filePath);
	if (!openedFile.is_open()) {
		return std::unique_ptr<Chunk>(nullptr);

	}

	std::string readedLine;
	std::stringstream iss;
	int extractedValue;

	getline(openedFile, readedLine);	//Version management

	getline(openedFile, readedLine);	//Chunk dimensions x y z
	iss.str(readedLine);
	iss >> extractedValue;
	Chunk* newChunk = new Chunk(chunkPos, extractedValue);

	getline(openedFile, readedLine);	//Number of lines

	int offset = 0;
	while (getline(openedFile, readedLine)) //Bloc data
	{
		std::stringstream ss;
		ss.str(readedLine);
		ss >> extractedValue;
		int blockCount = extractedValue;
		ss >> extractedValue;
		
		BlockType block = (BlockType)extractedValue;
		for (int i = 0; i < blockCount; i += 1) {
			int z = (i + offset) / (newChunk->size.y * newChunk->size.x);
			int y = ((i + offset) / newChunk->size.x) % newChunk->size.y;
			int x = (i + offset) % newChunk->size.x;
			newChunk->setBlock(glm::ivec3(x, y, z), AllBlocks::create_static(block));
		}
		offset += blockCount;

	}
	openedFile.close();

	return std::unique_ptr<Chunk>(newChunk);
	
}



bool SaveManager::saveChunk(std::shared_ptr<Chunk> chunk) {

	std::string filePath = save_path + "/chunk_" + std::to_string(chunk->chunkPos.x) + "_" + std::to_string(chunk->chunkPos.y) + "_" + std::to_string(chunk->chunkPos.z) + ".chunk";
	std::ofstream openedFile(filePath, std::fstream::trunc);
	if (!openedFile) return 0;

	openedFile << "MonCraft v1.0.2" << std::endl;
	int chunkSaveLenght = chunk->size.x * chunk->size.y * chunk->size.z;
	openedFile << chunk->size.x << " " << chunk->size.y << " " << chunk->size.z << std::endl;
	openedFile << chunkSaveLenght << std::endl;
	int continuous = 1;
	BlockType last = chunk->getBlock(glm::vec3(0))->type;
	for (int i = 1; i < chunkSaveLenght; i += 1) {
		int z = i / (chunk->size.y * chunk->size.x);
		int y = (i / chunk->size.x) % chunk->size.y;
		int x = i % chunk->size.x;

		if (chunk->getBlock(glm::vec3(x, y, z))->type == last) {
			continuous++;
		}
		else {
			openedFile << continuous << " " << (int)last << std::endl;
			continuous = 1;
			last = chunk->getBlock(glm::vec3(x, y, z))->type;
		}

	}
	openedFile << continuous << " " << (int)last << std::endl;
	openedFile.close();
	return 1;
}