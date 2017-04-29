#ifndef PATIENT_H
#define PATIENT_H

using namespace std;


class Patient {
	string illness;
	string name;
	int arrivalTime;

public:
	Patient(string n, string i) {
		name = n;
		illness = i;
	}
	string getIllness() {
		return illness;
	}
	string getName() {
		return name;
	}

};

#endif
