#include <functional>
#include <iostream>

static void run()
{
	auto up = std::make_unique<int>(10);

	std::move_only_function<void()> f{ [p = move(up)] {
		std::cout << *p << "\n";
	} };

	f();

	// yeah, a silly example. What about combining this with packaged_task?
}