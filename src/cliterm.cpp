#include <string.h>
#include <stdio.h>
#include <cliterm.h>

char *Terminal::keypress(char ch) {
  char *ret = NULL;
  switch (ch) {
    case '\0':
    case '\33':
      break;
    case '\r':
    case '\n':
      input[inp_len] = '\0';
      strcpy(cooked, input);
      ret = cooked;
      fprintf(stderr, "\n");
      break;
    case '\177':
      if (inp_len > 0) {
        inp_len --;
        fprintf(stderr, "\177");
      }
      break;
    default:
      if (inp_len + 1 < sizeof(input)) {
        input[inp_len ++] = ch;
        fprintf(stderr, "%c", ch);
      }
  }
  return ret;
}