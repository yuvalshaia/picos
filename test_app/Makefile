# MPLAB IDE generated this makefile for use with GNU make.
# Project: test_app.mcp
# Date: Fri Nov 09 00:18:44 2012

AS = MPASMWIN.exe
CC = mcc18.exe
LD = mplink.exe
AR = mplib.exe
RM = rm

bin/test_app.cof : obj/main.o
	$(LD) /p18F2455 "obj\main.o" "C:\Users\yuval\Dropbox\Dev\PIC\MyProj\pic18-os\pic18-os0.6.1\kernel\bin\kernel.lib" /u_CRUNTIME /u_DEBUG /z__MPLAB_BUILD=1 /z__MPLAB_DEBUG=1 /o"bin\test_app.cof" /M"bin\test_app.map" /W

obj/main.o : main.c ../kernel/include/task.h ../kernel/include/os.h ../kernel/include/mutex.h main.c ../../../../../../../../../Program\ Files/Microchip/mplabc18/v3.40/h/p18cxxx.h ../../../../../../../../../Program\ Files/Microchip/mplabc18/v3.40/h/p18f2455.h
	$(CC) -p=18F2455 /i"../kernel/include" "main.c" -fo="obj\main.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

clean : 
	$(RM) "obj\main.o" "bin\test_app.cof" "bin\test_app.hex"

