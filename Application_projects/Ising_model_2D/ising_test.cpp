/*******************************************************************************
*
* Test program for the Ising simulation
*
*******************************************************************************/

//----Preprocessor directives---------------------------------------------------
#include <iostream>
#include <string>
#include <chrono>

/* Import the Class lattice */
#include "ising_lattice.h"

/*
* CONFIGURATION PARAMETERS OF THE LATTICE
* SIDE = size of the lattice's side.
* G_FLAG = lattice's geometry flag:
*          0 and others not implemented yet.
*          1 for 1D periodic chain.
*          2 for 2D square with PBC.
* I_FLAG = lattice's initial configuration flag:
*          0 for cold initialization.
*          1 for hot (random) initialization.
*          2 for loading the previous configuration.
*/
#define SIDE 60
#define G_FLAG 2
#define I_FLAG 2

/*
* CONFIGURATION PARAMETERS OF THE SIMULATION
* BETA = adimensional reciprocal of the product of the temperature and k_B.
* EXTFIELD = adimensional intensity of the external magnetic field.
* I_DECORREL = MC-updates of the lattice between different measurements.
*/
#define BETA 0.440
#define EXTFIELD 0.
#define I_DECORREL 5
#define LOOPS 500

using namespace std;

//----Contents------------------------------------------------------------------

int main(){
    string file_path;
    double ener, magn;
    lattice ising(SIDE, G_FLAG, I_FLAG);

    file_path = "ising_state.dat";
    cout << "Running with beta = " << BETA << endl;

    if(I_FLAG == 2){
        // Loading configuration
        ising.load_configuration(file_path);
    } else {
        // Thermalization phase
        for(int i = 0; i < (10 * I_DECORREL); i++) ising.update(BETA, EXTFIELD);
    }

    ener = ising.energy(EXTFIELD);
    cout << "-> starting energy = " << ener << endl;
    magn = ising.magnetization();
    cout << "-> starting magnet = " << magn << endl << endl;
    ising.show_configuration();

    auto start = chrono::steady_clock::now();
    for(int i = 0; i < LOOPS; i++) ising.update(BETA, EXTFIELD);
    auto end = chrono::steady_clock::now();

    chrono::duration<double> elapsed_seconds = end - start;
    cout << "Elapsed time for " << LOOPS << " loops: "
         << elapsed_seconds.count() << "s " << endl;

    ener = ising.energy(EXTFIELD);
    cout << "-> final energy = " << ener << endl;
    magn = ising.magnetization();
    cout << "-> final magnet = " << magn << endl << endl;
    ising.show_configuration();

    ising.save_configuration(file_path);
}
