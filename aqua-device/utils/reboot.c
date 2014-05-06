#include "reboot.h"
#include <avr/wdt.h>

void reboot() {
  wdt_disable();
  wdt_enable(WDTO_15MS);
  while (1) {}
}
