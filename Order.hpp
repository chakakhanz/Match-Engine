#ifndef ORDER_HPP
#define ORDER_HPP
#include <string>

using namespace std;

class Order{
public:
	Order();
	Order(string partyIn, string instrumentIn, double priceIn, int sizeIn);
	~Order();
	string party;
	string instrument;
	double price;
	int size;
	int orderid;
	int check_best_match(Order orderIn, double bestPrice, int bestOrderID, int buyOrSell);
};

#endif