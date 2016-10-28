#ifndef ORDERLIST_HPP
#define ORDERLIST_HPP
#include <memory>
#include <vector>
#include "Order.hpp"

class OrderList{
public:
	OrderList();
	~OrderList();
	std::string partyName; /*store the name of the party to which the vector belongs,
						needed to ensure orders from one party go to only one vector
						*/
	std::vector<Order> orders;
	int numNodes;

	int add_node(Order newOrder);
	int update_node(int orderID, const int orderSize);
	void show_data() const; //for debugging
	
};

#endif