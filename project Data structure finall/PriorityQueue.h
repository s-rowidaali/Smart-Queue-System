#pragma once
#include "Person.h"
#include <iostream>
using namespace std;
class PriorityQueue {
private:
    Person arr[100];
    int size;
    void swapPersons(Person& a, Person& b);
    void heapifyUp(int index);
    void heapifyDown(int index);
public:
    PriorityQueue();
    bool isEmpty();
    void insert(Person p);
    Person extractMax();
    void updatePriority(int id, int newWaiting, double newPriority);
    void printQueue();
};