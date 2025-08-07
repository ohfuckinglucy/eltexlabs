#include <stdio.h>
#include "header.h"

void clearinput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(){
    int choise;

    int a, b;
    while(1){
        printf("1) Сложение\n2) Вычитание\n3) Умножение\n4) Деление\n5) Выход\n");
        if(scanf("%d", &choise) != 1){
            printf("Ошибка\n");
            clearinput();
            continue;
        }
        if(choise == 5){
            break;
        }

        printf("Введите два числа(через пробел), с которыми будет проведена операция: ");
        if(scanf("%d %d", &a, &b) != 2){
            printf("Ошибка\n");
            clearinput();
            continue;
        }

        switch(choise){
            case 1: {
                addf(a, b);
                continue;
            }
            case 2:{
                subf(a, b);
                continue;
            }
            case 3:{
                mulf(a, b);
                continue;
            }
            case 4:{
                divf(a, b);
                continue;
            }
            default: {
                printf("Выберите пункт из меню!\n");
            }
        }
        break;
    }
}