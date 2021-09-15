#include "stdafx.h"


int main(int argc, char* argv[]) {	
	//benchmark_string_engine_unique_sort_1m();

	if (!test_string_engine_unique_sort()) {
		std::cout << "Error: test_string_engine_unique_sort() failed !!" << std::endl;
	}

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

	benchmark_sort_strings_1m();
	benchamark_unique_strings_1m();
	benchmark_remove_strings_1m();

	sort_hashes();
	unique_hashes();
	return 0;
}
