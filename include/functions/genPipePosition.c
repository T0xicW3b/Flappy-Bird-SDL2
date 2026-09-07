#include "header/header.h"

int genPipePosition() {
  int position = (rand() % 201 + 100) * -1; // -100 to -300 pixels
  return position;
}
