INCDIR=./include
_DEPS=spi.h usart.h main.h
DEPS=$(patsubst %,$(INCDIR)/%,$(_DEPS))

CC=avr-gcc
CFLAGS=-I$(INCDIR) -mmcu=atmega328p -Os
LIBS=-lm

ODIR=./obj
_OBJ=spi.o usart.o main.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

ELF=main.elf

CONVERTER=avr-objcopy
CONVFLAGS=-O ihex -R .eeprom
HEX=main.hex
INSTALLPATH=../out

INSTALLER=avrdude
IFLAGS=-c arduino -p ATMEGA328P -b 115200 -P
PORT=/dev/ttyACM0

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)	

all: $(OBJ)
	$(CC) -o $(ELF) $^ $(CFLAGS) $(LIBS)
	$(CONVERTER) $(CONVFLAGS) $(ELF) $(HEX)
	cp $(HEX) $(INSTALLPATH)

install:
	$(INSTALLER) $(IFLAGS) $(PORT) -U flash:w:$(HEX)

clean:
	rm -f $(ODIR)/*.o *~  core $(ELF) $(HEX)
