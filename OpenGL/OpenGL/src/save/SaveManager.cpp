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
	std::ifstream openedFile(filePath, std::fstream::binary);
	if (!openedFile.is_open()) 
		return std::unique_ptr<Chunk>(nullptr);

	////MonCraft version
	//char versionBuffer[3];
	//openedFile.read(versionBuffer, 3);

	//Chunk size
	char chunkSizeBuffer;
	openedFile.read(&chunkSizeBuffer, 1);

	Chunk* newChunk = new Chunk(chunkPos, chunkSizeBuffer);

	int offset = 0;
	char buffer[3];
	while (openedFile.read(buffer, 3)) //Block data
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

	std::string filePath = save_path + "/chunk_" + std::to_string(chunk->chunkPos.x) + "_" + std::to_string(chunk->chunkPos.y) + "_" + std::to_string(chunk->chunkPos.z) + ".chunk";
	std::ofstream openedFile(filePath, std::fstream::trunc | std::fstream::binary);
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
			continuous++;
		}
		else {
			char buffer[3];
			buffer[0] = continuous / 256;
			buffer[1] = continuous % 256;
			buffer[2] = (char)last;
			openedFile.write(buffer, 3);
			continuous = 1;
			last = chunk->getBlock(glm::vec3(x, y, z))->type;
		}

	}
	char buffer[3];
	buffer[0] = continuous / 256;
	buffer[1] = continuous % 256;
	buffer[2] = (char)last;
	openedFile.write(buffer, 3);

	openedFile.close();
	return 1;
}