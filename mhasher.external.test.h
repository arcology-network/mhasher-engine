#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	void merkel_trie_insertion_test();
	void c_interface_test();
	void variable_raw_input_lengths_test();
	void performance_test_100_x_10000000();
	void basic_2d_input_test();
	void sort_hashes();
	void unique_hashes();

	bool test_sort_strings();
	bool test_unique_strings();
	bool test_remove_strings();
	bool test_remove_strings_2();

	void benchmark_sort_strings();
	void benchamark_unique_strings();
	bool benchmark_remove_strings_1();
#ifdef __cplusplus
}
#endif