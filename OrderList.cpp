#include "OrderList.hpp"
#include "Node.hpp"
#include <memory>
#include <iostream>
#include <algorithm>
#include <typeinfo>

OrderList::OrderList(): partyName(""), numNodes(0){
}

OrderList::~OrderList() {}

int OrderList::add_node(Order newOrder){
	for (auto iter : orders) { //check if this new order is a duplicate. if yes, return
		if (iter.instrument.compare(newOrder.instrument) == 0 && iter.size == newOrder.size) {
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

int OrderList::update_node(int orderID, int orderSize ){
	int returnVal = -1;
	vector<Order> *ord = &orders;
	for (int i = 0; i < numNodes; i++) {
		if (orderID == (*ord)[i].orderid) {
			cout << "Order from " << (*ord)[i].party << ", with " << (*ord)[i].size << " (" << (*ord)[i].size - orderSize << " after transaction) unit(s) of " << (*ord)[i].instrument << " for " << (*ord)[i].price << endl;
			if ((*ord)[i].size == orderSize){ //if order is completely filled
				//orders.erase(orders.begin() + i);
				(*ord).erase(remove_if((*ord).begin(), (*ord).end(), [orderSize](Order x) mutable {return orderSize == x.size;}));
				numNodes--;
				returnVal = 0;
			}
			else if ((*ord)[i].size > orderSize){
				(*ord)[i].size -= orderSize;
				returnVal = -1; //lets caller know an order was not removed
			}
			else {
				returnVal = -1; //lets caller know an order was not removed
			}		
		}
	}
	return returnVal;
}

void OrderList::show_data() const{
	for(auto iter: orders){
		cout << iter.party << " ";
		cout << iter.instrument << endl;
    }
}