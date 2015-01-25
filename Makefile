# Arduino 1.0 Makefile
# Based on Arduino 0018 Makefile
# last updated 12/15/2011, Kerry Wong
#
# Arduino adaptation by mellis, eighthave, oli.keller
#
# Modified by Kerry Wong to support NetBeans
# http://www.kerrywong.com
#
#
# This makefile allows you to build sketches from the command line
# without the Arduino environment (or Java).
#
# Detailed instructions for using the makefile:
#
#  1. Copy this file into the folder with your sketch. There should be a
#     file with the same name as the folder and with the extension .pde
#     (e.g. foo.pde in the foo/ folder).
#
#  2. Modify the line containg "INSTALL_DIR" to point to the directory that
#     contains the Arduino installation (for example, under Mac OS X, this
#     might be /Applications/arduino-0012).
#
#  3. Modify the line containing "PORT" to refer to the filename
#     representing the USB or serial connection to your Arduino board
#     (e.g. PORT = /dev/tty.USB0).  If the exact name of this file
#     changes, you can use * as a wildcard (e.g. PORT = /dev/tty.usb*).
#
#  4. Set the line containing "MCU" to match your board's processor.
#     Older one's are atmega8 based, newer ones like Arduino Mini, Bluetooth
#     or Diecimila have the atmega168.  If you're using a LilyPad Arduino,
#     change F_CPU to 8000000.
#
#  5. At the command line, change to the directory containing your
#     program's file and the makefile.
#
#  6. Type "make" and press enter to compile/verify your program.
#
#  7. Type "make upload", reset your Arduino board, and press enter to
#     upload your program to the Arduino board.
#
# $Id$

TARGET = $(notdir $(CURDIR))
OBJDIR = $(CURDIR)/applet
# Change this to match your arduino installation directory
INSTALL_DIR = $(HOME)/Downloads/Arduino.app/Contents/Resources/Java
PORT = /dev/cu.usbmodem1421
UPLOAD_RATE = 115200
AVRDUDE_PROGRAMMER = arduino
MCU = atmega328p
F_CPU = 16000000

VERSION=1.0
ARDUINO = $(INSTALL_DIR)/hardware/arduino/cores/arduino
VARIANTS = $(INSTALL_DIR)/hardware/arduino/variants/standard
# ARDUINO_LIB has moved and is broken into subdirectories. You may need to
# add each specific library to vpath
ARDUINO_LIB = $(INSTALL_DIR)/libraries
AVR_TOOLS_PATH = $(INSTALL_DIR)/hardware/tools/avr/bin
AVRDUDE_PATH = $(AVR_TOOLS_PATH)

#Note that if your program has dependencies other than those
#already listed below, you will need to add them accordingly.
C_MODULES =  \
wiring_pulse.c \
wiring_analog.c \
wiring.c \
wiring_digital.c \
WInterrupts.c \
wiring_shift.c \

CXX_MODULES = \
Tone.cpp \
WMath.cpp \
Print.cpp \
HardwareSerial.cpp \
CDC.cpp \
HID.cpp \
IPAddress.cpp \
new.cpp \
Stream.cpp \
USBCore.cpp \
WMath.cpp \
WString.cpp \
main.cpp \

CXX_APP = xmas.cpp
C_APP = FastOut.c
MODULES = $(C_MODULES) $(CXX_MODULES)
SRC = $(C_MODULES) $(C_APP)
CXXSRC = $(CXX_MODULES) $(CXX_APP)
FORMAT = ihex

# Define all object files.
OBJ_APP = $(C_APP:.c=.o) $(CXX_APP:.cpp=.o)
OBJ_APP_T = $(addprefix $(OBJDIR)/,$(notdir $(OBJ_APP)))
OBJ_MODULES = $(C_MODULES:.c=.o) $(CXX_MODULES:.cpp=.o)
OBJ_MODULES_T = $(addprefix $(OBJDIR)/,$(notdir $(OBJ_MODULES)))
CORE_OBJ = $(OBJDIR)/core.a
OBJ = $(OBJ_APP_T) $(OBJ_MODULES_T) $(CORE_OBJ)
# Name of this Makefile (used for "make depend").
MAKEFILE = Makefile

# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
#DEBUG = stabs
DEBUG =

OPT = s

# Place -D or -U options here
CDEFS = -DF_CPU=$(F_CPU)L -DARDUINO=$(VERSION)
CXXDEFS = -DF_CPU=$(F_CPU)L -DARDUINO=$(VERSION)

# Place -I options here
CINCS = -I$(ARDUINO)  -I$(VARIANTS) -I$(ARDUINO_LIB) -I.
CXXINCS = -I$(ARDUINO) -I$(VARIANTS) -I$(ARDUINO_LIB) -I.

# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
#CSTANDARD = -std=gnu99
CDEBUG = -g$(DEBUG)
#CWARN = -Wall -Wstrict-prototypes
#CWARN = -Wall   # show all warnings
CWARN = -w #suppress all warnings
####CTUNING = -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CTUNING = -ffunction-sections -fdata-sections
CXXTUNING = -fno-exceptions -ffunction-sections -fdata-sections
#CEXTRA = -Wa,-adhlns=$(<:.c=.lst)

CFLAGS = $(CDEBUG) -O$(OPT) $(CWARN) $(CTUNING) $(CDEFS) $(CINCS) $(CSTANDARD) $(CEXTRA)
CXXFLAGS = $(CDEBUG) -O$(OPT) $(CWARN) $(CXXTUNING) $(CDEFS) $(CINCS)
#ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs
LDFLAGS = -O$(OPT) -lm -Wl,--gc-sections


