#include "Command.h"
namespace utils
{
	void to_string_state(LED& led, std::string& dest)
	{
		if (led.get_state())
		{
			dest = "on";
		}
		else
		{
			dest = "off";
		}
	}
	void to_string_colour(LED& led, std::string& dest)
	{
		if (led.get_color() == LED::blue)
		{
			dest = "blue";
		}
		else if (led.get_color() == LED::red)
		{
			dest = "red";
		}
		else if (led.get_color() == LED::green)
		{
			dest = "green";
		}
	}
	void to_string_rate(LED& led, std::string& dest)
	{
		dest = boost::lexical_cast<std::string>(led.get_rate());
	}

	bool string_to_state(const std::string& src)
	{
		if ("on" == src)
		{
			return true;
		}
		else if ("off" == src)
		{
			return false;
		}
		throw std::runtime_error("failed: only on|off are allowed");
	}

	LED::colour string_to_colour(const std::string& src)
	{
		if ("red" == src)
		{
			return LED::red;
		}
		else if ("green" == src)
		{
			return LED::green;
		}
		else if ("blue" == src)
		{
			return LED::blue;
		}
		throw std::runtime_error("failed: color value is not allowed");
	}

	LED::RateType string_to_rate(const std::string& src)
	{
		return boost::lexical_cast<LED::RateType>(src);
	}
}
std::unique_ptr<Command> parse(const std::string& s1)
{
	std::vector<std::string> strs;
	boost::split(strs, s1, boost::is_any_of(" "));
	const std::string& command = strs[0];

	// a check
	if (command._Starts_with("set"))
	{
		if (strs.size() != 2)
		{
			throw std::runtime_error("failed: no arg");
		}
	}

	if (command == "set-led-state")
	{
		return std::make_unique<SetState>(Command::ParamContainer(utils::string_to_state(strs[1])));
	}
	if (command == "get-led-state")
	{
		return std::make_unique<GetState>();
	}
	if (command == "set-led-color")
	{
		return std::make_unique<SetColor>(Command::ParamContainer(utils::string_to_colour(strs[1])));
	}
	if (command == "get-led-color")
	{
		return std::make_unique<GetColor>();
	}
	if (command == "set-led-rate")
	{
		return std::make_unique<SetRate>(Command::ParamContainer(utils::string_to_rate(strs[1])));
	}
	if (command == "get-led-rate")
	{
		return std::make_unique<GetRate>();
	}
}