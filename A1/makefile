test: list.c list.h test.c test.h
	gcc -o test list.c test.c

selftest: test.c test.h list.h
	gcc -o selftest test.c

clean: 
	rm -f test selftest *.o