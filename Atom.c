//
// Created by alpharius on 18.07.2020.
//

#include "Atom.h"





static void * Atom_constructor(void *_self, va_list *app){
    /*  M XY X'Y' X''Y'' */
    Atom_t * self = _self;
    self->M = va_arg(*app, double);

    /* memory alocation */
    self->coordinate = (Vector2D*) calloc(1, sizeof(Vector2D));
    self->velocity = (Vector2D*) calloc(1, sizeof(Vector2D));
    self->force = (Vector2D*) calloc(1, sizeof(Vector2D));

    self->coordinate->x = va_arg(*app, double);
    self->coordinate->y = va_arg(*app, double);
    self->velocity->x = va_arg(*app, double);
    self->velocity->y = va_arg(*app, double);
    self->force->x = va_arg(*app, double);
    self->force->y = va_arg(*app, double);
    return self;
}

static void* Atom_destructor(void *_self){
    Atom_t *self = _self;
    free(self->coordinate);
    free(self->velocity);
    free(self->force);
    return self;
}

static const struct Class _Atom = {
        sizeof(Atom_t),
        Atom_constructor,
        Atom_destructor
};

const void * Atom = &_Atom;



/* physical calculation */
/*
 * логика следующая - мы сумируем взаимодействие частиц, считаем результирующую силу
 * через второй закон ньютона - интегрируем уравнение движения в самом простейшим виде
 * без привлечения точных численных методов
 *
 *
 */

void PhysicalInteration(Atom_t *dest, Atom_t *source, void* _PhyLaw){
    Vector2D (*PhyLaw)(Atom_t*, Atom_t*) = _PhyLaw; // cast to pointer to function
    Vector2D result = PhyLaw(dest, source);
    dest->force->x += result.x;
    dest->force->y += result.y;
}

void PhysicalMovement(double dt, Atom_t* particle){
    /*
     *  прописываем простое кинематическое уравнение
     *  r(T+dt) = r(T) + V(T)*dt + a(T)*dt**2 / 2
     *  V(T+dt) = V(T) + a(T)*dt
     *  where a(T) -aceleration calculated by Newton's second law   a(T) = F(T) / M
     *
     *  all equations represented here in vector form with Vector2d
     *
     */
    Vector2D acceleration = {particle->force->x / particle->M,
                             particle->force->y / particle->M};
    /* ugly but simple, dimention wise computation*/
    particle->coordinate->x = particle->coordinate->x + particle->velocity->x*dt + acceleration.x*dt*dt/2;
    particle->coordinate->y = particle->coordinate->y + particle->velocity->y*dt + acceleration.y*dt*dt/2;

    particle->velocity->x = particle->velocity->x + acceleration.x*dt;
    particle->velocity->y = particle->velocity->y + acceleration.y*dt;
}

void ForceZerofication(Atom_t* particle){
    particle->force->x = 0.;
    particle->force->y = 0.;
}

Vector2D LawOne(Atom_t* dest, Atom_t* source){
    /*
     * вторая сила
     */
    Vector2D result;
    double r = sqrt(SQR(dest->coordinate->x - source->coordinate->x) +
            SQR(dest->coordinate->y - source->coordinate->y));

    result.x = (dest->coordinate->x - source->coordinate->x) / r;
    result.y = (dest->coordinate->y - source->coordinate->y) / r;

    return result;
}
