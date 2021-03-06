#include "OrderBook.hpp"
#include "OrderList.hpp"
#include "variables.hpp"
#include <string>
#include <iostream>

OrderBook::OrderBook():buyOrderID(0), sellOrderID(0){
	
}

OrderBook::~OrderBook(){
}

int OrderBook::add_order(std::shared_ptr<Order> newOrder, int buyOrSell){
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
		if (newOrder->party.compare(iter->partyName) == 0) { //if a vector for this party already exists, add to that party
			if (buyOrSell == BUY) {
				newOrder->orderid = ++buyOrderID;
				std::cout << "New Order ID: " << buyOrderID << std::endl;
			}
			else if (buyOrSell == SELL) {
				newOrder->orderid = ++sellOrderID;
				std::cout << "New Order ID: " << sellOrderID << std::endl;
			}
			iter->add_node(newOrder);
			return 0; //tell calling function that order was added to an existing list
		}
	}
	//if execution falls through here, then a new list should be created and added to the orderlist vector
	try {
		OrderList temp = OrderList();
		if (buyOrSell == BUY) {
			newOrder->orderid = ++buyOrderID;
			std::cout << "New Order ID: " << buyOrderID << std::endl;
		}
		else if (buyOrSell == SELL) {
			newOrder->orderid = ++sellOrderID;
			std::cout << "New Order ID: " << sellOrderID << std::endl;
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


int OrderBook::check_for_match(std::shared_ptr<Order> newOrder, int buyOrSell) {
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
		if (iter->partyName.compare(newOrder->party) != 0) { //only check when parties aren't the same; not buying and selling to ourselves
			currentList = *iter;
			for (std::vector<std::shared_ptr<Order>>::iterator i = currentList.orders.begin(); i != currentList.orders.end(); i++) {
				if ((*i)->check_best_match(newOrder, bestPrice, currentMatch, buyOrSell) == 0) {
					currentMatch = (*i)->orderid;
					bestPrice = (*i)->price;
				}
			}
		}
	}
	return currentMatch;

}

int OrderBook::update_order(int orderID, int orderSize, int buyOrSell) {
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

void OrderBook::show_orders() {
	std::cout << "*******LIST OF BUY ORDERS******\n";
	for (std::vector<OrderList>::iterator iter = buyOrders.begin(); iter != buyOrders.end(); iter++) {
		iter->show_data();
	}
	std::cout << "\n";

	std::cout << "*******LIST OF SELL ORDERS******\n";
	for (std::vector<OrderList>::iterator iter = sellOrders.begin(); iter != sellOrders.end(); iter++) {
		iter->show_data();
	}
	std::cout << "\n";
}

std::shared_ptr<Order> OrderBook::change_order(int orderID, int buyOrSell) {
	std::vector<OrderList> *listIn = nullptr;
	if (buyOrSell == BUY) {
		listIn = &buyOrders;
	}
	else if (buyOrSell == SELL) {
		listIn = &sellOrders;
	}
	else {
		return nullptr;
	}

	std::shared_ptr<Order> orderObj = nullptr;
	for (std::vector<OrderList>::iterator iter = listIn->begin(); iter != listIn->end(); iter++) {
		if ((orderObj = iter->find_order(orderID)) != nullptr){ //if this does not return null, it means the order was found and that orderObj now points to it
			break;
		}
	}
	return orderObj;
}

std::shared_ptr<Order> OrderBook::reset_priority(std::shared_ptr<Order> result, int buyOrSell) {
	std::vector<OrderList> *listIn = nullptr;
	std::shared_ptr<Order> temp = std::make_shared<Order>(result->party, result->instrument, result->price, result->size); //make a copy of the object before removing from the list
	if (buyOrSell == BUY) {
		listIn = &buyOrders;
	}
	else if (buyOrSell == SELL) {
		listIn = &sellOrders;
	}
	else {
		return nullptr; //failure
	}
	
	int wasRemoved;
	for (std::vector<OrderList>::iterator iter = listIn->begin(); iter != listIn->end(); iter++) {
		if ((wasRemoved = iter->remove_order(result)) == 0) { //if this equals zero, it means that the order was removed
			return temp; //the copy of the object is returned for re-entry into the list
		}
	
	}
	return nullptr; //a failed removal returns null
}
