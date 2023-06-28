#include <iostream>

class Node {
    public:
        int data;
        Node *next;
    
        Node() : data{0}, next{nullptr} {}
        Node(int new_data) : data{new_data} {
            next = nullptr;
        }
};

class LinkedList {
    public:
        Node *head;
    public:
        LinkedList() : head{nullptr} {}

        void insert_at_the_start(int value);

        void delete_at_the_start();

        void print_list();

        bool detect_loop();
};

void LinkedList::insert_at_the_start(int value) {
    Node *node = new Node(value);
    node->next = head;
    head = node;
}

void LinkedList::delete_at_the_start() {
    if (head) {
        Node * tmp = head->next;
        delete head;
        head = tmp;
    }else{
        std::cout << "List is already empty" << '\n';
    }
}

void LinkedList::print_list() {
    Node * tmp = head;
    if (!tmp) {
        std::cout << "Empty" << '\n';
    }
    while (tmp) {
        std::cout << tmp->data << '\n';
        tmp = tmp->next;
    }
}

bool LinkedList::detect_loop() {
    Node *slow_ptr = head;
    Node *fast_ptr = head;
    while (slow_ptr != nullptr && fast_ptr != nullptr && fast_ptr->next != nullptr) {
        slow_ptr = slow_ptr->next;
        fast_ptr = fast_ptr->next->next;
        if (slow_ptr == fast_ptr) {
            return 1;
        }
    }
    return 0;
}

int main() {
    LinkedList list;
    list.insert_at_the_start(10);
    list.insert_at_the_start(20);
    list.insert_at_the_start(30);
    list.insert_at_the_start(50);
    list.insert_at_the_start(50);
    list.insert_at_the_start(60);
    list.insert_at_the_start(70);
    list.insert_at_the_start(80);
    list.print_list();
    
    // Node *tmp = list.head;
    // while (tmp->next) {
    //     tmp = tmp->next;
    // }
    // tmp->next = list.head;

    std::cout << '\n' << list.detect_loop() << '\n';
    return 0;
}
