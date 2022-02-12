# simulating-omnivory
Code written in C++ for simulating the evolution of omnivorous food web. Written for a bachelor project.

The code allows evolution of omnivorous food webs with successive additions of invasive species.

species.h defines the classes 'species', 'producer' and 'foodweb'. 
food_web.h defines the functions for adding and removing species, in addition to the interspecies dynamics.
stabiliy_analysis.h defines the functions for computing feasiblity and eigenvalues of Jacobian, as well as the functions for writing the data in files.
time_series.h defines the functions for performing the numerical integration of the food web.
distributions.h defines the distributions and ratios used in all other files.

The parameter distributions, ratio of producers to species and probability of consuming on two resources can bemodified in ditributions.h.

Before running the simulation the number of invasions and size of food web must be determined:
j_max in main.cpp determines the total number of additions.
N in species.h determines the total number of species allowed in the food web.

# Describing configurations
Files that must be modified before compiling:

__1__, main.cpp
* rename 5 folders: 1-link/2-link/omni

__2__, evolution/distributions.h
- set omnivorous true/false

__3__, evolution/distributions.cpp
- change return of addSecondResource()

__4__, evolution/time_series.cpp
  - save time series?
