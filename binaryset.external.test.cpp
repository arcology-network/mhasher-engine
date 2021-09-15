#include "stdafx.h"

bool test_sort_strings() {
	auto paths = std::vector<std::string>{
		"ctrn0/ctrn02/ctrn000/elem4",
		"ctrn0/ctrn00",
		"ctrn0/ctrn00/elem1",
		"ctrn0/ctrn00/elem0",
		"ctrn0/ctrn01/ctrn000/elem3/ele5",
		"ctrn0/ctrn00/elem2",
	};

	std::string bytes;
	std::vector<uint32_t> lengths;
	String::Concatenate(paths, bytes, lengths);
	std::vector<uint32_t> indices(paths.size());

	Sort(bytes.data(), lengths.data(), paths.size(), indices.data());

	std::vector<std::string> sorted(paths.size());
	for (int i = 0; i < indices.size(); i ++) {
		sorted[i] = paths[indices[i]];
	}
	
	return sorted == std::vector<std::string> {
		"ctrn0/ctrn00",
		"ctrn0/ctrn00/elem0",
		"ctrn0/ctrn00/elem1",
		"ctrn0/ctrn00/elem2",
		"ctrn0/ctrn02/ctrn000/elem4",
		"ctrn0/ctrn01/ctrn000/elem3/ele5",	
	};
}

bool test_string_engine_unique_sort() {
	auto paths = std::vector<std::string>{
		"ctrn0/ctrn02/ctrn000/elem4",
		"ctrn0/ctrn00",
		"ctrn0/ctrn00/elem1",
		"ctrn0/ctrn00/elem0",
		"ctrn0/ctrn00/elem2",
		"ctrn0/ctrn01/ctrn000/elem3/ele5",
		"ctrn0/ctrn00/elem2",
	};

	std::string bytes;
	std::vector<uint32_t> lengths;
	String::Concatenate(paths, bytes, lengths);
	std::vector<char> buffer(1024);
	
	void* engine = Start();
	ToBuffer(engine, (char*)bytes.data(), (char*)lengths.data(), paths.size());

	uint32_t count = 0;
	std::vector<uint32_t> outputLens(paths.size(), 0);
	FromBuffer(engine, (char*)buffer.data(), (char*)outputLens.data(), &count);
	

	std::vector<std::string> sorted(paths.size());
	uint32_t pos = 0;
	for (int i = 0; i < count; i++) {
		sorted[i] = std::string(buffer.data() + pos, outputLens[i]);
		pos += outputLens[i];
	}
	
	sorted.resize(count);
	return sorted == std::vector<std::string> {
			"ctrn0/ctrn00",
			"ctrn0/ctrn00/elem0",
			"ctrn0/ctrn00/elem1",
			"ctrn0/ctrn00/elem2",
			"ctrn0/ctrn02/ctrn000/elem4",
			"ctrn0/ctrn01/ctrn000/elem3/ele5",
	};
	Stop(engine);
}

void benchmark_string_engine_unique_sort_1m() {
	std::vector< std::string> paths(1000000);
	int size = 0;
	for (int i = 999999; i >= 0; i--) {
		paths[i] = ("ctrn0-" + std::to_string(i) + "/" + "elem-" + std::to_string(i));
		size += paths[i].size();
	}

	std::string bytes;
	std::vector<uint32_t> lengths;
	String::Concatenate(paths, bytes, lengths);
	std::vector<char> buffer(size);

	void* engine = Start();
	auto t0 = std::chrono::steady_clock::now();
	ToBuffer(engine, (char*)bytes.data(), (char*)lengths.data(), paths.size());
	std::cout << "ToBuffer() 1000000 entries: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t0).count() << " ms" << std::endl;

	t0 = std::chrono::steady_clock::now();
	uint32_t count = 0;
	GetBufferSize(engine, &count);
	std::cout << "GetBufferSize() 1000000 entries: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t0).count() << " ms" << std::endl;

	count = 0;
	t0 = std::chrono::steady_clock::now();
	std::vector<uint32_t> outputLens(paths.size(), 0);
	FromBuffer(engine, (char*)buffer.data(), (char*)outputLens.data(), &count);
	std::cout << "FromBuffer() 1000000 entries: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t0).count() << " ms" << std::endl;

	t0 = std::chrono::steady_clock::now();
	Stop(engine);
	std::cout << "Stop(engine): " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t0).count() << " ms" << std::endl;

	std::vector<std::string> sorted(paths.size());
	uint32_t pos = 0;
	for (int i = 0; i < count; i++) {
		sorted[i] = std::string(buffer.data() + pos, outputLens[i]);
		pos += outputLens[i];
	}
}


void benchmark_sort_strings_1m() {
	std::vector< std::string> paths;
	for (int i = 1000000 ; i > 0; i--) {
		paths.push_back("ctrn0-" + std::to_string(i) + "/" + "elem-" + std::to_string(i));
	}

	std::cout << "Concatenate input paths";
	std::string bytes;
	std::vector<uint32_t> lengths;
	String::Concatenate(paths, bytes, lengths);
	std::vector<uint32_t> indices(paths.size());
	std::cout << "====> Completed !" << std::endl;
	
	auto t0 = std::chrono::steady_clock::now();
	Sort(bytes.data(), lengths.data(), paths.size(), indices.data());
	std::cout << "Sort() 1000000 entries: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t0).count() << " ms" << std::endl;

	std::vector<std::string> sortedPaths(paths.size());
	for (auto i = 0; i < indices.size(); i++) {
		sortedPaths[i] = paths[indices[i]];
	}
}

