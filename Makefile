# smart-bike-light makefile
# adapted from a VUSB example project's makefile
# matt ruffner 2016

DEVICE  = attiny2313
F_CPU   = 8000000
FUSE_L  = 0xE4
FUSE_H  = 0x9F
AVRDUDE = avrdude -c usbtiny -P usb -p $(DEVICE)

CFLAGS  = -DDEBUG_LEVEL=0
OBJECTS =  synth.o

COMPILE = avr-gcc -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE)
COMPILEPP = avr-g++ -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE)

# symbolic targets:
help:
	@echo "This Makefile has no default rule. Use one of the following:"
	@echo "make hex ....... to build the firmware"
	@echo "make program ... to flash fuses and firmware"
	@echo "make fuse ...... to flash the fuses"
	@echo "make flash ..... to flash the firmware"
	@echo "make clean ..... to delete objects and hex file"

hex: synth synth.hex

program: flash fuse

# rule for programming fuse bits:
fuse:
	@[ "$(FUSE_H)" != "" -a "$(FUSE_L)" != "" ] || \
                { echo "*** Edit Makefile and choose values for FUSE_L and FUSE_H!"; exit 1; }
	$(AVRDUDE) -U hfuse:w:$(FUSE_H):m -U lfuse:w:$(FUSE_L):m
read:
	$(AVRDUDE) -U hfuse:r:high.txt:r -U lfuse:r:low.txt:r

# rule for uploading firmware:
flash: synth.hex
	$(AVRDUDE) -U flash:w:synth.hex:i

# rule for deleting dependent files (those which can be built by Make):
clean:
	rm -f *.o *.hex *.elf

# Generic rule for compiling C files:
.c.o:
	$(COMPILE) -c $< -o $@

# Generic rule for assembling Assembler source files:
.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

# Generic rule for compiling C to assembler, used for debugging only.
.c.s:
	$(COMPILE) -S $< -o $@

# file targets:

# Since we don't want to ship the driver multipe times, we copy it into this project:
#usbdrv:
#	cp -r ../../../usbdrv .

synth.elf: $(OBJECTS)	# usbdrv dependency only needed because we copy it
	$(COMPILE) -o synth.elf $(OBJECTS)

synth.hex: synth.elf
	rm -f synth.hex synth.eep.hex
	avr-objcopy -j .text -j .data -O ihex synth.elf synth.hex
	avr-size synth.hex

# debugging targets:

disasm:	synth.elf
	avr-objdump -d synth.elf