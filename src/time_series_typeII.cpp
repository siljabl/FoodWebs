#include "time_series.h"
#include "food_web.h"
#include <iostream>
#include <fstream>
#include <complex>
using namespace std;

void RK4(Species S[], Producer P[], double dt) {
	//	INITIALIZING TEMPORARY ARRAYS
	int i; 
	// temprary time derivatives
	double dS_1[nMAX], dS_2[nMAX], dS_3[nMAX], dS_4[nMAX] = { 0 };

	// temporary Species arrays
	Producer P_temp[nMAX];
	Species S_temp[nMAX];

	// initializing temporary species arrays
	for (int i = 0; i < Species::nTotal; i++)
	{
		P_temp[i] = P[i];
		S_temp[i] = S[i];
	}


	//	1ST ESTIMATE
	// computing derivatives
	computeDerivatives(S, P);

	// Producers
	for (i = 0; i < Producer::nProducer; i++) {
		// differential step: dS(S_0)/dt * dt
		dS_1[i] = S[i].derivative * dt;

		// updating temporary densities: S_0 + (1/2) * dS_1
		P_temp[i].density = P[i].density + 0.5 * dS_1[i];
		S_temp[i].density = S[i].density + 0.5 * dS_1[i];
	}

	// other Species
	for (i = Producer::nProducer; i < Species::nTotal; i++)	{
		// differential step: dS(S_0)/dt * dt
		dS_1[i] = S[i].derivative * dt;

		// updating temporary densities: S_0 + (1/2) * dS_1
		S_temp[i].density = S[i].density + 0.5 * dS_1[i];
	}



	//	2ND ESTIMATE
	// computing derivatives
	computeDerivatives(S_temp, P_temp);

	// Producers
	for (i = 0; i < Producer::nProducer; i++) {
		// differential step: dS(S_0 + (1/2) * dS_1)/dt * dt
		dS_2[i] = S_temp[i].derivative * dt;

		// updating temporary densities: s_0 + (1/2) * ds_2
		P_temp[i].density = P[i].density + 0.5 * dS_2[i];
		S_temp[i].density = S[i].density + 0.5 * dS_2[i];
	}

	// other Species
	for (i = Producer::nProducer; i < Species::nTotal; i++) {
		// differential step: dS(S_0 + (1/2) * dS_1)/dt * dt
		dS_2[i] = S_temp[i].derivative * dt;

		// updating temporary densities: S_0 + (1/2) * dS_2
		S_temp[i].density = S[i].density + 0.5 * dS_2[i];
	}



	//	3RD ESTIMATE
	// computing derivatives
	computeDerivatives(S_temp, P_temp);

	// Producers
	for (i = 0; i < Producer::nProducer; i++) {
		// differential step: dS(S_0 + (1/2) * dS_2)/dt * dt
		dS_3[i] = S_temp[i].derivative * dt;

		// updating temporary densities: S_0 + dS_3
		P_temp[i].density = P[i].density + dS_3[i];
		S_temp[i].density = S[i].density + dS_3[i];
	}

	// other Species
	for (i = Producer::nProducer; i < Species::nTotal; i++)	{
		// differential step: dS(S_0 + (1/2) * dS_2)/dt * dt
		dS_3[i] = S_temp[i].derivative * dt;

		// updating temporary densities: S_0 + dS_3
		S_temp[i].density = S[i].density + dS_3[i];
	}



	//	4TH ESTIMATE
	// computing derivatives
	computeDerivatives(S_temp, P_temp);

	// all Species
	for (i = 0; i < Species::nTotal; i++) {
		// differential step: dS(S_0 + dS_3)/dt *dt
		dS_4[i] = S_temp[i].derivative * dt;
	}



	//	CALCULATING S(t+dt)
	// computing density of Producers
	for (i = 0; i < Producer::nProducer; i++) {
		// cout << "S(t)_" << i << ": " << S[i].density;
		P[i].density += (dS_1[i] + 2 * dS_2[i] + 2 * dS_3[i] + dS_4[i]) / 6;
		S[i].density += (dS_1[i] + 2 * dS_2[i] + 2 * dS_3[i] + dS_4[i]) / 6;
		// cout << ", S(t+dt)_" << i << ": " << S[i].density << endl;
	}

	// computing density of other Species
	for (i = Producer::nProducer; i < Species::nTotal; i++)	{
		// cout << "S(t)_" << i << ": " << S[i].density;
		S[i].density += (dS_1[i] + 2 * dS_2[i] + 2 * dS_3[i] + dS_4[i]) / 6;
		// cout << ", S(t+dt)_" << i << ": " << S[i].density << endl;
	}
}


