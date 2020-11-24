#include "stdafx.h"


int main(int argc, char* argv[])
{
	sort_hashes();
	unique_hashes();
	performance_test_100_x_10000000();


	//basic_2d_input_test();

	//if (argc != 2)
	//{
	//	std::cout << "Expecting the second argument to be a binary file !" << std::endl;
	//	return 1;
	//}

	//if (std::string(argv[1]) == "-v")
	//{
	//	std::cout << product << ": " << version << std::endl;
	//	return 0;
	//}


	//std::ifstream ifs(argv[1], std::ios::binary);
	//std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	//std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
	//std::string hash(CryptoPP::RIPEMD160::DIGESTSIZE, 0);
	////Mhasher::Trie<CryptoPP::RIPEMD160, 2>::Checksum(content.data(), content.size(), &hash[0]);

	//Mhasher::Trie<CryptoPP::RIPEMD160, 2>::Checksum(content.data(), content.size(), &hash[0]);

	//std::cout << "Root Hash: "<< hash <<" using RIPEMD160 processed " << content.size() << " bytes " << " in " <<
	//	std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t0).count() << " ms" << std::endl;

	return 0;
}
