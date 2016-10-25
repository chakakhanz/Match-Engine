#include "OrderList.hpp"
#include <memory>
#include <iostream>
#include <typeinfo>

OrderList::OrderList(): partyName(""){}

OrderList::~OrderList(){
}

void OrderList::add_node(Order newOrder){
	if(partyName.empty()){
		partyName = newOrder.party; //check if partyName has been set for this list
	}
	orders.push_back(newOrder);
}

int OrderList::remove_node(int orderID){
	vector<int> orderid;
	for(auto iter: orders){
	}
	return -1;
}

vector<int> OrderList::check_list_match(Order newOrder) const{
	vector<int> returnVal; //starts empty, fills if match found
	for(auto iter: orders){ //iterate through all possible matches
		if(iter.compare_orders(newOrder) == 0){ //if orders match
			returnVal.push_back(iter.price);
			returnVal.push_back(iter.orderid);
			break;
			// break here because only one order in a given list can possibly be a match since each list belongs to one party
			// more than one match would imply a duplicate
		}
	}
	return returnVal; //if execution falls through to here, no match has been found, so return empty vector
}

void OrderList::show_data() const{
	for(auto iter: orders){
		cout << iter.party << " ";
		cout << iter.instrument << endl;
    }
}