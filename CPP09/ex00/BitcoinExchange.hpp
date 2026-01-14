#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <map>
# include <string>
# include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

class BitcoinExchange
{
  private:
	std::map<std::string, double> _database;

  public:
	BitcoinExchange();
	~BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &src);
	BitcoinExchange &operator=(const BitcoinExchange &rhs);

	void loadDatabase(const std::string &filename);
	void processInput(const std::string &filename);
	void printDatabase() const;
	std::string trim(std::string str);
};

#endif