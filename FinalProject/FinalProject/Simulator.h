
#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>
#include <ios>
#include <queue>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;


class Simulator
{
private:
	int total_time;  // total time to run simulation
	int clock;       // current time

	int patientArivalRate;
	int totalVisitTime;
	int totalVisits;

	int doctorTime;
	int nurseTime;

	int nurseTotal;
	int doctorTotal;

	vector<Doctor*> doctors;
	vector<Nurse*> nurses;


	queue<Patient*> *lowSeverity;
	queue<Patient*> *medSeverity;
	queue<Patient*> *highSeverity;

					 

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
	Simulator() { 
		total_time = 0;
		clock = 0;
		totalVisitTime = 0;
		totalVisits = 0;
		nurseTime = 0; nurseTotal = 0; doctorTime = 0; doctorTotal = 0;
		lowSeverity = new queue<Patient*>;
		medSeverity = new queue<Patient*>;
		highSeverity = new queue<Patient*>;
	
	};

	void enter_data(){
		// Get number of doctors and nurses from user
		int numNurses;
		int numDoctors = read_int("How many doctors would you like? 1 or 2? \n", 1, 2);

		if(numDoctors == 2){ numNurses = 1; }
		else { numNurses = read_int("How many nurses would you like? 1 or 2? \n", 1, 2); }

		patientArivalRate = read_int("What is the patient arrival rate in patients/hour? It must be less than 60. \n", 1, 60);


		for (int i = 0; i < numDoctors; i++) {
			doctors.push_back(new Doctor(20, 1));
		}
		for (int i = 0; i < numNurses; i++) {
			nurses.push_back(new Nurse(10, 1));
		}

		
	}

	// Generates random number between two int values
	int random(int max, int min) {
		return ((rand() % (max - min + 1)) + min);
	}

	void run_simulation(int sim_time){
		for (int i = 0; i <= sim_time; i++) {
			Patient* p;
			int severity;
			int wait;

			// Check if a Nurse is done with their patient and adjust stats accordingly
			for (int i = 0; i < nurses.size();i++) {
				if (nurses[i]->getPatient() != NULL) {
					wait = clock - nurses[i]->getPatient()->getArrivalTime();
					if (wait >= nurses[i]->getMinWaitTime()) {
						if (random(10, 1) == 1 || wait >= nurses[i]->getMaxWaitTime()) {
							// Add more stuff to save Patients information
							totalVisits += 1;
							totalVisitTime += wait;
							nurseTime += wait;
							nurseTotal += 1;
							nurses[i]->setPatient(NULL);
						}
					}

				}
			}

			// Check if a Doctor is done with their patient and adjust stats accordingly
			for (int i = 0; i < doctors.size();i++) {
				if (doctors[i]->getPatient() != NULL) {
					wait = clock - doctors[i]->getPatient()->getArrivalTime();
					if (wait >= doctors[i]->getMinWaitTime()) {
						if (random(20, 1) == 1 || wait >= doctors[i]->getMaxWaitTime()) {
							// Add more stuff to save Patients information
							totalVisits += 1;
							totalVisitTime += wait;
							doctorTime += wait;
							doctorTotal += 1;
							doctors[i]->setPatient(NULL);
						}
					}

				}
			}




			// Check Arrival

			// TODO integrate map of patients and illnesses into Arrival Process

			// Randomly decide if a new patient has arrived
			if (random(60, 1) < patientArivalRate) {
				// Randomly decide the severity of the patient's illness
				severity = random(10, 1);
				// Create a patient with the determined severity of illness and put them in the correct queue
				if (severity > 9) {
					p = new Patient("name", "illness", clock);
					highSeverity->push(p);
				}
				else if (severity > 7 && severity < 10) {
					p = new Patient("name", "illness", clock);
					medSeverity->push(p);
				}
				else {
					p = new Patient("name", "illness", clock);
					lowSeverity->push(p);
				}
			}

			// Check Nurse  Availability
			for (int i = 0; i < nurses.size();i++) {
				// Check Medium Severity Queue
				if (!medSeverity->empty()) {
					p = medSeverity->front();
					severity = 2;
				}
				// Check Low Severity Queue
				else if (!lowSeverity->empty()) {
					p = lowSeverity->front();
					severity = 1;
				}
				else { continue; }
				// Assign Patient to Nurse if Nurse is free, then Remove Patient from queue 
				if (nurses[i]->getPatient() == NULL) {
					nurses[i]->setPatient(p);
					if (severity == 2) { medSeverity->pop(); }
					else { lowSeverity->pop(); }
				}
			}

			// Check Doctor  Availability
			for (int i = 0; i < doctors.size();i++) {
				//Check High Severity Queue
				if (!highSeverity->empty()) {
					p = highSeverity->front();
					severity = 3;
				}
				// Check Medium Severity Queue
				if (!medSeverity->empty()) {
					p = medSeverity->front();
					severity = 2;
				}
				// Check Low Severity Queue
				else if (!lowSeverity->empty()) {
					p = lowSeverity->front();
					severity = 1;
				}
				else { continue; }
				// Assign Patient to Doctor if Doctor is free and remove them from the queue
				if (doctors[i]->getPatient() == NULL) {
					doctors[i]->setPatient(p);
					if (severity == 3) { highSeverity->pop(); }
					else if (severity == 2) { medSeverity->pop(); }
					else { lowSeverity->pop(); }
				}
			}






			// Increment the Clock
			clock++;
		}
	}

	void show_stats(){
		// Show final ER statistics
		cout << "People in Queues:" << endl << "Low:" << lowSeverity->size() << " Med:" << medSeverity->size() << " High:" << highSeverity->size() << endl;
		cout << "Total people treated: " << totalVisits << endl;
		cout << "Total wait time: " << totalVisitTime << endl;
		cout << "Total Hours " << clock / 60 << endl;

		cout << "Average nurse wait time: " << nurseTime / nurseTotal << endl;
		cout << "Average doctor wait time: " << doctorTime / doctorTotal << endl;
		

		
	}



};
