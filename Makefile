# Makefile for main.c
mdsq10socket: main.c
        gcc -Wall -W -O3 -o mdsq10socket main.c
		
		
末尾にオプション追加
ソケットを使うとき：-lpthread または -pthread