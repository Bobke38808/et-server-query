et-server-query.a: et-server-query.o
	ar rcs $@ $^

clean:
	rm -f et-server-query.o et-server-query.a
