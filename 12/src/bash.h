#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define ARGS_COUNT 32
#define PARTS_COUNT 10
#define BUFFER_SIZE 256

void parse(char *line, char *args[]);
char *trim(char *str);
int run_single_command(char *cmd);
int run_pipeline(char *parts[], int n_parts);