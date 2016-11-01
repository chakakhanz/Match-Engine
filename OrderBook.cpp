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
	std::vector<OrderList> *listIn = nullptr;
	if(buyOrSell == BUY){ 
		listIn = &buyOrders;
	}
	else if(buyOrSell == SELL){
		listIn = &sellOrders;
	}
	else {
		return FAIL;
	}

	for (std::vector<OrderList>::iterator iter = listIn->begin(); iter != listIn->end(); iter++) {
		if (newOrder.party.compare(iter->partyName) == 0) { //if a vector for this party already exists, add to that party
			if (buyOrSell == BUY) {
				newOrder.orderid = ++buyOrderID;
			}
			else if (buyOrSell == SELL) {
				newOrder.orderid = ++sellOrderID;
			}
			iter->add_node(newOrder);
			//std::cout << "OrderID: " << iter->orders[iter->orders.size()-1].orderid << std::endl;
			return 0; //tell calling function that order was added to an existing list
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
		return -1; //lets caller know order was added to a new list
	}
	catch (int e) {
		std::cout << "Exception occured: " << e << "\n";
		return FAIL;
	}
}


int OrderBook::check_for_match(Order newOrder, int buyOrSell){
	/***buyorder is checked each time for a matching sell**/
	/**vice versa for buy orders**/
	std::vector<OrderList> *listIn = nullptr;
	if (buyOrSell == BUY) {
		listIn = &sellOrders;
	}
	else if (buyOrSell == SELL) {
		listIn = &buyOrders;
	}
	else {
		return FAIL;
	}

	OrderList currentList;
	int currentMatch = -1;
	double bestPrice = -1.0;
	for (std::vector<OrderList>::iterator iter = listIn->begin(); iter != listIn->end(); iter++) {
		if (iter->partyName.compare(newOrder.party) != 0) { //only check when parties aren't the same; not buying and selling to ourselves
			currentList = *iter;
			for (std::vector<Order>::iterator i = currentList.orders.begin(); i != currentList.orders.end(); i++) {			
				if (i->check_best_match(newOrder, bestPrice, currentMatch, buyOrSell) == 0) {		
					currentMatch = i->orderid;
					bestPrice = i->price;
				}
			}
		}
	}
	return currentMatch;

}

int OrderBook::update_order(int orderID, int orderSize, int buyOrSell) {
	//shared_ptr<vector<OrderList>> listIn = make_shared<vector<OrderList>>();
	int returnVal = FAIL; //set default value to fail
	std::vector<OrderList> *listIn = nullptr;
	if (buyOrSell == BUY) {
		listIn = &buyOrders;
	}
	else if (buyOrSell == SELL) {
		listIn = &sellOrders;
	}
	else {
		return FAIL;
	}

	int updateNodeResult;
	for (std::vector<OrderList>::iterator iter = listIn->begin(); iter != listIn->end(); iter++) {
		(updateNodeResult = iter->update_node(orderID, orderSize));
		if (updateNodeResult != FAIL && updateNodeResult != NO_MATCH) {
			/*if (buyOrSell == BUY) {
				buyOrderID--;
			}
			else if (buyOrSell == SELL) {
				sellOrderID--;
			}*/
			returnVal = updateNodeResult; //lets caller know result of update
		}
	}
	//check if any lists are now empty; if true, remove. This is done so empty lists do not remain and waste heap space if they are never used again
	//done in a separate loop to avoid issues iterating through the list while checking for order to update
	int i = 0; //used for indexing; can't use orderID because it never gets decremented to esure IDs are not reused, may cause index error
	for (std::vector<OrderList>::iterator iter = listIn->begin(); iter != listIn->end(); iter++) {
		if (iter->orders.size() == 0) {
			std::cout << "Empty list: " << iter->partyName << " found. Removing...\n";
			listIn->erase(listIn->begin() + i);
			break;
		}
		i++;
	}
	return returnVal;
}

void OrderBook::show_orders(int buyOrSell){
	if(buyOrSell == BUY){
		for(std::vector<OrderList>::iterator iter = buyOrders.begin(); iter != buyOrders.end(); iter++){
			iter->show_data();
		}
	}
	else if(buyOrSell == SELL){
		for(std::vector<OrderList>::iterator iter = sellOrders.begin(); iter != sellOrders.end(); iter++){
			iter->show_data();
		}
	}
}