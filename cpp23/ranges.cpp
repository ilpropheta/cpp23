#include <ranges>
#include <vector>
#include <iostream>

auto compress_no_ranges(const std::string& input)
{
	std::vector<std::pair<char, size_t>> output;
	auto head = begin(input);
	auto tail = head;
	while (tail != end(input))
	{
		if (*head != *tail)
		{
			output.emplace_back(*head, distance(head, tail));
			head = tail;
		}
		else
		{
			++tail;
		}
	}
	output.emplace_back(*head, distance(head, tail));
	return output;
}

auto compress_with_ranges(const std::string& input)
{
	return input
		| std::views::chunk_by(std::equal_to{})
		| std::views::transform([](auto same_chars) {
			return std::make_pair(same_chars.front(), std::size(same_chars));
		})
		| std::ranges::to<std::vector>();
}

void product_of_first_n_odd_numbers(int n)
{
	const auto odds = std::views::iota(1) | std::views::stride(2) | std::views::take(n);
	auto prods = std::views::pairwise_transform(odds, std::multiplies{});
	for (auto i : prods)
		std::cout << i << " ";
}

static void run()
{
	// problem: transform strings like 'aaaabbbcc' into a "compressed" form (a,4) (b,3) (c,2)
	//			it's required to return a vector of pairs
	std::string input = "aaaabbbcc";
	std::cout << input << " --> ";
	for (auto [i,j] : compress_with_ranges(input)) // when committed, we didn't have "formatting ranges" yet :(
	{
		std::cout << "(" << i << "," << j << ") ";
	}

	std::cout << "\n";
	input = "a";
	std::cout << input << " --> ";
	for (auto [i, j] : compress_with_ranges(input))
	{
		std::cout << "(" << i << "," << j << ") ";
	}

	std::cout << "\n";
	input = "aaaaaaa";
	std::cout << input << " --> ";
	for (auto [i, j] : compress_with_ranges(input))
	{
		std::cout << "(" << i << "," << j << ") ";
	}

	// print the product of the first n odd numbers
	std::cout << "\n\n";
	product_of_first_n_odd_numbers(10);

	// pair values with indexes
	std::cout << "\n\n";
	std::vector values = { 10,20,30,40 };
	for (auto [i, j] : values | std::views::enumerate)
	{
		std::cout << i << "," << j << "\n";
	}

	std::cout << "\n";
}

static const auto _ = (run(), true);