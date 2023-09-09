#include <expected>
#include <chrono>
#include <iostream>
#include <set>

using namespace std::chrono;

// simple domain class representing coupon information
struct coupon_info
{
	std::string id;
	int amount;
	system_clock::time_point expiration = sys_days{ December / 31 / 2025 };
	bool active = true;

	auto operator<=>(const coupon_info& info) const = default;

	auto operator<=>(std::string_view other_id) const
	{
		return id <=> other_id;
	}
};

enum class coupon_error
{
	invalid, not_active, expired
};

// pretend to query the db
std::expected<coupon_info, coupon_error> find_coupon(std::string_view sv)
{
	static std::set<coupon_info, std::less<>> db = {
		{.id = "FORZA-ROMA", .amount = 100},
		{.id = "MAKEIT20", .amount = 20, .expiration = sys_days{September / 28 / 2023}},
		{.id = "CPP20", .amount = 45, .active = false},
		{.id = "CPP23", .amount = 50, .expiration = sys_days{December / 31 / 2026}}
	};

	if (const auto it = db.find(sv); it != end(db))
	{
		return *it;
	}
	return std::unexpected(coupon_error::invalid);
}

std::expected<coupon_info, coupon_error> check_active(const coupon_info& coupon)
{
	if (coupon.active)
	{
		return coupon;
	}
	return std::unexpected(coupon_error::not_active);
}

std::expected<coupon_info, coupon_error> check_expired(const coupon_info& coupon, system_clock::time_point ts = system_clock::now())
{
	if (coupon.expiration > ts)
	{
		return coupon;
	}
	return std::unexpected(coupon_error::expired);
}

// client's entry point
std::expected<int, coupon_error> use_coupon(std::string_view sv, system_clock::time_point ts = system_clock::now())
{
	return find_coupon(sv)
		.and_then(check_active)
		.and_then([&](const auto& ci) {
			return check_expired(ci, ts);
		}).transform([](const auto& ci) {
			return ci.amount;
		});
}

constexpr std::string_view to_string(coupon_error err)
{
	using enum coupon_error;
	switch(err)
	{
		case expired:
			return "expired";
		case invalid:
			return "invalid";
		case not_active:
			return "not_active";
	}
	throw std::runtime_error("can't convert error to string");
}

static void run()
{
	std::cout << use_coupon("FORZA-ROMA").value() << "\n";
	std::cout << use_coupon("MAKEIT20").value() << "\n";
	std::cout << to_string(use_coupon("CPP23", sys_days{ January / 1 / 2027 }).error()) << "\n";
	std::cout << to_string(use_coupon("CPP20").error());
}

