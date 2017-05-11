#include <iostream>
#include <string>
#include "Patient.h"
#include "CareGiver.h"
#include "Simulator.h"

using namespace std;

int main() {
	srand(time(NULL));

	Simulator* sim = new Simulator();
	sim->enter_data();
	sim->run_simulation();
	sim->show_stats();
	delete sim;

	// Test code to determine wait time at various patient arrival rates
	/*
	double t;
	ofstream f;
	f.open("Rates.txt");
	for (int i = 7;i < 12;i++) {
		for (int j = 0;j < 3;j++) {
			sim = new Simulator();
			sim->enter_data(i * 5);
			t = sim->run_simulation();
			if (f.is_open()) {
				f << i * 5 << "\t" << t << endl;
			}
			cout << i * 5 << endl;
		}
	}
	sim->show_stats();

	delete sim;

*/

}