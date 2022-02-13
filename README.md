# FoodWebs
Code and data for paper on food web eigenspectra.
Simulating evolution of food webs through successive additions of invasive species. The food webs follow a generalised set og Lotka-Volterra equations.

***main.cpp***: main simulation with Holling type I interactions

***main_typeII.cpp***: supplementary simulation with Holling type II interactions

***random_matrix.cpp***: computing eigenvalues of random matrices.

Most parameters are defined in *parameters.h* . Upper limit on species richness is set in *species.h* and total number of invasion attempts is set in *main.cpp / main_typeII.cpp*.

---

This project uses [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page 'Eigen') for matrix operations.
