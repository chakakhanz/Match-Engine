#include "OrderList.hpp"
#include "variables.hpp"
#include <memory>
#include <iostream>
#include <algorithm>


OrderList::OrderList(): partyName(""), numNodes(0){
}

OrderList::~OrderList() {}

int OrderList::add_node(Order *newOrder){
	for (auto iter : orders) { //check if this new order is a duplicate. if yes, return
		if (iter->party.compare(newOrder->party) == 0 && iter->instrument.compare(newOrder->instrument) == 0 && iter->size == newOrder->size && iter->price == newOrder->price) {
			std::cout << "***Error: duplicate entry***" << "\n";
			return -1;
		}
	}

	if(partyName.empty()){
		partyName = newOrder->party; //check if partyName has been set for this list
	}
	try {
		orders.push_back(newOrder);
		numNodes++;
		return 0;
	}
	catch (int e) {
		std::cout << "Exception occured: " << e << "\n";
		return -1;
	}
}

int OrderList::update_node(int orderID, const int orderSize ){
	int returnVal = NO_MATCH; //default return value
	for (int i = 0; i < numNodes; i++) {
		if (orderID == orders[i]->orderid) {
			std::cout << "Order from " << orders[i]->party << ", with " << orders[i]->size << " (" << orders[i]->size - orderSize << " after transaction) unit(s) of " << orders[i]->instrument << " for " << orders[i]->price << "\n";
			if (orders[i]->size == orderSize) { //if order is completely filled
				orders.erase(remove_if(orders.begin(), orders.end(), [orderID](Order* x) {return orderID == x->orderid;}));
				numNodes--;
				returnVal = 0;
			}
			else if (orders[i]->size > orderSize) { //if order in book is larger than incoming order
				orders[i]->size -= orderSize;
				returnVal = -1;
			}
			else if (orders[i]->size < orderSize) { //if new order is larger than order in book, remove the order and return the remainder
				returnVal = orderSize - orders[i]->size;
				orders.erase(remove_if(orders.begin(), orders.end(), [orderID](Order* x) {return orderID == x->orderid;}));
				numNodes--;
			}
			else {
				returnVal = FAIL;
			}
		}
	}
	return returnVal;
}

void OrderList::show_data() const{
	for(auto iter: orders){
		std::cout << "Order ID: " << iter->orderid << " | ";
		std::cout << "Party: " << iter->party << " | ";
		std::cout << "Instrument: " << iter->instrument << " | ";
		std::cout << "Price: " << iter->price << " | ";
		std::cout << "Size: " << iter->size << "\n";
    }
}

Order* OrderList::find_order(int orderID) {
	Order* returnVal = nullptr;
	for (auto iter: orders) {
		if (iter->orderid == orderID){
			returnVal = &(*iter);
		}
	}
	return returnVal;
}