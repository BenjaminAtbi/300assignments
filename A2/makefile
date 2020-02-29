test-snd: test.c client.c client.h
	gcc -o s-snd-test client.c test.c 

test-recv: testRecv.c client.c client.h
	gcc -o s-recv-test client.c testRecv.c 

test-read: testRead.c client.c client.h
	gcc -o s-read-test client.c testRead.c

test-write: testWrite.c client.c client.h
	gcc -o s-write-test client.c testWrite.c

clean :
		-rm s-*
