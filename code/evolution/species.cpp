#include "species.h"
#include "distributions.h"
#include <iostream>

// Declaring species
// default constructor
Species::Species() {
		density = 0;
		decay = 0;
		level = -1;
		derivative = 0;

		isProducer = 0;
		addAttempt = -1;
}

// constuctor
Species::Species(int j)	{
		density = initialDensity;
		decay = alpha();
		level = 0;

		addAttempt = j;
		isProducer = 0;
		nTotal++;
}

// destructor
Species::~Species() {

}


// Declaring producer
// default constructor
Producer::Producer() {
		growth = 0;
		level = -1;

		isProducer = 1;
		addAttempt = -1;
}

// constructor
Producer::Producer(int j) {
		density = initialDensity;
		growth = kappa();
		decay = alpha();
		level = 1;

		addAttempt = j;
		isProducer = 1;
		nProducer++;
		nTotal++;
}

// destructor
Producer::~Producer() {

}


// Time derivatives
void Species::computeDerivative(double strengthen, double weaken) {
		derivative = density * (strengthen - weaken - decay);
	}

void Producer::computeDerivative(double nutrients, double weaken) {
		derivative = density * (growth*nutrients - decay - weaken);
	}
	


// Printing
void Species::printParameters(int index) {
		// Printing parameters
		std::cout << "Species " << index;
		std::cout << "\nAdded as #: " << addAttempt;
		std::cout << "\ntrophic level: " << level;
		std::cout << "\ndensity " << density;
		std::cout << "\nprimary: " << isProducer;
		std::cout << "\na: " << decay;
		std::cout << std::endl;

		// Printing consumers
		std::cout << "Predators:\n";
		for (int i = 0; i < Species::nTotal; i++) {
			std::cout << i << ": " << consumers[i] << ", ";
		}
		
		// Printing resources
		std::cout << std::endl << "Preys:\n";
		for (int i = 0; i < Species::nTotal; i++) {
			std::cout << i << ": " << resources[i] << ", ";
		}
		std::cout << std::endl << std::endl;
	}

void Producer::printParameters(int index) {
		// Printing parameters
		std::cout << "Species " << index;
		std::cout << "\nAdded as #: " << addAttempt;
		std::cout << "\ntrophic level: " << level;
		std::cout << " \ndensity: " << density;
		std::cout << "\nprimary: " << isProducer;
		std::cout << "\nk: " << growth;
		std::cout << "\na: " << decay;
		std::cout << std::endl;

		// Printing consumers
		std::cout << "Predators:\n";
		for (int i = 0; i < Species::nTotal; i++) {
			std::cout << i << ": " << consumers[i] << ", ";
		}
		std::cout << std::endl << std::endl;
	}


// FOODWEB
// contructor
FoodWeb::FoodWeb() {

}

// destructor
FoodWeb::~FoodWeb() {

}

