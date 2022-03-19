#include "stdafx.h"

void Unique(char* bytes, uint32_t* lengths, uint32_t count, uint8_t* toKeep) {
	uint32_t pos = 0;
	std::vector<Address> strings(count);
	for (std::size_t i = 0; i < count; i++) {
		strings[i].ptr = bytes + pos;
		strings[i].length = lengths[i];
		strings[i].idx = i;
		pos += lengths[i];
	}

	tbb::parallel_sort(strings.begin(), strings.end());

	auto last = std::unique(strings.begin(), strings.end());
	tbb::parallel_for(std::size_t(0), std::size_t(std::distance(strings.begin(), last)), [&](std::size_t i) {
		toKeep[strings[i].idx] = 255;
	});
}

void UniqueSort(char* bytes, uint32_t* lengths, uint32_t count, uint32_t* newIndices, uint32_t* outCount) {
	uint32_t pos = 0;
	std::vector<Address> strings(count);
	for (std::size_t i = 0; i < count; i++) {
		strings[i].ptr = bytes + pos;
		strings[i].length = lengths[i];
		strings[i].idx = i;
		pos += lengths[i];
	}

	tbb::parallel_sort(strings.begin(), strings.end(), [](auto lhs, auto rhs) {
		int flag = memcmp(lhs.ptr, rhs.ptr, std::min<std::size_t>(lhs.length, rhs.length));
		if (flag == 0)
			return lhs.length < rhs.length;

		return flag < 0;
	});

	auto last = std::unique(strings.begin(), strings.end());
	tbb::parallel_for(std::size_t(0), std::size_t(std::distance(strings.begin(), last)), [&](std::size_t i) {
		newIndices[i] = strings[i].idx;
	});

	*outCount = std::distance(strings.begin(), last);
}

void Sort(char* bytes, uint32_t* lengths, uint32_t count, uint32_t* newIndices) {
	uint32_t pos = 0;
	std::vector<Address> strings(count);
	for (std::size_t i = 0; i < count; i++) {
		strings[i].ptr = bytes + pos;
		strings[i].length = lengths[i];
		strings[i].idx = i;
		pos += lengths[i];
	}

	tbb::parallel_sort(strings.begin(), strings.end(), [](auto lhs, auto rhs) {
		int flag = memcmp(lhs.ptr, rhs.ptr, std::min<std::size_t>(lhs.length, rhs.length)) ;
		if (flag == 0)
			return lhs.length < rhs.length;

		return flag < 0;
	});

	tbb::parallel_for(std::size_t(0), strings.size(), [&](std::size_t i) {
		newIndices[i] = strings[i].idx;
	});
}

void Remove(char* bytes, uint32_t* lengths, uint32_t count, char* toRemove, uint32_t* toRemoveLengths, uint32_t toRemoveCount, uint8_t* toKeep) {
	if (count == 0)
		return;

	// Source entries to remove from
	std::vector<Address> strings(count);
	std::vector<uint32_t> toRemovePositions(toRemoveCount + 1, 0);
	std::vector<uint32_t> srcPositions(count + 1, 0);

	for (std::size_t i = 1; i < toRemovePositions.size(); i++) {
		toRemovePositions[i] = toRemovePositions[i - 1] + toRemoveLengths[i - 1];
	}

	for (std::size_t i = 1; i < srcPositions.size(); i++) {
		srcPositions[i] = srcPositions[i - 1] + lengths[i - 1];
	}

	tbb::parallel_for(std::size_t(0), std::size_t(count), [&](std::size_t i) {
		//for (std::size_t i = 0; i < std::size_t(count); i++) {
		toKeep[i] = 255;
	});
	
	for (std::size_t i = 0; i < count; i++) {  // Faster than the parallelized version
		strings[i].ptr = bytes + srcPositions[i];
		strings[i].length = lengths[i];
		strings[i].idx = i;
	}
	tbb::parallel_sort(strings.begin(), strings.end());

	tbb::parallel_for(std::size_t(0), std::size_t(toRemoveCount), [&](std::size_t i) {
		//for (std::size_t i = 0; i < std::size_t(toRemoveCount); i++) {
		auto iter = std::equal_range(strings.begin(), strings.end(), Address(toRemove + toRemovePositions[i], toRemoveLengths[i], i));
		for (auto it = iter.first; it != iter.second ; it++) {
			toKeep[it->idx] = 0; //
		}			
	});
}
