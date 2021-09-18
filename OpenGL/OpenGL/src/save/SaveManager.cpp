#include "SaveManager.hpp"
#include <fstream>
#include <iostream>
#include<sstream>

#include <blocks/AllBlocks.hpp>



SaveManager::SaveManager(std::string save_path) : save_path(save_path) {

}

SaveManager::~SaveManager() {
	openedFile.close();
}

std::unique_ptr<Chunk> SaveManager::getChunk(glm::ivec3 chunkPos) {
	std::string filePath = save_path + "chunk_" + std::to_string(chunkPos.x) + "_" + std::to_string(chunkPos.y) + "_" + std::to_string(chunkPos.z) + ".dat";
	openedFile.open(filePath, std::fstream::in);
	if (!openedFile) return std::unique_ptr<Chunk>(nullptr);

	std::string readedLine;
	std::stringstream ss;
	int extractedValue;

	getline(openedFile, readedLine);	//Version management

	getline(openedFile, readedLine);	//Chunk dimensions x y z
	ss.str(readedLine);
	ss >> extractedValue;
	Chunk* newChunk = new Chunk(chunkPos, extractedValue);

	getline(openedFile, readedLine);	//Number of lines

	int offset = 0;
	while (getline(openedFile, readedLine)) //Bloc data
	{
		ss.str(readedLine);
		ss >> extractedValue;
		int blockCount = extractedValue;
		ss >> extractedValue;
		BlockType block = (BlockType)extractedValue;
		for (int i = 0; i < blockCount; i += 1) {
			int x = (i + offset) / (newChunk->size.y * newChunk->size.z);
			int y = (i + offset) / newChunk->size.z;
			int z = (i + offset) % newChunk->size.z;
			newChunk->setBlock(glm::ivec3(x, y, z), AllBlocks::create_static(block));
		}
		offset += blockCount;

	}


	
}



bool SaveManager::saveChunk(Chunk chunk) {

	std::string filePath = save_path + "chunk_" + std::to_string(chunk.chunkPos.x) + "_" + std::to_string(chunk.chunkPos.y) + "_" + std::to_string(chunk.chunkPos.z) + ".dat";
	openedFile.open(filePath, std::fstream::out | std::fstream::trunc);

	openedFile << "MonCraft v1.0.2" << std::endl;
	int chunkSaveLenght = chunk.size.x * chunk.size.y * chunk.size.z;
	openedFile << chunk.size.x << " " << chunk.size.y << " " << chunk.size.z << std::endl;
	openedFile << chunkSaveLenght << std::endl;
	int continuous = 1;
	BlockType last = chunk.getBlock(glm::vec3(0))->type;
	for (int i = 1; i < chunkSaveLenght; i += 1) {
		int x = i / (chunk.size.y * chunk.size.z);
		int y = i / chunk.size.z;
		int z = i % chunk.size.z;

		if (chunk.getBlock(glm::vec3(x, y, z))->type == last) {
			continuous++;
		}
		else {
			openedFile << continuous << " " << (int)last << std::endl;
			continuous = 1;
			last = chunk.getBlock(glm::vec3(x, y, z))->type;
		}

	}
	openedFile << continuous << " " << (int)last << std::endl;
	openedFile.close();
}