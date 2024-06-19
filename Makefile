#CC           = avr-gcc
#CFLAGS       = -Wall -mmcu=atmega16 -Os -Wl,-Map,test.map
#OBJCOPY      = avr-objcopy
CC           = gcc
LD           = gcc
AR           = ar
ARFLAGS      = rcs
CFLAGS       = -Wall -Os -c
LDFLAGS      = -Wall -Os -Wl,-Map,test.map
ifdef AES192
CFLAGS += -DAES192=1
endif
ifdef AES256
CFLAGS += -DAES256=1
endif

OBJCOPYFLAGS = -j .text -O ihex
OBJCOPY      = objcopy

# include path to AVR library
INCLUDE_PATH = /usr/lib/avr/include
# splint static check
SPLINT       = splint test.c aes.c -I$(INCLUDE_PATH) +charindex -unrecog

default: gen_ecb.elf

.SILENT:
.PHONY:  lint clean

test.hex : test.elf
	echo copy object-code to new image and format in hex
	$(OBJCOPY) ${OBJCOPYFLAGS} $< $@

test.o : test.c aes.h aes.o
	echo [CC] $@ $(CFLAGS)
	$(CC) $(CFLAGS) -o  $@ $<

aes.o : aes.c aes.h
	echo [CC] $@ $(CFLAGS)
	$(CC) $(CFLAGS) -o $@ $<

base64.o : base64.c base64.h
	echo [CC] $@ $(CFLAGS)
	$(CC) $(CFLAGS) -o $@ $<

test.elf : aes.o test.o
	echo [LD] $@
	$(LD) $(LDFLAGS) -o $@ $^

aes.a : aes.o
	echo [AR] $@
	$(AR) $(ARFLAGS) $@ $^

lib : aes.a

clean:
	rm -f *.OBJ *.LST *.o *.gch *.out *.hex *.map *.elf *.a

test:
	make clean && make test.elf && ./test.elf
	# make clean && make AES192=1 && ./test.elf
	# make clean && make AES256=1 && ./test.elf

gen_ecb.o : gen_ecb.c aes.h aes.o base64.h base64.o
	echo [CC] $@ $(CFLAGS)
	$(CC) $(CFLAGS) -o  $@ $<
gen_ecb.elf : aes.o base64.o gen_ecb.o
	echo [LD] $@
	$(LD) $(LDFLAGS) -o $@ $^
get_ecb.o : get_ecb.c aes.h aes.o base64.h base64.o
	echo [CC] $@ $(CFLAGS)
	$(CC) $(CFLAGS) -o  $@ $<
get_ecb.elf : aes.o base64.o get_ecb.o
	echo [LD] $@
	$(LD) $(LDFLAGS) -o $@ $^
gen:
	make clean && make gen_ecb.elf && ./gen_ecb.elf
get:
	make clean && make get_ecb.elf && ./get_ecb.elf

all:
	gcc -o gen gen_ecb.c aes.c base64.c -I.
	gcc -o get get_ecb.c aes.c base64.c -I.

lint:
	$(call SPLINT)