bool test_unique_strings() {
	auto paths = std::vector<std::string>{
		"ctrn0/ctrn00",+
		"ctrn0/ctrn00/elem0",
		"ctrn0/ctrn00/elem1",
		"ctrn0/ctrn00/elem2",
		"ctrn0/ctrn00/ctrn000/elem3",
		"ctrn0/ctrn00/ctrn000/elem4",
		"ctrn0/ctrn01/elem3",
		"ctrn0/ctrn01/elem4",
		"ctrn0/ctrn02/elem3",
		"ctrn0/ctrn02/elem4",
		"ctrn0/ctrn00/elem0",
		"ctrn0/ctrn00/elem2",
	};

	std::string bytes;
	std::vector<uint32_t> lengths;
	String::Concatenate(paths, bytes, lengths);
	std::vector<uint8_t> toKeep(paths.size(), 0);

	std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
	Unique(bytes.data(), lengths.data(), lengths.size(), toKeep.data());
	std::cout << "test_unique_strings: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t0).count() << " ms" << std::endl;

	auto num = std::count_if(toKeep.begin(), toKeep.end(), [](uint8_t idx) { return idx == 255; });
	return num == 10;
}

void benchamark_unique_strings_1m() {
	std::vector< std::string> paths;
	for (int i = 1000000; i > 0; i--) {
		paths.push_back("ctrn0-" + std::to_string(i) + "/" + "elem-" + std::to_string(i));
	}

	std::string bytes;
	std::vector<uint32_t> lengths;
	String::Concatenate(paths, bytes, lengths);
	std::vector<uint8_t> toKeep(paths.size());

	std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
	Unique(bytes.data(), lengths.data(), lengths.size(), toKeep.data());
	std::cout << "Unique()  1000000 entries: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t0).count() << " ms" << std::endl;
}

bool test_remove_strings() {
	std::vector< std::string> paths;	
	paths.push_back("ctrn0-" + std::to_string(0) + "/" + "elem-" + std::to_string(1));
	paths.push_back("ctrn0-" + std::to_string(0) + "/" + "elem-" + std::to_string(45));
	paths.push_back("ctrn0-" + std::to_string(0) + "/" + "elem-" + std::to_string(6789));
	paths.push_back("ctrn0-" + std::to_string(0) + "/" + "elem-" + std::to_string(11234));
	paths.push_back("ctrn0-" + std::to_string(0) + "/" + "elem-" + std::to_string(11234));
	//paths.push_back("ctrn0-" + std::to_string(0) + "/" + "elem-" + std::to_string(11));
	paths.push_back("ctrn0-" + std::to_string(0) + "/" + "elem-" + std::to_string(89));
	paths.push_back("ctrn0-" + std::to_string(0) + "/" + "elem-" + std::to_string(89));

	std::string bytes;
	std::vector<uint32_t> lengths;
	String::Concatenate(paths, bytes, lengths);

	std::vector<std::string> toRemove  {
		("ctrn0-" + std::to_string(0) + "/" + "elem-" + std::to_string(11234)),
		("ctrn0-" + std::to_string(0) + "/" + "elem-" + std::to_string(89))
	};
	
	std::string toRemoveBytes;
	std::vector<uint32_t> toRemoveLengths;
	String::Concatenate(toRemove, toRemoveBytes, toRemoveLengths);
	std::vector<uint8_t> toKeep(paths.size(), 255);

	Remove(bytes.data(), lengths.data(), lengths.size(), toRemoveBytes.data(), toRemoveLengths.data(), toRemove.size(), toKeep.data());

	// Check the results
	auto num = std::count_if(toKeep.begin(), toKeep.end(), [](uint8_t idx) { return idx == 255; });
	return num == 3;
}

bool test_remove_strings_2() {
	std::vector< std::string> paths;
	for (int i = 0; i < 10; i++) {
		paths.push_back("ctrn0-" + std::to_string(i) + "/" + "elem-" + std::to_string(i));
	}

	std::string bytes;
	std::vector<uint32_t> lengths;
	String::Concatenate(paths, bytes, lengths);

	std::vector<std::string> toRemove{
		("ctrn0-" + std::to_string(0) + "/" + "elem-" + std::to_string(0)),
		("ctrn0-" + std::to_string(0) + "/" + "elem-" + std::to_string(1))
	};

	std::string toRemoveBytes;
	std::vector<uint32_t> toRemoveLengths;
	String::Concatenate(toRemove, toRemoveBytes, toRemoveLengths);
	std::vector<uint8_t> toKeep(paths.size(), 0);
	Remove(bytes.data(), lengths.data(), lengths.size(), toRemoveBytes.data(), toRemoveLengths.data(), toRemove.size(), toKeep.data());
	
	// Check the results
	auto num = std::count_if(toKeep.begin(), toKeep.end(), [](uint32_t idx) { return idx == 255; });
	return num == 9;
}

void benchmark_remove_strings_1m() {
	std::vector< std::string> paths;
	for (auto i = 0; i < 1000000; i ++) {  //length problem !!!
		paths.push_back("ctrn0-" + std::to_string(0) + "/" + "elem-" + std::to_string(i));
	}

	std::string bytes;
	std::vector<uint32_t> lengths;
	String::Concatenate(paths, bytes, lengths);

	std::vector<std::string> toRemove = paths;
	std::string toRemoveBytes;
	std::vector<uint32_t> toRemoveLengths;
	String::Concatenate(toRemove, toRemoveBytes, toRemoveLengths);
	std::vector<uint8_t> toKeep(paths.size(), 0);

	std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
	Remove(bytes.data(), lengths.data(), lengths.size(), toRemoveBytes.data(), toRemoveLengths.data(), toRemove.size(), toKeep.data());
	std::cout << "benchmark_remove_strings_1(): Remove 1000000 entries in: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t0).count() << " ms" << std::endl;

	// Check the results
	auto num = std::count(toKeep.begin(), toKeep.end(), 255);
}