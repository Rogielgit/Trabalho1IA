all:
	@gcc -std=c99 Adj_List.c utils.c coloringMap.c -o colMap

run:
	@./colMap
