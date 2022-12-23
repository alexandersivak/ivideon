#pragma once
class LED
{
public:
	enum colour {red, green, blue};
	typedef unsigned RateType;

	LED(): state(true), color(red), rate(3U)
	{
	}

	LED(const LED&) = delete;
	const LED& operator=(const LED&) = delete;

	bool get_state() const
	{
		return this->state;
	}
	void set_state(bool s)
	{
		this->state = s;
	}

	colour get_color() const
	{
		return this->color;
	}
	void set_color(colour c)
	{
		this->color = c;
	}

	unsigned get_rate() const
	{
		return this->rate;
	}
	void set_rate(unsigned r)
	{
		if (r >= 0 && r <= 5) // Hz
		{
			this->rate = r;
		}
	}
private:
	bool state;
	colour color;
	unsigned rate;
};

