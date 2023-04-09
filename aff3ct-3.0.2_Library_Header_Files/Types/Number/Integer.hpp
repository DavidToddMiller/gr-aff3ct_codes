#ifndef ARGUMENT_TYPE_INTEGER_HPP_
#define ARGUMENT_TYPE_INTEGER_HPP_

#include <stdexcept>
#include <string>
#include "Types/Argument_type_limited.hpp"

namespace cli
{

template <typename T = int, typename... Ranges>
class Integer_type : public Argument_type_limited<T,Ranges...>
{
public:
	template <typename r, typename... R>
	Integer_type(const r* range, const R*... ranges)
	: Argument_type_limited<T,Ranges...>("integer", range, ranges...)
	{ }

	Integer_type()
	: Argument_type_limited<T,Ranges...>("integer")
	{ }

public:
	virtual ~Integer_type() {};

	virtual Integer_type<T,Ranges...>* clone() const
	{
		auto* clone = new Integer_type<T,Ranges...>();

		return dynamic_cast<Integer_type<T,Ranges...>*>(this->clone_ranges(clone));
	}

	template <typename... NewRanges>
	Integer_type<T, Ranges..., NewRanges...>*
	clone(NewRanges*... new_ranges)
	{
		auto* clone = new Integer_type<T, Ranges..., NewRanges...>();

		this->clone_ranges(clone);

		clone->add_ranges(new_ranges...);

		return clone;
	}

	static T convert_to_int(const std::string& val)
	{
		T conv;
		std::istringstream iss(val);
		iss.exceptions(std::ios::failbit | std::ios::badbit);
		iss >> conv;

		return conv;
	}

	virtual T convert(const std::string& val) const
	{
		return convert_to_int(val);
	}

	virtual void check(const std::string& val) const
	{
		T int_num;

		try
		{
			int_num = this->convert(val);
		}
		catch(std::exception&)
		{
			throw std::runtime_error("shall be an integer");
		}

		this->check_ranges(int_num);
	}
};

template <typename T = int, typename... Ranges>
Integer_type<T,Ranges...>* Integer(Ranges*... ranges)
{
	return new Integer_type<T,Ranges...>(ranges...);
}

}

#endif /* ARGUMENT_TYPE_INTEGER_HPP_ */