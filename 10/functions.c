
// functions.c
// Реализация лабораторных пунктов 1–3.
#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


 // Пункт 1: демонстрация fork() и передачи кода завершения.
 //
 // Родитель создаёт дочерний процесс, ждёт его завершения и выводит статус.
 //
 // return 0 при успешном выполнении.
int point1(){
    pid_t pid = fork();
    int status;

    if (pid > 0){
        printf("PID, PPID родительского процесса: %d, %d\n", getpid(), getppid());
        wait(&status);

        printf("Status: %d\n", WEXITSTATUS(status));
    } else if(pid == 0){
        printf("PID, PPID дочерненего процесса: %d, %d\n", getpid(), getppid());
        exit(3);
    } else{
        perror("fork");
    }
    
    return 0;
}


 // Пункт 2: Создание древовидной иеарархии процессов.
 //
 // Родитель создаёт дочерние процессы, которые так же создают дочерние процессы.
 //
 // return 0 при успешном выполнении.
int point2(){
    pid_t pid_1_proc = fork();
    int status;
    if (pid_1_proc < 0){
        perror("fork");
    }

    if (pid_1_proc == 0){
        printf("PID, PPID процесса 1: %d, %d\n", getpid(), getppid());

        pid_t pid_3_proc = fork();
        if (pid_3_proc < 0){
            perror("fork");
        }

        if (pid_3_proc == 0){
            printf("PID, PPID процесса 3: %d, %d\n", getpid(), getppid());
            exit(3);
        } else {
            pid_t pid_4_proc = fork();
            if (pid_4_proc < 0){
                perror("fork");
            }

            if (pid_4_proc == 0){
                printf("PID, PPID процесса 4: %d, %d\n", getpid(), getppid());
                exit(4);
            }
        }
        wait(&status);
        printf("Статус: %d\n", WEXITSTATUS(status));
        wait(&status);
        printf("Статус: %d\n", WEXITSTATUS(status));

        exit(1);
    }
    
    pid_t pid_2_proc = fork();
    if (pid_2_proc < 0){
        perror("fork");
    }

    if (pid_2_proc == 0){
        printf("PID, PPID процесса 2: %d, %d\n", getpid(), getppid());
        pid_t pid_5_proc = fork();
        if (pid_5_proc < 0){
            perror("fork");
        }

        if (pid_5_proc == 0){
            printf("PID, PPID процесса 5: %d, %d\n", getpid(), getppid());
            exit(5);
        }

        wait(&status);
        printf("Статус: %d\n", WEXITSTATUS(status));

        exit(2);
    }

    wait(&status);
    printf("Статус: %d\n", WEXITSTATUS(status));
    wait(&status);
    printf("Статус: %d\n", WEXITSTATUS(status));

    return 0;
}


 // Пункт 3: Простейшая командная оболочка.
 //
 // Наглядный пример случая когда fork может пригодиться, с помощью execvp в дочернем потоке обрабатываются бинарники.
 //
 // return 0 при успешном выполнении.
int point3(){
    char input[256];
    char *args[32];
    int status;

    while (1){
        size_t args_it = 0;
        printf("Введите команду или exit для выхода: ");
        if (!fgets(input, sizeof(input), stdin)){
            perror("fgets");
        }

        input[strcspn(input, "\n")] = '\0';

        char *cur = strtok(input, " ");

        if (strncmp(cur, "exit", 4) == 0){
            printf("Выход из программы.\n");
            break;
        }

        while((cur != NULL) && (args_it < 127)){
            args[args_it++] = cur;
            cur = strtok(NULL, " ");
        }

        args[args_it] = NULL;

        if (args_it == 0){
            printf("Введите что-нибудь\n");
            exit(1);
        }

        pid_t pid = fork();
        if (pid < 0){
            perror("fork");
            return 1;
        }

        if (pid == 0){
            execvp(args[0], args);

            perror("execvp");
            exit(2);
        }

        wait(&status);
    }

    return 0;
}