
#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>
#include <ios>
#include <queue>
#include <vector>


class Simulator
{
private:
	int total_time;  // total time to run simulation
	int clock;       // current time

	int patientArivalRate;
	int totalVisitTime;
	int totalVisits;

	vector<Doctor*> doctors;
	vector<Nurse*> nurses;


	queue<Patient*> lowSeverity;
	queue<Patient*> medSeverity;
	queue<Patient*> highSeverity;

					 

	// Remember me!
	int read_int(const std::string &prompt, int low, int high)
	{
		if (low >= high) // invalid range
			throw std::invalid_argument("invalid range specified");

		std::cin.exceptions(std::ios_base::failbit);
		int num = 0;
		while (true) {
			try {
				while (true) {
					std::cout << prompt;
					std::cin >> num;
					if (num >= low && num <= high) { // within the specified range
						std::cout << std::endl;
						return num;
					}
				}
			}
			catch (std::ios_base::failure) {
				std::cout << "Bad numeric string -- try again\n";
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<int>::max(), '\n');
			}
		}
	}

public:
	Simulator() { total_time = 0; };

	void enter_data(){
		// Get number of doctors and nurses from user
		int numNurses;
		int numDoctors = read_int("How many doctors would you like? 1 or 2? \n", 1, 2);

		if(numDoctors == 2){ numNurses = 1; }
		else { numNurses = read_int("How many nurses would you like? 1 or 2? \n", 1, 2); }

		patientArivalRate = read_int("What is the patient arrival rate in patients/hour? It must be less than 60. \n", 1, 60);


		for (int i = 0; i < numDoctors; i++) {
			doctors.push_back(new Doctor(30, 10));
		}
		for (int i = 0; i < numNurses; i++) {
			nurses.push_back(new Nurse(20, 10));
		}

		
	}

	void run_simulation(){
		// Check Arrival
		// TODO integrate map of patients and illnesses into Arrival Process
		if ((rand() % 60) < patientArivalRate) {
			int severity = rand() % 10 +1;
			if(severity > 9){
				Patient* p = new Patient("name", "illness");
				highSeverity.push(p);
			}
			else if (severity >7 && severity <10) {
				Patient* p = new Patient("name", "illness");
				medSeverity.push(p);
			}
			else{
				Patient* p = new Patient("name", "illness");
				lowSeverity.push(p);
			}
		}

		// Check Queues


		// Place new patients in Queues accordingly


		// Increment the Clock
		clock++;
	}

	void show_stats(){
		// Show final ER statistics
		
	}

	int getTotalTime() {
		return total_time;
	}


};
