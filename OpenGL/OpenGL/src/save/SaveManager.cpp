#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

#include "blocks/AllBlocks.hpp"
#include "SaveManager.hpp"
#include "entity/character/Character.hpp"
#include "debug/Debug.hpp"

template<typename T>
class Binary {
public:
	Binary() {  }
	Binary(T val) : val(val) {  }
	T val;
};
template<typename T>
std::ostream& operator<<(std::ostream& stream, Binary<T>& bin) {
	stream.write((char*)&bin.val, sizeof(T));
	return stream;
}
template<typename T>
std::istream& operator>>(std::istream& stream, Binary<T>& bin) {
	stream.read((char*)&bin.val, sizeof(T));
	return stream;
}

SaveManager::SaveManager(std::string save_path) {
	this->save_path = save_path;
	if (!std::filesystem::exists(save_path))
		std::filesystem::create_directories(save_path);

	//std::cout << "World will be saved at " << std::filesystem::current_path() << " in " save_path << std::endl;

}

std::ostream& operator<<(std::ostream& stream, const glm::vec3& vec) {
	Binary<int> x = vec.x, y = vec.y, z = vec.z;
	stream << x << y << z;
	return stream;
}
std::istream& operator>>(std::istream& stream, glm::vec3& vec) {
	Binary<int> x, y, z;
	stream >> x >> y >> z;
	vec.x = x.val;
	vec.y = y.val;
	vec.z = z.val;
	return stream;
}

std::ostream& operator<<(std::ostream& stream, State state) {
	Binary temp((uint8_t)state);
	stream << temp;
	return stream;
}
std::istream& operator>>(std::istream& stream, State& state) {
	Binary<uint8_t> temp;
	stream >> temp;
	state = (State)temp.val;
	return stream;
}

std::ostream& operator<<(std::ostream& stream, EntityClass entityClass) {
	Binary temp((uint8_t)entityClass);
	stream << temp;
	return stream;
}
std::istream& operator>>(std::istream& stream, EntityClass& entityClass) {
	Binary<uint8_t> temp;
	stream >> temp;
	entityClass = (EntityClass)temp.val;
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const Node& node) {
	stream << node.loc << node.rot << node.sca;
	return stream;
}
std::istream& operator>>(std::istream& stream, Node& node) {
	glm::vec3 loc, rot, sca;

	stream >> loc >> rot >> sca;
	node.loc = loc;
	node.rot = rot;
	node.sca = sca;
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const Entity& entity) {
	stream << entity.getNode() << entity.getHead() << entity.state;
	return stream;
}
std::istream& operator>>(std::istream& stream, Entity& entity) {
	Node head, node;
	stream >> node >> head >> entity.state;
	entity.setNode(node);
	entity.setHead(head);
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const Character& character) {
	stream << EntityClass::Character << (const Entity&)character;
	return stream;
}

bool SaveManager::saveEntity(const Entity& entity) {
	std::string filePath = save_path + "/entity_" + std::to_string(entity.uid) + ".entity";
	std::ofstream openedFile(filePath, std::fstream::trunc | std::fstream::binary | std::fstream::out);
	if (!openedFile) return 0;

	openedFile << entity;
	openedFile.close();
	return 1;
}

std::unique_ptr<Entity> SaveManager::getEntity(Identifier uid) {
	std::string filePath = save_path + "/entity_" + std::to_string(uid) + ".entity";
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



std::ostream& operator<<(std::ostream& stream, BlockType& type) {
	Binary bin((uint8_t)type);
	stream << bin;
	return stream;
}
std::istream& operator>>(std::istream& stream, BlockType& type) {
	Binary<uint8_t> temp;
	stream >> temp;
	type = (BlockType)temp.val;
	return stream;
}

std::istream& operator>>(std::istream& stream, Chunk& chunk) {
	int maxIndex = chunk.size.x * chunk.size.y * chunk.size.z;
	int offset = 0;

	while(offset != maxIndex) {
		Binary<uint16_t> blockCount;
		BlockType type;
		stream >> blockCount >> type;

		for (int i = offset + 0; i < offset + blockCount.val; i += 1) {
			chunk.at(i) = AllBlocks::create_static(type);
		}

		offset += blockCount.val;
	}

	return stream;
}
std::ostream& operator<<(std::ostream& stream, Chunk const& chunk) {
	int maxIndex = chunk.size.x * chunk.size.y * chunk.size.z;
	Binary<uint16_t> blockCount(1);
	BlockType last = chunk.at(0)->type;

	for (int i = 1; i < maxIndex; i += 1) {
		if (chunk.at(i)->type == last) {
			blockCount.val += 1;
		}
		else {
			stream << blockCount << last;
			blockCount.val = 1;
			last = chunk.at(i)->type;
		}
	}

	stream << blockCount << last;
	return stream;
}

std::unique_ptr<Chunk> SaveManager::getChunk(glm::ivec3 chunkPos) {
	std::string filePath = save_path + "/chunk_"
		+ std::to_string(chunkPos.x) + "_"
		+ std::to_string(chunkPos.y) + "_"
		+ std::to_string(chunkPos.z) + ".chunk";

	std::ifstream openedFile(filePath, std::fstream::binary);
	if (!openedFile.is_open()) return nullptr;


	Binary<uint8_t> chunkSize;
	openedFile >> chunkSize;

	if(chunkPos == glm::ivec3(0, 2, -1)) {
	}

	Chunk* newChunk = new Chunk(chunkPos, chunkSize.val);
	openedFile >> *newChunk;
	openedFile.close();

	return std::unique_ptr<Chunk>(newChunk);
}

bool SaveManager::saveChunk(Chunk const& chunk) {
	std::string filePath = save_path + "/chunk_"
		+ std::to_string(chunk.chunkPos.x) + "_"
		+ std::to_string(chunk.chunkPos.y) + "_"
		+ std::to_string(chunk.chunkPos.z) + ".chunk";

	std::ofstream openedFile(filePath, std::fstream::trunc | std::fstream::binary);
	if (!openedFile) return false;

	Binary<uint8_t> chunkSize(chunk.size.x);
	openedFile << chunkSize << chunk;
	openedFile.close();
	return true;
}

sf::Packet& operator<<(sf::Packet& packet, Chunk const& chunk) {
	std::ostringstream stream;
	stream << chunk;
	packet << stream.str();
	return packet;
}

sf::Packet& operator>>(sf::Packet& packet, Chunk& chunk) {
	std::string str;
	packet >> str;
	std::stringstream stream(str);
	stream >> chunk;
	return packet;
}
