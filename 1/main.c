#include <stdio.h>

void First(){
    int input;

    printf("Двоичное представление положительного числа\nВведите целое положительное число ");

    while(1){
        scanf("%d", &input);

        if (input < 0){
            printf("Введите положительное число\n");
        } else{
            break;
        }
    }

    for (int i = 31; i >= 0; i--){
        if (((input >> i) & 1)){
            printf("1");
        }else{
            printf("0");
        }
    }
    printf("\n\n");
}

void Second(){
    int input;

    printf("Двоичное представление отрицательного числа\nВведите целое отрицательное число ");

    while(1){
        scanf("%d", &input);

        if (input >= 0){
            printf("Введите отрицательное число\n");
        } else{
            break;
        }
    }

    for (int i = 31; i >= 0; i--){
        if (((input >> i) & 1)){
            printf("1");
        }else{
            printf("0");
        }
    }
    printf("\n\n");
}

void Third(){
    int input;
    int count = 0;

    printf("Количество единиц в двоичном представлении целого положительного числа\nВведите целое положительное число ");

    while(1){
        scanf("%d", &input);

        if (input < 0){
            printf("Введите положительное число\n");
        } else{
            break;
        }
    }

    for (int i = 31; i >= 0; i--){
        if (((input >> i) & 1)){
            count++;
        }
    }
    printf("%d", count);
    printf("\n\n");
}

void Fourth(){
    int input;

    printf("Количество единиц в двоичном представлении целого положительного числа\nВведите целое положительное число ");

    while(1){
        scanf("%d", &input);

        if (input < 0){
            printf("Введите положительное число\n");
        } else{
            break;
        }
    }

    unsigned int value;
    printf("Введите значение, на которое хотите поменять третий байт введенного числа ");
    scanf("%d", &value);

    int output = input | ((input | value) << 16);

    for (int i = 31; i >= 0; i--){
        if (((output >> i) & 1)){
            printf("1");
        }else{
            printf("0");
        }
    }

    printf("\n");

    printf("%d", output);

    printf("\n");
}

int main(){
    First();
    Second();
    Third();
    Fourth();

    return 0;
}