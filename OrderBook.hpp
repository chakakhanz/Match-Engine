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
	void add_order(Order newOrder, int buyOrSell); //wrapper function for adding an order
	void remove_order(int orderID, int buyOrSell); //wrapper function for removing an order
   	int check_for_match(Order newOrder, int buyOrSell);
    void show_orders(int buyOrSell); //for debugging
    shared_ptr<int> buyOrderID; //using shared_ptr to avoid potential errors from destruction is using move() with a unique_ptr
    shared_ptr<int> sellOrderID;

private:
	int check_for_match(Order newOrder, shared_ptr<vector<OrderList>> listIn);
	void remove_order(int orderID, shared_ptr<vector<OrderList>> listIn);
	void add_order(Order newOrder, shared_ptr<vector<OrderList>> listIn, shared_ptr<int> orderID);
	shared_ptr<vector<OrderList>> buyOrders; //using shared_ptr to avoid potential errors from destruction is using move() with a unique_ptr
	shared_ptr<vector<OrderList>> sellOrders;
};

#endif