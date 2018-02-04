# MPLAB IDE generated this makefile for use with GNU make.
# Project: test4.mcp
# Date: Wed Oct 05 22:17:49 2011

AS = MPASMWIN.exe
CC = mcc18.exe
LD = mplink.exe
AR = mplib.exe
RM = rm

test4.cof : main.o
	$(LD) /p18F2455 /l"C:\Program Files\Microchip\mplabc18\v3.40\lib" "rm18f2455.lkr" "main.o" /u_CRUNTIME /z__MPLAB_BUILD=1 /o"test4.cof" /M"test4.map" /W

main.o : main.c ../../../../../../../Program\ Files/Microchip/mplabc18/v3.40/h/delays.h main.c ../../../../../../../Program\ Files/Microchip/mplabc18/v3.40/h/p18cxxx.h ../../../../../../../Program\ Files/Microchip/mplabc18/v3.40/h/p18f2455.h
	$(CC) -p=18F2455 "main.c" -fo="main.o" -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

clean : 
	$(RM) "main.o" "test4.cof" "test4.hex"

