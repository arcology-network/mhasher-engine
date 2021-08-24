#pragma once
#include "stdafx.h"

namespace Hasher {
	const static unsigned pieces = 8096;

	static std::string HexToString(std::string digest)	{
		std::string result;

		CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(result));
		CryptoPP::StringSource(digest, true, new CryptoPP::Redirector(encoder));

		std::transform(result.begin(), result.end(), result.begin(), ::tolower);
		return result;
	}

	static std::string StringToHex(std::string& encoded)	{
		std::string decoded;
		CryptoPP::HexDecoder decoder(new CryptoPP::StringSink(decoded));
		decoder.Put((unsigned char*)encoded.data(), encoded.size());
		decoder.MessageEnd();

		return  decoded;
	}

	template<typename T, unsigned N>
	static std::string Hash(std::array<std::string, N>& hashes)	{
		T hasher;
		for (unsigned i = 0; i < hashes.size(); i++)
			hasher.Update((const unsigned char*)hashes[i].data(), hashes[i].size());

		std::string hash(hasher.DigestSize(), '0');
		hasher.Final((unsigned char*)&hash.data()[0]);
		return hash;
	}

	template<typename H>
	static std::string Hash(std::string& msg)	{
		H hasher;
		std::string digest(hasher.DigestSize(), '0');
		hasher.CalculateDigest((unsigned char*)digest.data(), (const unsigned char*)msg.data(), msg.size());
		return digest;
	}

	template<typename H>
	void Hash(char* bytes, uint64_t length, char* hash)	{
		std::string msg(length, '0');
		std::memcpy((char*)&msg[0], bytes, length);

		std::string hashStr = Hasher::Hash<H>(msg);
		std::memcpy(hash, &hashStr[0], H::DIGESTSIZE);
	}

	template<unsigned N>
	static std::string BytesToHash(std::string& bytes, uint64_t loc) {
		char buffer[N + 1];
		std::memcpy(&buffer[0], &bytes[loc], N);
		buffer[N] = '\0';
		return std::string(buffer);
	}

	template<typename H, unsigned N>
	static bool VerifyProofs(std::vector<std::array<std::string, N>>& path)	{
		if (path.empty())
			return false;

		for (unsigned i = 0; i < path.size() - 1; i++)	{
			std::string raw = Hasher::Hash<H, N>(path[i]);
			std::string hash = Hasher::HexToString(raw);

			bool flag = false;
			for (uint64_t j = 0; j < N; j++)
			{
				if (Hasher::HexToString(path[i + 1][j]) == hash)
					flag = true;
			}
			if (!flag)
				return false;
		}
		return true;
	}
	
	static void Recut(char* bytes, uint64_t* counts, uint64_t length, unsigned unitSize, std::vector<unsigned>& newSizes) {
		newSizes.clear();

		unsigned total = std::accumulate(counts, counts + length, 0, [&](unsigned sum, auto& iter) { return iter + sum; });		
		if (total <= unitSize)
			newSizes.push_back(unitSize);
		else {
			newSizes.resize(std::ceil(total / unitSize), unitSize);
			newSizes.back() += total % unitSize;
		}
		assert(total == std::accumulate(newSizes.begin(), newSizes.end(), 0, [&](unsigned sum, auto& iter) { return iter + sum; }));
	}

	static void Reslice(uint64_t length, std::vector<unsigned>& newSizes) {
		if (length < Hasher::pieces)		{
			newSizes.push_back(length);
		}
		else {
			newSizes.clear();
			newSizes.resize(Hasher::pieces, length / Hasher::pieces);
			newSizes.back() += length % Hasher::pieces;
		}
		assert(length == std::accumulate(newSizes.begin(), newSizes.end(), 0, [&](unsigned sum, auto& iter) { return iter + sum; }));
	}
}
