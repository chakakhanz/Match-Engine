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

TEST(orderList, test_add_node) {
	OrderList ol = OrderList();
	ASSERT_EQ(0, ol.numNodes);
	Order test1 = Order("company", "instrument", 100, 2);
	ASSERT_EQ(0, ol.add_node(test1));
	ASSERT_EQ(-1, ol.add_node(test1));
	ASSERT_EQ(-1, ol.add_node(test1));
	EXPECT_EQ(1, ol.orders.size());

}

TEST(orderList, test_remove_node_in_list) {
	OrderList ol = OrderList();
	Order test1 = Order("company", "instrument", 100, 2);
	ol.add_node(test1);
	EXPECT_EQ(1, ol.numNodes);
	EXPECT_EQ(1, ol.orders.size());
	EXPECT_EQ(0, ol.update_node(test1.orderid, test1.size));
	EXPECT_EQ(0, ol.numNodes);
	EXPECT_EQ(0, ol.orders.size());
	ol.add_node(test1);
	EXPECT_EQ(1, ol.orders.size());
	EXPECT_EQ(2, ol.orders[0].size);

}

TEST(orderList, test_update_node_outside_list) {
	OrderList ol = OrderList();
	Order test1 = Order("company", "instrument", 100, 2);
	EXPECT_EQ(NO_MATCH, ol.update_node(test1.orderid, test1.size));
	
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
	EXPECT_EQ(-1, ob.add_order(test1, BUY));
	EXPECT_EQ(-1, ob.add_order(test2, BUY));
	EXPECT_EQ(-1, ob.add_order(test2, SELL));
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
	Order test2 = Order("company b", "instrument", 42, 2);
	Order test3 = Order("company c", "instrument", 1, 2);
	Order test4 = Order("what", "instrument", 42, 2);
	OrderBook ob = OrderBook();
	ob.add_order(test1, BUY);
	ob.add_order(test2, BUY);
	ob.add_order(test3, BUY);
	int val = ob.check_for_match(test4, SELL);

	EXPECT_EQ(2, val);
}


TEST(testOrderBook, test_input){
	OrderBook ob = OrderBook();
	Order test1 = Order("company a", "contract", 100, 1);
	Order test2 = Order("company b", "bond", 200, 4);
	Order test3 = Order("company c", "contract", 100, 1);
	Order test4 = Order("company d", "contract", 100, 1);
	ob.add_order(test1, BUY);
	ob.add_order(test2, BUY);
	ob.add_order(test3, SELL);
	ob.add_order(test4, SELL);
	EXPECT_EQ(2, ob.buyOrderID);
	EXPECT_EQ(1, ob.buyOrders[0].orders.size());
	EXPECT_EQ(4, ob.buyOrders[1].orders[0].size);
}

TEST(testOrderList, test_update_node) {
	OrderBook ob = OrderBook();
	Order buy1 = Order("me", "coke", 3.5, 1000);
	Order buy2 = Order("a", "coke", 3.5, 200);
	Order buy3 = Order("b", "coke", 3.5, 100);
	Order buy4 = Order("c", "coke", 4.5, 65);
	Order buy5 = Order("d", "coke", 4.5, 200);
	Order buy6 = Order("e", "coke", 4.5, 100);
	Order buy7 = Order("f", "coke", 4.5, 65);
	ob.add_order(buy1, BUY);
	ob.add_order(buy2, BUY);
	ob.add_order(buy3, BUY);
	ob.add_order(buy4, BUY);
	ob.add_order(buy5, BUY);
	ob.add_order(buy6, BUY);
	ob.add_order(buy7, BUY);
	Order sell1 = Order("they", "coke", 4.0, 2000);
	int id = ob.check_for_match(sell1, SELL);
	EXPECT_EQ(4, id);
	
}

TEST(testOrderBook, test_partial_delete_1) {

	Order coke = Order("me", "coke", 4.5, 1000);
	Order buy1 = Order("you", "coke", 4.4, 200);
	Order buy2 = Order("you", "coke", 4.4, 100);
	Order buy3 = Order("you", "coke", 4.4, 65);
	shared_ptr<OrderBook> ob = make_shared<OrderBook>();
	Input ip = Input();
	ob->add_order(coke, BUY);
	//int orderType = SELL;
	ip.check_input(ob, buy1, SELL);
	EXPECT_EQ(1, ob->buyOrders.size());
	EXPECT_EQ(0, ob->sellOrders.size());
	EXPECT_EQ(800, ob->buyOrders[0].orders[0].size);
	ip.check_input(ob, buy2, SELL);
	EXPECT_EQ(1, ob->buyOrders.size());
	ip.check_input(ob, buy3, SELL);
	int size = ob->buyOrders[0].orders[0].size;
	EXPECT_EQ(635, size);
	EXPECT_EQ(1, ob->buyOrders.size());
	Order buy4 = Order("you", "coke", 4.5, 635);
	ip.check_input(ob, buy4, SELL);
	EXPECT_TRUE(ob->buyOrders.empty());
}

TEST(testOrder, test_remove_erase_idiom) {
	vector<Order> *ord = new vector<Order>();
	Order coke = Order("me", "cheese", 4.5, 1000);
	Order buy1 = Order("you", "cheese", 4.5, 200);
	Order buy2 = Order("you", "cheese", 4.5, 800);
	Order buy3 = Order("you", "chesse", 4.5, 65);
	ord->push_back(coke);
	ord->push_back(buy1);
	ord->push_back(buy2);
	EXPECT_EQ(3, ord->size());
	int orderSize = 1000;
	(*ord).erase(remove_if((*ord).begin(), (*ord).end(), [orderSize](Order x) mutable {return orderSize == x.size;}));
	EXPECT_EQ(200, (*ord)[0].size);
	EXPECT_EQ(800, (*ord)[1].size);
	EXPECT_EQ(2, ord->size());
	orderSize = 200;
	(*ord).erase(remove_if((*ord).begin(), (*ord).end(), [orderSize](Order x) mutable {return orderSize == x.size;}));
	EXPECT_EQ(1, ord->size());
	EXPECT_EQ(800, (*ord)[0].size);
	delete ord;
}

