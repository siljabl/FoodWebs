#pragma once
/* CONSTANTS AND DISTRIBUTIONS CONTROLLING EVOLUTION OF FOOD WEB*/


// CONSTANTS
// default species parameters
const double initialDensity = 1e-10;
const double defaultKappa = 1;
const double defaultBeta = 0.75;
const double h = 0.1;					// type-II parameter

// default threshold
const double epsilon = 1e-14;

// lower limit on species richness before allowing species with two resources
const int nMin = 2;
const bool omnivorous = true;
const int responseType = 2; 		// 1 or 2


//DISTRIBUTIONS
// random numbers in [min, max)
double randomDouble(double min, double max);
int randomInt(int min, int max);

// returning true with probability a/b
bool ratio(int a, int b);

// Species PARAMETER DISTRIUTIONS
double kappa(); 	// growth rate
double alpha();	  	// decay rate
double eta();		// interaction strength
double beta();		// reproduction efficiency			
bool type();		// determines type of invasive Species: 
					// Producer if true, Species at l>1 if false


// LINK DISTRIBUTIONS
bool addSecondResource();

