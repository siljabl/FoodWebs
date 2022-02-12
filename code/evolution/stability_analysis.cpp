#include "food_web.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>


//	COMPUTING STEADY STATES
	MatrixXd initializeR(MatrixXd R, Species S[], Producer P[]) {
		// i - row number, j - column number
		int i, j, k;

		// filling rows of Producers, nProducer x N
		for (i = 0; i < Producer::nProducer; i++) {
			
			// Producer x Producer entries, nProducer x nProducer
			for (j = 0; j < Producer::nProducer; j++) {
				R(i, j) = 1;								
				//cout << "R(" << i << "," << j << "): " << R(i, j) << endl;
			}

			// Producer x Species entries, nProducer x (N - nProducer)
			for (k = Producer::nProducer; k < Species::nTotal; k++) {
				R(i, k) = P[i].consumers[k] / P[i].growth;
				//cout << "R(" << i << "," << k << "): " << R(i, k) << endl;
			}
		}

		// filling rows of Species, (N - nProducer) x N (row (nProducer + 1) to N)
		for (k = Producer::nProducer; k < Species::nTotal; k++) {
			for (j = 0; j < Species::nTotal; j++) {
				R(k, j) = S[k].resources[j] - S[k].consumers[j];
				//cout << "R(" << k << "," << j << "): " << R(k, j) << endl;
			}
		}

		return R;
	}


	VectorXd initializeK(VectorXd K, Species S[], Producer P[]) {
		// filling entries of Producers
		for (int i = 0; i < Producer::nProducer; i++) {
			K(i) = (P[i].growth - P[i].decay) / P[i].growth;
		}

		// filling entries of Species
		for (int k = Producer::nProducer; k < Species::nTotal; k++) {
			K(k) = S[k].decay;
		}

		return K;
	}


//	CHECKING FOR FEASIBILITY AND STABILITY
	bool feasible(VectorXd S) {
		// counter
		int S_positive = 0;

		// counting
		for (int i = 0; i < Species::nTotal; i++) {
			if (S(i) > epsilon) {
				S_positive++;
			}
		}

		// if all Species has a positive steady state
		if (S_positive == Species::nTotal) { return true; }

		else { return false; }
	}


	bool negativeRealValues(VectorXcd eigenvalues) {
		// counter 
		int S_stable = 0;

		// counting
		for (int i = 0; i < Species::nTotal; i++) {
			if (eigenvalues(i).real() < 0) {
				S_stable++;
			}
		}

		// if all Species have negative real parts
		if (S_stable == Species::nTotal) { return true; }

		else { return false; }
	}


