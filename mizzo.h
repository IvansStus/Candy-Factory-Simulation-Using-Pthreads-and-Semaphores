/*
Ivan Stus - 821201908
Kenny Kieu - 819180993
CS 570-05
Spring 2021
Assignment #4 - Candy Factory using Semaphores and Threads
Description - Simulate a candy factory production line with a belt space of 10, with no more than 3 frog bites
on the belt. Limit of 100. 
*/
#ifndef MIZZO_H
#define MIZZO_H

#include <unistd.h>
#include <ctype.h>
#include <semaphore.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include "production.h"
#include "io.h"

//Struct Definitions
typedef struct Belt {
    int OnBelt[2];
    int Produced[2];
    int ConsumedLucy[2];
    int ConsumedEthel[2];
    int *consumed[2];
    int limit;
    int processedProducer;
    int processedConsumer;
    sem_t mutex;
    sem_t empty;
    sem_t full;
    sem_t max_frogs;
} Belt;

typedef struct Name {
    struct Belt *beltTest;
    size_t delay;
    int name;
} Name;

//Function Definitions
Belt *setUpBelt(Belt *beltName);
void enqueue(int num);
int dequeue();

#endif




