#include "Order.hpp"
#include "variables.hpp"
#include <iostream>
#include <string>

Order::Order(){

}
Order::Order(string partyIn, string instrumentIn, double priceIn, int sizeIn):
	party(partyIn), instrument(instrumentIn), price(priceIn), size(sizeIn), orderid(-1) {}
//orderid is set to -1 by default; this will change when it is actually assigned an orderID when it is added to the order book

Order::~Order(){
}

int Order::check_best_match(Order orderIn, double bestPrice, int bestOrderID, int buyOrSell){
	int returnVal = 0; //default value if this order is the best current match
	if(instrument.compare(orderIn.instrument) != 0){ 
		returnVal = -1;
	}

	if(buyOrSell == BUY){ 
		if (orderIn.size > size) {// if size of buy order > sale order
			returnVal = -1;
		}
		else {
			if (price > bestPrice) { //if price of this object is higher than current best price
				returnVal = -1;
			}
			else if ((price < orderIn.price) && price < bestPrice) {  //if sale price is lower than buy price BUT sale price is higher than current best sale price
				returnVal = -1;
			}
			else if (price == bestPrice && orderid > bestOrderID) { //if price is equal and the ID of this order is bigger than the current best (i.e. it is newer), then fail
				returnVal = -1;
			}
		}
	}
	
	if (buyOrSell == SELL) {
		if (orderIn.size < size) {// if size of buy order < sale order
			returnVal = -1;
		}
		else {
			if (price < orderIn.price) { //if price of this object is LESS THAN than best price, it is best match found relative to the buy list
				returnVal = -1;
			}
			else if ((price > orderIn.price) && price < bestPrice) { //if buy price is > than sale price BUT buy price is lower than the current best buy price

			}
			else if (price == bestPrice && orderid > bestOrderID) { //if price is equal and the ID of this order is bigger than the current best (i.e. it is newer), then fail
				returnVal = -1;
			}
		}
	}
	return returnVal;
}