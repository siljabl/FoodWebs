#include "evolution/parameters.cpp"
#include "evolution/food_web.cpp"
#include "evolution/time_series.cpp"
#include "evolution/stability_analysis.cpp"
#include "evolution/species.cpp"
#include <iostream>
#include <sstream>
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end

string folder = "new_data";

// initializing Species numbers
int Species::nTotal = 0;
int Producer::nProducer = 0;

// default stability and feasibility of food web
bool FoodWeb::feasible = 0;
bool FoodWeb::stable = 0;

// default behavior of last iteration
int FoodWeb::prevIteration = 0;
int FoodWeb::prevExtinct = 0;

// max number of addition attempts
double addAttemptMAX = 1e5;


int main() {
	// random seed
	srand(10);


//	FILES
	// data is saved as: iteration - number of species - real - imaginary
	ofstream stabEigen("../data/"+folder+"/eig_stab.txt");			// eigenvalues of linearly stable food web
	ofstream ustabEigen("../data/"+folder+"/eig_unstab.txt");		// eigenvalues of linearly unstable food web

	// data is saved as:
	// iteration - number of Species - feasibility - stability - convergence
	// 1 - converged, 2 - decreasing amplitude, 9 - not converged
	ofstream webData("../data/"+folder+"/stab_data.txt");			// behavior of food web

	// data is saved as:
	// iteration - l - S(0) - k - a - consumer - eta
	// iteration - l - S(0) - a - consumer/resource - eta - beta*eta	
	ofstream sFile("../data/"+folder+"/species_param.txt");			// Species parameters
	ofstream pFile("../data/"+folder+"/producer_param.txt");			// Producer parameters
	
	

//	DECLARING ARRAYS
	double steadyStates[nMAX] = {};
	Species S[nMAX];
	Producer P[nMAX];


//	ADDING FIRST Species
	// declaring Species and updating arrays
	Producer s(0);
	P[0] = s;
	S[0] = s;

	// running food web
	checkFeasibility(S, P, steadyStates, stabEigen, ustabEigen, 0);
	timeSeries(S, P, steadyStates, stabEigen, ustabEigen, webData, 0);
	saveParameters(S, P, sFile, pFile, 0);


//	RUNNING EVOLUTION
	for (int addAttempt = 1; addAttempt < addAttemptMAX; addAttempt++) {
		// printing addition attempt
		cout << "\n\n---------------------------\n";
		cout << " " << addAttempt << ". ADDITION ATTEMPT\n";
		cout << "---------------------------\n\n";
		
		// adding invasive species
		addSpecies(S, P, addAttempt);
		updateTrophicLevel(S);
		saveParameters(S, P, sFile, pFile, addAttempt);

		// printing number of species
		cout << "Number of Species in food web " << Species::nTotal;
		cout << ", number of Producers " << Producer::nProducer << endl << endl;

		// running time series
		checkFeasibility(S, P, steadyStates, stabEigen, ustabEigen, addAttempt);
		timeSeries(S, P, steadyStates, stabEigen, ustabEigen, webData, addAttempt);

		if (Species::nTotal == 0) {
			Producer s(addAttempt);
			P[0] = s;
			S[0] = s;

			// running food web
			checkFeasibility(S, P, steadyStates, stabEigen, ustabEigen, 0);
			timeSeries(S, P, steadyStates, stabEigen, ustabEigen, webData, 0);
			saveParameters(S, P, sFile, pFile, 0);
		}
		updateTrophicLevel(S);
	}
	
	// closing all files
	stabEigen.close();
	ustabEigen.close();
	webData.close();
	sFile.close();
	pFile.close();

	return 0;
}

