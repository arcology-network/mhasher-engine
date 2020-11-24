#include "stdafx.h"

namespace Mhasher {
	template<typename H, unsigned N>
	struct Trie	{
		std::string GetHexRootHash() { return rootHash; }
		std::string GetRootHash() { return Hasher::HexToString(GetHexRootHash()); }

		unsigned Count() { // total number of entires in the whole trie		
			return std::accumulate(hierarchy.begin(), hierarchy.end(), 0, [&](unsigned sum, auto& iter) { return iter.size() + sum; });
		}

		Trie(std::vector<std::string>& hashes)	{
			std::string concatenated;
			String::ConcatenateUniformStrings(hashes, concatenated);

			Init(concatenated, H::DIGESTSIZE);
		}

		Trie(std::string concatenated) {
			Init(concatenated, H::DIGESTSIZE);
		}

		void Init(std::string& current, uint64_t hashLength) {
			unsigned numHashes = current.size() / hashLength;
			while (true) {
				if (numHashes > 1) {
					numHashes = std::ceil(numHashes / (float)N) * N;
					current.resize(numHashes * hashLength, '0');
					hierarchy.push_back(current);
				}
				else {
					hierarchy.push_back(current);
					break;
				}

				std::string next(current.size() / N, '0');
				//for (unsigned i = 0; i < numHashes; i += N)
				tbb::parallel_for(std::size_t(0), std::size_t(numHashes), std::size_t(N), [&](std::size_t i) {
					std::array<std::string, N> section;
					for (unsigned j = 0; j < N; j++)
						section[j] = current.substr(i * hashLength + (hashLength * j), hashLength);

					std::string hash = Hasher::Hash<H, N>(section);
					next.replace(hashLength * (i / N), hash.size(), hash);
				}
				);
				numHashes = numHashes / N;
				current = next;
			}
			rootHash = hierarchy.back();
		}


		std::vector<std::array<std::string, N>> GetMhasherProof(std::string& targetHash) {
			std::vector<std::array<std::string, N>> proofs;
			int64_t pos = -1;

			auto first = hierarchy.begin();
			for (unsigned i = 0; i < first->size() / targetHash.size(); i++) {
				std::string segment = first->substr(i * targetHash.size(), targetHash.size());
				if (segment == targetHash) {
					pos = i;
					break;
				}
			}

			if (pos > -1)
				for (unsigned i = 0; i < hierarchy.size(); i++) {
					uint64_t rem = pos % N;
					if (rem != 0)
						pos -= rem;

					std::array<std::string, N> hashes;
					if (i < hierarchy.size() - 1) {
						for (unsigned j = 0; j < N; j++)
							hashes[j] = hierarchy[i].substr((pos + j) * targetHash.size(), targetHash.size());
					}
					else
						if (i == hierarchy.size() - 1) {
							hashes[0] = hierarchy[i];
							for (unsigned j = 1; j < N; j++)
								hashes[j] = std::string(N, '0');
						}

					proofs.push_back(hashes);
					pos = pos / N;

					/* Debug code */
					//std::string combined = ComputeSHA3256({ left, right });	
					//std::string str = hierarchy[i + 1].substr(pos * targetHash.size(), targetHash.size());

					//if (combined != str && !str.empty())
					//	std::cout << "mismatch !" << std::endl;

				}

			/* verify first before retuning */
			if (!Hasher::VerifyProofs<H, N>(proofs)) // validate before returning 
				proofs.clear();

			return proofs;
		}

		static void RootFromHashes(char* bytes, uint64_t count, char* hash)	{
			std::string msg(count * H::DIGESTSIZE, '0');
			std::memcpy(&msg[0], bytes, msg.size());

			auto Mhasher = Mhasher::Trie<H, N>(msg);
			std::string hashStr = Mhasher.GetHexRootHash();

			std::memcpy(hash, &hashStr[0], H::DIGESTSIZE);
		}

		static void MhasherFromRaw2D(char** bytes, uint64_t* lengths, uint64_t count, char* hash) {
			char* begin = new char[H::DIGESTSIZE * count];

			//for(unsigned i = 0 ; i < count; i ++)
			tbb::parallel_for(std::size_t(0), std::size_t(count), [&](std::size_t i) {
				Hasher::Hash<H>((char*)bytes[i], lengths[i], (begin + H::DIGESTSIZE * (i)));
			});

			Mhasher::Trie<H, N>::RootFromHashes(begin, count, hash);
			delete[] begin;
		}

		static void Checksum(char* bytes, unsigned length, char* hash)	{
			std::vector<unsigned> countVec;
			Hasher::Reslice(length, countVec); // replace input data for better performance in parallel

			std::vector<unsigned> indVec(countVec.size() + 1, 0);
			unsigned total = 0;
			for (unsigned i = 0; i < countVec.size(); i++) {
				total += countVec[i];
				indVec[i + 1] = total;
			}

			std::string concatenatedHashes(H::DIGESTSIZE * countVec.size(), '0');
			char* begin = (char*)concatenatedHashes.data();

			//for(unsigned i = 1 ; i < indVec.size(); i ++)
			tbb::parallel_for(std::size_t(1), indVec.size(), [&](std::size_t i)	{
				Hasher::Hash<H>(bytes + indVec[i - 1], countVec[i - 1], (begin + H::DIGESTSIZE * (i - 1)));
			}
			);
	
			Mhasher::Trie<H, N>::RootFromHashes((char*)concatenatedHashes.data(), countVec.size(), hash);
		}

		static void HashesFromRaw(char* bytes, uint64_t* counts, uint64_t length, char* concatenatedHashes)	{
			std::vector<unsigned> indVec(length + 1, 0);
			unsigned total = 0;
			for (unsigned i = 0; i < length; i++) {
				total += counts[i];
				indVec[i + 1] = total;
			}

			//for(unsigned i = 1 ; i < indVec.size(); i ++)
			tbb::parallel_for(std::size_t(1), indVec.size(), [&](std::size_t i) {
				std::string data(bytes + indVec[i - 1], counts[i - 1]);
			/*	std::string hash(H::DIGESTSIZE, '0');*/
				Hasher::Hash<H>(&data[0], data.size(), (concatenatedHashes + H::DIGESTSIZE * (i - 1)));
			}
			);
		}

		static void HashesFromRaw2D(char** bytes, uint64_t* counts, uint64_t length, char** hashes)	{
			std::vector<unsigned> indVec(length + 1, 0);
			unsigned total = 0;
			for (unsigned i = 0; i < length; i++) {
				total += counts[i];
				indVec[i + 1] = total;
			}

			//for(unsigned i = 1 ; i < indVec.size(); i ++)
			tbb::parallel_for(std::size_t(0), indVec.size() - 1, [&](std::size_t i)	{
				std::string data(bytes[i] + indVec[i], counts[i]);
				std::string hash(H::DIGESTSIZE, '0');
				Hasher::Hash<H>(&data[0], data.size(), hashes[i - 1]);
			}
			);
		}
		
	private:
		std::string rootHash = "";
		std::string message;
		std::vector<std::string> hierarchy;
		//T Hasher;
		//N branches;
	};

}



