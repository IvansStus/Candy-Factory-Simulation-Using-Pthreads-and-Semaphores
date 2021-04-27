# Ivan Stus - 821201908
# Kenny Kieu - 819180993
# CS 570-05
# Spring 2021
# Assignment #4 - Candy Factory using Semaphores and Threads
# Description - Simulate a candy factory production line with a belt space of 10, with no more than 3 frog bites
# on the belt. Limit of 100.

# CXX Make variable for compiler
###CXX=gcc
# Make variable for compiler options
#    -std=c++11  C/C++ variant to use, e.g. C++ 2011
#    -g          include information for symbolic debugger e.g. gdb 
#CXXFLAGS=-std=c++11 -g

# Rules format:
# target : dependency1 dependency2 ... dependencyN
#     Command to make target, uses default rules if not specified

# First target is the one executed if you just type make
# make target specifies a specific target
# $^ is an example of a special variable.  It substitutes all dependencies

CC=	gcc

CFLAGS=	-c -Wall

mizzo: mizzo.o	io.o	consumer.c	producer.c
	$(CC)	mizzo.o	io.o	consumer.c	producer.c	$(LFLAGS)	-o	mizzo	-lm	-lpthread	-lrt

mizzo.o: mizzo.c	io.c	consumer.c	producer.c	
	$(CC) $(CFLAGS) mizzo.c	io.c	consumer.c	producer.c

clean:
	$(RM) *.o mizzo