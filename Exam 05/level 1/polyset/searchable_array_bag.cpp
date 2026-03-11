#include "searchable_array_bag.hpp"

searchable_array_bag::searchable_array_bag() : array_bag()
{
}

searchable_array_bag::searchable_array_bag(const searchable_array_bag &src) : array_bag(src)
{
}

searchable_array_bag &searchable_array_bag::operator=(const searchable_array_bag &src)
{
	this->array_bag::operator=(src);
	return (*this);
}

searchable_array_bag::~searchable_array_bag()
{
}

bool searchable_array_bag::has(int nb) const
{
	for (int i = 0; i < size; i++)
	{
		if (data[i] == nb)
			return (true);
	}
	return (false);
}