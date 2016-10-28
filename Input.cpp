#include "Input.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include "Order.hpp"
#include "OrderBook.hpp"
#include "OrderList.hpp"
#include "variables.hpp"


Input::Input() {

}

Input::~Input() {

}

void Input::check_input(std::shared_ptr<OrderBook> orderBook, Order temp, int orderType) {
	int listToUpdate = !orderType; //list to search in order to update if a match is found. it will the opposite of the new order type
	/*if (orderType == BUY) {
		listToUpdate = 
	}
	else if (orderType == SELL) {
		listIn = &sellOrders;
	}*/
	int matchID = orderBook->check_for_match(temp, orderType);
	if (matchID != -1) { //match found; update order book
		std::cout << "Match found for " << temp.party << " " << temp.instrument << " " << temp.price << " " << temp.size << std::endl;

		int updateResult = (orderBook->update_order(matchID, temp.size, listToUpdate));
		if (updateResult >= 1) { //means an order was removed and the remainder of the new order must be added to the book
			std::cout << "order removed, adding remainder of new order" << std::endl;
			temp.size = updateResult;
			orderBook->add_order(temp, orderType);
		}
		else if (updateResult == 0) { //if both orders were completely filled
			std::cout << "order removed, new order filled completely" << std::endl;
		}
		else if (updateResult == -1) { //if new order was filled but did not fill order in book completely
			std::cout << "new order filled, matching order updated" << std::endl;
		}
		else {
			std::cout << "Error during update" << std::endl;
		}
		

	}
	else { //no match was found; add order to list
		orderBook->add_order(temp, orderType);
	}
}