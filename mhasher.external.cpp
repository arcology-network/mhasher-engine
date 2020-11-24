#include "stdafx.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif

	void QuickUniqueHash256(char* bytes, uint64_t inCount, char* uniqueHashes, uint64_t* outCount) {
		Hashes::Unique<uint64_t, 32>(bytes, inCount, uniqueHashes, outCount);
	}

	void UniqueHash256(char* bytes, uint64_t inCount, char* uniqueHashes, uint64_t* outCount) {
		Hashes::Unique<std::array<char, 32>, 32>(bytes, inCount, uniqueHashes, outCount);
	}

	void SortHash256(char* bytes, uint64_t count, uint64_t* indices) {
		if (!Hashes::Sort<uint64_t, 32>(bytes, count, indices)) 
			Hashes::Sort<std::array<char, 32>, 32>(bytes, count, indices);
	}

	void ChecksumMD5(char* bytes, uint64_t count, char* rootHash) {
		Mhasher::Trie<CryptoPP::MD5, 2>::Checksum(bytes, count, rootHash);
	}

	/*==== Compute hashes from multiple raw input data sets, build the binary Mhasher trie and compute the root hash  ====*/
	void ChecksumKecaak256(char* bytes, uint64_t length, char* rootHash) {
		Mhasher::Trie<CryptoPP::Keccak_256, 2>::Checksum(bytes, length, rootHash);
	}

	void ChecksumRIPEMD160(char* bytes, uint64_t length, char* rootHash) {
		Mhasher::Trie<CryptoPP::RIPEMD160, 2>::Checksum(bytes, length, rootHash);
	}
	
	void ChecksumSHA3256(char* bytes,   uint64_t length, char* rootHash) {
		Mhasher::Trie<CryptoPP::SHA3_256, 2>::Checksum(bytes, length, rootHash);
	}

	/*============== 2D input array =============== */
	void ChecksumRIPEMD1602D(char** bytes, uint64_t* lengthVec, uint64_t count, char* rootHash)	{
		Mhasher::Trie<CryptoPP::RIPEMD160, 2>::MhasherFromRaw2D(bytes, lengthVec, count, rootHash);
	}

	void ChecksumSHA32562D(char** bytes, uint64_t* lengthVec, uint64_t count, char* rootHash){
		Mhasher::Trie<CryptoPP::SHA3_256, 2>::MhasherFromRaw2D(bytes, lengthVec, count, rootHash);
	}

	void ChecksumKecaak2562D(char** bytes, uint64_t* lengthVec, uint64_t count, char* rootHash)	{
		Mhasher::Trie<CryptoPP::Keccak_256, 2>::MhasherFromRaw2D(bytes, lengthVec, count, rootHash);
	}


	/*==============   Build binary Mhasher tree from hashes and compute root hash ==============*/
	void BinaryMhasherSHA3256(char* bytes, uint64_t count, char* hash) {
		Mhasher::Trie<CryptoPP::SHA3_256, 2>::RootFromHashes(bytes, count, hash);
	}

	void BinaryMhasherRIPEMD160(char* bytes, uint64_t count, char* hash) {
		Mhasher::Trie<CryptoPP::RIPEMD160, 2>::RootFromHashes(bytes, count, hash);
	}

	void BinaryMhasherKeccak256(char* bytes, uint64_t count, char* hash) {
		Mhasher::Trie<CryptoPP::Keccak_256, 2>::RootFromHashes(bytes, count, hash);
	}
	
	/*==============   Build Sexdec Mhasher tree from hashes and compute root hash ==============*/
	void SexdecMhasherKeccak256(char* bytes, uint64_t count, char* hash) {
		Mhasher::Trie<CryptoPP::Keccak_256, 16>::RootFromHashes(bytes, count, hash);
	}
	
	/* ==============  Compute hashes from single raw input ============== */
	void SingleHashKeccak256(char* bytes, uint64_t length, char* hash) {
		Hasher::Hash<CryptoPP::Keccak_256>(bytes, length, hash);
	}

	void SingleHashRIPEMD160(char* bytes, uint64_t length, char* hash) {
		Hasher::Hash<CryptoPP::RIPEMD160>(bytes, length, hash);
	}

	void SingleHashSHA3256(char* bytes, uint64_t length, char* hash) {
		Hasher::Hash<CryptoPP::SHA3_256>(bytes, length, hash);
	}

	/*==============  Compute hashes from multiple raw input  =============== */
	void MultipleHashesKecaak256(char* bytes, uint64_t* counts, uint64_t length, char* concatenatedHashes) {
		Mhasher::Trie<CryptoPP::Keccak_256, 2>::HashesFromRaw(bytes, counts, length, concatenatedHashes);
	}

	void MultipleHashesRIPEMD160(char* bytes, uint64_t* counts, uint64_t length, char* concatenatedHashes) {	//Compute hashes from multiple raw input using RIPEMD160
		Mhasher::Trie<CryptoPP::RIPEMD160, 2>::HashesFromRaw(bytes, counts, length, concatenatedHashes);
	}

	void MultipleHashesSHA3256(char* bytes, uint64_t* counts, uint64_t length, char* concatenatedHashes) {  	//Compute hashes from multiple raw input using RIPEMD160
		Mhasher::Trie<CryptoPP::SHA3_256, 2>::HashesFromRaw(bytes, counts, length, concatenatedHashes);
	}


	/*============== 2D input  =============== */
	void MultipleHashesKecaak2562D(char** bytes, uint64_t* counts, uint64_t length, char** hashes) {
		Mhasher::Trie<CryptoPP::Keccak_256, 2>::HashesFromRaw2D(bytes, counts, length, hashes);
	}

	void MultipleHashesRIPEMD1602D(char** bytes, uint64_t* counts, uint64_t length, char** hashes) { 	//Compute hashes from multiple raw input using RIPEMD160
		Mhasher::Trie<CryptoPP::RIPEMD160, 2>::HashesFromRaw2D(bytes, counts, length, hashes);
	}

	void MultipleHashesSHA32562D(char** bytes, uint64_t* counts, uint64_t length, char** hashes) {  	//Compute hashes from multiple raw input using RIPEMD160
		Mhasher::Trie<CryptoPP::SHA3_256, 2>::HashesFromRaw2D(bytes, counts, length, hashes);
	}

	void GetVersion(char* ver) {
		std::strcpy(ver, version);
	}

	void GetProduct(char* info)	{
		std::strcpy(info, product);
	}


#ifdef __cplusplus
}
#endif