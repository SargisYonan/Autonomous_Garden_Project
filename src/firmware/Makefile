AVR_GCC_CONTROLLER           = atmega2560
BURNER						 = avrdude
AVRDUDE_CONTROLLER 	         = m2560
CC							 = avr-gcc
CSOURCE						 = main.c UART/uart.c
#${CSOURCE:.c=.o}
OBJECTS  					 = main.o uart.o
EXECUTABLE					 = main
COMPILER_PATH				 = /dev/tty.usbmodemFA131
#cu.usbmodemfd121
COMPILER  					 = stk500v2
BAUDRATE					 = 115200
OBJECT_COPY					 = avr-objcopy
CFLAGS						 = -Wall -Werror -Wextra -Wimplicit -std=gnu99
PHASE_ONE					 = -Os -mmcu=${AVR_GCC_CONTROLLER} -DF_CPU=16000000UL ${CFLAGS} -c
PHASE_TWO					 = -mmcu=${AVR_GCC_CONTROLLER} -Wl,-u,vfprintf -lprintf_flt -lm -o${EXECUTABLE}.elf ${OBJECTS}

all:
	${CC} ${PHASE_ONE} ${CSOURCE}
	${CC} ${PHASE_TWO}
	${OBJECT_COPY} -j .text -j .data -O ihex ${EXECUTABLE}.elf ${EXECUTABLE}.hex
	${BURNER} -P ${COMPILER_PATH} -p ${AVRDUDE_CONTROLLER} -c ${COMPILER} -e -b ${BAUDRATE} -U flash:w:${EXECUTABLE}.hex 
	rm -f ${OBJECTS} ${EXECUTABLE}.elf ${EXECUTABLE}.hex
clean:
	rm -f ${OBJECTS} ${EXECUTABLE}.hex ${EXECUTABLE}.elf
push:
	git push -u origin master