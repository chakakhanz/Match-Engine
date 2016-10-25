#ifndef ORDERLIST_HPP
#define ORDERLIST_HPP
#include <memory>
#include <vector>
#include "Order.hpp"

class OrderList{
public:
	OrderList();
	~OrderList();
	string partyName; /*store the name of the party to which the vector belongs,
						needed to ensure orders from one party go to only one vector
						*/
	vector<Order> orders;
	int numNodes;

	int add_node(Order newOrder);
	int remove_node(int orderID);
	int check_list_match(Order newOrder, int bestPrice);
	void show_data() const; //for debugging
	
};

#endif