/*
Ivan Stus - 821201908
Kenny Kieu - 819180993
CS 570-05
Spring 2021
Assignment #4 - Candy Factory using Semaphores and Threads
Description - Simulate a candy factory production line with a belt space of 10, with no more than 3 frog bites
on the belt. Limit of 100. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sched.h>
#include <unistd.h>
#include <ctype.h>
#include <semaphore.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include "producer.h"
#include "mizzo.h"

extern int enqueueFlag;

void *producer(void *arg) {
    int producerFlag = 0;       //Local Flag, checks if produced <= 100

    Name *producer = (Name*)arg;    //Initialize Producer

    struct timespec delay = {producer->delay / 1000, (producer->delay % 1000) * 1000000};       //Grab producer delay
    
    while (1) {
        nanosleep(&delay, NULL);

        if (producer->name == FrogBite) {         
            sem_wait(&producer->beltTest->max_frogs);       //Decrease frog space
        }

        sem_wait(&producer->beltTest->full);        //Decrease Belt space 
        sem_wait(&producer->beltTest->mutex);       //Wait for access to memory

        //If 100 produced, exit thread
        if (producer->beltTest->processedProducer >= producer->beltTest->limit) {
            producerFlag = 0;
        }
        else {
            enqueue(producer->name);        //Add candy to belt queue
            if (enqueueFlag == 0) {     //Candy successfully placed on belt
                if (producer->name == 0) {      //Frog produced
                    producer->beltTest->Produced[0]++;
                    io_add_type(FrogBite, producer->beltTest->OnBelt, producer->beltTest->Produced);
                } else {    //Sucker produced
                    producer->beltTest->Produced[1]++;
                    io_add_type(Escargot, producer->beltTest->OnBelt, producer->beltTest->Produced);
                }
                producerFlag = 1;   //Produce success
            }
        }

        sem_post(&producer->beltTest->mutex);   //Access memory

        if (producerFlag == 1) {        //Successful production
            sem_post(&producer->beltTest->empty);       //Candy on belt
            producer->beltTest->processedProducer++;
            if (producer->name == FrogBite) {
                producer->beltTest->OnBelt[0]++;
            } else {
                producer->beltTest->OnBelt[1]++;
            }
        }
        else {
            return NULL;
        }
        
    }
}