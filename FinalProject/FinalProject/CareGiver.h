#ifndef CAREGIVER_H
#define CAREGIVER_H

using namespace std;


class careGiver {
protected:
	int maxWaitTime;
	int minWaitTime;
	Patient* patient;
public:
	careGiver(int max, int min) {
		maxWaitTime = max;
		minWaitTime = min;
	}
	
	virtual int getMaxWaitTime() = 0;
	virtual int getMinWaitTime() = 0;
	virtual Patient* getPatient() = 0;
	virtual void setPatient(Patient* p) = 0;
};


class Nurse : public careGiver {
public:
	Nurse(int max, int min) : careGiver(max, min) { patient = NULL; };

	int getMaxWaitTime() {
		return maxWaitTime;
	}
	int getMinWaitTime() {
		return minWaitTime;
	}
	Patient* getPatient() {
		return patient;
	}
	void setPatient(Patient* p) {
		patient = p;
	}
};

class Doctor : public careGiver {
public:
	Doctor(int max, int min) : careGiver(max, min) { patient = NULL; };

	int getMaxWaitTime() {
		return maxWaitTime;
	}
	int getMinWaitTime() {
		return minWaitTime;
	}
	Patient* getPatient() {
		return patient;
	}
	void setPatient(Patient* p) {
		patient = p;
	}
};

#endif // !CAREGIVER_H

