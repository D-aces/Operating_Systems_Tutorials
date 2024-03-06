# Makefile for all T7 programs

q1 q2 q3 q4 q5 signals: Q1T7.c Q2T7.c Q3T7.c Q4T7.c Q5T7.c sigtrap.c
	gcc sigtrap.c -o signals
	gcc Q1T7.c -o q1
	gcc Q2T7.c -o q2
	gcc Q3T7.c -o q3
	gcc Q4T7.c -o q4
	gcc Q5T7.c -o q5

clean:
	rm -f q1 q2 q3 q4 q5 signals
