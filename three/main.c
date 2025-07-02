#include "stdio.h"

void prog1(){
    int number, byte;
    char *point;

    point = (char*)&number;

    printf("Введите исходное число: ");
    scanf("%d", &number);
    printf("\nВведите значение, которое хотите поместить в третий байт: ");
    scanf("%d", &byte);

    point = point + 2;
    *point = (char)byte;

    printf("\nИтоговое число: %d\n", number);
}

void prog2(){
    float x = 5.0;
    printf("x = %f, ", x);
    float y = 6.0;
    printf("y = %f\n", y);
    float *xp = &x + 1;

    printf("Расположение x и y в памяти: %p, %p\n", (void*)&x, (void*)&y);

    float *yp = &y;
    printf("Результат: %f\n", *xp + *yp);
}

void prog3(){
    int massive[10];
    int n = 0;

    for (int i = 0;i < 10 ;i++){
        massive[i] = i + 1;
    }

    int *point;

    point = massive;

    while(1){
        printf("Введите индекс элемента массива(с нуля), который вы хотите вывести: ");
        scanf("%d", &n);
        if ((n < 0) || (n >= 10)){
            printf("Введите число от 0 до 9\n");
        } else{break;}
    }

    point = point + n;

    printf("%d\n", *point);
}

void prog4() {
    char str1[100];
    char str2[100];
    char c;
    int i = 0;
    char *point = NULL;

    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    printf("Введите строку: ");
    while ((c = getchar()) != '\n'){
        if (i < 99){
            str1[i++] = c;
        }
    }
    str1[i] = '\0';

    i = 0;

    printf("Введите подстроку: ");
    while ((c = getchar()) != '\n' && c != EOF) {
        if (i < 99) {
            str2[i++] = c;
        }
    }
    str2[i] = '\0';

    i = 0;

    for (int i = 0; str1[i] != '\0'; i++) {
        int j, k;
        for (j = 0, k = i; str2[j] != '\0' && str1[k] != '\0'; j++, k++) {
            if (str1[k] != str2[j]) break;
        }
        if (str2[j] == '\0') {
            point = &str1[i];
            break;
        }
    }
    
    if (point != NULL) {
        printf("Адрес начала подстроки: %p\n", (void*)point);
    } else {
        printf("Подстрока не найдена.\n");
    }
}

int main(){
    prog1();
    prog2();
    prog3();
    prog4();

    return 0;
}