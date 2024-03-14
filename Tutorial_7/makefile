# Makefile for all T7 programs

q1 q2 q3 q4 q5 signals: Q1T7.c Q2T7.c Q3T7.c Q4T7.c Q5T7.c sigtrap.c
	gcc sigtrap.c -o process
	gcc Q1T7.c -o q1.o
	gcc Q2T7.c -o q2.o
	gcc Q3T7.c -o q3.o
	gcc Q4T7.c -o q4.o
	gcc Q5T7.c -o q5.o

clean:
	rm -f q1.o q2.o q3.o q4.o q5.o signals
