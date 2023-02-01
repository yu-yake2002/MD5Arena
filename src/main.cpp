#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <MD5.h>
#include <arena.h>
#include <cliterm.h>

static char *readlinen();

static int cmd_help(char *args);
static int cmd_add(char *args);
static int cmd_rm(char *args);
static int cmd_ls(char *args);
static int cmd_start(char *args);
static int cmd_exit(char *args);

static struct {
  const char *name;
  const char *description;
  int (*handler)(char *);
} cmd_table[] = {
    {"help", "Display informations about all supported commands", cmd_help},
    {"add", "Add a name to the arena", cmd_add},
    {"rm", "Remove a name from the arena", cmd_rm},
    {"ls", "List all names in the arena", cmd_ls},
    {"start", "Start the game", cmd_start},
    {"exit", "Exit the game", cmd_exit},
};
#define ARRLEN(arr) (int)(sizeof(arr) / sizeof(arr[0]))
#define NR_CMD ARRLEN(cmd_table)

int main() {
  printf(
    "------------ MD5Arena, V1.0 ------------\n"
    "This game is inspired by MD5War, a Flash\n"
    "game utilizing MD5 encryption. Add names\n"
    "to the arena, and their MD5 values will \n"
    "decide the result.\n"
    "----------------------------------------\n");
  InitGame();
  while (1) {
    fprintf(stderr, "(MD5-Arena) ");
    char *buffer = readlinen();
    char *buffer_end = buffer + strlen(buffer);
    char *cmd = strtok(buffer, " ");
    if (cmd == NULL) {
      continue;
    }
    char *args = cmd + strlen(cmd) + 1;
    if (args >= buffer_end) {
      args = NULL;
    }

    int i;
    for (i = 0; i < NR_CMD; ++i) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { 
          return 0;
        }
        break;
      }
    }
    if (i == NR_CMD) {
      fprintf(stderr, "Unknown command '%s'\n", cmd);
    }
  }
  return 0;
}

Terminal term;

static char *readlinen() {
  term.inp_len = 0;
  term.input[0] = '\0';
  char buf[2];
  while (1) {
    int read_ret = read(0, buf, 1);
    if (read_ret != -1 && read_ret != 0) {
      char *res = term.keypress(buf[0]);
      if (res) {
        return res;
      }
    }
  }
}

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  } else {
    for (i = 0; i < NR_CMD; i++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_add(char *name) {
  MD5_CTX md5c;
  MD5Init(&md5c);
  MD5Update(&md5c, (unsigned char *)name, strlen(name));
  unsigned char result[16] = {0};
  MD5Final(&md5c, result);
  Player *player = AllocPlayer();
  if (player) {
    FillPlayerProperty(player, name, result);
  }
  return player == NULL;
}

static int cmd_rm(char *name) {
  Player *player = FindPlayer(name);
  if (player) {
    DeletePlayerProperty(player);
  }
  return player == NULL;
}

static int cmd_ls(char *args) {
  PrintAllPlayerName();
  return 0;
}

static int cmd_start(char *args) {
  if (CountAlive() < 2) {
    printf("Error: The number of names should more than 1. Now the number is %d.\n", CountAlive());
  } else {
    PrintAllPlayerInfo();
    printf("---------------- Start! ----------------\n");
    srand(114514);
    StartGame();
  }
  InitGame();
  return 0;
}

static int cmd_exit(char *args) { return -1; }