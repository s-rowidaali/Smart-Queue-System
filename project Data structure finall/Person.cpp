#include "Person.h"
Person::Person() {
    id = 0;
    urgency = 0;
    waitingTime = 0;
    serviceTime = 0;
    serviceType = "";
    priority = 0;
}
Person::Person(int id, int urgency, int waitingTime, int serviceTime, string serviceType) {
    this->id = id;
    this->urgency = urgency;
    this->waitingTime = waitingTime;
    this->serviceTime = serviceTime;
    this->serviceType = serviceType;
    this->priority = 0;
}