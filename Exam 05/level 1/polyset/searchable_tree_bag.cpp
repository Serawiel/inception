#include "searchable_tree_bag.hpp"

searchable_tree_bag::searchable_tree_bag() : tree_bag()
{
}

searchable_tree_bag::searchable_tree_bag(const searchable_tree_bag &src) : tree_bag(src)
{
}

searchable_tree_bag &searchable_tree_bag::operator=(const searchable_tree_bag &src)
{
	this->tree_bag::operator=(src);
	return (*this);
}

searchable_tree_bag::~searchable_tree_bag()
{
}

bool searchable_tree_bag::has(int nb) const
{
	node *current = tree;
	while (current)
	{
		if (nb < current->value)
			current = current->l;
		else if (nb > current->value)
			current = current->r;
		else
			return (true);
	}
	return false;
}