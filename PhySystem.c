//
// Created by alpharius on 18.07.2020.
//

#include "Class.h"
#include <stdlib.h>
#include <time.h>

#include "Atom.h"
#include "MathCore.h"
#include "new.h"
#include "PhySystem.h"
#include <zconf.h>


static void force_computation(Sys_t* _sys);
static void border_collision(Sys_t* _sys);
static void movement_solution(Sys_t* _sys);
static void get_statistic(Sys_t* _sys, FILE* gnu);

Sys_t* create_sys(size_t number_of_particles, double dt){
    srand(time(NULL));

    Sys_t* result = calloc(1, sizeof(Sys_t));
    result->amount_of_particles = number_of_particles;
    result->dt = dt;
    result->aptr = calloc(number_of_particles, sizeof(Atom_t));
    Atom_t* atom_ptr = result->aptr;

    double x_cord, y_cord, v_x, v_y;
    /*
     * 1
     * |
     * |  *    *
     * |    *
     * |  *    .
     * 0------------------1
     */

    for(size_t i=0; i< number_of_particles; i++){

         x_cord = (double)rand()/RAND_MAX;
         y_cord = (double)rand()/RAND_MAX;
         v_x = (double)rand()/RAND_MAX * 10. - 5.;
         v_y = (double)rand()/RAND_MAX * 10. - 5;
         *atom_ptr = *(Atom_t*) new(Atom, 1., x_cord, y_cord, v_x, v_y, 0., 0.);
         atom_ptr++;
    }

    return result;
}

double E_Kin(const Sys_t* _sys){
    double K = 0;
    for(size_t i=0; i<_sys->amount_of_particles; i++){
        K += _sys->aptr[i].M * (
                SQR(_sys->aptr[i].velocity->x) +
                SQR(_sys->aptr[i].velocity->y)
                               ) / 2;
    }
    return K;
}

void RunSystem(Sys_t* _sys){
    FILE * gnuplotPipe = popen ("gnuplot ", "w");
    while (1){
        /* расчет сил */
        force_computation(_sys);

        /* учет коллизий  со стенкой и отскоки*/
        border_collision(_sys);

        /* решение уравнений движения */
        movement_solution(_sys);

        /* сбор статистики */
        get_statistic( _sys, gnuplotPipe);

    }
}

static void force_computation(Sys_t* _sys){
    /* обнуление сил */
    for(size_t i=0; i< _sys->amount_of_particles; i++){
        ForceZerofication(&_sys->aptr[i]);
    }

    /* сумирование сил */
    for(size_t i=0; i < _sys->amount_of_particles; i++){
        for(size_t j=0; j< _sys->amount_of_particles; j++){
            if (i == j) continue;

            Vector2D F_ij = LawOne(&_sys->aptr[i], &_sys->aptr[j]);
            _sys->aptr[i].force->x = F_ij.x;
            _sys->aptr[i].force->y = F_ij.y;
        }
    }
}

static void border_collision(Sys_t* _sys){
    /* пробегаем по всем частицам
     * у нас есть отражение по скоростям и перемещение координат до ближайшей стенки
     * условие колизии
     * r + v*dt > боковая сторона
     * */
    double dt = _sys->dt;
    for(size_t i=0; i < _sys->amount_of_particles; i++){
        Atom_t particle = _sys->aptr[i];
        if (particle.coordinate->x + particle.velocity->x*dt > 1 ||
            particle.coordinate->x + particle.velocity->x*dt < 0
        ) {
            if (particle.coordinate->x + particle.velocity->x*dt > 1) particle.coordinate->x = 1;
            else particle.coordinate->x = 0;
            particle.velocity->x *= -1;
        }

        if (particle.coordinate->y + particle.velocity->y*dt > 1 ||
            particle.coordinate->y + particle.velocity->y*dt < 0
        ){

            if (particle.coordinate->y + particle.velocity->y*dt >1) {
                particle.coordinate->y = 1;
            }
            else particle.coordinate->y = 0;
            particle.velocity->y *= -1;
        }
    }
}

static void movement_solution(Sys_t* _sys){
    /*
     * r(T+dt) = r(T) + V(T)*dt + a(T)*dt**2/2
     * V(T+dt) = V(T) + a(T)*dt;
     */
#define NEW_COORD(D) particle.coordinate->D = particle.coordinate->D + particle.velocity->D * dt\
    + particle.force->D / M * SQR(dt) / 2.
#define NEW_VELOCITY(D) particle.velocity->D = particle.velocity->D  \
    + particle.force->D / M * dt / 2.

    double dt = _sys->dt;
    for(size_t i=0; i < _sys->amount_of_particles; i++){
        Atom_t particle = _sys->aptr[i];
        double M = particle.M;
        NEW_COORD(x);
        NEW_COORD(y);
        NEW_VELOCITY(x);
        NEW_VELOCITY(y);
    }
#undef NEW_COORD
#undef NEW_VELOCITY
}

static void get_statistic(Sys_t* _sys, FILE* gnu){
    double K = E_Kin(_sys);
    printf("E = %lf \n" , K);
    printf("coord_0 = {%lf, %lf}\n" , _sys->aptr->coordinate->x,_sys->aptr->coordinate->y);

    FILE * gnuplotPipe = gnu ;popen ("gnuplot ", "w");
    if (gnuplotPipe) {
        fprintf(gnuplotPipe, "set xrange [0:1] \n");
        fprintf(gnuplotPipe, "set yrange [0:1] \n");
        fprintf(gnuplotPipe, "plot '-'\n");
        for(size_t i=0; i<_sys->amount_of_particles; i++){
            fprintf(gnuplotPipe, "%lf %lf \n", _sys->aptr[i].coordinate->x,
                    _sys->aptr[i].coordinate->y);
        }
        fprintf(gnuplotPipe, "e\n");
        fflush(gnuplotPipe);

        fprintf(gnuplotPipe, "refresh\n");
        //pclose(gnuplotPipe);
    }
}
