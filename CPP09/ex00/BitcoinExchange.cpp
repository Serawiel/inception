#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::~BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &src)
{
	*this = src;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &rhs)
{
	if (this != &rhs)
	{
		this->_database = rhs._database;
	}
	return (*this);
}

void BitcoinExchange::loadDatabase(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error : data.csv not found." << std::endl;
		exit(1);
	}
	std::string line;
	std::getline(file, line);
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string date;
		std::string valueStr;
		std::getline(ss, date, ',');
		std::getline(ss, valueStr);
		double	price = std::atof(valueStr.c_str());
		_database[date] = price;
	}
}

void BitcoinExchange::printDatabase() const
{
	std::map<std::string, double>::const_iterator it;
	for (it = _database.begin(); it != _database.end(); it++)
	{
		std::cout << it->first << " => " << it->second << std::endl;
	}
}

std::string BitcoinExchange::trim(std::string str)
{
	size_t	start;
	size_t	end;

	start = str.find_first_not_of(" \t");
	if (start == std::string::npos)
		return ("");
	end = str.find_last_not_of(" \t");
	return (str.substr(start, end - start + 1));
}

void BitcoinExchange::processInput(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error : could not open file." << std::endl;
		return ;
	}
	std::string line;
	while (getline(file, line))
	{
		std::stringstream ss(line);
		std::string date;
		std::string valueStr;
		std::getline(ss, date, '|');
		std::getline(ss, valueStr);
		if (valueStr.empty())
		{
			std::cerr << "Error: bad input =>" << line << std::endl;
			continue ;
		}
		date = trim(date);
		valueStr = trim(valueStr);
		if (date.length() != 10 || date[4] != '-' || date[7] != '-')
		{
			std::cerr << "Error: bad input => " << date << std::endl;
			continue ;
		}
		double value = std::atof(valueStr.c_str());
		// 6. Chercher dans _database et afficher
		// (on verra après)
	}
}