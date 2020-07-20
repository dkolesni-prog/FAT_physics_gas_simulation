//
// Created by alpharius on 18.07.2020.
//

#ifndef FAT_PHY_GAS_TASK_ATOM_H
#define FAT_PHY_GAS_TASK_ATOM_H
#include "MathCore.c"
#include "Class.h"
#include "malloc.h"
#include <math.h>

extern const void * Atom;
typedef struct _atom {
    void * class;
    double M;
    Vector2D* coordinate;
    Vector2D* velocity;
    Vector2D* force;     /* to calculate collision and 2nt Newton law */
} Atom_t;

void PhysicalInteration(Atom_t *dest, Atom_t *source, void* PhyLaw);
void PhysicalMovement(double dt, Atom_t* particle);
void ForceZerofication(Atom_t* particle);
Vector2D LawOne(Atom_t* dest, Atom_t* source);

#endif //FAT_PHY_GAS_TASK_ATOM_H