//	INITIALIZING COMMUNITY MATRIX
	void setDensitiesSteady(double steadyStates[], Species S_temp[], Producer P_temp[]) {
		int i;
		// densities of Producers
		for (i = 0; i < Producer::nProducer; i++) {
			S_temp[i].density = steadyStates[i];
			P_temp[i].density = S_temp[i].density;
		}

		// densities of other Species
		for (i = Producer::nProducer; i < Species::nTotal; i++) {
			S_temp[i].density = steadyStates[i];
		}
	}


	MatrixXd CommunityMatrix(MatrixXd C, Species S[], Producer P[]) {
		int i, j, k;
		double nutrients = availableNutrients(P);

		// filling rows of Producers, nProducer x N
		for (i = 0; i < Producer::nProducer; i++) {
	
			// Producer x Producer, nProducer x nProducer
			for (j = 0; j < Producer::nProducer; j++) {
				// filling diagonal entries
				if (i == j) {
					C(i,i) = P[i].growth * (nutrients - P[i].density) - P[i].decay - weaken(S, i);
					//cout << "C(" << i << "," << j << "): " << C(i,j) << endl;
				}

				// filling non-diagonal entries
				else {
					C(i,j) = -P[i].growth * P[i].density;
					//cout << "C(" << i << "," << j << "): " << C(i,j) << endl;
				}
			}

			// Producer x Species, nProducer x(N - nProducer)
			for (k = Producer::nProducer; k < Species::nTotal; k++) {
				C(i,k) = -P[i].consumers[k] * P[i].density;
				//cout << "C(" << i << "," << k << "): " << C(i, k) << endl;
			}
		}

		// filling rows of Species, (N - nProducer) x N (row (nProducer + 1) to N)
		for (k = Producer::nProducer; k < Species::nTotal; k++) {
			for (j = 0; j < Species::nTotal; j++) {
	
				// filling diagonal entries
				if (k == j) {
					C(k,k) = strengthen(S, k) - weaken(S, k) - S[k].decay;
					//cout << "C(" << k << "," << j << "): " << C(k,j) << endl;
				}

				// filling non-diagonal entries
				else {
					C(k,j) = (S[k].resources[j] - S[k].consumers[j]) * S[k].density;
					//cout << "C(" << k << "," << j << "): " << C(k,j) << endl;
				}
			}
		}

		return C;
	}


	MatrixXd CommunityMatrix2(MatrixXd C, Species S[], Producer P[]) {
		int i, j, k;
		double nutrients = availableNutrients(P);

		// filling rows of Producers, nProducer x N
		for (i = 0; i < Producer::nProducer; i++) {
			double sum_consumers = 0;
			for (k = Producer::nProducer; k < Species::nTotal; k++) {
				sum_consumers += (double) P[i].consumers[k] * S[k].density / ((1 + h * P[i].consumers[k] * P[i].density) * (1 + h * P[i].consumers[k] * P[i].density));
			}

	
			// Producer x Producer, nProducer x nProducer
			for (j = 0; j < Producer::nProducer; j++) {
				// filling diagonal entries
				if (i == j) {
					C(i,i) = P[i].growth * (nutrients - P[i].density) - P[i].decay - sum_consumers;
					//cout << "C(" << i << "," << j << "): " << C(i,j) << endl;
				}

				// filling non-diagonal entries
				else {
					C(i,j) = -P[i].growth * P[i].density;
					//cout << "C(" << i << "," << j << "): " << C(i,j) << endl;
				}
			}

			// Producer x Species, nProducer x(N - nProducer)
			for (k = Producer::nProducer; k < Species::nTotal; k++) {
				C(i,k) = -P[i].consumers[k] * P[i].density / (1 + h * P[i].consumers[k] * P[i].density);
				//cout << "C(" << i << "," << k << "): " << C(i, k) << endl;
			}
		}

		// filling rows of Species, (N - nProducer) x N (row (nProducer + 1) to N)
		for (k = Producer::nProducer; k < Species::nTotal; k++) {
			double sum_consumers = 0;
			for (int m = Producer::nProducer; m < Species::nTotal; m++) {
				sum_consumers += (double) S[k].consumers[m] * S[m].density / ((1 + h * S[k].consumers[m] * S[k].density) * (1 + h * S[k].consumers[m] * S[k].density));
			}

			for (j = 0; j < Species::nTotal; j++) {
	
				// filling diagonal entries
				if (k == j) {
					C(k,k) = strengthen(S, k) - S[k].decay - sum_consumers;
					//cout << "C(" << k << "," << j << "): " << C(k,j) << endl;
				}

				// filling non-diagonal entries
				else {
					double res = S[j].consumers[k] / ((1 + h * S[j].consumers[k] * S[j].density) * (1 + h * S[j].consumers[k] * S[j].density));
					double cons = S[k].consumers[j] / (1 + h * S[k].consumers[j] * S[k].density);
					C(k,j) = (res - cons) * S[k].density;
					//cout << "C(" << k << "," << j << "): " << C(k,j) << endl;
				}
			}
		}

		return C;
	}



