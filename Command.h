#pragma once
#include "LED.h"
#include <array>
#include <variant>
#include <memory>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
namespace utils
{
	void to_string_state(LED& led, std::string& dest);
	void to_string_colour(LED& led, std::string& dest);
	void to_string_rate(LED& led, std::string& dest);

	bool string_to_state(const std::string& src);
	LED::colour string_to_colour(const std::string& src);
	LED::RateType string_to_rate(const std::string& src);
}

class Command // to execute it by manually calling LED functions (e.g. turning it off)
{
public:
	typedef std::variant<bool, LED::colour, LED::RateType, nullptr_t> ParamContainer;

	Command(const ParamContainer& c = nullptr) : // no arg by default
		has_res(false),
		param(c)
	{
	}
	
	const ParamContainer& get_param() const
	{
		return this->param;
	}

	bool has_result() const
	{
		return this->has_res;
	}

	virtual bool apply(LED& l) = 0;
	virtual void get_result(LED& l, std::string& d) = 0;

protected:
	void set_flag(bool f)
	{
		this->has_res = f;
	}

private:
	bool has_res;
	ParamContainer param;
};

class GetState : public Command
{
public:
	GetState()
	{
		set_flag(true);
	}

	bool apply(LED&) override
	{
		// pass
		
		return true;
	}

	void get_result(LED& l, std::string& d) override
	{
		if (has_result())
		{
			utils::to_string_state(l, d);
		}
	}
};
class SetState : public Command
{
public:

	SetState(const ParamContainer& params) : Command(params)
	{
	}

	bool apply(LED& led) override
	{
		try
		{
			bool v = std::get<bool>(this->get_param());			
			led.set_state(v);
		}
		catch (std::bad_variant_access&) // no arg
		{
			return false;
		}
		return true;
	}

	void get_result(LED& l, std::string& d) override
	{
		// pass 
		// this command has no output
	}
};
class GetColor : public Command
{
public:
	GetColor()
	{
		set_flag(true);
	}

	bool apply(LED&) override
	{
		// pass
		
		return true;
	}

	void get_result(LED& l, std::string& d) override
	{
		if (has_result())
		{
			utils::to_string_colour(l, d);
		}
	}
};
class SetColor : public Command
{
public:

	SetColor(const ParamContainer& params) : Command(params)
	{
	}

	bool apply(LED& led) override
	{
		try
		{
			auto v = std::get<LED::colour>(this->get_param());
			led.set_color(v);
		}
		catch (std::bad_variant_access&) // no arg
		{
			return false;
		}
		return true;
	}

	void get_result(LED& l, std::string& d) override
	{
		// pass 
		// this command has no output
	}
};
class GetRate : public Command
{
public:
	GetRate()
	{
		set_flag(true);
	}

	bool apply(LED&) override
	{
		// pass
		
		return true;
	}

	void get_result(LED& l, std::string& d) override
	{
		if (has_result())
		{
			utils::to_string_rate(l, d);
		}
	}
};
class SetRate : public Command
{
public:

	SetRate(const ParamContainer& params) : Command(params)
	{
	}

	bool apply(LED& led) override
	{
		try
		{
			auto v = std::get<LED::RateType>(this->get_param());
			if (v < 1 || v > 5)
			{
				throw std::runtime_error("failed: allowed range is [1,5]");
			}
			led.set_rate(v);
		}
		catch (std::bad_variant_access&) // no arg
		{
			return false;
		}
		return true;
	}

	void get_result(LED& l, std::string& d) override
	{
		// pass 
		// this command has no output
	}
};
std::unique_ptr<Command> parse(const std::string& s1);


