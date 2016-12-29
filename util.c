#include <stdlib.h>
#include "util.h"
#include "constants.h"

/*
  UTILITY FUNCTIONS
 */

void randStr(char *dest, int length) {
  char charset[] = "abcdefghijlmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  
  while (length-- > 0) {
    int index = (double) rand() / RAND_MAX * (sizeof charset - 1);
    *dest++ = charset[index];
  }
  *dest = '\0';
}

// Copy from into dest, while maintaining dest's size.
// ASSUMES FROM HAS SIZE <= DEST.
void strCpy(char *dest, char *from) {
  while(*from != '\0')
    *dest++ = *from++;
  while(*dest != '\0')
    *dest++ = ' ';
}