double RKF45(Species S[], Producer P[], double dt) {
	// tolerance on time step
	double tol = 5e-4;

	// finding Species with largest derivative
	int i, j = 0;
	for (i = 0; i < Species::nTotal; i++) {
		if (abs(S[i].derivative) >= abs(S[j].derivative)) {
			j = i;
		}
	}

	// temporary derivative arrays
	double dS_1[nMAX], dS_2[nMAX], dS_3[nMAX], dS_4[nMAX], dS_5[nMAX], dS_6[nMAX];

	//temporary Species arrays
	Species S_temp[nMAX];
	Producer P_temp[nMAX];

	// initializing temporary arrays
	for (int i = 0; i < nMAX; i++)	{
		P_temp[i] = P[i];
		S_temp[i] = S[i];
	}


	//	FIRST ESTIMATE
	// Producers
	for (i = 0; i < Producer::nProducer; i++) {
		dS_1[i] = S[i].derivative * dt;

		// updating temporary densities
		P_temp[i].density = P_temp[i].density + 0.25 * dS_1[i];
		S_temp[i].density = S_temp[i].density + 0.25 * dS_1[i];
	}

	// other Species
	for (i = Producer::nProducer; i < Species::nTotal; i++) {
		dS_1[i] = S[i].derivative * dt;

		// updating temporary densities
		S_temp[i].density = S_temp[i].density + 0.25 * dS_1[i];
	}


	//	SECOND ESTIMATE
	// computing derivatives
	computeDerivatives(S_temp, P_temp);

	// Producers
	for (i = 0; i < Producer::nProducer; i++) {
		dS_2[i] = S_temp[i].derivative * dt;

		// updating temporary densities
		P_temp[i].density = P[i].density + (3 / 32) * dS_1[i] + (9 / 32) *dS_2[i];
		S_temp[i].density = S[i].density + (3 / 32) * dS_1[i] + (9 / 32) *dS_2[i];
	}

	// other Species
	for (i = Producer::nProducer; i < Species::nTotal; i++) {
		dS_2[i] = S_temp[i].derivative * dt;

		// updating temporary densities
		S_temp[i].density = S[i].density + (3 / 32) * dS_1[i] + (9 / 32) *dS_2[i];
	}


	//	THIRD ESTIMATE
	computeDerivatives(S_temp, P_temp);
	// computing derivatives
	// Producers
	for (i = 0; i < Producer::nProducer; i++) {
		dS_3[i] = S_temp[i].derivative * dt;

		// upddating temporary densities
		P_temp[i].density = P[i].density + (1932 / 2197) * dS_1[i] - (7200 / 2197) * dS_2[i] + (7296 / 2197) * dS_3[i];
		S_temp[i].density = S[i].density + (1932 / 2197) * dS_1[i] - (7200 / 2197) * dS_2[i] + (7296 / 2197) * dS_3[i];
	}

	// other Species
	for (i = Producer::nProducer; i < Species::nTotal; i++) {
		dS_3[i] = S_temp[i].derivative * dt;

		// updating temporary densities
		S_temp[i].density = S[i].density + (1932 / 2197) * dS_1[i] - (7200 / 2197) * dS_2[i] + (7296 / 2197) * dS_3[i];
	}


	//	FORTH ESTIMATE
	// computing derivatives
	computeDerivatives(S_temp, P_temp);

	// Producers
	for (i = 0; i < Producer::nProducer; i++) {
		dS_4[i] = S_temp[i].derivative * dt;

		// updating temporary densities
		P_temp[i].density = P[i].density + (439 / 216) * dS_1[i] - 8 * dS_2[i] + (3680 / 513) * dS_3[i] - (845 / 4104) * dS_4[i];
		S_temp[i].density = S[i].density + (439 / 216) * dS_1[i] - 8 * dS_2[i] + (3680 / 513) * dS_3[i] - (845 / 4104) * dS_4[i];
	}

	// other Species
	for (i = Producer::nProducer; i < Species::nTotal; i++) {
		dS_4[i] = S_temp[i].derivative * dt;

		// updating temporary densities
		S_temp[i].density = S[i].density + (439 / 216) * dS_1[i] - 8 * dS_2[i] + (3680 / 513) * dS_3[i] - (845 / 4104) * dS_4[i];
	}


	//	FIFTH ESTIMATE
	// computing derivatives
	computeDerivatives(S_temp, P_temp);

	// Producers
	for (i = 0; i < Producer::nProducer; i++) {
		dS_5[i] = S_temp[i].derivative * dt;

		// updating temporary densities
		P_temp[i].density = P[i].density - (8 / 27) * dS_1[i] + 2 * dS_2[i] - (3544 / 2565) * dS_3[i] + (1859 / 4104) * dS_4[i] - (11 / 40)*dS_5[i];
		S_temp[i].density = S[i].density - (8 / 27) * dS_1[i] + 2 * dS_2[i] - (3544 / 2565) * dS_3[i] + (1859 / 4104) * dS_4[i] - (11 / 40)*dS_5[i];
	}
	
	// other Species
	for (i = Producer::nProducer; i < Species::nTotal; i++)	{
		dS_5[i] = S_temp[i].derivative * dt;

		// updating temporary densities
		S_temp[i].density = S[i].density - (8 / 27) * dS_1[i] + 2 * dS_2[i] - (3544 / 2565) * dS_3[i] + (1859 / 4104) * dS_4[i] - (11 / 40)*dS_5[i];
	}


	//	SIXTH ESTIMATE
	// computing derivatives
	computeDerivatives(S_temp, P_temp);

	// all Species
	for (i = 0; i < Species::nTotal; i++) {
		dS_6[i] = S_temp[i].derivative * dt;
	}

	// 5th and 4th order Runge-Kutta-Fehlberg
	double S_0, S_1;
	S_0 = S[j].density + (16 / 135)*dS_1[j] + (6656 / 12825)*dS_3[j] + (28561 / 56430)*dS_4[j] - (9 / 50)*dS_5[j] + (2 / 55)*dS_6[j];
	S_1 = S[j].density + (25 / 216)*dS_1[j] + (1408 / 2565)*dS_3[j] + (2197 / 4101)*dS_4[j] - 0.2*dS_5[j];

	// returning last timestep if tau is zero
	double tau = abs(S_0 - S_1);
	if (tau <= epsilon) { return dt; }

	else {
		// computing timestep
		double s = sqrt(sqrt(tol / (2 * tau)));
		return 0.01 * s;
	}
}


