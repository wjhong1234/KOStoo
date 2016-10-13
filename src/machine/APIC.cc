/******************************************************************************
    Copyright � 2012-2015 Martin Karsten

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#include "machine/APIC.h"
#include "machine/CPU.h"

void PIC::disable() { // see http://wiki.osdev.org/PIC
  CPU::out8( 0x20, 0x11 ); // start PIC 1 initialization
  CPU::out8( 0xa0, 0x11 ); // start PIC 2 initialization
#if 0
  CPU::out8( 0x21, 0x20 ); // map 0..7 to 0x20..0x27
  CPU::out8( 0xa1, 0x28 ); // map 8..15 to 0x28..0x2f
  CPU::out8( 0x21, 0x04 ); // tell master PIC about slave
  CPU::out8( 0xa1, 0x02 ); // tell slave its identity
  CPU::out8( 0x21, 0x01 ); // set master to 8086/88 (MCS-80/85) mode
  CPU::out8( 0xa1, 0x01 ); // set slave to 8086/88 (MCS-80/85) mode
#endif
  CPU::out8( 0x21, 0xff ); // mask all PIC 1 interrupts
  CPU::out8( 0xa1, 0xff ); // mask all PIC 2 interrupts
}

uint8_t IOAPIC::getVersion() {
  uint8_t result = Version.get(read(IOAPICVER));
  return result;
}

uint8_t IOAPIC::getRedirects() {
  uint8_t result = MaxRedirectionEntry.get(read(IOAPICVER));
  return result;
}

void IOAPIC::maskIRQ(uint8_t irq) {
  uint64_t val = Mask();
  write( IOREDTBL + irq * 2, val & 0xFFFFFFFF );
  write( IOREDTBL + irq * 2 + 1, val >> 32 );
}

void IOAPIC::mapIRQ(uint8_t irq, uint8_t intr, bool low, bool level) {
  uint64_t val = Vector.put(intr)
               | DeliveryMode.put(APIC::LowestPriority)
               | DestinationMode()
               | Polarity.put(low)
               | TriggerModeLevel.put(level)
               | DestinationSet.put(0x01);
  write( IOREDTBL + irq * 2, val & 0xFFFFFFFF );
  write( IOREDTBL + irq * 2 + 1, val >> 32 );
}
