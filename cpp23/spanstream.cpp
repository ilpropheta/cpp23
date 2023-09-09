#include <spanstream>
#include <iostream>

static void run()
{
	char data[] = "10 20"; // suppose this is coming from a C api or whatever
	std::ispanstream s{ std::span<char>{data} };
	int i, j;
	s >> i >> j;
	std::cout << i << " " << j << "\n";

	char out[10]{};
	std::ospanstream ous{ std::span{out} }; // adapt a fixed buffer to ostream
	ous << "hello how are you?" << "\n"; // won't allocate extra memory
	if (ous.fail())
	{
		std::cout << "out of space...\n";
		*std::rbegin(out) = 0;
	}
	std::cout << out << "\n";
}

