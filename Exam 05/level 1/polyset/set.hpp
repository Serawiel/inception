#pragma once

#include "searchable_bag.hpp"
#include "bag.hpp"
#include <cstdlib>

class set
{
  private:
	searchable_bag *bag;
  public:
	set();
	set(searchable_bag & src);
	set(const set & src);
	set & operator=(const set & src);
	~set();

	void insert(int nb);
	void insert(int *array, int size);
	bool has(int nb);
	void print();
	void clear();
	searchable_bag & get_bag();
};
