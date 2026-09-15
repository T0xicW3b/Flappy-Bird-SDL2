#include "header/header.h"

int GetTextWidth(TTF_Font *font, const char *text) {
  int width = 0;
  int height = 0;

  // Calculate the size
  if (TTF_SizeUTF8(font, text, &width, &height) == 0) {
    return width; // Return the width to whoever called this function
  }
  return 0;
}
