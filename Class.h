//
// Created by alpharius on 18.07.2020.
//

#ifndef FAT_PHY_GAS_TASK_CLASS_H
#define FAT_PHY_GAS_TASK_CLASS_H

#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>

typedef struct Class {
    size_t size;
    void * (* ctor) (void *self, va_list *app);
    void * (* dtor) (void *self);
} Class;

#endif //FAT_PHY_GAS_TASK_CLASS_H
