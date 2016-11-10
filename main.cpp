
#include <iostream>
#include <string>
#include <memory>
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

				Order* temp = new Order(party, instrument, price, size);
				inputObj.check_input(orderBook, temp, orderType);
			}
			fileName.close();
			std::cout << "File reading complete. If you'd like to view the order book, press v. Otherwise, press any key to exit\n";
			std::cin >> option;
			if (option.compare("v") == 0) {
				orderBook->show_orders();
			}
		}
	}

	else if(option.compare("m") == 0){ //manual entry
		while(1){ //input loop
			std::cout << "Would you like to place an order? y/n: \n";
			std::cout << "You can also view the order book (enter v), or c to modify an order\n";
			std::cin >> option;
			if (option.compare("y") == 0){
				std::cout << "Is this a buy or sell order? (enter b or s)\n";
				
				
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
					else {
						std::cout << "Invalid option. Please enter b or s\n";
					}
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

				Order* temp = new Order(party, instrument, price, size); 
				std::cout << "checking..\n";
				inputObj.check_input(orderBook, temp, orderType); //checks for match already in book and updates lists as needed
				continue;
			}
			else if (option.compare("n") == 0) {
				break;
			}
			else if (option.compare("v") == 0) {
				orderBook->show_orders();
			}
			else if (option.compare("c") == 0) {
				int ID;
				std::cout << "Is the order (b)uy or (s)ell?\n";
				std::cin >> buyOrSell;
				if (buyOrSell.compare("b") == 0) {
					orderType = BUY;
				}
				else if (buyOrSell.compare("s") == 0) {
					orderType = SELL;
				}
				std::cout << "Enter orderID: \n";
				std::cin >> ID;
				Order* result = orderBook->change_order(ID, orderType);
				if (result != nullptr) {
					double newPrice = -1;
					int newSize = -1;
					double oldPrice = result->price;
					int oldSize = result-> size;
					std::cout << "Enter new price: (or press s to skip)";
					std::cin >> option;
					if (option.compare("s") == 0) {
						;
					}
					else {
						newPrice = std::stod(option);
					}

					std::cout << "Enter new size: (or press s to skip)";
					std::cin >> option;
					if (option.compare("s") == 0) {
						;
					}
					else {
						std::string temp;
						newSize = std::stoi(option);
					}
					
					if (newPrice == -1 && newSize == -1) {
						std::cout << "Error\n";
						continue;
					}
					else {
						if (newPrice != -1) {
							result->price = newPrice;
						}
						if (newSize != -1){
							result->size = newSize;
						}
						if (newPrice != -1 && (newPrice != oldPrice) || newSize < oldSize) {
							orderBook->reset_priority(result, orderType);
						}
						
					}
				}
			}
			else {
				std::cout << "Invalid option. Please enter y, n, or v \n";
			}
		}

	}
	else{
		std::cout << "Invalid option. Exiting...";
	}
	
}
