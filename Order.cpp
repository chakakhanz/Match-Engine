#include "Order.hpp"
#include "variables.hpp"
#include <iostream>
#include <string>
#include <memory>

Order::Order(){

}
Order::Order(std::string partyIn, std::string instrumentIn, double priceIn, int sizeIn):
	party(partyIn), instrument(instrumentIn), price(priceIn), size(sizeIn), orderid(-1) {}
//orderid is set to -1 by default; this will change when it is actually assigned an orderID when it is added to the order book

 Order::~Order(){
}

int Order::check_best_match(std::shared_ptr<Order> orderIn, double bestPrice, int bestOrderID, int buyOrSell){
	int returnVal = 0; //default value if this order is the best current match
	if(instrument.compare(orderIn->instrument) != 0){ 
		return -1; //instant failure
	}

	if(buyOrSell == BUY){ 
		
		if (price > orderIn->price) { //not buying from order with greater price than the buy order
			returnVal = -1;
		}
		else if ((price < orderIn->price) && price > bestPrice && bestPrice != -1.0) {  //if sale price is lower than buy price BUT sale price is higher than current best sale price
			returnVal = -1;
		}
		else if (price == bestPrice && orderid > bestOrderID) { //if price is equal and the ID of this order is bigger than the current best (i.e. it is newer), then fail
			returnVal = -1;
		}		
	}
	
	if (buyOrSell == SELL) {
		
		if (price < orderIn->price) { //cant sell to an order buying for less than the sell price
			returnVal = -1;
		}
		if ((price >= orderIn->price) && price != bestPrice && bestPrice != -1) { //if buy price is > than sale price BUT buy price is lower than the current best buy price
			if (price < bestPrice) { //^must check if price != best price to avoid a false positive
				returnVal = -1;
			}
		}
		else if (price == bestPrice && orderid > bestOrderID) { //if price is equal and the ID of this order is bigger than the current best (i.e. it is newer), then fail
			returnVal = -1;
		}	
	}
	return returnVal;
}