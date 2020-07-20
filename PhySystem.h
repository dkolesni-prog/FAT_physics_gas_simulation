//
// Created by alpharius on 20.07.2020.
//

#ifndef FAT_PHY_GAS_TASK_PHYSYSTEM_H
#define FAT_PHY_GAS_TASK_PHYSYSTEM_H

typedef struct Sys {
    int amount_of_particles;
    double dt;
    Atom_t* aptr;
} Sys_t;
Sys_t* create_sys(size_t number_of_particles, double dt);
void RunSystem(Sys_t* _sys);

#endif //FAT_PHY_GAS_TASK_PHYSYSTEM_H
