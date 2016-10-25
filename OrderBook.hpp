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
	vector<OrderList> buyOrders; //using shared_ptr to avoid potential errors from destruction is using move() with a unique_ptr
	vector<OrderList> sellOrders;

	int add_order(Order newOrder, int buyOrSell);
	int remove_order(int orderID, int buyOrSell); 
    void show_orders(int buyOrSell); //for debugging
	int check_for_match(Order newOrder, int buyOrSell);	
};

#endif