#include "SystemController.h"
#include <algorithm>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

int SystemController::hashKey(string key) {
    int sum = 0;

    for (int i = 0; i < key.length(); i++) {
        sum += key[i];
    }

    return sum % SETTINGS_SIZE;
}

void SystemController::setSetting(string key, double value) {
    int index = hashKey(key);

    while (settingUsed[index] && settingKeys[index] != key) {
        index = (index + 1) % SETTINGS_SIZE;
    }

    settingKeys[index] = key;
    settingValues[index] = value;
    settingUsed[index] = true;
}

double SystemController::getSetting(string key) {
    int index = hashKey(key);
    int start = index;

    while (settingUsed[index]) {
        if (settingKeys[index] == key) {
            return settingValues[index];
        }

        index = (index + 1) % SETTINGS_SIZE;

        if (index == start) {
            break;
        }
    }

    return 0;
}

SystemController::SystemController() {
    for (int i = 0; i < SETTINGS_SIZE; i++) {
        settingUsed[i] = false;
        settingValues[i] = 0;
        settingKeys[i] = "";
    }

    setSetting("urgency_weight", 0.5);
    setSetting("waiting_weight", 0.3);
    setSetting("service_weight", 0.2);
    setSetting("max_wait_time", 25);
    setSetting("boost_multiplier", 0.5);

    setSetting("score_emergency", 10);
    setSetting("score_vip", 8);
    setSetting("score_regular", 5);

    servedCount = 0;
}

int SystemController::getServiceScore(string type) {
    if (type == "emergency") {
        return (int)getSetting("score_emergency");
    }

    if (type == "vip") {
        return (int)getSetting("score_vip");
    }

    return (int)getSetting("score_regular");
}

double SystemController::calculatePriority(Person p) {
    double urgencyWeight = getSetting("urgency_weight");
    double waitingWeight = getSetting("waiting_weight");
    double serviceWeight = getSetting("service_weight");

    return (p.urgency * urgencyWeight) + (p.waitingTime * waitingWeight) + (getServiceScore(p.serviceType) * serviceWeight);
}

void SystemController::addPerson(Person p) {
    p.priority = calculatePriority(p);
    mainQueue.insert(p);

    cout << "Added ID: " << p.id << " Priority: " << p.priority << endl;
}

void SystemController::serveNext(int time) {
    if (mainQueue.isEmpty()) {
        cout << "No person to serve" << endl;
        return;
    }

    Person p = mainQueue.extractMax();

    cout << "Time " << time << " Serving ID: " << p.id << endl;

    addReport(p);
}

void SystemController::updatePersonPriority(Person p, int newWaiting) {
    p.waitingTime = newWaiting;

    double newPriority = calculatePriority(p);

    mainQueue.updatePriority(p.id, newWaiting, newPriority);

    cout << "Updated ID: " << p.id << " New Priority: " << newPriority << endl;
}

void SystemController::applyFairness(Person p) {
    p.priority = calculatePriority(p);

    int maxWaitTime = (int)getSetting("max_wait_time");
    double boostMultiplier = getSetting("boost_multiplier");

    if (p.waitingTime > maxWaitTime) {
        int extra = p.waitingTime - maxWaitTime;
        double boost = extra * boostMultiplier;
        double newPriority = p.priority + boost;

        mainQueue.updatePriority(p.id, p.waitingTime, newPriority);

        cout << "ID: " << p.id << " waited " << p.waitingTime << " minutes" << endl;
        cout << "Boost Applied: " << boost << endl;
        cout << "New Priority: " << newPriority << endl;
    }
    else {
        cout << "No fairness boost needed" << endl;
    }
}

void SystemController::addToTypeQueue(Person p) {
    p.priority = calculatePriority(p);

    if (p.serviceType == "vip") {
        vipQueue.insert(p);
    }
    else if (p.serviceType == "emergency") {
        emergencyQueue.insert(p);
    }
    else {
        regularQueue.insert(p);
    }
}

void SystemController::mergeRegularToVip() {
    cout << "Before merging" << endl;

    cout << "VIP Queue" << endl;
    vipQueue.printQueue();

    cout << "Regular Queue" << endl;
    regularQueue.printQueue();

    while (!regularQueue.isEmpty()) {
        Person p = regularQueue.extractMax();
        vipQueue.insert(p);
    }

    cout << "Regular queue moved to VIP service counter" << endl;

    cout << "After merging" << endl;
    vipQueue.printQueue();
}

void SystemController::simulate() {
    cout << "Simulation started" << endl;

    for (int time = 0; time <= 6; time += 2) {
        Person p(100 + time, 2 + time % 3, time + 1, 5 + time, "regular");

        p.priority = calculatePriority(p);
        mainQueue.insert(p);

        cout << "Time " << time << endl;
        cout << "New Arrival ID: " << p.id << " Priority: " << p.priority << endl;

        serveNext(time);
    }

    cout << "Simulation ended" << endl;
}

void SystemController::addReport(Person p) {
    if (servedCount < 100) {
        served[servedCount] = p;
        servedCount++;
    }
}

void SystemController::sortReport() {
    sort(served, served + servedCount, [](Person a, Person b) {
        return a.waitingTime > b.waitingTime;
        });
}

void SystemController::showReport() {
    sortReport();

    cout << "Served people report" << endl;

    for (int i = 0; i < servedCount; i++) {
        cout << "ID: " << served[i].id
            << " Type: " << served[i].serviceType
            << " Waiting: " << served[i].waitingTime
            << " Service: " << served[i].serviceTime << endl;
    }
}

void SystemController::updateWeights(double u, double w, double s) {
    setSetting("urgency_weight", u);
    setSetting("waiting_weight", w);
    setSetting("service_weight", s);

    cout << "Admin Console using Hash Table" << endl;
    cout << "Weights updated in Hash Table" << endl;
    cout << "Urgency Weight: " << getSetting("urgency_weight") << endl;
    cout << "Waiting Weight: " << getSetting("waiting_weight") << endl;
    cout << "Service Weight: " << getSetting("service_weight") << endl;
}

void SystemController::loadFromFile(string fileName) {
    ifstream inputFile(fileName);

    if (!inputFile) {
        cout << "Cannot open input file" << endl;
        return;
    }

    string line;
    int count = 0;
    bool firstLine = true;

    while (getline(inputFile, line)) {
        if (line == "") {
            continue;
        }

        stringstream ss(line);

        if (firstLine) {
            int n;
            ss >> n;

            if (!ss.fail() && ss.eof()) {
                firstLine = false;
                continue;
            }
        }

        firstLine = false;

        int id;
        int urgency;
        int waitingTime;
        int serviceTime;
        string serviceType;

        stringstream personLine(line);
        personLine >> id >> urgency >> waitingTime >> serviceTime >> serviceType;

        if (personLine.fail()) {
            cout << "Wrong line format: " << line << endl;
            continue;
        }

        Person p(id, urgency, waitingTime, serviceTime, serviceType);

        addPerson(p);
        addToTypeQueue(p);
        count++;
    }

    inputFile.close();

    cout << "Total loaded people: " << count << endl;
}

void SystemController::printMainQueue() {
    mainQueue.printQueue();
}

void SystemController::serveAll() {
    int time = 1;

    while (!mainQueue.isEmpty()) {
        serveNext(time);
        time++;
    }
}