#ifndef PATIENT_H
#define PATIENT_H

using namespace std;


class Patient {
	string illness;
	string name;
	int arrivalTime;

public:
	Patient(string n, string i, int t) {
		name = n;
		illness = i;
		arrivalTime = t;
	}
	string getIllness() {
		return illness;
	}
	string getName() {
		return name;
	}
	int getArrivalTime() {
		return arrivalTime;
	}

};

#endif
