#include "gtest/gtest.h"
#include "../MatchEngine/Order.hpp"
#include "../MatchEngine/OrderBook.hpp"
#include "../MatchEngine/OrderList.hpp"
#include "../MatchEngine/variables.hpp"
#include "../MatchEngine/Order.cpp"
#include "../MatchEngine/OrderBook.cpp"
#include "../MatchEngine/OrderList.cpp"

TEST(orderList, test_add_node) {
	OrderList ol = OrderList();
	ASSERT_EQ(0, ol.numNodes);
	Order test1 = Order("company", "instrument", 100, 2);
	ASSERT_EQ(0, ol.add_node(test1));
	ASSERT_EQ(-1, ol.add_node(test1));
	ASSERT_EQ(-1, ol.add_node(test1));
	EXPECT_EQ(1, ol.numNodes);
}

TEST(orderList, test_remove_node_in_list) {
	OrderList ol = OrderList();
	Order test1 = Order("company", "instrument", 100, 2);
	ol.add_node(test1);
	EXPECT_EQ(1, ol.numNodes);
	EXPECT_EQ(1, ol.orders.size());
	EXPECT_EQ(0, ol.remove_node(test1.orderid));
	EXPECT_EQ(0, ol.numNodes);
	EXPECT_FALSE(ol.orders.size());
}

TEST(orderList, test_remove_node_outside_list) {
	OrderList ol = OrderList();
	Order test1 = Order("company", "instrument", 100, 2);
	EXPECT_EQ(-1, ol.remove_node(test1.orderid));
}

TEST(testOrderBook, test_create) {
	OrderBook ob = OrderBook();
	ASSERT_EQ(0, ob.buyOrderID);
	ASSERT_EQ(0, ob.sellOrderID);
	ASSERT_EQ(0, ob.buyOrders.size());
	ASSERT_EQ(0, ob.sellOrders.size());
}

TEST(testOrderBook, test_add_order) {
	OrderBook ob = OrderBook();
	Order test1 = Order("company", "instrument", 100, 2);
	Order test2 = Order("me", "instrument", 100, 2);
	EXPECT_EQ(0, ob.add_order(test1, BUY));
	EXPECT_EQ(0, ob.add_order(test2, BUY));
	EXPECT_EQ(0, ob.add_order(test2, SELL));
	EXPECT_EQ(2, ob.buyOrderID);
	EXPECT_EQ(1, ob.sellOrderID);
}

TEST(testOrderList, test_check_for_match) {
	Order test1 = Order("company a", "instrument", 100, 2);
	Order test2 = Order("company b", "instrument", 10, 2);
	Order test3 = Order("company c", "instrument", 1, 2);
	Order test4 = Order("what", "instrument", 1, 2);
	OrderBook ob = OrderBook();
	ob.add_order(test1, BUY);
	ob.add_order(test2, BUY);
	ob.add_order(test3, BUY);
	int val = ob.check_for_match(test4, SELL);
	
	EXPECT_EQ(3, val);
}

TEST(testOrderList, test_check_for_match_2) {
	Order test1 = Order("company a", "instrument", 100, 2);
	Order test2 = Order("company b", "instrument", 10, 2);
	Order test3 = Order("company c", "instrument", 1, 2);
	Order test4 = Order("what", "instrument", 100, 2);
	OrderBook ob = OrderBook();
	ob.add_order(test1, BUY);
	ob.add_order(test2, BUY);
	ob.add_order(test3, BUY);
	int val = ob.check_for_match(test4, SELL);

	EXPECT_EQ(1, val);
}
