#include <iostream>

// old school CRTP
template<typename Derived>
struct printer
{
	auto& print(const char* message) const
	{
		std::cout << message << "\n";
		return static_cast<const Derived&>(*this);
	}
};

struct better_printer : printer<better_printer>
{
	auto& endl() const
	{
		std::cout << std::endl;
		return *this;
	}
};

// with EOP:
namespace eop
{
	struct printer
	{
		auto& print(this auto const& self, const char* message)
		{
			std::cout << message << "\n";
			return self;
		}
	};

	struct better_printer : printer
	{
		auto& endl() const
		{
			std::cout << std::endl;
			return *this;
		}
	};
}

// just to play with EOP
struct X
{
	int x;

	void foo(this X& self)
	{
		std::cout << self.x << "\n";
	}

	void foo() const // try removing 'const' and compile...
	{
		std::cout << x << "\n";
	}
};

static void run()
{
	better_printer bp;
	bp.print("ciao").endl().print("hello");
}

