#ifndef INPUT_HPP
#define INPUT_HPP

#include <memory>
#include "OrderBook.hpp"
#include "Order.hpp"

class Input{
public:
	Input();
	~Input();
	void begin();
	void check_input(std::shared_ptr<OrderBook> orderBook, std::shared_ptr<Order> temp, int orderType);
	void file_mode(std::shared_ptr<OrderBook> orderBook);
	void manual_mode(std::shared_ptr<OrderBook> orderBook);
	void change_order(std::shared_ptr<OrderBook> orderBook);
	void change_order(std::shared_ptr<OrderBook> orderBook, std::shared_ptr<std::ifstream> fileIn);
	
};

#endif 