#include <stdio.h>
#include "Atom.h"
#include "new.h"
#include "PhySystem.h"

int main() {
    //Atom_t  *a = new(Atom,2.3, 1,1,2,2,3,3);
    Sys_t *system =  create_sys(3, 0.01);
    RunSystem(system);

    return 0;
}