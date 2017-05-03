#ifndef PATIENT_H
#define PATIENT_H

using namespace std;


class Patient {
	string illness;
	string name;
	int arrivalTime;
	int priorityNumber;
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
	
	int getPriority() { return priorityNumber;}
	void setPriority(int p) { priorityNumber = p; }

	int getVisits() { return visits; }

};

#endif
