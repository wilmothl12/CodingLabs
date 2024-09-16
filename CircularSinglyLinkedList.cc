// Logan Wilmoth
// ECE 2160 - 001
// Homework B1 Makeup	

#include <iostream>
const int EMPTY_LIST = 1;
using namespace std;
#ifndef SLL_NODE
#define SLL_NODE

class intCSLLNode {  // Objects of this class are nodes in our CSLL
    friend class intCSLList; // so that we may access node members
                             // in intCSLList class that follows
public:
    intCSLLNode(int el = 0, intCSLLNode *ptr = nullptr) {
        info = el;
        next = ptr;
    }

protected:
    int info;          // int type data member
    intCSLLNode *next; // pointer type data member
};

#endif

#ifndef SLL
#define SLL

class intCSLList {
	public:
		intCSLList();
  	  	~intCSLList();
  	  	bool isEmpty() const;
  	 	void addToTail(int);
  	  	int deleteFromTail(); // delete tail & return its info
  		void searchAndDelete(int);
  	  	void printList() const;

	private:
    	intCSLLNode *tail;
};

#endif

// Empty list function
bool intCSLList::isEmpty() const {
    return (tail == nullptr);
}

// Constructor
intCSLList::intCSLList() : tail(nullptr) {}

// Destructor
intCSLList::~intCSLList() {
    intCSLLNode *p;
    while (!isEmpty()) {
        p = tail->next;
        if (tail == tail->next) { // Only one node in the list
            delete tail;
            tail = nullptr;
        } else {
            tail->next = p->next;
            delete p;
        }
    }
}

// Function for add to tail
void intCSLList::addToTail(int el) {
  	  intCSLLNode *p = new intCSLLNode(el, nullptr);
    if (isEmpty()) {
        tail = p;
        tail->next = p; //  Point to itself
    } else {
        p->next = tail->next;
        tail->next = p;
        tail = p;
    }
}

// Function to delete from tail
int intCSLList::deleteFromTail() {
    if (!isEmpty()) {
        int el = tail->info;
        if (tail->next == tail) { // Only one node in the list
            delete tail;
            tail = nullptr;
        } else {
            intCSLLNode *tmp, *tmp2;

			// Loop to find last node
            for (tmp = tail->next; tmp->next != tail; tmp = tmp->next);

			tmp2 = tail->next; // Store first node
            delete tail;
            tail = tmp; 
            tail->next = tmp2; // Update next pointer
        }
        return el;
    } 
	else throw(EMPTY_LIST);
}


// Search and delete function
void intCSLList::searchAndDelete(int el) {
    if (!isEmpty()) {
        if (tail->next == tail && el == tail->info) { // 1 node in the list
            delete tail;
            tail = nullptr;
        } else {
            intCSLLNode *pred = tail, *tmp = tail->next;
			// Do while loop because a for loop causes seg fault when trying to delete last node
            do {
                if (tmp->info == el) {
                    pred->next = tmp->next;
                    if (tmp == tail) {
                        tail = pred;
                    }
                    delete tmp;
                    return;
                }
                pred = tmp;
                tmp = tmp->next;
            } while (tmp != tail->next);
        }
    }
}

// Print function
void intCSLList::printList() const {
    if (!isEmpty()) {
        intCSLLNode *tmp = tail->next;
        do {
            cout << tmp->info << " ";
            tmp = tmp->next;
        } while (tmp != tail->next);
        cout << endl;
    }
}


// Main function to test if functions work
int main() {
    intCSLList list;

    // Check if list is empty
    cout << "Is list empty? " << (list.isEmpty() ? "Yes" : "No") << endl;

    // Add elements
    list.addToTail(10);
    list.addToTail(20);
    list.addToTail(30);
	
    // Print list
    cout << "List after adding elements: ";
    list.printList();
	
	// Add more to ensure that it adds to the end of the list
	list.addToTail(40);
    list.addToTail(50);

	// Print list
    cout << "List after adding elements: ";
    list.printList();
	
    // Delete from tail
    list.deleteFromTail();

    // Print list
    cout << "List after deleting from tail: ";
    list.printList();

    // Search and delete
    list.searchAndDelete(40);

    // Print list
    cout << "List after deleting 40: ";
    list.printList();

    return 0;
}
