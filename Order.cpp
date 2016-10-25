#include "Order.hpp"
#include <iostream>
#include <string>

Order::Order(){

}
Order::Order(string partyIn, string instrumentIn, double priceIn, int sizeIn):
	party(partyIn), instrument(instrumentIn), price(priceIn), size(sizeIn), orderid(-1) {}
//orderid is set to -1 by default; this will change when it is actually assigned an orderID when it is added to the order book

Order::~Order(){
}

int Order::compare_orders(Order orderIn){
	int returnVal = 0; //flag that changes to -1 if orders do NOT match. set to 0 (match) by default
	if(instrument.compare(orderIn.instrument) != 0){
		returnVal = -1;
	}
	else if(size != orderIn.size){
		returnVal = -1;
	}

	return returnVal;
}