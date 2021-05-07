run1:
	./ex1
run2:
	./ex2

compil1: 
	 gcc -o ex1 ex1.c 

compil2: 
	 gcc -o ex2 ex2.c 

clean: 
	rm -f ex1
	rm -f ex2
	