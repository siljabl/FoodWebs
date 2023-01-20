#pragma once
/* DEFINITION OF SPECIES, PRODUCER AND FOOD WEB CLASSES */

// Parameters
const int nMAX = 20;					// maximal number of species allowed in food web


class Species
{
public:
	Species();					// default constructor
	Species(int added_i);				// constructor
	~Species();					// destructor

//	counters
	static int nTotal;				// total number of species
	bool isProducer;				// determining if species is a producer. (if producer: 1, else: 0)
	int addAttempt;					// addition attempt of the species

//	parameters
	double density, decay;				// time-derivative, density, decay rate
	double derivative;
	double level;					// trophic level
	double consumers[nMAX] = { 0 };			// array of interactions (with the species as resource)
	double resources[nMAX] = { 0 };			// array of interactions (with the species as consumer)
									/* The array entry at index i corresponds to the species at index i in the 
									   species array. If the species doesn't have an interaction with species i,
									   the corresponding entries are 0*/
	
//	functions
	void computeDerivative(double strengthen, double weaken);
	// Lotka-Volterra equation for time derivative

	void printParameters(int index);
	// printing all paramters
};



class Producer : public Species
{
public:
	Producer();					// default constructor
	Producer(int added_i);				// constructor
	~Producer();					// destructor

//	counters
	static int nProducer;			// total number of primary producers

//	parameters
	double growth;					// growth rate

//	functions
	void computeDerivative(double nutrients, double weaken);
	// Lotka-Voterra equation for time derivative

	void printParameters(int index);
	// printing all parameters
};



class FoodWeb
{
public:
	FoodWeb();
	~FoodWeb();

	// characteristics of food web
	static bool feasible;			// true if food web is feasible
	static bool stable;				// true if food web is linearly stable

	// characteristics of previous food web
	static int prevIteration;		// behavior after previous invasion
	static int prevExtinct;			// previous species to go extinct
};



