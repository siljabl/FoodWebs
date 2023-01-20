#pragma once
#include "parameters.h"
#include "stability_analysis.h"


// ------------------------------------------------------------------------------------------// 
//	  		    FUNCTIONS FOR MANIPULATING FOODWEB				     //
// ------------------------------------------------------------------------------------------//  



//	INTERACTIONS
	bool addLink(Species S[], Producer P[], int resource, int consumer);
	// creating link between resource and consumer

	void removeLinks(Species S[], Producer P[], int index);
	// removing all links of a species

	void updateTrophicLevel(Species S[]);
	// computing trophic level of all species



//	ADDING AND REMOVING Species
	int freeIndex(Species S[], Producer P[], bool isProducer);
	// finding lowest free index in species array, keeping Producers at lowest indices

	void addSpecies(Species S[], Producer P[], int addAttempt);
	// adding n'th species to the foodweb

	void removeSpecies(Species s[], Producer p[], int index);
	// removing species with index from foodweb



//	COMPUTING DERIVATIVES
	double availableNutrients(Producer P[]);
	// computing available nutrients from the nutreint source
	// assuming all producers consume at the same rate

	double strengthen(Species S[], int index);
	// computing the amount Species[index] is strengthened when consuming its resources

	double weaken(Species S[], int index);
	// computing the amount Species[index] is weakened when being consumed by its consumers

	void computeDerivatives(Species S[], Producer P[]);
	// computing derivatives of all species in foodweb
