#ifndef CAREGIVER_H
#define CAREGIVER_H

using namespace std;


class careGiver {
protected:
	int maxWaitTime;
	int minWaitTime;
public:
	careGiver(int max, int min) {
		maxWaitTime = max;
		minWaitTime = min;
	}
	
	virtual int getMaxWaitTime() = 0;
	virtual int getMinWaitTime() = 0;
};

class Receptionist : public careGiver {
public:
	Receptionist(int max, int min) : careGiver(max, min) {};

	int getMaxWaitTime() {
		return maxWaitTime;
	}
	int getMinWaitTime() {
		return minWaitTime;
	}
};

class Nurse : public careGiver {
public:
	Nurse(int max, int min) : careGiver(max, min) {};

	int getMaxWaitTime() {
		return maxWaitTime;
	}
	int getMinWaitTime() {
		return minWaitTime;
	}
};

class Doctor : public careGiver {
public:
	Doctor(int max, int min) : careGiver(max, min) {};

	int getMaxWaitTime() {
		return maxWaitTime;
	}
	int getMinWaitTime() {
		return minWaitTime;
	}
};

#endif // !CAREGIVER_H

