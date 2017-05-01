#include <iostream>
#include <string>
#include "Patient.h"
#include "CareGiver.h"
#include "Simulator.h"

using namespace std;

int main() {
	Simulator* sim = new Simulator();
	sim->enter_data();
	sim->run_simulation();


	return 0;
}