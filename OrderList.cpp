#include "OrderList.hpp"
#include "variables.hpp"
#include <memory>
#include <iostream>
#include <algorithm>


OrderList::OrderList(): partyName(""), numNodes(0){
}

OrderList::~OrderList() {}

int OrderList::add_node(std::shared_ptr<Order> newOrder){
	for (std::vector<std::shared_ptr<Order>>::iterator iter = orders.begin(); iter != orders.end(); iter++) { //check if this new order is a duplicate. if yes, return
		if ((*iter)->party.compare(newOrder->party) == 0 && (*iter)->instrument.compare(newOrder->instrument) == 0 && (*iter)->size == newOrder->size && (*iter)->price == newOrder->price) {
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
	for (std::vector<std::shared_ptr<Order>>::iterator iter = orders.begin(); iter != orders.end(); iter++) {
		if (orderID == (*iter)->orderid) {
			int difference = (*iter)->size - orderSize;
			if (difference < 0){
				difference = 0; //show that the order is empty after subtracting the size of the matching order instead of showing a negative number
			}

			std::cout << "Order from " << (*iter)->party << ", with " << (*iter)->size << " (" << difference << " after transaction) unit(s) of " << (*iter)->instrument << " for " << (*iter)->price << "\n";
			if ((*iter)->size == orderSize) { //if order is completely filled
				orders.erase(remove_if(orders.begin(), orders.end(), [orderID](std::shared_ptr<Order> x) mutable {return orderID == x->orderid;}));
				returnVal = 0;
			}
			else if ((*iter)->size > orderSize) { //if order in book is larger than incoming order
				(*iter)->size -= orderSize;
				returnVal = -1;
			}
			else if ((*iter)->size < orderSize) { //if new order is larger than order in book, remove the order and return the remainder
				returnVal = orderSize - (*iter)->size;
				orders.erase(remove_if(orders.begin(), orders.end(), [orderID](std::shared_ptr<Order> x) mutable {return orderID == x->orderid;}));
			}
			else {
				returnVal = FAIL;
			}
			break; //break here; letting iteration continue after erase-remove idiom can lead to attempts to manipulate an invalid iterator
		}
	}
	return returnVal;
}

void OrderList::show_data(){
	for(std::vector<std::shared_ptr<Order>>::iterator iter = orders.begin(); iter != orders.end(); iter++){
		std::cout << "Order ID: " << (*iter)->orderid << " | ";
		std::cout << "Party: " << (*iter)->party << " | ";
		std::cout << "Instrument: " << (*iter)->instrument << " | ";
		std::cout << "Price: " << (*iter)->price << " | ";
		std::cout << "Size: " << (*iter)->size << "\n";
    }
}

std::shared_ptr<Order> OrderList::find_order(int orderID) {
	std::shared_ptr<Order> returnVal = nullptr;
	for (std::vector<std::shared_ptr<Order>>::iterator iter = orders.begin(); iter != orders.end(); iter++) {
		if ((*iter)->orderid == orderID){
			return *iter;
		}
	}
	return returnVal;
}

int OrderList::remove_order(std::shared_ptr<Order> orderIn) {
	int ID = orderIn->orderid;
	for (std::vector<std::shared_ptr<Order>>::iterator iter = orders.begin(); iter != orders.end(); iter++) {
		if (ID == (*iter)->orderid) {
			orders.erase(remove_if(orders.begin(), orders.end(), [ID](std::shared_ptr<Order> x) mutable {return ID == x->orderid;}));
			return 0;
		}
	}
	return -1;
	//unique pointer should automatically get deleted once this function is out of scope
}