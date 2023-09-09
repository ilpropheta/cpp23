#include <iostream>
#include <ranges>
#include <stacktrace>

void foo()
{
	throw std::runtime_error("baaaaaad");
}

void call_somewhere()
{
	foo();
}

static void run()
{
	// set termination handler
	std::set_terminate([] {
		if (std::current_exception())
		{
			std::cout << "Something bad happened...\n\n";
			// we can print backwards this way:
			for (const auto& se : std::views::reverse(std::stacktrace::current()))
			{
				std::cout << se << "\n";
			}
		}
		std::abort();
	});

	call_somewhere();
}