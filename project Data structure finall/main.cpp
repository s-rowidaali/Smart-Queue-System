#include <iostream>
#include <string>
#include <windows.h>
#include <commdlg.h>
#include "SystemController.h"
using namespace std;

string selectInputFile() {
    char fileName[MAX_PATH] = "";

    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = "Select input file";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileNameA(&ofn)) {
        return string(fileName);
    }

    return "";
}

int main() {
    SystemController system;

    cout << "Smart Queue Management System" << endl;
    cout << "=============================" << endl;

    string fileName = selectInputFile();

    if (fileName == "") {
        cout << "No file selected" << endl;
        return 0;
    }

    cout << endl;
    cout << "Selected file: " << fileName << endl;

    cout << endl;
    cout << "Loading data from file" << endl;
    system.loadFromFile(fileName);

    cout << endl;
    cout << "Main Queue" << endl;
    system.printMainQueue();

    cout << endl;
    cout << "Dynamic priority update" << endl;
    system.updatePersonPriority(Person(102, 3, 10, 8, "regular"), 20);

    cout << endl;
    cout << "Fairness monitor" << endl;
    system.applyFairness(Person(401, 2, 30, 10, "regular"));

    cout << endl;
    cout << "Multiple queues with merging" << endl;
    system.mergeRegularToVip();

    cout << endl;
    cout << "Admin console" << endl;
    system.updateWeights(0.4, 0.4, 0.2);
    cout << endl;

    cout << endl;
    cout << "Simulation mode" << endl;
    SystemController simulationSystem;
    simulationSystem.simulate();

    cout << endl;
    cout << "Serving people" << endl;
    system.serveAll();

    cout << endl;
    cout << "Advanced reporting" << endl;
    system.showReport();

    return 0;
}