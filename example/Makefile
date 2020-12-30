CC=gcc
CFLAGS=-Wall

examples: etq list_all etq-json

etq: etq.c
	$(CC) $(CLAGS) -o etq etq.c ../et-server-query.a

list_all: list_all.c
	$(CC) $(CLAGS) -o list_all list_all.c ../et-server-query.a

etq-json: etq-json.c
	$(CC) $(CLAGS) -o etq-json etq-json.c ../et-server-query.a

clean:
	rm -f etq list_all etq-json
