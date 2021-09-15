#include "stdafx.h"
struct Compare {
	bool operator () (const std::pair<char*, uint32_t>& lhs, const std::pair<char*, uint32_t>& rhs) const {
		if (lhs.second != rhs.second)
			return lhs.second < rhs.second;
		return std::memcmp(lhs.first, rhs.first, rhs.second) < 0;
	}
};

struct Presorter {
	std::set<std::pair<char*, uint32_t>, Compare> buffer;
	uint32_t offset = 0;
};

void* Start() {
	return new std::set<std::pair<char*, uint32_t>, Compare>();
}

void ToBuffer(void* engine, char* path, char* pathLen, uint32_t count) {
	auto presorter = (Presorter*)engine;
	uint32_t pos = 0;
	for (std::size_t i = 0; i < count; i++) {
		presorter->buffer.insert({ path + pos, ((uint32_t*)pathLen)[i] });
		pos += ((uint32_t*)pathLen)[i];
	}		
}

void GetBufferSize(void* engine, uint32_t* count) {
	auto presorter = (Presorter*)engine;
	*count = presorter->offset;
}

void FromBuffer(void* engine, char* buffer, char* lengths, uint32_t* count) {
	uint32_t pos = 0;
	uint32_t i = 0;
	auto presorter = (Presorter*)engine;
	for (auto iter = presorter->buffer.begin(); iter != presorter->buffer.end(); iter++) {
		std::memcpy(buffer + pos, iter->first, iter->second);
		((uint32_t*)lengths)[i++] = iter->second;
		pos += iter->second;
	}
	*count = presorter->buffer.size();
}

void Clear(void* engine) {
	((std::set<std::pair<char*, uint32_t>, Compare>*)engine)->clear();
}

void Stop(void* engine) {
	delete (std::set<std::pair<char*, uint32_t>, Compare>*)engine;
}