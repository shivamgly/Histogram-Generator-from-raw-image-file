all: histogram.c
	gcc histogram.c -o histogram 
clean: histogram
	rm histogram