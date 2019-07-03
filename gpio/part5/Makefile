all: demo1 demo2 demo3 demo4

demo1:  demo1.c
	gcc -Wall -O2 -o demo1 demo1.c

demo2:  demo2.c
	gcc -Wall -o demo2 demo2.c -lwiringPi

demo3:  demo3.c
	gcc -Wall -o demo3 demo3.c -lwiringPi

demo4:  demo4.c
	gcc -Wall -o demo4 demo4.c -lwiringPi

clean:
	$(RM) demo1 demo2 demo3 demo4
