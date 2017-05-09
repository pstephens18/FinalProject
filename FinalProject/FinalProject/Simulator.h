
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
	map<int, Patient*> patientsByPriorityNumber;
	map<string, Patient*> searchMap;

	// Variables used to calculate Caregiver treatment time
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
		nurseTime = 0; 
		nurseTotal = 0; 
		doctorTime = 0; 
		doctorTotal = 0;

		priorityIterator = 0;

		nurseWaitlist = new priority_queue<int>;
		doctorWaitlist = new priority_queue<int>;

		Patients_init();
	
	};

	void enter_data() {
		
		// Get number of doctors and nurses from user
		int numNurses;
		int numDoctors = read_int("How many doctors would you like? 1 or 2? \n", 1, 2);

		if(numDoctors == 2){ numNurses = 1; }
		else { numNurses = read_int("How many nurses would you like? 1 or 2? \n", 1, 2); }

		// Get Patient Arrival Rate from user
		patientArivalRate = read_int("What is the patient arrival rate in patients/hour? It must be less than 60. \n", 1, 60);
		
		// Put doctors and nurses into their own vectors 
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

	double run_simulation(){ 
		Patient* p;
		int severity;
		int wait;
		string name;

		// Simulate for a week
		for (int i = 0; i <= 7*60*24; i++) { 

			
			// Iterate through all nurses
			for (int i = 0; i < nurses.size();i++) {

				// Make sure the nurse has a patient
				if (nurses[i]->getPatient() != NULL) {

					// Calculate the time the nurse has been with the patient
					wait = clock - nurses[i]->getPatient()->getTreatTime();

						// Decided if the nurse should be done with the patient
						if (random(6, 1) == 1 || wait == nurses[i]->getMaxWaitTime()) {
							

							// Increment total visit time
							if (clock - patientsByName[nurses[i]->getPatient()->getName()]->getArrivalTime() <= 20) {
								totalVisitTime = totalVisitTime + clock - patientsByName[nurses[i]->getPatient()->getName()]->getArrivalTime();
							}
							else { totalVisitTime += 8; }

							
							// Heal the patient, remove name from sick list and add to healthy list
							healthyNames.push_back(nurses[i]->getPatient()->getName()); 
							for (int j = 0;j < sickNames.size();j++) {
								if (sickNames[j] == nurses[i]->getPatient()->getName()) {
									sickNames.erase(sickNames.begin() + j);
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

						// Heal Patient, remove name from sick list and add to healthy list
						healthyNames.push_back(doctors[i]->getPatient()->getName());
						for (int j = 0;j < sickNames.size();j++) {
							if (sickNames[j] == doctors[i]->getPatient()->getName()) {
								sickNames.erase(sickNames.begin() + j);
								break;
							}
						}

						// Increment total visit time
						if (clock - patientsByName[doctors[i]->getPatient()->getName()]->getArrivalTime() <= 20) {
							totalVisitTime = totalVisitTime + clock - patientsByName[doctors[i]->getPatient()->getName()]->getArrivalTime();
						}
						else { totalVisitTime += 12; }
						
								
							

							// Update hospital stats
							totalVisits += 1;
							doctorTime += wait;
							doctorTotal += 1;

							// Unassign the Patient from the doctor
							doctors[i]->setPatient(NULL);
							
						}
					

				}
			}


			
			// Randomly decide if a new patient has arrived, Also consider if the entire town is at the ER
			if (random(60, 0) < patientArivalRate && healthyNames.size() > 0) {

				// Randomly decide the severity of the patient's illness
				if (random(10, 1) == 1) { severity = random(20, 16); }
				else if (random(5, 1) == 1) { severity = random(15, 11); }
				else { severity = random(10, 1); }

				
				// Select someone to get sick from the healthy list
				string key = healthyNames[random(healthyNames.size() - 1, 0)];
				p = patientsByName[key];
				
				// Make Patient Sick, add name to sick list and remove from healthy list
				sickNames.push_back(p->getName());
				
				for (int i = 0;i < healthyNames.size();i++) {
					if (healthyNames[i] == p->getName()) {
						healthyNames.erase(healthyNames.begin() + i);
						break;
					}
				}
				
				
				// Set Arrival Time
				p->setArrivalTime(clock);


				// Set Priority 
				// Every patient is initialized with priority number
				if (p->getNumVisits() != 1) {
					p->setPriority(severity * 10000 + 9999 - priorityIterator);

					// Increment the priority iterator
					priorityIterator++;
				}

				// Update Maps
				patientsByName[p->getName()] = p;
				patientsByPriorityNumber[p->getPriority()] = p; 

							
				bool newVisitor = true;
				
				// Check if the patient is a new visitor
				for (int i = visitors.size()-1;i > 0;i--) { 
					if (visitors[i]->getName() == p->getName()) { 
						newVisitor = false; 
						break;
					} 
				}
				
				// Add to visitors list if the patient is a new visitor
				if (newVisitor) { 
					visitors.push_back(p); 
				}
				
				// Put the patient's priority number into one of the queues
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
				
					// Make sure the nurse does not already have a patient
					else {
						if (nurses[i]->getPatient() == NULL) {
							
							// Set Treat time 
							patientsByPriorityNumber[priority]->setTreatTime(clock);
							
							// Update Maps
							patientsByName[patientsByPriorityNumber[priority]->getName()] = patientsByPriorityNumber[priority];
							
							// Assign Nurse to patient							
							nurses[i]->setPatient(patientsByPriorityNumber[priority]);

							// Remove patient from waitlist
							nurseWaitlist->pop(); 
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

					// Make sure the doctor does not already have a patient
						if (doctors[i]->getPatient() == NULL) {

							// Set Treat time 
							patientsByPriorityNumber[priority]->setTreatTime(clock);

							// Update Maps
							patientsByName[patientsByPriorityNumber[priority]->getName()] = patientsByPriorityNumber[priority];
							
							// Assign doctor to patient							
							doctors[i]->setPatient(patientsByPriorityNumber[priority]);

							// Remove patient from waitlist
							if (priority < 110000) { nurseWaitlist->pop(); }
							else { doctorWaitlist->pop(); 
							}
						}
					}
				} 
			
			// Increment the Clock
			clock++;
		}

		return static_cast<double>(totalVisitTime) / static_cast<double>(totalVisits);
	}

	// Show final ER statistics and let user search patients
	void show_stats(){
		
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
		cout << "Average nurse treatment time: " << static_cast<double>(nurseTime) / static_cast<double>(nurseTotal) << endl;
		cout << "Average doctor treatment time: " << static_cast<double>(doctorTime) / static_cast<double>(doctorTotal) << endl;
		
		// Average wait time is calculated from entering waitlist to end of treatment 
		cout << "Average wait time: " << static_cast<double>(totalVisitTime) / static_cast<double>(totalVisits) << endl << endl;


		// Put visitors into searchable map
		int tVisits = 0;
		for (int i = 0;i < visitors.size();i++) {
			searchMap[visitors[i]->getName()] = visitors[i];
			tVisits += visitors[i]->getNumVisits();
		}
		cout << "Visits = " << tVisits << endl;
		
	
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
			if (searchMap.find(n) != searchMap.end()) {
				p = searchMap.find(n)->second;
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

		
		
		for (int i = 0;i < names.size();i++) {
			if (random(10, 1) == 1) { severity = random(20, 16); }
			else if (random(5, 1) == 1) { severity = random(15, 11); }
			else { severity = random(10, 1); }

			// Assign an illness severity to each potential patient
			patients[i]->setPriority(severity * 10000 + 9999 - iterator);
			iterator++;

			// Initialize Maps and store patient names in a vector
			patientsByName[patients[i]->getName()] = patients[i];
			patientsByPriorityNumber[patients[i]->getPriority()] = patients[i];
			healthyNames.push_back(patients[i]->getName());
			
		}

		
	}

	

};