//	SAVING PARAMETERS
	void saveEigenvalues(VectorXcd eigenvalues, ofstream& file, int addAttempt) {
		// addAttempt and number of Species
		file << addAttempt << " " << Species::nTotal << " ";

		// eigenvalues of all Species in food webs
		for (int i = 0; i < Species::nTotal; i++) {
			file << eigenvalues(i).real() << " " << eigenvalues(i).imag() << " ";
		}

		// zeroes for all empty entries in S[] and P[]
		for (int i = Species::nTotal; i < nMAX; i++) {
			file << 0 << " " << 0 << " ";
		}

		file << endl;
	}



	void saveParameters(Species S[], Producer P[], ofstream& sFile, ofstream& pFile, int addAttempt) {
		// saving parameters of Producers
		for (int i = 0; i < Producer::nProducer; i++) {
			// addAttempt
			pFile << addAttempt << " ";

			// index - addAttempt of invasion - level - final density - growth rate - decay rate
			pFile << i << " " << P[i].addAttempt << " " << P[i].level << " " << P[i].density << " " << P[i].growth << " " << P[i].decay << " ";

			// index of other Species - interaction strength
			for (int k = 0; k < nMAX; k++) {
				pFile << k << " " << P[i].consumers[k] << " ";
			}

			pFile << endl;
		}

		// saving parameters of Species
		for (int k = Producer::nProducer; k < Species::nTotal; k++) {
			// addAttempt
			sFile << addAttempt << " ";

			// index - addAttempt of invasion - level - final density - decay rate
			sFile << k << " " << S[k].addAttempt << " " << S[k].level << " " << S[k].density << " " << S[k].decay << " ";

			
			// index of other Species - interaction strength(as resource) - interaction strength(as consumer)
			for (int j = 0; j < nMAX; j++) {
				sFile << j << " " << S[k].consumers[j] << " " << S[k].resources[j] << " ";
			}

			sFile << endl;
		}
	}



//	INVESTIGATING LINEAR STABILITY
	void checkFeasibility(Species S[], Producer P[], double steadyStates[], ofstream& stabEigen, ofstream& unstabEigen, int addAttempt) {
		//	COMPUTING S* = R^(-1)*k
		// declaring vectors and matrices
		MatrixXd R(Species::nTotal, Species::nTotal);
		VectorXd K(Species::nTotal);
		VectorXd K_verify(Species::nTotal);
		double Cond;
		VectorXd Ssteady(Species::nTotal);

		// initializing R and K
		R = initializeR(R, S, P);				//cout << R << endl;
		K = initializeK(K, S, P);				//cout << K << endl;
		FullPivLU<MatrixXd> lu(R);

		// computing steady states
		Ssteady = R.inverse() * K;
		std::fill_n(steadyStates, nMAX, 0);
		K_verify = R * Ssteady;

		// computing condition number
		JacobiSVD<MatrixXd> svd(R);
		double cond = svd.singularValues()(0) / svd.singularValues()(svd.singularValues().size()-1);
		cout << "Condition number: " << cond << endl;
		cout << "Invertible: " << lu.isInvertible() << endl;


		for (int i=0; i<Species::nTotal; i++) {
			steadyStates[i] = Ssteady(i);
		}

		/*cout << "checking STEADY states \n";
		for (int i = 0; i < Species::nTotal; i++)
		{
			std::cout << steadyStates[i] << endl;
			std::cout << Ssteady(i) << endl;
			
		}
		for (int i = Species::nTotal; i < nMAX; i++) {
			std::cout << steadyStates[i] << endl;
		}
		*/
			
		//	CHECKING IF SYSTEM IS FEASIBLE AND LINEARLY STABLE
		if (feasible(Ssteady)) {
			// updating feasibility of food web
			FoodWeb::feasible = true;

			// printing steady states
			cout << "The system does have a feasible steady state!\n";
			
			// checking for linear stability
			checkLinearStability(S, P, steadyStates, stabEigen, unstabEigen, addAttempt);		
			cout << "---------------\n Steady states: \n---------------\n";
			cout << Ssteady << "\n---------------\n\n";		
		}
		else {
			// updating feasibility and stability of food web
			FoodWeb::feasible = false;
			FoodWeb::stable = false;

			// printing feasibility
			cout << "The system does not have a feasible steady state\n\n";
		}
	}


	void checkLinearStability(Species S[], Producer P[], double steadyStates[], ofstream& stabEigen, ofstream& unstabEigen, int addAttempt)	{
		//	INITIALIZING
		// declaring matrix and arrays
		MatrixXd C(Species::nTotal, Species::nTotal);
		VectorXcd eigenvalues(Species::nTotal);
		Species S_temp[nMAX];
		Producer P_temp[nMAX];

		// initializing temporary species arrays and setting density to steady state
		for (int i=0; i<nMAX; i++) {
			P_temp[i] = P[i];
			S_temp[i] = S[i];
		}
		setDensitiesSteady(steadyStates, S_temp, P_temp);

		
		//	COMPUTING JACOBIAN AND EIGENVALUES
		// initializing Jacobian
		C = CommunityMatrix(C, S_temp, P_temp);

		// computing eigenvalues of Jacobian
		EigenSolver<MatrixXd> es(C);
		eigenvalues = es.eigenvalues();

		
		//	CHECKING LINEAR STABILITY
		// if linearly stable
		if (negativeRealValues(eigenvalues)) {
			// updating stability of food web
			FoodWeb::stable = true;

			cout << "The steady states are linearly stable!\n\n";
			saveEigenvalues(eigenvalues, stabEigen, addAttempt);
		}

		else {
			// updating stability of food web
			FoodWeb::stable = false;

			cout << "The steady states are not linearly stable.\n\n";
			saveEigenvalues(eigenvalues, unstabEigen, addAttempt);
		}
	}



