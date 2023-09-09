#include <iostream>

size_t external_api(char* buff, size_t max_size)
{
	const auto s = std::min(size_t{ 20 }, max_size); // 100uz from C++23 (not supported yet at the time of commit)
	std::fill_n(buff, s, 'A');
	return s;
}

static void run()
{
	std::string str; // no initialization
	str.resize_and_overwrite(1024, [](char* raw, size_t n) {
		return external_api(raw, n);
	});
	std::cout << str;
}

