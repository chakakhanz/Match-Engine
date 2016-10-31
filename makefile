GXX = g++
GXXFLAGS = -std=c++0x -Wall

all: main.o Order.o OrderBook.o OrderList.o Input.o
	$(GXX) $(GXXFLAGS) main.o Order.o OrderList.o OrderBook.o Input.o -o match_engine.exe

debug: main.o Order.o OrderBook.o OrderList.o Input.o
	$(GXX) $(GXXFLAGS) -g main.o Order.o OrderList.o OrderBook.o Input.o -o match_engine

main.o: main.cpp
	$(GXX) $(GXXFLAGS) main.cpp -c

Order.o: Order.cpp
	$(GXX) $(GXXFLAGS) Order.cpp -c

OrderBook.o: OrderBook.cpp
	$(GXX) $(GXXFLAGS) OrderBook.cpp -c

OrderList.o: OrderList.cpp
	$(GXX) $(GXXFLAGS) OrderList.cpp -c

Input.o: Input.cpp
	$(GXX) $(GXXFLAGS) Input.cpp -c

clean:
	rm -f *.o *.exe *.gch