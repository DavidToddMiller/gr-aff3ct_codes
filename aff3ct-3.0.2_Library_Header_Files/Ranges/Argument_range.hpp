#ifndef ARGUMENT_RANGE_HPP_
#define ARGUMENT_RANGE_HPP_

#include <string>

namespace cli
{

class Argument_range
{
protected:
	std::string title;

public:
	explicit Argument_range(const std::string& title)
	: title(title)
	{ }

	virtual ~Argument_range() {};

	virtual Argument_range* clone() const = 0;

	virtual std::string get_title() const
	{
		return title;
	}
};

}

#include "Min.hpp"
#include "Max.hpp"
#include "Set/Set.hpp"
#include "Non_zero.hpp"
#include "Negative.hpp"
#include "Positive.hpp"
#include "Length.hpp"
#include "Function.hpp"

#endif /* ARGUMENT_RANGE_HPP_ */