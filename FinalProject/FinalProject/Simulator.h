
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
	map<string, Patient*> sickPeople;
	map<string, Patient*> healthyPeople;

	int doctorTime;
	int nurseTime;

	int nurseTotal;
	int doctorTotal;

	vector<Doctor*> doctors;
	vector<Nurse*> nurses;
	vector<Patient*> patients;
	vector<Patient*> visitors;
	vector<string> sickNames;
	vector<string> healthyNames;


	priority_queue<int> *nurseWaitlist;
	priority_queue<int> *doctorWaitlist;



					 

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
		nurseWaitlist = new priority_queue<int>;
		doctorWaitlist = new priority_queue<int>;
		
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

			// Iterate through all nurses
			for (int i = 0; i < nurses.size();i++) {

				// Make sure the nurse has a patient
				if (nurses[i]->getPatient() != NULL) {

					// Calculate the time the nurse has been with the patient
					wait = clock - nurses[i]->getPatient()->getTreatTime();

						// Decided if the nurse should be done with the patient
						if (random(6, 1) == 1 || wait == nurses[i]->getMaxWaitTime()) {

							// Find Patient in vector of Patients
							for (int j = 0;j < patients.size();j++) {
								if (patients[j]->getPriority() == nurses[i]->getPatient()->getPriority()) {

									// Increment total visit time
									if (clock - patients[j]->getArrivalTime() <= 20) {
										totalVisitTime = totalVisitTime + clock - patients[j]->getArrivalTime();
									}
									else { totalVisitTime += 8; }

									// Heal the patient
									patients[j]->setSick(false);
									break;
								}
							}
							// Update hospital stats
							totalVisits += 1;
							nurseTime += wait;
							nurseTotal += 1;
							// Unassign the Patient from the nurse
							nurses[i]->setPatient(NULL);
						}
					

				}
			}

			// Iterate through all doctors
			for (int i = 0; i < doctors.size();i++) {

				// Make sure the doctor has a patient
				if (doctors[i]->getPatient() != NULL) {

					// Check how long the Patient has been with the doctor
					wait = (clock - doctors[i]->getPatient()->getTreatTime());

						// Decide if the Patient can leave
						if (random(12, 1) == 1 || wait == doctors[i]->getMaxWaitTime()) {

							// Find the patient in the vector of Patients
							for (int j = 0;j < patients.size();j++) {
								if (patients[j]->getPriority() == doctors[i]->getPatient()->getPriority()) {

									// Heal Patient
									patients[j]->setSick(false);

									// Increment total visit time
									if (clock - patients[j]->getArrivalTime() <= 20) {
										totalVisitTime = totalVisitTime + clock - patients[j]->getArrivalTime();
									}
									else { totalVisitTime += 12; }
									break;
								}
							}

							// Update hospital stats
							totalVisits += 1;
							doctorTime += wait;
							doctorTotal += 1;

							// Unassign the Patient from the doctor
							doctors[i]->setPatient(NULL);
							
						}
					

				}
			}


			
			// Randomly decide if a new patient has arrived, ALso consider if the entire town is at the ER
			if (random(60, 0) < patientArivalRate && (doctorWaitlist->size() + nurseWaitlist->size() + nurses.size() + doctors.size()) != patients.size()) {

				// Randomly decide the severity of the patient's illness
				if (random(10, 1) == 1) { severity = random(20, 16); }
				else if (random(5, 1) == 1) { severity = random(15, 11); }
				else { severity = random(10, 1); }

				
				// Select someone to get sick
				int index;
				bool con = true;

				// if more than 90% of people are already sick, iterate through potential patients
				if ((patients.size() * 9) / 10 < (nurseWaitlist->size()+doctorWaitlist->size())) {
					for (int i = 0;i < patients.size();i++) {
						if (!(patients[i]->getSick())) {

							// Make Patient Sick
							patients[i]->setSick(true);

							// Set Arrival Time
							patients[i]->setArrivalTime(clock);


							// Set Priority 
							// Every patient is initialized with priority number
							if (patients[i]->getNumVisits() != 1) {
								patients[i]->setPriority(severity * 10000 + 9999 - priorityIterator);

								// Increment the priority iterator
								priorityIterator++;
							}
							// Store patient
							p = patients[i];

							break;
						}

					}
				}
				else {
					do {
						index = random(patients.size() - 1, 0);

						// Make sure the patient is not sick already
						if (!(patients[index]->getSick())) {

							// Make Patient Sick
							patients[index]->setSick(true);

							// Set Arrival Time
							patients[index]->setArrivalTime(clock);


							// Set Priority 
							// Every Patient is initialized with a priority number
							if (patients[index]->getNumVisits() != 1) {
								patients[index]->setPriority(severity * 10000 + 9999 - priorityIterator);

								// Increment the priority iterator
								priorityIterator++;
							}
							// Store patient
							p = patients[index];



							// Continue
							con = false;
						}
					} while (con);
				}


				bool newVisitor = true;
				
				// Check if the patient is a new visitor
				for (int i = visitors.size()-1;i > 0;i--) { 
					if (visitors[i]->getName() == p->getName()) { 
						newVisitor = false; 
						break;
					} 
				}
				
				// Add to list if the patient is a new visitor
				if (newVisitor) { 
					visitors.push_back(p); 
				}
				
				// Put the patient's priority number into the queue
				if (p->getPriority() > 110000) { doctorWaitlist->push(p->getPriority()); }
				else { nurseWaitlist->push(p->getPriority()); }
			
			}
			
			
			// Check Nurse  Availability
			int priority; 
			for (int i = 0; i < nurses.size();i++) {

				// Make Sure waitlist is not empty
				if (!nurseWaitlist->empty()) {

					// Access top of waitlist
					priority = nurseWaitlist->top();
					
					// Make sure the patient can be treated by a nurse
					if (priority >= 110000) { continue; }
				
					// Make sure the nurse is not busy
					else {
						if (nurses[i]->getPatient() == NULL) {
							
							// Find and assign the patient to the nurse
							for (int j = patients.size()-1;j >= 0; j--) {
								if (patients[j]->getPriority() == priority) {
									patients[j]->setTreatTime(clock);
									nurses[i]->setPatient(patients[j]);
									nurseWaitlist->pop();
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
				if (!doctorWaitlist->empty() || !nurseWaitlist->empty()) {

					// Access top of waitlist
					if (!doctorWaitlist->empty()) { priority = doctorWaitlist->top(); }
					else { priority = nurseWaitlist->top(); }

					// Make sure the doctor is not busy
						if (doctors[i]->getPatient() == NULL) {
							
							// Find and assign the patient to the doctor
							for (int j = patients.size() - 1;j >= 0; j--) {
								if (patients[j]->getPriority() == priority) {
									patients[j]->setTreatTime(clock);
									doctors[i]->setPatient(patients[j]);
									if (priority > 110000) { doctorWaitlist->pop(); }
									else { nurseWaitlist->pop(); }
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

		// Number of people waiting with an illness severity greater than 10
		cout << "Doctor Queue:" <<  doctorWaitlist->size() << endl;

		// Number of people waiting with an illness severity less than 11
		cout << "Nurse Queue:" << nurseWaitlist->size() << endl;

		cout << "Total patients treated: " << totalVisits << endl;
		cout << "Individual patients treated: " << visitors.size() << endl;

		// 
		cout << "Total wait time: " << totalVisitTime << endl;
		cout << "Total Hours " << clock / 60 << endl;

		// These times are from start of treatment to end of treatment
		cout << "Average nurse wait time: " << static_cast<double>(nurseTime) / static_cast<double>(nurseTotal) << endl;
		cout << "Average doctor wait time: " << static_cast<double>(doctorTime) / static_cast<double>(doctorTotal) << endl;
		
		// Average wait time is calculated from entering waitlist to end of treatment 
		cout << "Average wait time: " << static_cast<double>(totalVisitTime) / static_cast<double>(totalVisits) << endl << endl;

		// Put Patients into map
		for (int i = 0;i < visitors.size();i++) {
			patientsByName[visitors[i]->getName()] = visitors[i];
		}
		
		
		//cout << "Some patients: " << visitors[0]->getName() << " " << visitors[1]->getName() << " " << visitors[2]->getName() << endl << endl;
		string n;
		Patient* p;

		// Search Map of Patients by Patient name
		for(int i = 0;i<100;i++){
			cout << "Enter a name to view records or enter 'L' to list all patients treated or 'done' to quit: \n";
			cin >> n;
			cout << endl;

			// List Patients
			if (n == "L" || n == "l") {
				for (int i = 0;i < visitors.size();i++) {
					cout << visitors[i]->getName() << " ";
				}
				cout << endl << endl;
			}

			// Check if the user is done
			if (n == "done") { break; }

			// Find patient if they exist
			if (patientsByName.find(n) != patientsByName.end()) {
				p = patientsByName.find(n)->second;
				cout << "	" << p->getName() << " visited " << p->getNumVisits() << " time(s) with severity level(s) "; p->printPriority(); cout << endl << endl;
			}
		}
		
	}

	

	void Patients_init() {
		string name ="";
		vector<string> names;
		int severity;
		int iterator = 0;
		Patient* p;

		// Read in Names from text file and store in vector
		ifstream s("Names.txt");
		while (std::getline(s,name)) {
			names.push_back(name);
		}
		s.close();
		
		// Assign Names to Patients and store patients in vector
		for (int i = 0;i < names.size();i++) {
			patients.push_back(new Patient(names[i]));
		}

		
		// Assign a severity to each potential patient
		for (int i = 0;i < names.size();i++) {
			if (random(10, 1) == 1) { severity = random(20, 16); }
			else if (random(5, 1) == 1) { severity = random(15, 11); }
			else { severity = random(10, 1); }

			patients[i]->setPriority(severity * 10000 + 9999 - iterator);
			iterator++;

			healthyPeople[patients[i]->getName()] = patients[i];
			healthyNames.push_back(patients[i]->getName());
			
		}

		
	}

	

};
