
#include <iostream>
#include <string>
#include <memory>
#include <type_traits>
#include <fstream>
#include "Order.hpp"
#include "OrderBook.hpp"
#include "OrderList.hpp"
#include "variables.hpp"
#include "Input.hpp"


int main(){
	std::shared_ptr<OrderBook> orderBook = std::make_shared<OrderBook>();
	Input inputObj = Input();
	std::string option;
	std::string party, instrument;
	double price;
	int size;


	std::string tempStr; //for storing number strings from command line for conversion
	std::string buyOrSell;
	int orderType;
	
	std::cout << "Choose (f)ile mode or (m)anual mode:" << std::endl;
	std::cin >> option;
	if(option.compare("f") == 0){ //automated entry
		std::ifstream fileName;
		std::string fIn;
		std::cout << "Enter file name" << std::endl;
		std::cin >> fIn;
		fileName.open(fIn);
		if(!fileName.is_open()){
			std::cout << "Invalid file. Exiting...";
		}
		else{
			std::string buf;
			while(!fileName.eof()){ //grabs 5 lines from the file and creates an object. files are formatted for this
				std::getline(fileName, buf);
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
				inputObj.check_input(orderBook, temp, orderType);
			}
			fileName.close();
		}
	}

	else if(option.compare("m") == 0){ //manual entry
		while(1){ //input loop
			std::cout << "Would you like to place an order? y/n: ";
			std::cin >> option;
			if(option.compare("y") != 0 && option.compare("n") != 0){
				std::cout << "Invalid option. Enter y or n" << std::endl;
				continue;
			}
			
			else if(option.compare("n") == 0){
				break;
			}
			else{
				std::cout << "Is this a buy or sell order? (enter b or s)";
				
				while (1) {
					std::cin >> buyOrSell;
					if (buyOrSell.compare("b") == 0) {
						orderType = BUY;
						break;
					}
					else if (buyOrSell.compare("s") == 0) {
						orderType = SELL;
						break;
					}
					std::cout << "Invalid option. Please enter b or s\n";
				}

				std::cout << "Enter the party name: ";
				std::ws(std::cin);
				std::getline(std::cin, party);
				std::cout << "Enter the instrument: ";
				std::getline(std::cin, instrument);
				std::cout << "Enter the price: ";
				std::cin >> tempStr;
				price = stod(tempStr);
				std::cout << "Enter the size: ";
				std::cin >> tempStr;
				size = stoi(tempStr);

				Order temp = Order(party, instrument, price, size); 
				std::cout << "checking..\n";
				inputObj.check_input(orderBook, temp, orderType); //checks for match already in book and updates lists as needed
				continue;
			}
		}

	}
	else{
		std::cout << "Invalid option. Exiting...";
	}
	
}
