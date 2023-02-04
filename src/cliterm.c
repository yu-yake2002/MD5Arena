#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <cliterm.h>

char *TermKeyPress(Terminal *term, char ch) {
  char *ret = NULL;
  switch (ch) {
    case '\0':
    case '\33':
      break;
    case '\r':
    case '\n':
      term->input[term->inp_len] = '\0';
      strcpy(term->cooked, term->input);
      ret = term->cooked;
      fprintf(stderr, "\n");
      break;
    case '\177':
      if (term->inp_len > 0) {
        term->inp_len --;
        fprintf(stderr, "\177");
      }
      break;
    default:
      if (term->inp_len + 1 < sizeof(term->input)) {
        term->input[term->inp_len ++] = ch;
        fprintf(stderr, "%c", ch);
      }
  }
  return ret;
}

char *TermReadLine(Terminal *term) {
  term->inp_len = 0;
  term->input[0] = '\0';
  char buf[2];
  while (1) {
    int read_ret = read(0, buf, 1);
    if (read_ret != -1 && read_ret != 0) {
      char *res = TermKeyPress(term, buf[0]);
      if (res) {
        return res;
      }
    }
  }
}