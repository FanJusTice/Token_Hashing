Token: test.o Token_hashing.o hash.o
	cc -o Token test.o Token_hashing.o hash.o -lm -g

test.o: test.c Token_hashing.h
	cc -c test.c -lm -g
Token_hashing.o : Token_hashing.c Token_hashing.h
	cc -c Token_hashing.c -lm -g
hash.o : hash.c hash.h
	cc -c hash.c -lm -g

clean:
	rm *.o Token
