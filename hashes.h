#pragma once
namespace Hashes {
	template<typename T, int H>
	static void Sort(char* bytes, std::vector<std::pair<T, uint64_t>>& numbers) {
		tbb::parallel_for(std::size_t(0), std::size_t(numbers.size()), [&](std::size_t i) {
			numbers[i].second = i;
			memcpy(&numbers[i].first, bytes + i * H, sizeof(T));
		});
		tbb::parallel_sort(numbers.begin(), numbers.end(), [&](auto& lft, auto& rgt) { return lft.first < rgt.first; });
	}

	template<typename T, int H>
	static bool Sort(char* bytes, uint64_t count, uint64_t* indices) {
		std::vector<std::pair<T, uint64_t>> numbers(count);
		Sort<T, H>(bytes, numbers);
				
		tbb::parallel_for(std::size_t(0), numbers.size(), [&](std::size_t i) {
			indices[i] = numbers[i].second;
		});

		return std::distance(numbers.begin(),
			std::unique(numbers.begin(), numbers.end(), [](auto& lft, auto& rgt) {return lft.first == rgt.first; })) == numbers.size();
	}
	
	template<typename T, int H>
	static void Unique(char* bytes, uint64_t inCount, char* uniqueHashes, uint64_t* outCount) {
		std::vector<std::pair<T, uint64_t>> numbers(inCount);
		Sort<T, H>(bytes, numbers);

		*outCount = std::distance(numbers.begin(), 
			std::unique(numbers.begin(), numbers.end(), [](auto& lft, auto& rgt) {return lft.first == rgt.first; }));

		tbb::parallel_for(std::size_t(0), *outCount, [&](std::size_t i) {
			memcpy(uniqueHashes + i * H, bytes + numbers[i].second * H, H);
		});
	}
}

