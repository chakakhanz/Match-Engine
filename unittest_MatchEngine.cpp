#include "gtest/gtest.h"
#include "../MatchEngine/Order.hpp"
#include "../MatchEngine/OrderBook.hpp"
#include "../MatchEngine/OrderList.hpp"
#include "../MatchEngine/variables.hpp"
#include "../MatchEngine/Input.hpp"

#include "../MatchEngine/Order.cpp"
#include "../MatchEngine/OrderBook.cpp"
#include "../MatchEngine/OrderList.cpp"
#include "../MatchEngine/Input.cpp"

using namespace std;

TEST(testOrderBook, test_create) { //test to see that orderbook is properly initialized
	OrderBook ob = OrderBook();
	ASSERT_EQ(0, ob.buyOrderID);
	ASSERT_EQ(0, ob.sellOrderID);
	ASSERT_EQ(0, ob.buyOrders.size());
	ASSERT_EQ(0, ob.sellOrders.size());
}

TEST(orderList, test_add_node) { //test to check if duplicate nodes are not added
	OrderList ol = OrderList();
	shared_ptr<Order> test1 = make_shared<Order>("company", "instrument", 100, 2);
	ASSERT_EQ(0, ol.add_node(test1));
	ASSERT_EQ(-1, ol.add_node(test1));
	ASSERT_EQ(-1, ol.add_node(test1));
	EXPECT_EQ(1, ol.orders.size());

}

TEST(orderList, test_remove_node_in_list) { //test to check if 
	OrderList ol = OrderList();
	shared_ptr<Order> test1 = make_shared<Order>("company", "instrument", 100, 2);
	ol.add_node(test1);
	EXPECT_EQ(1, ol.orders.size());
	EXPECT_EQ(0, ol.update_node(test1->orderid, test1->size));
	EXPECT_EQ(0, ol.orders.size());
	ol.add_node(test1);
	EXPECT_EQ(1, ol.orders.size());
	EXPECT_EQ(2, ol.orders[0]->size);

}

TEST(orderList, test_update_node_outside_list) { //test to check if an attempted update that yields no match return proper value
	OrderList ol = OrderList();
	Order test1 = Order("company", "instrument", 100, 2);
	EXPECT_EQ(NO_MATCH, ol.update_node(test1.orderid, test1.size));
	
}

TEST(testOrderBook, test_add_order) { //test to see if add order distinguishes between new entities and exisitng ones in the book
	OrderBook ob = OrderBook();
	shared_ptr<Order> test1 = make_shared<Order>("company", "instrument", 100, 2);
	shared_ptr<Order> test2 = make_shared<Order>("company", "ducks", 100, 2);
	shared_ptr<Order> test3 = make_shared<Order>("me", "instrument", 100, 2);
	EXPECT_EQ(-1, ob.add_order(test1, BUY)); //add to new list
	EXPECT_EQ(0, ob.add_order(test2, BUY)); //add to existing list
	EXPECT_EQ(-1, ob.add_order(test3, BUY)); //add to new list
	EXPECT_EQ(-1, ob.add_order(test3, SELL)); //add to new list
	EXPECT_EQ(3, ob.buyOrderID);
	EXPECT_EQ(1, ob.sellOrderID);
}

TEST(testOrderList, test_check_for_match) { //check to see if match algorithm chooses the best matching order
	shared_ptr<Order> test1 = make_shared<Order>("company a", "instrument", 100, 2);
	shared_ptr<Order> test2 = make_shared<Order>("company b", "instrument", 10, 2);
	shared_ptr<Order> test3 = make_shared<Order>("company c", "instrument", 1, 2);
	shared_ptr<Order> test4 = make_shared<Order>("what", "instrument", 1, 2);
	OrderBook ob = OrderBook();
	ob.add_order(test1, BUY);
	ob.add_order(test2, BUY);
	ob.add_order(test3, BUY);
	int val = ob.check_for_match(test4, SELL);
	
	EXPECT_EQ(1, val);
}

TEST(testOrderList, test_check_for_match_2) { //check to see if matching algorithm chooses the earlier order, given two equal prices
	shared_ptr<Order> test1 = make_shared<Order>("company a", "coke", 100, 2);
	shared_ptr<Order> test2 = make_shared<Order>("company b", "instrument", 42, 2);
	shared_ptr<Order> test3 = make_shared<Order>("company c", "instrument", 42, 2);
	shared_ptr<Order> test4 = make_shared<Order>("what", "instrument", 42, 2);
	OrderBook ob = OrderBook();
	ob.add_order(test1, BUY);
	ob.add_order(test2, BUY);
	ob.add_order(test3, BUY);
	int val = ob.check_for_match(test4, SELL);

	EXPECT_EQ(2, val);
}

TEST(testOrderBook, test_partial_delete) {
	shared_ptr<Order> coke = make_shared<Order>("me", "coke", 4.5, 1000);
	shared_ptr<Order> buy1 = make_shared<Order>("you", "coke", 4.4, 200);
	shared_ptr<Order> buy2 = make_shared<Order>("you", "coke", 4.4, 100);
	shared_ptr<Order> buy3 = make_shared<Order>("you", "coke", 4.4, 65);
	shared_ptr<OrderBook> ob = make_shared<OrderBook>();
	Input ip = Input();
	ob->add_order(coke, BUY);
	ip.check_input(ob, buy1, SELL);
	EXPECT_EQ(1, ob->buyOrders.size());
	EXPECT_EQ(0, ob->sellOrders.size());
	EXPECT_EQ(800, ob->buyOrders[0].orders[0]->size);
	ip.check_input(ob, buy2, SELL);
	EXPECT_EQ(1, ob->buyOrders.size());
	ip.check_input(ob, buy3, SELL);
	int size = ob->buyOrders[0].orders[0]->size;
	EXPECT_EQ(635, size);
	EXPECT_EQ(1, ob->buyOrders.size());

	shared_ptr<Order> buy4 = make_shared<Order>("you", "coke", 4.5, 635);
	ip.check_input(ob, buy4, SELL);
	EXPECT_TRUE(ob->buyOrders.empty());
}