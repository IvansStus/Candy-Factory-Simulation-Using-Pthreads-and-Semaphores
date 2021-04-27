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
#include "consumer.h"
#include "mizzo.h"

extern int candy;       //Specifies candy being consumed

void *consumer(void *arg) {
    int consumerFlag = 0;       //Stop at limit flag, end consumption

    Name *consumer = (Name*)arg;        //Initialize consumer
    
    struct timespec delay = {consumer->delay / 1000, (consumer->delay % 1000) * 1000000};       //Grab consumer delay

    while (1) {
        nanosleep(&delay, NULL);    

        //If 100 consumed, exit thread
        if (consumer->beltTest->processedConsumer >= consumer->beltTest->limit) {   
            consumerFlag = 0;
        }
        else {      //Continue consuming
            sem_wait(&consumer->beltTest->empty);   //Wait for candy to be in belt
            sem_wait(&consumer->beltTest->mutex);   //Waits for access to memory
            consumerFlag = dequeue();       //Remove candy from belt
            if (consumer->name == 0) {      //If consumer is Lucy
                if (candy == 0) {       //Lucy consumed frog
                    consumer->beltTest->ConsumedLucy[0]++;      
                    io_remove_type(Lucy, FrogBite, consumer->beltTest->OnBelt, consumer->beltTest->ConsumedLucy);
                } else {        //Lucy consumed sucker
                    consumer->beltTest->ConsumedLucy[1]++;
                    io_remove_type(Lucy, Escargot, consumer->beltTest->OnBelt, consumer->beltTest->ConsumedLucy);
                }            
            } else {
                if (candy == 0) {   //Ethel consumed frog
                    consumer->beltTest->ConsumedEthel[0]++;
                    io_remove_type(Ethel, FrogBite, consumer->beltTest->OnBelt, consumer->beltTest->ConsumedEthel);
                } else {            //Ethel consumed sucker
                    consumer->beltTest->ConsumedEthel[1]++;
                    io_remove_type(Ethel, Escargot, consumer->beltTest->OnBelt, consumer->beltTest->ConsumedEthel);
                }
            }
        } 

        sem_post(&consumer->beltTest->mutex);       //Access memory

        if (consumerFlag == 1) {        //If consumed was successful
            sem_post(&consumer->beltTest->full);        //Increase Belt Space
            if (candy == 0) {       //Increase frog limit
                sem_post(&consumer->beltTest->max_frogs);
                consumer->beltTest->OnBelt[0]--;
            }
            else {      
                consumer->beltTest->OnBelt[1]--;
            }
            consumer->beltTest->processedConsumer++;
        }
        else {
            return NULL;
        }
    }
}

