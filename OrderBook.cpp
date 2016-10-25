#include "OrderBook.hpp"
#include "OrderList.hpp"
#include "variables.hpp"
#include <string>
#include <iostream>

OrderBook::OrderBook():buyOrderID(0), sellOrderID(0){
	
}

OrderBook::~OrderBook(){
}

int OrderBook::add_order(Order newOrder, int buyOrSell){
	//shared_ptr<vector<OrderList>> listIn = make_shared<vector<OrderList>>();
	vector<OrderList> *listIn;
	if(buyOrSell == BUY){ 
		listIn = &buyOrders;
	}
	else if(buyOrSell == SELL){
		listIn = &sellOrders;
	}
	for (vector<OrderList>::iterator iter = listIn->begin(); iter != listIn->end(); iter++) {
		if (newOrder.party.compare(iter->partyName) == 0) { //if a vector for this party already exists, add to that party
			if (buyOrSell == BUY) {
				newOrder.orderid = ++buyOrderID;
			}
			else if (buyOrSell == SELL) {
				newOrder.orderid = ++sellOrderID;
			}
			iter->add_node(newOrder);
			return 0;
		}
	}
	//if execution falls through here, then a new list should be created and added to the orderlist vector
	try {
		OrderList temp = OrderList();
		if (buyOrSell == BUY) {
			newOrder.orderid = ++buyOrderID;
		}
		else if (buyOrSell == SELL) {
			newOrder.orderid = ++sellOrderID;
		}
		temp.add_node(newOrder);
		listIn->push_back(temp);
		return 0;
	}
	catch (int e) {
		cout << "Exception occured: " << e << endl;
		return -1;
	}
}


int OrderBook::check_for_match(Order newOrder, int buyOrSell){
	/***buyorder is checked each time for a matching sell**/

	OrderList currentList;
	int currentMatch = -1;
	double bestPrice = -1.0;
	for (vector<OrderList>::iterator iter = buyOrders.begin(); iter != buyOrders.end(); iter++) {
		if (iter->partyName.compare(newOrder.party) != 0) { //only check if parties aren't the same; not buying and selling to ourselves
			currentList = *iter;
			for (vector<Order>::iterator i = currentList.orders.begin(); i != currentList.orders.end(); i++) {
				if (currentMatch == -1 && bestPrice == -1.0) {
					currentMatch = i->orderid;
					bestPrice = i->price;
				}
				
				else if (i->check_best_match(newOrder, bestPrice, currentMatch, buyOrSell) == 0) {
					currentMatch = i->orderid;
					bestPrice = i->price;
				}
			}
		}
	}
	cout << currentMatch << " " << bestPrice << endl;
	return currentMatch;

}

int OrderBook::remove_order(int orderID, int buyOrSell) {
	//shared_ptr<vector<OrderList>> listIn = make_shared<vector<OrderList>>();
	vector<OrderList> *listIn;
	if (buyOrSell == BUY) {
		listIn = &buyOrders;
	}
	else if (buyOrSell == SELL) {
		listIn = &sellOrders;
	}

	for (vector<OrderList>::iterator iter = listIn->begin(); iter != listIn->end(); iter++) {
		if (iter->remove_node(orderID) == 0) {
			if (buyOrSell == BUY) {
				buyOrderID--;
			}
			else if (buyOrSell == SELL) {
				sellOrderID--;
			}
			return 0;
		}
	}
	return -1;
}

void OrderBook::show_orders(int buyOrSell){
	if(buyOrSell == BUY){
		for(vector<OrderList>::iterator iter = buyOrders.begin(); iter != buyOrders.end(); iter++){
			iter->show_data();
		}
	}
	else if(buyOrSell == SELL){
		for(vector<OrderList>::iterator iter = sellOrders.begin(); iter != sellOrders.end(); iter++){
			iter->show_data();
		}
	}
}