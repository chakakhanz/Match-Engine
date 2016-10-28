#ifndef ORDER_HPP
#define ORDER_HPP
#include <string>

class Order{
public:
	Order();
	Order(std::string partyIn, std::string instrumentIn, double priceIn, int sizeIn);
	~Order();
	std::string party;
	std::string instrument;
	double price;
	int size;
	int orderid;
	int check_best_match(Order orderIn, double bestPrice, int bestOrderID, int buyOrSell);
};

#endif