#include "Input.hpp"
#include <string>
#include <memory>
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

void Input::begin() {
	std::shared_ptr<OrderBook> orderBook = std::make_shared<OrderBook>();
	
	std::string option;
	std::cout << "Choose (f)ile mode or (m)anual mode:" << std::endl;
	std::cin >> option;
	if (option.compare("f") == 0) { //automated entry
		file_mode(orderBook);
	}
	else if (option.compare("m") == 0) {
		manual_mode(orderBook);
	}
	else {
		std::cout << "Invalid option. Exiting...";
	}

}

void Input::check_input(std::shared_ptr<OrderBook> orderBook, std::shared_ptr<Order> temp, int orderType) {
	int listToUpdate = !orderType; //list to search in order to update if a match is found. it will the opposite of the new order type
	int matchID = orderBook->check_for_match(temp, orderType);

	if (matchID != -1) { //match found; update order book
		std::cout << "Match found for " << temp->party << ", with " << temp->size << " unit(s) of " << temp->instrument << " at " << temp->price << " each" << std::endl;

		int updateResult = (orderBook->update_order(matchID, temp->size, listToUpdate));
		if (updateResult >= 1) { //means an order was removed and the remainder of the new order must be added to the book
			std::cout << "order removed, checking for other matches.." << std::endl;
			temp->size = updateResult;
			check_input(orderBook, temp, orderType);
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
	else if (matchID == -1){ //no match was found; add order to list
		orderBook->add_order(temp, orderType);
		std::cout << "No match found, adding new order to book" << std::endl;
	}
	else{ //error has occured
		std::cout << "Error during update" << std::endl;
	}
	std::cout << "\n";
}

void Input::manual_mode(std::shared_ptr<OrderBook> orderBook) {
	std::string option;
	std::string party, instrument;
	double price;
	int size;
	std::string tempStr; //for storing number strings from command line for conversion
	std::string buyOrSell;
	int orderType;
	while (1) { //input loop
		std::cout << "Would you like to place an order? y/n: \n";
		std::cout << "You can also view the order book (enter v), or c to modify an order\n";
		std::cin >> option;
		if (option.compare("y") == 0) {
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

			std::shared_ptr<Order> temp = std::make_shared<Order>(party, instrument, price, size);
			std::cout << "checking..\n";
			check_input(orderBook, temp, orderType); //checks for match already in book and updates lists as needed
			continue;
		}
		else if (option.compare("n") == 0) {
			break;
		}
		else if (option.compare("v") == 0) {
			orderBook->show_orders();
		}
		else if (option.compare("c") == 0) {
			change_order(orderBook);
		}
		else {
			std::cout << "Invalid option. Please enter y, n, or v \n";
		}
	}
}

void Input::file_mode(std::shared_ptr<OrderBook> orderBook) {
	std::string option;
	std::string party, instrument;
	double price;
	int size;
	std::string tempStr; //for storing number strings from command line for conversion
	int orderType;

	std::shared_ptr<std::ifstream> fileName = std::make_shared<std::ifstream>();
	std::string fIn;
	std::cout << "Enter file name" << std::endl;
	std::cin >> fIn;
	fileName->open(fIn);
	if (!fileName->is_open()) {
		std::cout << "Invalid file. Exiting...";
	}
	else {
		std::string buf;
		while (!fileName->eof()) { //grabs 5 lines from the file and creates an object. files are formatted for this
			std::getline(*fileName, buf);

			if (buf.compare("b") == 0) {
				orderType = BUY;
			}
			if (buf.compare("s") == 0) {
				orderType = SELL;
			}
			if (buf.compare("c") == 0) {
				change_order(orderBook, fileName);
				continue;
			}
			if (buf.compare("v") == 0) {
				orderBook->show_orders();
				continue;
			}
			getline(*fileName, buf);
			party = buf;

			getline(*fileName, buf);
			instrument = buf;

			getline(*fileName, buf);
			price = stod(buf);

			getline(*fileName, buf);
			size = stoi(buf);

			std::shared_ptr<Order> temp = std::make_shared<Order>(party, instrument, price, size);
			check_input(orderBook, temp, orderType);
		}
		fileName->close();
		std::cout << "File reading complete.\n -Enter m to enter manual mode\n -Enter any other key to exit\n";
		std::cin >> option;
		if (option.compare("m") == 0) {
			manual_mode(orderBook);
		}
	}
}

void Input::change_order(std::shared_ptr<OrderBook> orderBook) {
	std::string buyOrSell, option;
	int ID, orderType;
	std::cout << "Is the order (b)uy or (s)ell?\n";
	std::cin >> buyOrSell;
	if (buyOrSell.compare("b") == 0) {
		orderType = BUY;
	}
	else if (buyOrSell.compare("s") == 0) {
		orderType = SELL;
	}
	std::cout << "Enter orderID: \n";
	std::cin >> option;
	ID = stoi(option);
	std::shared_ptr<Order> result = orderBook->change_order(ID, orderType);
	if (result != nullptr) {
		double newPrice = -1;
		int newSize = -1;
		double oldPrice = result->price;
		int oldSize = result->size;
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
			std::cout << "No update\n";
		}
		else {
			if (newPrice != -1) {
				result->price = newPrice;
			}
			if (newSize != -1) {
				result->size = newSize;
			}
			if ((newPrice != -1 && (newPrice != oldPrice)) || newSize < oldSize) {
				std::shared_ptr<Order> temp = orderBook->reset_priority(result, orderType); //deletes old order, returns updated order in a new object
				check_input(orderBook, temp, orderType); //re-add the order to the list, effectively resetting its priority
			}
		}
	}
}

void Input::change_order(std::shared_ptr<OrderBook> orderBook, std::shared_ptr<std::ifstream> fileIn) {
	std::string option;
	int ID, orderType;
	getline(*fileIn, option);
	if (option.compare("b") == 0) {
		orderType = BUY;
	}
	else if (option.compare("s") == 0) {
		orderType = SELL;
	}
	getline(*fileIn, option);
	ID = stoi(option);
	std::shared_ptr<Order> result = orderBook->change_order(ID, orderType);
	if (result != nullptr) {
		double newPrice = -1;
		int newSize = -1;
		double oldPrice = result->price;
		int oldSize = result->size;
		getline(*fileIn, option);
		if (option.compare("s") == 0) {
			;
		}
		else {
			newPrice = std::stod(option);
		}

		getline(*fileIn, option);
		if (option.compare("s") == 0) {
			;
		}
		else {
			std::string temp;
			newSize = std::stoi(option);
		}

		if (newPrice == -1 && newSize == -1) {
			std::cout << "No update\n";
		}
		else {
			if (newPrice != -1) {
				result->price = newPrice;
			}
			if (newSize != -1) {
				result->size = newSize;
			}
			if ((newPrice != -1 && (newPrice != oldPrice)) || newSize < oldSize) {
				std::shared_ptr<Order> temp = orderBook->reset_priority(result, orderType); //deletes old order, returns updated order in a new object
				check_input(orderBook, temp, orderType); //re-add the order to the list, effectively resetting its priority
			}
		}
	}
}