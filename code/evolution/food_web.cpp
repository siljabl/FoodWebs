#include "food_web.h"
#include <iostream>
#include <fstream>
#include <algorithm> 
using namespace std;


// ------------------------------------------------------------------------------------------// 
//				FUNCTIONS FOR MANIPULATING FOODWEB  	                     //
// ------------------------------------------------------------------------------------------//  



//	INTERACTIONS
	bool addLink(Species S[], Producer P[], int res, int cons) {
		// preventing absurd links
		if (res == cons) {
			//cout << "Error: Species cannot consume itself!" << endl << endl;
			return false;
		}
		else if (S[cons].isProducer) {
			//cout << "Error: consumer is a Producer!\n" << endl << endl;
			return false;
		}
		else if (S[res].consumers[cons] != 0 && S[cons].resources[res] != 0 || S[res].resources[cons] != 0 && S[cons].consumers[res] != 0) {
			//cout << "Error: there is already a link between these Species!" << endl << endl;
			return false;
		}

		else {
			// initializing interaction
			double consumptionDecay = eta();
			double consumptionGrowth = consumptionDecay * beta();

			S[res].consumers[cons] = consumptionDecay;
			S[cons].resources[res] = consumptionGrowth;

			// transmitting links to producer array
			if (S[res].isProducer) {
				P[res].consumers[cons] = S[res].consumers[cons];
			}
			
			//cout << "adding link between " << cons << " and " << res << endl; 
			return true;
		}
	}


	void removeLinks(Species S[], Producer P[], int index) {
		for (int i=0; i < nMAX; i++) {
			// checking for errendous links
			if (S[i].consumers[index]*S[index].resources[i] == 0 && S[i].consumers[index]+S[index].resources[i] != 0) {
				cout << "Error: mismatched link between " << index << " and " << i << endl;
			}
			else if (S[index].consumers[i]*S[i].resources[index] == 0 && S[index].consumers[i]+S[i].resources[index] != 0) {
				cout << "Error: mismatched link between " << i << " and " << index << endl;
			}

			else {
				// remving all consumers
				S[index].consumers[i] = 0;
				P[index].consumers[i] = 0;
				S[i].resources[index] = 0;

				// removing resources 
				S[i].consumers[index] = 0;
				P[i].consumers[index] = 0;
				S[index].resources[i] = 0;
			}
		}
	}


	void updateTrophicLevel(Species S[]) {
		// UPDATING LEVELS OF PRODUCERS
		// counting species of known level
		int counter = Producer::nProducer;

		// array for keeping track of the number of levels that is yet to be computed
		bool levelUnknown[nMAX] = {};
		std::fill_n(levelUnknown + counter, Species::nTotal - counter, 1);
	
		// UPDATING LEVEL OF OTHER SPECIES
		while (counter < Species::nTotal) {
			
			for (int i = Producer::nProducer; i < Species::nTotal; i++) {
				
				// if level of species is yet to be computed
				if ( levelUnknown[i] ) {
					// resetting sums to zero
					double sum_eta_l = 0;
					double sum_eta = 0;
					int resources_of_unknown_level = 0;

					// Checking if species is consuming species of unknown levels
					for (int j = 0; j < Species::nTotal; j++) {
						if (levelUnknown[j]*S[i].resources[j] > 0) {
							resources_of_unknown_level++;
							break;
						}
					}
					
					// computing level if already computed level of all resources
					if (resources_of_unknown_level == 0) {
						// updating sums
						for (int j = 0; j < Species::nTotal; j++) {
							if (S[i].resources[j] > 0) {
								sum_eta_l += S[j].consumers[i] * S[j].level;
								sum_eta += S[j].consumers[i];
							}
						}

						// computing level
						S[i].level = 1 + sum_eta_l / sum_eta;

						// updating counters
						levelUnknown[i] = false;
						counter++;
					}
				}
			}
		}
		// running loop until all species' levels have been calculated
	}



