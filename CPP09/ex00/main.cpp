#include "BitcoinExchange.hpp"

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Error: wrong number of arguments. USAGE : ./btc <file>." << std::endl;
	}
	(void)av;
	BitcoinExchange	btc;
	btc.loadDatabase("data.csv");
	btc.printDatabase();
	return (0);
}
