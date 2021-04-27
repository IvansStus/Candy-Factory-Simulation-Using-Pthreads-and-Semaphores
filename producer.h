/*
Ivan Stus - 821201908
Kenny Kieu - 819180993
CS 570-05
Spring 2021
Assignment #4 - Candy Factory using Semaphores and Threads
Description - Simulate a candy factory production line with a belt space of 10, with no more than 3 frog bites
on the belt. Limit of 100. 
*/
#ifndef PRODUCER_H
#define PRODUCER_H

#include <unistd.h>
#include <ctype.h>
#include <semaphore.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include "production.h"
#include "io.h"

void *producer(void *arg);

#endif

