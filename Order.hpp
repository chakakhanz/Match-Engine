#ifndef ORDER_HPP
#define ORDER_HPP
#include <string>
#include <memory>

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
	int check_best_match(std::shared_ptr<Order> orderIn, double bestPrice, int bestOrderID, int buyOrSell);
};

#endif