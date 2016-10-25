
#include <iostream>
#include <string>
#include <memory>
#include <type_traits>
#include <fstream>
#include "Order.hpp"
#include "OrderBook.hpp"
#include "OrderList.hpp"
#include "variables.hpp"

using namespace std;

int main(){
	unique_ptr<OrderBook> orderBook = make_unique<OrderBook>();
	string option;
	string party, instrument;
	double price;
	int size;


	string tempStr; //for storing number strings from command line for conversion
	string buyOrSell;
	int orderType;
	
	cout << "Choose (f)ile mode or (m)anual mode:" << endl;
	cin >> option;
	if(option.compare("f") == 0){ //automated entry
		ifstream fileName;
		string fIn;
		cout << "Enter file name" << endl;
		cin >> fIn;
		fileName.open(fIn);
		if(!fileName.is_open()){
			cout << "Invalid file. Exiting...";
		}
		else{
			string buf;
			while(!fileName.eof()){ //grabs 5 lines from the file and creates an object. files are formatted for this
				getline(fileName, buf);
				buyOrSell = buf;

				if(buyOrSell.compare("b") == 0){
					orderType = BUY;
				}
				else if(buyOrSell.compare("s") == 0){
					orderType = SELL;
				}

				getline(fileName, buf);
				party = buf;

				getline(fileName, buf);
				instrument = buf;

				getline(fileName, buf);
				price = stod(buf);

				getline(fileName, buf);
				size = stoi(buf);

				Order temp = Order(party, instrument, price, size); 
				int match = orderBook->check_for_match(temp, orderType); /*check to see if there is a match before entering into the book
																			if yes, no need to actually insert it*/
				
				if(match != -1){ //match found; remove matching order already in the book
					cout << "Match found for " << party << ", selling " << size << " unit(s) of " << instrument << " for " << price << endl;
					if(orderType == BUY){
						if(orderBook->remove_order(match, SELL) == 0){
						cout << " order removed\n";
						}
					}
					else if(orderType == SELL){
						if (orderBook->remove_order(match, BUY) == 0) { cout << "order removed\n"; }
					}
				}
				else{ //no match was found; add order to list
					if(orderType == BUY){
						orderBook->add_order(temp, orderType);
					}
					else if(orderType == SELL){
						orderBook->add_order(temp, orderType);
					}
				}
			}
			fileName.close();
		}
	}

	else if(option.compare("m") == 0){ //manual entry
		while(1){ //input loop
			cout << "Would you like to place an order? y/n: ";
			cin >> option;
			if(option.compare("y") != 0 && option.compare("n") != 0){
				cout << "Invalid option. Enter y or n" << endl;
				continue;
			}
			
			else if(option.compare("n") == 0){
				break;
			}
			else{
				cout << "Is this a buy or sell order? (enter b or s)";
				cin >> buyOrSell;
				if(buyOrSell.compare("b") == 0){
					orderType = BUY;
				}
				else if(buyOrSell.compare("s") == 0){
					orderType = SELL;
				}
				cout << "Enter the party name: ";
				cin >> party;
				cout << "Enter the instrument: ";
				cin >> instrument;
				cout << "Enter the price: ";
				cin >> tempStr;
				price = stod(tempStr);
				cout << "Enter the size: ";
				cin >> tempStr;
				size = stoi(tempStr);

				Order temp = Order(party, instrument, price, size); 
				int match = orderBook->check_for_match(temp, orderType); /*check to see if there is a match before entering into the book
																			if yes, no need to actually insert it*/
				if(match != -1){ //match found; remove matching order already in the book
					cout << "Match found for " << party << " " << instrument << " " << price << " " << size << endl;
					if(orderType == BUY){
						orderBook->remove_order(match, SELL);
					}
					else if(orderType == SELL){
						orderBook->remove_order(match, BUY);
					}
				}
				else{ //no match was found; add order to list
					if(orderType == BUY){
						orderBook->add_order(temp, orderType);
					}
					else if(orderType == SELL){
						orderBook->add_order(temp, orderType);
					}
				}
				continue;
			}
		}

	}
	else{
		cout << "Invalid option. Exiting...";
	}
	
}
