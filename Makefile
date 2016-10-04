all:
	@gcc -std=c99 Adj_List.c coloringMap.c -o colMap

run:
	@./colMap