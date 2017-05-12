#ifndef PATIENT_H
#define PATIENT_H

#include <vector>
using namespace std;

// Patient Class
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

	// Returns Patient's name
	string getName() {
		return name;
	}

	// Used to calculate total time spent in the ER
	int getArrivalTime() {
		return arrivalTime;
	}
	void setArrivalTime(int a) {
		arrivalTime = a;
	}

	// Sets the Patient's status to sick
	bool getSick() { return sick; }
	void setSick(bool b) { sick = b; }

	// Returns most recent priority number
	int getPriority() { return priorityNumber[priorityNumber.size()-1];}
	void setPriority(int p) { 
		if (priorityNumber[0] = 0) { priorityNumber[0] = p; }
		else { priorityNumber.push_back(p); }
	}

	// Returns the number of Priority Numbers
	int getNumVisits() { return priorityNumber.size() -1; }

	// Prints all the Prioity Numbers the patient was assigned
	void printPriority() {
		for (int i = 0;i < priorityNumber.size();i++) {
			if (priorityNumber[i] != 0) {
				cout << priorityNumber[i] / 10000 << " ";
			}
		}
	}

	// Used to check how long Nurses and Doctors take to treat patients
	void setTreatTime(int t) {
		treatTime = t;
	}
	int getTreatTime() {
		return treatTime;
	}

};

#endif
