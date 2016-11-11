# Match-Engine

To build the executable, simply compile the match_engine executable with make.

After executing, a prompt will appear for file mode (option 'f') or manual mode (option 'm'). A prompt for a filename will appear if option f is chosen. If option m is chosen, prompts for user input will appear (input not validated; format is string,string,double,int)

Input test files are included, as well as the output from a memory leak check. Pictures of sample output are also shown.

The data structure used is a vector of vectors, which essentially work like a hash table. This makes it easier to check for duplicate orders from a given entity. Each order is given a unique order ID for its respective side (buy or sell), which is tracked by the order book. The ID counter is only decremented to ensure that an ID is never reused; this allows the ID to be used as a timestamp to compare when orders were placed given that the prices are equal.

The program will not allow duplicates of orders on the same side, i.e. an exact copy of an already exisiting buy order will not be allowed. However, a copy of that order on the sell side will be allowed. Orders are first compared by entity to ensure that buy and sell orders from the same entity are not being matched. When a match is found for an order, the user is notified, and the matching order is shown. The matching order's size after the order is shown. If this number is 0, the order will be deleted from the order book. Any remaining units from either order will be kept in the book (or added to the book if it is the new order.)

Orders can be modified after being added to the order book. If the price changes or the size of the order decreases, the order's priority is reset by deleting and re-adding it to the end of the list. If the size of the order increases, priorty remains the same.
