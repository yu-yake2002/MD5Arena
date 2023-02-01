#ifndef __CLITERM_H__
#define __CLITERM_H__

struct Terminal {
  char input[256], cooked[256];
  int inp_len, cursor;
  char *keypress(char ch);
};

#endif