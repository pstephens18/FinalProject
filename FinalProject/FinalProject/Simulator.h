
#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>
#include <ios>
#include <queue>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <map>
#include <fstream>
using namespace std;


class Simulator
{
private:
	int total_time;  // total time to run simulation
	int clock;       // current time

	int patientArivalRate;
	int totalVisitTime;
	int totalVisits;

	int priorityIterator;

	
	map<string, Patient*> patientsByName;

	int doctorTime;
	int nurseTime;

	int nurseTotal;
	int doctorTotal;

	vector<Doctor*> doctors;
	vector<Nurse*> nurses;
	vector<Patient*> patients;
	vector<Patient*> visitors;


	priority_queue<int> *waitlist;



					 

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
		waitlist = new priority_queue<int>;
		
		Patients_init();
		priorityIterator = 0;
	
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
		Patient* p;
		int severity;
		int wait;
		string name;
		string illness;
		for (int i = 0; i <= sim_time; i++) {

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


			// TODO integrate map of patients and illnesses into Arrival Process
			
			// Randomly decide if a new patient has arrived 
			if (random(60, 0) < patientArivalRate) {
				// Randomly decide the severity of the patient's illness
				if (random(10, 1) == 1) { severity = random(20, 16); }
				else if (random(5, 1) == 1) { severity = random(15, 11); }
				else { severity = random(10, 1); }

				
				// Select someone to get sick
				int index;
				bool con = true;
				int size = patients.size();
				
				do {
					index = random(size-1,0);

					// Make sure the patient is not sick already
					if (!(patients[index]->getSick())) {

						// Make Patient Sick
						patients[index]->setSick(true);

						// Set Arrival Time
						patients[index]->setArrivalTime(clock);


						// Set Priority 
						p = patients[index];
						p->setPriority(severity*1000 + 999 - priorityIterator);
						priorityIterator++;
						con = false;
					}
				} while (con); 

				bool visited_before = false;

				// Check to see if the patient has visited before
				for (int i = 0;i < visitors.size();i++) { 
					if (visitors[i]->getName() == p->getName()) { 
						// Increment visits
						visitors[i]->incrementVisits();
						visited_before == true; 
					} 
				}

				// Add to list if they have not visited before
				if (!visited_before) { 
					p->incrementVisits();
					visitors.push_back(p); 
				}
				
				// Put the patient's priority number into the queue
				waitlist->push(p->getPriority());
			
			}

			
			// Check Nurse  Availability
			int priority; 
			for (int i = 0; i < nurses.size();i++) {

				// Make Sure waitlist is not empty
				if (!waitlist->empty()) {

					// Access top of waitlist
					priority = waitlist->top();
					
					// Make sure the patient can be treated by a nurse
					if (priority >= 11000) { continue; }
				
					// Make sure the nurse is not busy
					else {
						if (nurses[i]->getPatient() == NULL) {
							
							// Find and assign the patient to the nurse
							for (int j = visitors.size()-1;j >= 0; j--) {
								if (visitors[j]->getPriority() == priority) {
									nurses[i]->setPatient(visitors[j]);
									waitlist->pop();
									break;
								}
							}
						}
					}
				}
			}
		
			
			// Check Doctor Availability
			for (int i = 0; i < doctors.size();i++) {

				// Make Sure waitlist is not empty
				if (!waitlist->empty()) {

					// Access top of waitlist
					priority = waitlist->top();

					// Make sure the nurse is not busy
						if (doctors[i]->getPatient() == NULL) {
							
							// Find and assign the patient to the nurse
							for (int j = visitors.size() - 1;j >= 0; j--) {
								if (visitors[j]->getPriority() == priority) {
									doctors[i]->setPatient(visitors[j]);
									waitlist->pop();
									break;
								}
							}
						}
				}
			}
			
			// Increment the Clock
			clock++;
		}
	}

	void show_stats(){
		// Show final ER statistics
		cout << "Patients in Queue:" << waitlist->size() << endl;
		cout << "Total people treated: " << totalVisits << endl;
		cout << "Total wait time: " << totalVisitTime << endl;
		cout << "Total Hours " << clock / 60 << endl;

		cout << "Average nurse wait time: " << static_cast<double>(nurseTime) / static_cast<double>(nurseTotal) << endl;
		cout << "Average doctor wait time: " << static_cast<double>(doctorTime) / static_cast<double>(doctorTotal) << endl;
		cout << "Average wait time: " << static_cast<double>(totalVisitTime) / static_cast<double>(totalVisits) << endl << endl;

		// Put Patients into map
		for (int i = 0;i < visitors.size();i++) {
			patientsByName[visitors[i]->getName()] = visitors[i];
		}
		
		
		cout << "Some patients: " << visitors[0]->getName() << " " << visitors[1]->getName() << " " << visitors[2]->getName() << endl << endl;
		string n;
		Patient* p;

		// Search Map of Patients by Patient name
		for(int i = 0;i<1000;i++){
			cout << "Enter a name: ";
			cin >> n;
			if (n == "done") { continue; }
			if (patientsByName.find(n) != patientsByName.end()) {
				p = patientsByName.find(n)->second;
				cout << p->getName() << " visited " << p->getVisits() << " time(s) and the severity was " << p->getPriority() / 1000 << endl << endl;
			}
		}
		
	}

	

	void Patients_init() {
		string name ="";
		string illness ="";
		vector<string> names;
		vector<string> illnesses;

		// Read in Names from text file and store in vector
		ifstream s("Names.txt");
		while (std::getline(s,name)) {
			names.push_back(name);
		}
		s.close();

		// Read in Illnesses from text file and store in vector
		ifstream t("Illnesses.txt");
		while (std::getline(t, illness)) {
			illnesses.push_back(illness);
		}
		t.close();
		
		// Assign Names to Patients
		for (int i = 0;i < names.size();i++) {
			patients.push_back(new Patient(names[i]));
		}
		
	}

	

};
