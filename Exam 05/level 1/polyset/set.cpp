#include "set.hpp"
#include "bag.hpp"

set::set()
{
	bag = NULL;
}

set::set(searchable_bag & src)
{
	bag = &src;
}

set::set(const set & src)
{
	bag = src.bag;
}

set & set::operator=(const set & src)
{
	if (src.bag != bag)
		bag = src.bag;
	return (*this);
}

set::~set()
{
}

void set::insert(int nb)
{
	if (!bag->has(nb))
		bag->insert(nb);
}

void set::insert(int *array, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!bag->has(array[i]))
			(*bag).insert(array[i]);
	}
}

bool set::has(int nb)
{
	return bag->has(nb);
}

void set::print()
{
	bag->print();
}

void set::clear()
{
	bag->clear();
}

searchable_bag & set::get_bag()
{
	return(*bag);
}