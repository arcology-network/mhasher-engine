#include "stdafx.h"
#include <iostream>

void c_interface_test()
{
	std::string source = "0123456789abcdef!@#$%^&*()+_)&*"; // 16

	std::string concatenated;
	std::vector<std::string> individualHashes;
	for (uint64_t i = 0; i < source.size(); i++)
	{
		std::string toHash = source.substr(i, 1);
		std::string hashStr = Hasher::Hash<CryptoPP::SHA3_256>(toHash);
		individualHashes.push_back(hashStr);
		concatenated.insert(concatenated.end(), hashStr.begin(), hashStr.end());
	}

	char rootHash[32];
	BinaryMhasherSHA3256((char*)&concatenated[0], source.size(), (char*)rootHash);

	auto Mhasher = Mhasher::Trie<CryptoPP::SHA3_256, 2>(individualHashes);

	std::string hash = Mhasher.GetHexRootHash();
	if (std::memcmp(&hash[0], rootHash, 32) == 0)
		std::cout << " Match !";
	else
		std::cout << "Error !";
}


void merkel_trie_insertion_test() {
	//Mhasher::Test::merkel_trie_insertion_test();
	std::string source = "0123456789abcdef!@#$%^&*()+_)&*"; // 16

	//std::string concatenated;
	std::vector<std::string> individualHashes;
	for (uint64_t i = 0; i < source.size(); i++)
	{
		std::string toHash = source.substr(i, 1);
		std::string hashStr = Hasher::Hash<CryptoPP::SHA3_256>(toHash);
		individualHashes.push_back(hashStr);
		//concatenated.insert(concatenated.end(), hashStr.begin(), hashStr.end());
	}

	clock_t t = clock();
	auto Mhasher = Mhasher::Trie<CryptoPP::SHA3_256, 2>(individualHashes);

	for (auto hash : individualHashes) {
		auto proofs = Mhasher.GetMhasherProof(hash);
		if (
			std::find(proofs.back().begin(), proofs.back().end(), Mhasher.GetHexRootHash()) == proofs.back().end() ||     // leaf hash matches 
			std::find(proofs.front().begin(), proofs.front().end(), hash) == proofs.front().end() ||    // root hash matches
			!Hasher::VerifyProofs<CryptoPP::SHA3_256, 2>(proofs)) {     // path verified 
		
			std::cout << "Failed to get proofs for " + hash << std::endl;
			return;
		}
	}
	std::cout << "All Mhasher proofs have been successfully retrieved and validated !" << std::endl;
	return;
}

void variable_raw_input_lengths_test() {
	int unitCount = 4;

	char  msg[] = "0123456789a";
	uint64_t* sizeVec = new uint64_t[unitCount];
	sizeVec[0] = 1;
	sizeVec[1] = 2;	
	sizeVec[2] = 3;
	sizeVec[3] = 5;

	std::string hash(20, 0);
	Mhasher::Trie<CryptoPP::RIPEMD160, 2>::Checksum(msg, strlen(msg), &hash[0]);
}

void performance_test_100_x_10000000() {
	int unitLength = 100;
	int unitCount = 10000000;

	char* msg = new char[unitCount * unitLength];
	std::fill(msg, msg + unitCount * unitLength, 19999999);

	uint64_t* sizeVec = new uint64_t[unitCount];
	std::fill(sizeVec, sizeVec + unitCount, unitLength);

	std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
	std::string sha256(32, 0);
	Mhasher::Trie<CryptoPP::SHA3_256, 2>::Checksum(msg, unitCount * unitLength, &sha256[0]);
	std::cout << "SHA3_256: " << unitCount <<" blocks "<<  " x " << unitLength << " bytes " << " in "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t0).count() << " ms" << std::endl;

	t0 = std::chrono::steady_clock::now();
	std::string md5(32, 0);
	Mhasher::Trie<CryptoPP::MD5, 2>::Checksum(msg, unitCount * unitLength, &md5[0]);
	std::cout << "MD5: " << unitCount << " blocks " << " x " << unitLength << " bytes " << " in " 
		<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t0).count() <<" ms" << std::endl;
}

void basic_2d_input_test() {
	uint64_t lengthVec[3] = { 3,3 };

	int row = 2;
	int col = 3;

	char** bytes = new char*[row];	
	bytes[0] = new char[col];
	std::fill_n(bytes[0], col,'a');
	bytes[1] = new char[col];
	std::fill_n(bytes[1], col, 'b');
	bytes[2] = new char[col];
	std::fill_n(bytes[2], col, 'c');

	char rootHash[CryptoPP::RIPEMD160::DIGESTSIZE];
	ChecksumRIPEMD1602D(bytes, lengthVec, 2, rootHash);
}

void sort_hashes() {
	std::vector<std::string> hashes(500000);
	std::string bytes;
	
	std::vector<uint64_t> idx(hashes.size(), 0);
	for (uint64_t i = 0; i < hashes.size(); i++) {
		std::string str = std::to_string(i);
		std::string hash = Hasher::Hash<CryptoPP::SHA3_256>(str);
		bytes += hash;
		memcpy(&idx[i], hash.data(), sizeof(uint64_t));
	}

	std::vector<uint64_t> indices(hashes.size());
	std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
	SortHash256(bytes.data(), hashes.size(), indices.data());
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t0).count() << " ms" << std::endl;

	std::vector<uint64_t> target(indices.size());
	for (std::size_t i = 0; i < indices.size(); i++) {
		target[indices[i]] = idx[i];
	}

	std::sort(idx.begin(), idx.end());
	if (idx == target) {
		std::cout << "Mismatch !" <<" Failed" ;
	}
	std::cout << "sort_hashes(): Successful !" << std::endl;
}

void unique_hashes() {
	std::vector<std::string> hashes(500000);
	std::string bytes;
	for (uint64_t i = 0; i < hashes.size(); i++) {
		std::string str = std::to_string(i);
		std::string hash = Hasher::Hash<CryptoPP::SHA3_256>(str);
		hashes[i] = (hash);
		bytes += hash;
	}

	std::vector<uint64_t> indices(hashes.size());
	std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
	uint64_t num = 0;
	std::string out = bytes;

	QuickUniqueHash256(bytes.data(), hashes.size(), out.data(), &num);
	std::cout << "QuickUniqueHash256(): "<< std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t0).count() << " ms" << std::endl;

	if (num != hashes.size()) {
		std::cout << "Mismatch !";
	}
	else {
		std::cout << "unique_hashes(): Successful !" << std::endl;
	}

	for (uint64_t i = 0; i < hashes.size(); i++) {
		std::string str = std::to_string(i);
		std::string hash = Hasher::Hash<CryptoPP::SHA3_256>(str);
		bytes += hash;
	}

	UniqueHash256(bytes.data(), hashes.size() * 2, out.data(), &num);

	std::vector<std::string> resultHashes;
	
	std::vector<unsigned> unitLength(num, 32);
	String::Reshape(out, unitLength, resultHashes);
		
	std::sort(resultHashes.begin(), resultHashes.end());
	std::sort(hashes.begin(), hashes.end());

	std::vector<std::string> v_intersection;
	std::set_intersection(resultHashes.begin(), resultHashes.end(), hashes.begin(), hashes.end(), std::back_inserter(v_intersection));

	if (v_intersection.size() != hashes.size() || v_intersection != resultHashes) {
		std::cout << "Mismatch !" << "Failed" << std::endl;
	}
	else {
		std::cout << "unique_hashes(): Successful !" << std::endl;
	}
}

