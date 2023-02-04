#ifndef __CLITERM_H__
#define __CLITERM_H__

typedef struct {
  char input[256], cooked[256];
  int inp_len, cursor;
} Terminal;

char *TermKeyPress(Terminal *term, char ch);
char *TermReadLine(Terminal *term);

#endif
