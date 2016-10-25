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
	int compare_orders(Order orderIn);
};

#endif