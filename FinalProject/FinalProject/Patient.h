#ifndef PATIENT_H
#define PATIENT_H

#include <vector>
using namespace std;

class Patient {
	string illness;
	string name;
	int arrivalTime;
	int treatTime;
	vector<int> priorityNumber;
	bool sick;
	int visits;

public:
	Patient(string n) {
		name = n;
		sick = false;
		visits = 0;
	}
	Patient() { visits = 0; }
	string getIllness() {
		return illness;
	}
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

	void incrementVisits() { visits++; }
	
	int getPriority() { return priorityNumber[priorityNumber.size()-1];}
	void setPriority(int p) { priorityNumber.push_back(p); }

	
	void printPriority() {
		for (int i = 0;i < priorityNumber.size();i++) {
			cout << priorityNumber[i] / 10000 << " ";
		}
	}


	int getVisits() { return visits; }

	void setTreatTime(int t) {
		treatTime = t;
	}
	int getTreatTime() {
		return treatTime;
	}

};

#endif
