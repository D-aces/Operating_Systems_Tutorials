# Makefile for all T7 programs

q1 q2 q3 q4 q5 signals: Q1.c Q2.c Q3.c Q4.c Q5.c sigtrap.c
	gcc sigtrap.c -o process
	gcc Q1.c -o q1.o
	gcc Q2.c -o q2.o
	gcc Q3.c -o q3.o
	gcc Q4.c -o q4.o
	gcc Q5.c -o q5.o

clean:
	rm -f q1 q2 q3 q4 q5 signals
