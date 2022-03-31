detect: detect.c structs.c structs.h
	gcc -Wall -o detect detect.c structs.c -g