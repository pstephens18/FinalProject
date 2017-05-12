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


}