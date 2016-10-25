#include "OrderBook.hpp"
#include "OrderList.hpp"
#include "variables.hpp"
#include <string>
#include <iostream>

OrderBook::OrderBook():buyOrderID(make_shared<int>(0)), sellOrderID(make_shared<int>(0)),
 buyOrders(make_shared<vector<OrderList>>()),  sellOrders(make_shared<vector<OrderList>>()){
	
}

OrderBook::~OrderBook(){
}

void OrderBook::add_order(Order newOrder, int buyOrSell){
	if(buyOrSell == BUY){
		(*buyOrderID)++; //dereference to increment before sending to second add_order function
		add_order(newOrder, buyOrders, buyOrderID);	
	}
	else if(buyOrSell == SELL){
		(*sellOrderID)++;
		add_order(newOrder, sellOrders, sellOrderID);
	}
}

void OrderBook::add_order(Order newOrder, shared_ptr<vector<OrderList>> listIn, shared_ptr<int> orderID){	
	for(vector<OrderList>::iterator iter = listIn->begin(); iter != listIn->end(); iter++){
		if(newOrder.party.compare(iter->partyName) == 0){ //if a vector for this party already exists, add to that party
			newOrder.orderid = *orderID;
			iter->add_node(newOrder);
			return;
		}
	}	
	//if execution falls through here, then a new list should be created and added to the orderlist vector
		OrderList temp = OrderList();
		newOrder.orderid = *orderID;
		temp.add_node(newOrder);
		listIn->push_back(temp);	
	

}

int OrderBook::check_for_match(Order newOrder, int buyOrSell){
	if(buyOrSell == BUY){
		return check_for_match(newOrder, sellOrders);	
	}
	else if(buyOrSell == SELL){
		return check_for_match(newOrder, buyOrders);
	}
	return -1; //returns null if some error occurs where match is not checked for
}

int OrderBook::check_for_match(Order newOrder, shared_ptr<vector<OrderList>> listIn){
	vector<int> bestMatch;
	for(vector<OrderList>::iterator iter = listIn->begin(); iter != listIn->end(); iter++){
		if(iter->partyName.compare(newOrder.party) == 0){ //skip list if it belongs to the same party; not buying and selling to ourselves
         	continue;
        }
        else{
        	vector<int> checkResult = iter->check_list_match(newOrder);
       		if(!checkResult.empty()){ //if a match is found, result vector will be populated; otherwise it will be empty	
                if(bestMatch.empty()){
                    bestMatch = checkResult; //if bestMatch is empty (i.e. no match has been found), set the result
                }
                else if(bestMatch[0] > checkResult[0]){ //prices are stored in the first index
                    bestMatch = checkResult; //if a match currently exists but new order has a lower price, set newOrder to bestMatch
                } 
                else if((bestMatch[0] == checkResult[0]) && (bestMatch[1] > checkResult[1])){ //orderid stored in second index
                  	bestMatch = checkResult;
                        //if prices are equal, but the current match is newer than the result (i.e. its orderid is bigger), set newOrder to bestMatch
                    
                }
            }
        }
	}
	if(bestMatch.empty()){
		return -1;
	}
	else{
		return bestMatch[1]; //return the orderid
	}
}

void OrderBook::remove_order(int orderID, int buyOrSell){
	if(buyOrSell == BUY){
		remove_order(orderID, sellOrders);	
	}
	else if(buyOrSell == SELL){
		remove_order(orderID, buyOrders);
	}
}

void OrderBook::remove_order(int orderID, shared_ptr<vector<OrderList>> listIn){
	for(vector<OrderList>::iterator iter = listIn->begin(); iter != listIn->end(); iter++){
		if(iter->remove_node(orderID) == 0){
			return;
		}
	}
}

void OrderBook::show_orders(int buyOrSell){
	if(buyOrSell == BUY){
		for(vector<OrderList>::iterator iter = buyOrders->begin(); iter != buyOrders->end(); iter++){
			iter->show_data();
		}
	}
	else if(buyOrSell == SELL){
		for(vector<OrderList>::iterator iter = sellOrders->begin(); iter != sellOrders->end(); iter++){
			iter->show_data();
		}
	}
}