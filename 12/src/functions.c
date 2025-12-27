#include "bash.h"

void parse(char *line, char *args[]){
    int i = 0;
    char *token = strtok(line, " ");
    while (token && i < ARGS_COUNT - 1){
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}

char *trim(char *str){
    while (*str == ' ') str++;
    if (*str == '\0') return str;

    char *end = str + strlen(str) - 1;

    while(end > str && *end == ' '){
        *end = '\0';
        end--;
    }
    return str;
}

int run_single_command(char *cmd){
    char *args[ARGS_COUNT];
    char cmd_copy[BUFFER_SIZE];
    strncpy(cmd_copy, cmd, BUFFER_SIZE-1);
    cmd_copy[BUFFER_SIZE - 1] = '\0';

    parse(cmd_copy, args);
    if (!args[0]) return 0;

    pid_t pid = fork();
    if (pid == 0){
        execvp(args[0], args);
        perror("execvp");
        exit(1);
    } else if (pid > 0){
        int status;
        waitpid(pid, &status, 0);
    } else {
        perror("fork");
        return 1;
    }
    return 0;
}

int run_pipeline(char *parts[], int n_parts){
    int fds[n_parts - 1][2];
    for (int i =0 ; i < n_parts-1; i++){
        if(pipe(fds[i]) == -1){
            perror("pipe");
            return 1;
        }
    }
    pid_t pids[n_parts];

    for (int i = 0; i < n_parts; i ++){
        pids[i] = fork();
        if (pids[i] == 0){
            if (i > 0){
                dup2(fds[i-1][0], STDIN_FILENO);
            }
            if (i < n_parts - 1){
                dup2(fds[i][1], STDOUT_FILENO);
            }

            for (int j = 0; j < n_parts - 1; j++) {
                close(fds[j][0]);
                close(fds[j][1]);
            }

            char cmd[BUFFER_SIZE];
            strncpy(cmd, parts[i], BUFFER_SIZE - 1);
            cmd[BUFFER_SIZE - 1] = '\0';
            char *args[ARGS_COUNT];
            parse(cmd, args);
            if (!args[0]) exit(1);

            execvp(args[0], args);
            perror("execv[]");
            exit(1);
        }
    }

    for (int i = 0; i < n_parts - 1; i++) {
        close(fds[i][0]);
        close(fds[i][1]);
    }

    for (int i = 0; i < n_parts; i++) {
        waitpid(pids[i], NULL, 0);
    }

    return 0;
}