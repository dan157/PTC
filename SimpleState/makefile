all: State1 BlinkLed
 
# $@ matches the target; $< matches the first dependent
BlinkLed:	BlinkLed.o
	cc -o $@ $< -L/usr/local/lib -lwiringPi

BlinkLed.o: BlinkLed.c
	cc -c $<

State1: State1.o
	cc -o $@ $< -L/usr/local/lib -lwiringPi

State1.o: State1.c State1.h
	cc -c $<
     
clean:
	rm State1.o State1 BlinkLed.o BlinkLed
