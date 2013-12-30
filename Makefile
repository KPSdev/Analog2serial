MCU		= atmega328p
TARGET_ARCH	= -mmcu=$(MCU)
TARGET		= analog2serial
CC		= avr-gcc
CPPFLAGS	= -mmcu=$(MCU)
CFLAGS		= -Os -g -Wall -I. -DF_CPU=16000000
LDFLAGS		= -g -mmcu=$(MCU) -lm -Wl,--gc-sections -OS
PGMER		= -c buspirate
PGMPORT		= -P /dev/tty.usbserial3
AVRISP		= -c avrispv2
DUDE		= avrdude -F -V

C_SRCS		= $(wildcard *.c)
OBJ_FILES	= $(C_SRCS:.c=.o)

all:	$(TARGET).hex

clean:
	rm -f $(TARGET).elf *.o *.hex

%.o: %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(TARGET).elf: $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o $@ $(OBJ_FILES)

$(TARGET).hex: $(TARGET).elf
	avr-objcopy -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex
	avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ihex $(TARGET).elf eeprom.hex

upload: $(TARGET).hex
	$(DUDE) $(PGMER) -p $(MCU) $(PGMPORT) -U flash:w:$(TARGET).hex

size: $(TARGET).elf
	avr-size --format=avr --mcu=$(MCU) $(TARGET).elf
