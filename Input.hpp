#ifndef INPUT_HPP
#define INPUT_HPP

#include <memory>
#include "OrderBook.hpp"
#include "Order.hpp"

class Input{
public:
	Input();
	~Input();
	void check_input(std::shared_ptr<OrderBook> orderBook, Order *temp, int orderType);
};

#endif 