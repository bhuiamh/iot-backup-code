#include "disableCan.h"

void disableCANBus(){
  // CAN controller cs lines are only for ig100 as the spi bus is also connected to CAN controller
  pinMode(CAN_CONTROLLER_CS, OUTPUT);
  digitalWrite(CAN_CONTROLLER_CS, HIGH);  // Disabling the CAN controller chip select
}