bool checkForExtinction(Species S[], Producer P[], double steadyStates[], ofstream& stabEigen, ofstream& unstabEigen, ofstream& webData, int addAttempt) {
	int k, i = 0;
	// determining Species with the smallest density
	for (k = 0; k < Species::nTotal; k++) {
		if (S[k].density < S[i].density) {
			i = k;
		}
	}

	// Removing smallest Species if it is extinct
	if (S[i].density <=  1e2 * epsilon) {
		// printing
		cout << "Species " << i << " has gone extinct" << endl;

		// save behavior of food web
		webData << 0 << S[i].addAttempt << endl;
		
		// updating food web
		FoodWeb::prevExtinct = S[i].addAttempt;

		// removing extinct Species
		removeSpecies(S, P, i);
	
		// saving behavior of new food web
		webData << addAttempt << " " << Species::nTotal << " " << FoodWeb::feasible << " " << FoodWeb::stable << " ";

		return true;
	}

	// else nothing
	else { return false; }
}


// Converged, type-II response
bool convergedII(Species S[], Producer P[], double steadyStates[], ofstream& webData, double t, int addAttempt) {
	int steadyCount = 0;
	computeDerivatives(S, P);
	
	if (t < 1*1e4) {	
		// counting Species with steady density
		for (int i = 0; i < Species::nTotal; i++) {
			if (abs(S[i].derivative) / S[i].density <= 1e-10) {
				steadyCount++;
			}
		}

		// if all Species have reached steady states
		if (steadyCount == Species::nTotal) {
			// saving behavior of food web
			FoodWeb::prevIteration = 1;
			webData << 1 << endl;

			return true;
		}

		else { return false; }
	}

	// feasible, linearly stable food webs that take very long to stabilize
	else {
		FoodWeb::prevIteration = 9;
		webData << 9 << endl;

		return true;
	}
}


void timeSeries(Species S[], Producer P[], double steadyStates[], ofstream& stab_file, ofstream& unstab_file, ofstream& webData, int iter) {
	double t = 0;
	double tTot = 0;
	double dt = 0.01;
	
	// number of Species when invasion starts
	int nInitial = Species::nTotal;

	// saving behavior of food web
	webData << iter << " " << Species::nTotal << " " << FoodWeb::feasible << " " << FoodWeb::stable<< " ";
	
	// Integrating food web until sustainable behavior is detected
	while (!convergedII(S, P, steadyStates, webData, t, iter) && Species::nTotal != 0) {
		// computing time step and setting lower limit
		t += dt;
		dt = RKF45(S, P, dt);
		if (dt < 0.01) { dt = 0.01; }

		// performing time step
		RK4(S, P, dt);
	
		// checking for extinction
		bool extinctionEvent = checkForExtinction(S, P, steadyStates, stab_file, unstab_file, webData, iter);

		if (extinctionEvent) {
			// restarting temporal counter after extinction
			tTot += t;
			t = 0;

			// food web is integrated for some time before considered as damped oscillatiry if no other Species go extinct
			// if food web damped oscillatory before invasion and invasive species is extinct
			if (FoodWeb::prevIteration == 2 && FoodWeb::prevExtinct == iter && Species::nTotal == nInitial - 1) {	
				bool secondExtinctionEvent = 0;
				while (t < 1e4 && !secondExtinctionEvent) {
					t += dt;
					dt = RKF45(S, P, dt);
					if (dt < 0.01) { dt = 0.01; }
					RK4(S, P, dt);

					secondExtinctionEvent = checkForExtinction(S, P, steadyStates, stab_file, unstab_file, webData, iter);				
				}
				if (!secondExtinctionEvent) {
					webData << FoodWeb::prevIteration << endl;	
					setDensitiesSteady(steadyStates, S, P);
					break;
				}
			}
		}
	}
	tTot += t;	
	cout << "Converged/not converged at t = " << tTot << endl;
}

