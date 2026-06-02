#pragma once
#include <string>
using namespace std;
class Person {
public:
    int id;
    int urgency;
    int waitingTime;
    int serviceTime;
    string serviceType;
    double priority;
    Person();
    Person(int id, int urgency, int waitingTime, int serviceTime, string serviceType);
};