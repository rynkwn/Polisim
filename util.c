#include <stdlib.h>
#include "util.h"
#include "constants.h"

/*
  UTILITY FUNCTIONS
 */

// Selects a random country name and then adds it to dest.
void randName(char *dest) {
  int index = (int) rand() % 65;

  char *name = COUNTRY_NAMES[index];

  strCpy(dest, name);
}

// Copy from into dest, while maintaining dest's size.
// ASSUMES FROM HAS SIZE <= DEST.
void strCpy(char *dest, char *from) {
  while(*from != '\0')
    *dest++ = *from++;
  while(*dest != '\0')
    *dest++ = ' ';
}
