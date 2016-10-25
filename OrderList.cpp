#include "OrderList.hpp"
#include "Node.hpp"
#include <memory>
#include <iostream>
#include <typeinfo>

OrderList::OrderList(): partyName(""), numNodes(0){}

OrderList::~OrderList(){
}

int OrderList::add_node(Order newOrder){
	for (auto iter : orders) { //check if this new order is a duplicate. if yes, return
		if (iter.instrument.compare(newOrder.instrument) == 0) {
			cout << "***Error: duplicate entry***" << endl;
			return -1;
		}
	}

	if(partyName.empty()){
		partyName = newOrder.party; //check if partyName has been set for this list
	}
	try {
		orders.push_back(newOrder);
		numNodes++;
		return 0;
	}
	catch (int e) {
		cout << "Exception occured: " << e << endl;
		return -1;
	}
}

int OrderList::remove_node(int orderID){
	for (int i = 0; i < numNodes; i++) {
		if (orderID == orders[i].orderid) {
			cout << "Order from " << orders[i].party << ", selling " << orders[i].size << " unit(s) of " << orders[i].instrument << " for " << orders[i].price << endl;
			orders.erase(orders.begin()+i);
			numNodes--;
			return 0;
		}
	}
	return -1;
}

/*int OrderList::check_list_match(Order newOrder, int bestPrice){
	int returnVal = -1; //default fail values
	for(auto iter: orders){ //iterate through all possible matches
		if(iter.compare_orders(newOrder, bestPrice) == 0){ //if orders match
			
			return returnVal;
			// break here because only one order in a given list can possibly be a match since each list belongs to one party
			// more than one match would imply a duplicate
		}
	}
	return returnVal; //if execution falls through to here, no match has been found, so return empty vector
}*/

void OrderList::show_data() const{
	for(auto iter: orders){
		cout << iter.party << " ";
		cout << iter.instrument << endl;
    }
}