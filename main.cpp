#include "stdafx.h"


int main(int argc, char* argv[]) {
	if (!test_sort_strings()) {
		std::cout << "Error: test_sort_strings() failed !!" << std::endl;
	}
	
	if (!test_unique_strings()) {
		std::cout << "Error: test_unique_strings() failed !!" << std::endl;
	}

	if (!test_remove_strings()) {
		std::cout << "Error: test_remove_strings() failed !!" << std::endl;
	}

	if (!test_remove_strings_2()) {
		std::cout << "Error: test_remove_strings_2() failed !!" << std::endl;
	}

	std::cout << "All successful !!" << std::endl << std::endl;

	benchmark_sort_strings();
	benchamark_unique_strings();
	benchmark_remove_strings_1();

	sort_hashes();
	unique_hashes();
	return 0;
}
