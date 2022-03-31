EXE_DETECT=detect

$(EXE_DETECT): detect.c structs.c structs.h
	gcc -Wall -o detect detect.c structs.c -g
clean:
	rm -f *.o $(EXE_DETECT)