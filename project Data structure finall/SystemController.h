#pragma once
#include "PriorityQueue.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class SystemController {
private:
    static const int SETTINGS_SIZE = 20;
    string settingKeys[SETTINGS_SIZE];
    double settingValues[SETTINGS_SIZE];
    bool settingUsed[SETTINGS_SIZE];

    int hashKey(string key);
    void setSetting(string key, double value);
    double getSetting(string key);

    PriorityQueue mainQueue;
    PriorityQueue vipQueue;
    PriorityQueue regularQueue;
    PriorityQueue emergencyQueue;

    Person served[100];
    int servedCount;

public:
    SystemController();

    int getServiceScore(string type);
    double calculatePriority(Person p);

    void addPerson(Person p);
    void serveNext(int time);
    void updatePersonPriority(Person p, int newWaiting);
    void applyFairness(Person p);

    void addToTypeQueue(Person p);
    void mergeRegularToVip();

    void loadFromFile(string fileName);
    void printMainQueue();
    void serveAll();

    void simulate();

    void addReport(Person p);
    void sortReport();
    void showReport();

    void updateWeights(double u, double w, double s);
};