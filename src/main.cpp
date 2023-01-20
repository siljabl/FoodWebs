#include "parameters.cpp"
#include "food_web.cpp"
#include "time_series.cpp"
#include "stability_analysis.cpp"
#include "species.cpp"
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
	ofstream stabVal("../data/"+folder+"/eigval_stab.txt");			// eigenvalues of linearly stable food web
	ofstream ustabVal("../data/"+folder+"/eigval_unstab.txt");		// eigenvalues of linearly unstable food web
	ofstream stabVec("../data/"+folder+"/eigvec_stab.txt");			// eigenvalues of linearly stable food web
	ofstream ustabVec("../data/"+folder+"/eigvec_unstab.txt");		// eigenvalues of linearly unstable food web

	// data is saved as:
	// iteration - number of Species - feasibility - stability - convergence
	// 1 - converged, 2 - decreasing amplitude, 9 - not converged
	ofstream webData("../data/"+folder+"/stab_data.txt");			// behavior of food web
	ofstream biomass("../data/"+folder+"/biomass.txt");
	ofstream level("../data/"+folder+"/level.txt");
	ofstream links("../data/"+folder+"/links.txt");

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
	checkFeasibility(S, P, steadyStates, stabVal, ustabVal, stabVec, ustabVec, biomass, level, links, 0);
	timeSeries(S, P, steadyStates, stabVal, ustabVal, stabVec, ustabVec, biomass, level, links, webData, 0);
	saveParameters(S, P, steadyStates, sFile, pFile, 0);


//	RUNNING EVOLUTION
	for (int addAttempt = 1; addAttempt < addAttemptMAX; addAttempt++) {
		// printing addition attempt
		cout << "\n\n---------------------------\n";
		cout << " " << addAttempt << ". ADDITION ATTEMPT\n";
		cout << "---------------------------\n\n";
		
		// adding invasive species
		addSpecies(S, P, addAttempt);
		updateTrophicLevel(S);
		saveParameters(S, P, steadyStates, sFile, pFile, addAttempt);

		// printing number of species
		cout << "Number of Species in food web " << Species::nTotal;
		cout << ", number of Producers " << Producer::nProducer << endl << endl;

		// running time series
		checkFeasibility(S, P, steadyStates, stabVal, ustabVal, stabVec, ustabVec, biomass, level, links, addAttempt);
		timeSeries(S, P, steadyStates, stabVal, ustabVal, stabVec, ustabVec, biomass, level, links, webData, addAttempt);

		if (Species::nTotal == 0) {
			Producer s(addAttempt);
			P[0] = s;
			S[0] = s;

			// running food web
			checkFeasibility(S, P, steadyStates, stabVal, ustabVal, stabVec, ustabVec, biomass, level, links, 0);
			timeSeries(S, P, steadyStates, stabVal, ustabVal, stabVec, ustabVec, biomass, level, links, webData, 0);
			saveParameters(S, P, steadyStates, sFile, pFile, 0);
		}
		updateTrophicLevel(S);
	}
	
	// closing all files
	stabVal.close();
	ustabVal.close();
	stabVec.close();
	ustabVec.close();
	webData.close();
	biomass.close();
	level.close();
	links.close();
	sFile.close();
	pFile.close();

	return 0;
}

