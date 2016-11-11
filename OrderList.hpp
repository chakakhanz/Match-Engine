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
	std::vector<std::shared_ptr<Order>> orders;

	int add_node(std::shared_ptr<Order> newOrder);
	int update_node(int orderID, const int orderSize);
	void show_data();
	std::shared_ptr<Order> find_order(int orderID);
	int remove_order(std::shared_ptr<Order> orderIn);
};

#endif