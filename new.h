//
// Created by alpharius on 18.07.2020.
//

#ifndef FAT_PHY_GAS_TASK_NEW_H
#define FAT_PHY_GAS_TASK_NEW_H
#include <stddef.h>
#include <malloc.h>
#include <assert.h>

#include "Class.h"


void * new (const void * class, ...);
void delete (void * item);

size_t sizeOf (const void * self);

#endif //FAT_PHY_GAS_TASK_NEW_H
