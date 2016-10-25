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
	void add_node(Order newOrder);
	int remove_node(int orderID);
	vector<int> check_list_match(Order newOrder) const;
	void show_data() const; //for debugging

private:
	vector<Order> orders;
};

#endif