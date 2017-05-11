#ifndef PATIENT_H
#define PATIENT_H

#include <vector>
using namespace std;

class Patient {
	string name;
	int arrivalTime;
	int treatTime;
	vector<int> priorityNumber;
	bool sick;

public:
	Patient(string n) {
		name = n;
		sick = false;
		priorityNumber.push_back(0);
	}
	Patient() {};

	string getName() {
		return name;
	}
	int getArrivalTime() {
		return arrivalTime;
	}
	void setArrivalTime(int a) {
		arrivalTime = a;
	}
	bool getSick() { return sick; }
	void setSick(bool b) { sick = b; }

	
	int getPriority() { return priorityNumber[priorityNumber.size()-1];}
	void setPriority(int p) { 
		if (priorityNumber[0] = 0) { priorityNumber[0] = p; }
		else { priorityNumber.push_back(p); }
	}

	int getNumVisits() { return priorityNumber.size() -1; }

	
	void printPriority() {
		for (int i = 0;i < priorityNumber.size();i++) {
			if (priorityNumber[i] != 0) {
				cout << priorityNumber[i] / 10000 << " ";
			}
		}
	}


	void setTreatTime(int t) {
		treatTime = t;
	}
	int getTreatTime() {
		return treatTime;
	}

};

#endif
