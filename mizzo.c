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
#include "mizzo.h"
#include "consumer.h"
#include "producer.h"

#define CAPACITY 100;   //Capacity Limit of candy factory

int candy = 0;          //Specifies candy being consumed
int enqueueFlag = 0;    //If placing on belt was successful

//Queue Variables
int size = 0;           
int inp_arr[100];
int Rear = - 1;
int Front = - 1;

int main(int argc, char **argv) {
    char Option;        

    //Initialize Belt
    Belt *belt = malloc(sizeof(Belt));
    setUpBelt(belt);
    
    //Initialize linking consumers/producers to Belt
    Name *lucy = malloc(sizeof(Name));
    lucy->beltTest = belt;
    lucy->name = 0;

    Name *ethel = malloc(sizeof(Name));
    ethel->beltTest = belt;
    ethel->name = 1;

    Name *frogs = malloc(sizeof(Name));
    frogs->beltTest = belt;
    frogs->name = 0;

    Name *suckers = malloc(sizeof(Name));
    suckers->beltTest = belt;
    suckers->name = 1;    

    while ((Option = getopt(argc, argv, "E:L:f:e:")) != -1) {   //Parse command line 
        //Save delay for specific producer/consumer  
        switch (Option) {

        case 'E':       
        ethel->delay = (unsigned)atoi(optarg);                 
        break;

        case 'e':   
        suckers->delay = (unsigned)atoi(optarg);
        break;

        case 'f':   
        frogs->delay = (unsigned)atoi(optarg);
        break;

        case 'L':   
        lucy->delay = (unsigned)atoi(optarg);
        break;

        default:
        exit(0);

        }
    }

    //Initialize threads
    pthread_t thread[4];

    pthread_create(&thread[0], NULL, producer, (void *)frogs);
    pthread_create(&thread[1], NULL, producer, (void *)suckers);
    pthread_create(&thread[2], NULL, consumer, (void *)lucy);
    pthread_create(&thread[3], NULL, consumer, (void *)ethel);


    //Exit threads on completion
    int i = 0;
    for (i = 0; i < 4; i++) {
        void *p;
        pthread_join(thread[i], &p);
    }

    //Production Report
    belt->consumed[0] = belt->ConsumedLucy;
    belt->consumed[1] = belt->ConsumedEthel;
    io_production_report(belt->Produced, belt->consumed);

    return 0;       
}

Belt *setUpBelt(Belt *beltName) {       //Constructor for Belt
    beltName->limit = 100;
    beltName->processedProducer = 0;
    beltName->processedConsumer = 0;
    beltName->OnBelt[0] = 0;
    beltName->OnBelt[1] = 0;
    beltName->Produced[0] = 0;
    beltName->Produced[1] = 0;
    beltName->ConsumedLucy[0] = 0;
    beltName->ConsumedLucy[1] = 0;
    beltName->ConsumedEthel[0] = 0;
    beltName->ConsumedEthel[1] = 0;
    beltName->consumed[0] = beltName->ConsumedLucy;
    beltName->consumed[1] = beltName->ConsumedEthel;
    sem_init(&beltName->mutex, 0, 1);
    sem_init(&beltName->empty, 0, 0);
    sem_init(&beltName->full, 0, 10);
    sem_init(&beltName->max_frogs, 0, 3);
    return NULL;
}

void enqueue(int num)       //Add to queue
{    
    int beltSpace = 10;     
    if (size == beltSpace) {
        enqueueFlag = 1;
    }
    else {
        enqueueFlag = 0;

        Rear = (Rear + 1) % CAPACITY;

        // Increment queue size
        size++;  //acts as counter

        // Enqueue new element to queue
        inp_arr[Rear] = num;
    }
   
} 

int dequeue()       //Remove candy from queue
{
    if (size == 0)
    {        
        return 0;
    }
    else {
        candy  = inp_arr[Front];        

    // Ensure front never crosses array bounds
        Front = (Front + 1) % CAPACITY;

    // Decrease queue size
        size--;

        return 1;
    }
   
}