void compEig(Species S[], Producer P[], ofstream& stabEigen, ofstream& unstabEigen, int addAttempt)	{
		//	INITIALIZING
		// declaring matrix and arrays
		MatrixXd C(Species::nTotal, Species::nTotal);
		VectorXcd eigenvalues(Species::nTotal);
		Species S_temp[nMAX];
		Producer P_temp[nMAX];

		// initializing temporary species arrays and setting density to steady state
		for (int i=0; i<nMAX; i++) {
			P_temp[i] = P[i];
			S_temp[i] = S[i];
		}
		

		if (Species::nTotal == 2 && Producer::nProducer == 1) {
			double betaPrime = defaultBeta - h * S[1].decay;
			double S1 = S[1].decay / (S[0].consumers[1] * betaPrime);
		 	double S2 = ((P[0].growth / S[0].consumers[1]) * (1 - S1) - S[0].decay) * (defaultBeta / betaPrime);
		 	cout << "Analytical steady states:\n" << S1 << endl;
		 	cout << S2 << endl << endl;
		 	P_temp[0].density = S1;
		 	S_temp[0].density = S1;
		 	S_temp[1].density = S2;

		}
		
		//	COMPUTING JACOBIAN AND EIGENVALUES
		// initializing Jacobian
		C = CommunityMatrix2(C, S_temp, P_temp);

		// computing eigenvalues of Jacobian
		EigenSolver<MatrixXd> es(C);
		eigenvalues = es.eigenvalues();

		
		//	CHECKING LINEAR STABILITY
		// if linearly stable
		if (negativeRealValues(eigenvalues)) {
			// updating stability of food web
			FoodWeb::stable = true;

			cout << "The steady states are linearly stable!\n\n";
			saveEigenvalues(eigenvalues, stabEigen, addAttempt);
		}

		else {
			// updating stability of food web
			FoodWeb::stable = false;

			cout << "The steady states are not linearly stable.\n\n";
			saveEigenvalues(eigenvalues, unstabEigen, addAttempt);
		}
	}
