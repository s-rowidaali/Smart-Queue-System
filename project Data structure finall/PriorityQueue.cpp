#include "PriorityQueue.h"
PriorityQueue::PriorityQueue() {
    size = 0;
}

bool PriorityQueue::isEmpty() {
    return size == 0;
}

void PriorityQueue::swapPersons(Person& a, Person& b) {
    Person temp = a;
    a = b;
    b = temp;
}

void PriorityQueue::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;

        if (arr[index].priority > arr[parent].priority) {
            swapPersons(arr[index], arr[parent]);
            index = parent;
        }
        else {
            break;
        }
    }
}
void PriorityQueue::heapifyDown(int index) {
    while (true) {
        int left = index * 2 + 1;
        int right = index * 2 + 2;
        int largest = index;

        if (left < size && arr[left].priority > arr[largest].priority) {
            largest = left;
        }

        if (right < size && arr[right].priority > arr[largest].priority) {
            largest = right;
        }

        if (largest != index) {
            swapPersons(arr[index], arr[largest]);
            index = largest;
        }
        else {
            break;
        }
    }
}
void PriorityQueue::insert(Person p) {
    if (size >= 100) {
        cout << "Queue is full" << endl;
        return;
    }

    arr[size] = p;
    heapifyUp(size);
    size++;
}

Person PriorityQueue::extractMax() {
    if (size == 0) {
        return Person();
    }

    Person top = arr[0];
    arr[0] = arr[size - 1];
    size--;

    heapifyDown(0);

    return top;
}

void PriorityQueue::updatePriority(int id, int newWaiting, double newPriority) {
    for (int i = 0; i < size; i++) {
        if (arr[i].id == id) {
            arr[i].waitingTime = newWaiting;
            arr[i].priority = newPriority;

            heapifyUp(i);
            heapifyDown(i);
            return;
        }
    }

    cout << "Person not found in queue" << endl;
}

void PriorityQueue::printQueue() {
    for (int i = 0; i < size; i++) {
        cout << "ID: " << arr[i].id << " Priority: " << arr[i].priority << " Waiting: " << arr[i].waitingTime << " Type: " << arr[i].serviceType << endl;
    }
}