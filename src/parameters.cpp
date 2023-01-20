#include "parameters.h"
#include <cstdlib>
#include <cmath>
#include <random>


// UNIFORM DISTRIBUTIONS
double randomDouble(double min, double max){
	return (max - min) * ((double)rand() / (double)RAND_MAX) + min;
}

int randomInt(int min, int max){
	return min + (rand() % static_cast<int>(max - min));
}

bool ratio(int a, int b){
	double p = ((double)a/b);
	double r = randomDouble(0,1);
	if (r < p) { return true; }
	else { return false; }
}


// SPECIFIC DISTRIBUTIONS FOR MANIPULATION
double kappa(){
	return defaultKappa;
}	

double alpha(){
	return randomDouble(0.05, 0.5);

	// std::random_device rd;
	// std::mt19937 e2(rd());

	// normal distribution
	/*double mu = 0.25;
	double sig = ((double)1 / 6);
	std::normal_distribution<> dist(mu, sig);
	double a = dist(e2);
	while (a < 0.05){
		a = dist(e2);
	}

	// exponential distribution
	double lambda = 2.5;
  	std::exponential_distribution<double> distribution(lambda);
    double a = 0.05 + distribution(e2);
    while (a > 0.5) {
    	a = 0.05 + distribution(e2);
	} */

	// return a;
}

double eta(){
	return randomDouble(0.01, 1);

	// std::random_device rd;
	// std::mt19937 e2(rd());

	// Normal distribution
	/* double mu = 0.5;
	double sig = ((double)1 / 3);
	std::normal_distribution<> dist(mu, sig);
	double eta = dist(e2);
    while (eta < 0 || eta > 1) {
    	eta = dist(e2);
	}

	// Exponential distribution
	std::default_random_engine generator;
  	std::exponential_distribution<double> distribution(1.5);
    double eta = 0.01 + distribution(e2);
    while (eta > 1) {
    	eta = 0.01 + distribution(e2);
	} */

	// return eta;
}

double beta(){
	return defaultBeta;
}

bool type(){		
	return ratio(1,3);
}



// LINKS
bool addSecondResource(){
//		return false;		// 1-link
//		return ratio(3,4);	// 2-link
		return ratio(1,2);	// omni
}