# Programming support using avrdude. Settings and variables.
AVRDUDE_PORT = $(PORT)
AVRDUDE_WRITE_FLASH = -U flash:w:$(OBJDIR)/main.hex

AVRDUDE_FLAGS = -V -F -C $(INSTALL_DIR)/hardware/tools/avr/etc/avrdude.conf \
-p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) \
-b $(UPLOAD_RATE) -D

# Program settings
CC = $(AVR_TOOLS_PATH)/avr-gcc
CXX = $(AVR_TOOLS_PATH)/avr-g++
LD = $(AVR_TOOLS_PATH)/avr-gcc
OBJCOPY = $(AVR_TOOLS_PATH)/avr-objcopy
OBJDUMP = $(AVR_TOOLS_PATH)/avr-objdump
AR  = $(AVR_TOOLS_PATH)/avr-ar
SIZE = $(AVR_TOOLS_PATH)/avr-size
NM = $(AVR_TOOLS_PATH)/avr-nm
AVRDUDE = $(AVRDUDE_PATH)/avrdude
REMOVE = rm -f
MV = mv -f

# Define all listing files.
LST = $(ASRC:.S=.lst) $(CXXSRC:.cpp=.lst) $(SRC:.c=.lst)

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = $(CFLAGS) -mmcu=$(MCU)
ALL_CXXFLAGS = $(CXXFLAGS) -mmcu=$(MCU)
ALL_ASFLAGS = -x assembler-with-cpp $(ASFLAGS) -mmcu=$(MCU)
ALL_LDFLAGS = $(LDFLAGS) -mmcu=$(MCU)
vpath %.cpp $(ARDUINO)
vpath %.c $(ARDUINO)
# Default target.
all: applet applet_files build sizeafter

build: elf hex

applet:
	test -d applet || mkdir applet

elf: $(OBJDIR)/main.elf
hex: $(OBJDIR)/main.hex
eep: $(OBJDIR)/main.eep
lss: $(OBJDIR)/main.lss
sym: $(OBJDIR)/main.sym

# Program the device.
upload: $(OBJDIR)/main.hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)

	# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(OBJDIR)/main.hex
ELFSIZE = $(SIZE)  $(OBJDIR)/main.elf
sizebefore:
	@if [ -f $(OBJDIR)/main.elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(HEXSIZE); echo; fi

sizeafter:
	@if [ -f $(OBJDIR)/main.elf ]; then echo; echo $(MSG_SIZE_AFTER); $(HEXSIZE); echo; fi


# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT=$(OBJCOPY) --debugging \
--change-section-address .data-0x800000 \
--change-section-address .bss-0x800000 \
--change-section-address .noinit-0x800000 \
--change-section-address .eeprom-0x810000


coff: $(OBJDIR)/main.elf
	$(COFFCONVERT) -O coff-avr $(OBJDIR)/main.elf $(OBJDIR)/main.cof


extcoff: $(OBJDIR)/main.elf
	$(COFFCONVERT) -O coff-ext-avr $(OBJDIR)/main.elf $(OBJDIR)/main.cof


.SUFFIXES: .elf .hex .eep .lss .sym

.elf.hex:
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

.elf.eep:
	$(OBJCOPY) -O $(FORMAT) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--no-change-warnings \
	--change-section-lma .eeprom=0 $< $@

# Create extended listing file from ELF output file.
.elf.lss:
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
.elf.sym:
	$(NM) -n $< > $@

# Link: create ELF output file from library.
#applet/$(TARGET).elf: $(TARGET).pde applet/core.a
$(OBJDIR)/main.elf: $(OBJ_APP_T) $(CORE_OBJ)
	$(LD) $(ALL_LDFLAGS) -o $@ $(OBJ_APP_T) $(CORE_OBJ) -lc

$(CORE_OBJ): $(OBJ_MODULES_T)
	@for i in $(OBJ_MODULES_T); do echo $(AR) rcs $(CORE_OBJ) $$i; $(AR) rcs $(CORE_OBJ) $$i; done


# Compile: create object files from C++ source files.
#.cpp.o:
$(OBJDIR)/%.o: %.cpp
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $@

# Compile: create object files from C source files.
#.c.o:
$(OBJDIR)/%.o: %.c
	$(CC) -c $(ALL_CFLAGS) $< -o $@


# Compile: create assembler files from C source files.
.c.s:
	$(CC) -S $(ALL_CFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
.S.o:
	$(CC) -c $(ALL_ASFLAGS) $< -o $@


# Automatic dependencies
%.d: %.c
	$(CC) -M $(ALL_CFLAGS) $< | sed "s;$(notdir $*).o:;$*.o $*.d:;" > $@

%.d: %.cpp
	$(CXX) -M $(ALL_CXXFLAGS) $< | sed "s;$(notdir $*).o:;$*.o $*.d:;" > $@


# Target: clean project.
clean:
	$(REMOVE) $(OBJDIR)/main.hex $(OBJDIR)/main.eep $(OBJDIR)/main.cof $(OBJDIR)/main.elf \
	$(OBJDIR)/main.map $(OBJDIR)/main.sym $(OBJDIR)/main.o $(OBJDIR)/main.lss $(CORE_OBJ) \
	$(OBJ) $(LST) $(SRC:.c=.s) $(SRC:.c=.d) $(CXXSRC:.cpp=.s) $(CXXSRC:.cpp=.d)

.PHONY:	all build elf hex eep lss sym program coff extcoff clean applet_files sizebefore sizeafter

#include $(SRC:.c=.d)
#include $(CXXSRC:.cpp=.d)
