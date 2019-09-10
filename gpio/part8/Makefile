# Typical values for compiling natively on Raspberry Pi
CXX    = g++
CFLAGS = 

# Typical values for cross-compiling for Toradex
#CXX    = arm-angstrom-linux-gnueabi-g++ -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=hard -mcpu=cortex-a9 --sysroot=/opt/oecore-x86_64/sysroots/cortexa9t2hf-neon-angstrom-linux-gnueabi
#CFLAGS = -O2 -pipe -g -feliminate-unused-debug-types

all: demo1 demo2

demo1: demo1.cpp
	${CXX} ${CFLAGS} -o demo1 demo1.cpp -lsoc

demo2: demo2.cpp
	${CXX} ${CFLAGS} -o demo2 demo2.cpp -lsoc

clean:
	$(RM) demo1 demo2
