#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP
#include "Order.hpp"
#include "OrderList.hpp"
#include <memory>
#include <vector>

class OrderBook{
public:

	OrderBook();
	~OrderBook();
	int buyOrderID;
	int sellOrderID;
	std::vector<OrderList> buyOrders;
	std::vector<OrderList> sellOrders;

	int add_order(std::shared_ptr<Order> newOrder, int buyOrSell);
	int update_order(int orderID, int orderSize, int buyOrSell); 
    void show_orders();
	int check_for_match(std::shared_ptr<Order> newOrder, int buyOrSell);	
	std::shared_ptr<Order> change_order(int orderID, int buyOrSell);
	std::shared_ptr<Order> reset_priority(std::shared_ptr<Order> result, int buyOrSell);
};

#endif