//	ADDING AND REMOVING Species
	int freeIndex(Species S[], Producer P[], bool isProducer) {
		// checking if array is full
		if (Species::nTotal == nMAX){
			cout << "Error: no free indices/array is too small!" << endl << endl;
			return -1;
		}

		// if new species is a producer, and not all species are producers
		else if (isProducer && Producer::nProducer != Species::nTotal) {
				int newIndex = Producer::nProducer;
				// index of new species

				S[Species::nTotal] = S[newIndex];
				// moving species with index nProducer to index n

				// updating links of species n
				for (int j = 0; j < Species::nTotal; j++) {
					// updating consumers
					if (S[newIndex].consumers[j] != 0) {
						S[j].resources[Species::nTotal] = S[j].resources[newIndex];
						S[j].resources[newIndex] = 0;
					}

					// updating resources
					else if (S[newIndex].resources[j] != 0) {
						S[j].consumers[Species::nTotal] = S[j].consumers[newIndex];
						S[j].consumers[newIndex] = 0;

						// transmiting links to producer array if resource is a isProducer producer
						if (S[j].level == 1) {
							P[j].consumers[Species::nTotal] = P[j].consumers[newIndex];
							P[j].consumers[newIndex] = 0;
						}
					}
				}

				return newIndex;
		}

		// if new species is not a producer, or all species are producers
		else {
			return Species::nTotal;
		}
	}


	void addSpecies(Species S[], Producer P[], int addAttempt) {
		// Determining type and index of invasive species
		bool isProducer = type();
		int index = freeIndex(S, P, isProducer);

		// new species is a producer
		if (isProducer){
			// declaring Species and putting it in arrays
			Producer s(addAttempt);
			P[index] = s;
			S[index] = s;
	
			// printing paramters of new producer	
			P[index].printParameters(index);
		}

		// new species is not a producer
		else {
			// declaring species and putting it in array
			Species s(addAttempt);
			S[index] = s;
			P[index].isProducer = 0;

			// adding resource
			bool link = 0;
			int resource1 = index;
			int resource2 = index;
			while (!link) {
				resource1 = randomInt(0, Species::nTotal);
				link = addLink(S, P, resource1, index);
			}

			// adding second resource if criteria fulfilled
			link = 0;
			if (Species::nTotal > nMin && addSecondResource()) {
				while (!link){
					resource2 = randomInt(0, Species::nTotal);
					link = addLink(S, P, resource2, index);
				}

				// avoiding omnivory
				if (!omnivorous && S[resource2].level != S[resource1].level) {
					S[resource2].consumers[index] = 0;
					P[resource2].consumers[index] = 0;
					S[index].resources[resource2] = 0;
				}
			}
				
			// printing Species parameters
			S[index].printParameters(index);
		}
	}


	void removeSpecies(Species S[], Producer P[], int index) {
		// if species at index "index" doesn't exist
		if (index >= Species::nTotal) {
			cout << "Error: this Species does not exist" << endl << endl;
		}

		else{
		//	SETTING ALL INTERACTIONS TO ZERO
			removeLinks(S, P, index);

		//	UPDATING NUMBER OF Species
			Species::nTotal--;
			if (S[index].isProducer) { Producer::nProducer--; }

		//	SETTING ALL PARAMETERS TO ZERO
			P[index].growth = 0;
			P[index].decay = S[index].decay = 0;
			P[index].density = S[index].density = 0;
			P[index].derivative = S[index].derivative = 0;
			P[index].level = S[index].level = -1;

		//	UPDATING INDICES OF OTHER Species
			// producers
			if (index < Producer::nProducer) {
				// moving last producer to empty index
				S[index] = S[Producer::nProducer];
				P[index] = P[Producer::nProducer];
				cout << "Species " << Producer::nProducer << " replaced Species " << index << " in the array.\n";

				// moving links of consumers
				for (int j = 0; j <= Species::nTotal; j++) {
					if (S[Producer::nProducer].consumers[j] != 0) {
						S[j].resources[index] = S[j].resources[Producer::nProducer];
						S[j].resources[Producer::nProducer] = 0;
					}
				}
				index = Producer::nProducer;
			}
			
			// other species
			if (index < Species::nTotal) {
				// moving last species to empty index
				S[index] = S[Species::nTotal];
				cout << "Species " << Species::nTotal << " replaced Species " << index << " in the array.\n";

				// moving links of resources and consumers
				for (int j = 0; j < Species::nTotal; j++) {
					// moving consumers
					if (S[Species::nTotal].consumers[j] != 0) {
						S[j].resources[index] = S[j].resources[Species::nTotal];
						S[j].resources[Species::nTotal] = 0;
					}

					// moving resources
					else if (S[Species::nTotal].resources[j] != 0) {
						S[j].consumers[index] = S[j].consumers[Species::nTotal];
						S[j].consumers[Species::nTotal] = 0;

						// transmitting links to producer array if resource is a producer
						if (S[j].isProducer) {
							P[j].consumers[index] = S[j].consumers[index];
							P[j].consumers[Species::nTotal] = 0;
						}
					}
				}
			}

		//	REMOVING LAST ARRAY ELEMENTS
			// using default constructors to remove Species at last active indices without affecting counters
			Species s;
			Producer p;
			P[index] = p;
			S[Species::nTotal] = s;
			P[Species::nTotal] = p;
		}
		// if Species does exist

		//for (int i = 0; i < Species::nTotal; i++)
		//{
		//	S[i].printParameters(i);
		//}
	}
	


//	COMPUTING DERIVATIVES
	double availableNutrients(Producer P[]) {
		// initial amount of nutrients
		double sum = 1;

		// removing amount of nutrients consumed by the producers
		for (int i = 0; i < Producer::nProducer; i++) {
			sum -= P[i].density;
		}

		return sum;
	}



	double strengthen(Species S[], int index) {
		double sum = 0;
		// computing how much species is strengthed when consuming its resources
		for (int j = 0; j < Species::nTotal; j++) {
			if (responseType == 1) {
				sum += S[index].resources[j] * S[j].density; // type-I
			}
			else if (responseType == 2) {
				sum += ((double) S[index].resources[j] * S[j].density / (1 + h * S[j].consumers[index] * S[j].density)); // type-II
			}
			else {
				cout << "Error in responseType" << endl;
			}
		}

		return sum;
	}



	double weaken(Species S[], int index) {
		double sum = 0;
		// computing how much species is hurt when consumed by its consumers
		for (int j = 0; j < Species::nTotal; j++) {
			if (responseType == 1) {
				sum += S[index].consumers[j] * S[j].density; // type-I
			}
			else if (responseType == 2) {			
				sum += ((double) S[index].consumers[j] * S[j].density) / (1 + h * S[index].consumers[j] * S[index].density); // type-II
			}
			else {
				cout << "Error in responseType" << endl;
			}
		}

		return sum;
	}



	void computeDerivatives(Species S[], Producer P[]) {
		double nutrients = availableNutrients(P);

		// computing derivatives of producers
		for (int i = 0; i < Producer::nProducer; i++) {
			P[i].computeDerivative(nutrients, weaken(S, i));
			S[i].derivative = P[i].derivative;
		}

		// computing derivatives of other species
		for (int i = Producer::nProducer; i < Species::nTotal; i++) {
			S[i].computeDerivative(strengthen(S, i), weaken(S, i));
		}
	}


