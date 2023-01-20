
// Script to compute eigenvalues of random matrices

#include "src/parameters.cpp"
#include <iostream>
#include <fstream>
#include <random>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

using namespace Eigen;
using namespace std;


// Defining functions used in script
// void printMatrixEigen(MatrixXd M, int N) {
// 	for(int i=0; i<N; i++) {
// 		for(int j=0; j<N; j++) { cout << "C(" << i << "," << j << "): " << M(i, j) << " "; }
// 		cout << endl;
// 	}
// }


int main() {
srand(27);

// setting parameters, etc.
int mu = 0;			// mean of gaussian
int sig = 1;		// std of gaussian
int Nrep = 1e4;		// number of community matrices in spectrum
int d = 1;			// diagonal element

// size of community matrix
int N = 2;			
cout << "Enter size of community matrix: ";
cin >> N;

// occupation probability, propotional to species richness
double prob = ((double)(N*N + 21*N - 28)) / (9*N*(N-1));
cout << "Occupation probability: " << prob << endl;

ofstream file("data/random_matrix/EigenMay_N" + to_string(N) + ".txt");

for(int rep=0; rep<Nrep; rep++) {
	MatrixXd C(N, N);		// community matrix
	
	random_device rd;
	mt19937 e2(rd());
	normal_distribution<> dist(mu, sig);

	// filling community matrix with random numbers
	for(int i=0; i<N; i++) {
		for(int j=0; j<N; j++) {
			C(i,j) = 0;
			if (i != j) {
			 	double u = randomDouble(0,1);
				double entry = dist(e2);
				if (u < prob) {
					double entry = dist(e2);
					C(i,j) = entry;
				}
			}
		}
	}
	
	// filling diagonal
	for (int i=0; i<N; i++) {
		C(i,i) = -d;
	}

	// Check condition number
	FullPivLU<MatrixXd> lu(C);
	JacobiSVD<MatrixXd> svd(C);
	double cond = svd.singularValues()(0) / svd.singularValues()(svd.singularValues().size()-1);


	// computing eigenvalues
	EigenSolver<MatrixXd> ES(C);
	VectorXcd Eigval(N);			// Eigen values
	Eigval = ES.eigenvalues();

	// saving
	for (int i=0; i<N; i++) {
		file << Eigval(i).real() << " " << Eigval(i).imag() << " ";
	}
	file << endl;
}


file.close();
return 0;
}
