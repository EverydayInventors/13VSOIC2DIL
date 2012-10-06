#
# Makefile for ATtiny13V 8SOICto8DIL Board LED timer PWM test program
# Programmed by David Dahl, Everyday Inventors, LLC.
# Copyright 2011, 2012 Free Software Foundation
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#                                    
CC=avr-gcc
CFLAGS=-mmcu=attiny13 -Wall -Os -D F_CPU=9600000UL
PRG=ledpwm

$(PRG).elf: $(PRG).c
	$(CC) $(CFLAGS) -c $(PRG).c
	$(CC) $(CFLAGS) $(PRG).o -o $(PRG).elf

clean:
	rm -f *.o *.elf *.hex *~

$(PRG).hex: $(PRG).elf
	avr-objcopy -j .text -j .data -O ihex $(PRG).elf $(PRG).hex

# you may need to modify the device programmer type to match the one you have
#  if you are not using the Adafruit USBtinyISP programmer
upload: $(PRG).hex
	avrdude -p t13 -c usbtiny -e -U flash:w:$(PRG).hex

ihex: $(PRG).hex

