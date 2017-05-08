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
	void setPriority(int p) { priorityNumber.push_back(p); }
	int getNumVisits() { return priorityNumber.size(); }

	
	void printPriority() {
		for (int i = 0;i < priorityNumber.size();i++) {
			cout << priorityNumber[i] / 10000 << " ";
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
