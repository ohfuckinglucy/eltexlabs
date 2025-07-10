#include <stdio.h>
#include <string.h>

struct abonent
{
    char name[10];
    char second_name[10];
    char tel[10];
};

void clearinput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int equal(const char *a, const char *b){
    while (*a != '\0' && *b != '\0'){
        if (*a != *b) return 0;
        a++;
        b++;
    }
    return (*a == '\0' && *b == '\0');
}

int main(){
    struct abonent abonent[100] = {0};

    short choise;

    while (1)
    {
        printf("1) Добавить абонента\n");
        printf("2) Удалить абонента\n");
        printf("3) Поиск абонентлв по имени\n");
        printf("4) Вывод всех записей\n");
        printf("5) Выход\n");

        if (scanf("%hd", &choise) == 0){
            printf("Ошибка\n");
            clearinput();
            continue;
        }

        switch (choise)
        {
        case 1:
            for (int i = 0; i < 100; i ++){
                if ((abonent[i].name[0] == '\0') && (abonent[i].second_name[0] == '\0') && (abonent[i].tel[0] == '\0')){
                    printf("Введите в формате name second_name tel: ");
                    if (scanf("%9s", abonent[i].name) == 0){
                        printf("Ошибка\n");
                        clearinput();
                        continue;
                    }
                    if (scanf("%9s", abonent[i].second_name) == 0){
                        printf("Ошибка\n");
                        clearinput();
                        continue;
                    }
                    if (scanf("%9s", abonent[i].tel) == 0){
                        printf("Ошибка\n");
                        clearinput();
                        continue;
                    }
                    break;
                } else{
                    printf("Внимание!!!\nПереполнение структуры\n");
                }
            }
            continue;
        case 2:
            int i;
            printf("Введите номер абонента, которого вы хотите удалить: ");
            scanf("%d", &i);
            for (int j = 0; j < 10; j++){
                abonent[i].name[j] = '\0';
                abonent[i].second_name[j] = '\0';
                abonent[i].tel[j] = '\0';
            }
            continue;
        case 3:
            char searhed_name[10];
            printf("Введите имя, которое хотели бы найти: ");
            if (scanf("%9s", searhed_name) == 0){
                printf("Ошибка\n");
                clearinput();
                continue;
            }
            for (int i = 0; i < 100; i++){
                if (equal(searhed_name, abonent[i].name) == 1){
                    printf("Абонент номер: %d | %s %s %s\n", i, abonent[i].name, abonent[i].second_name, abonent[i].tel);
                }
            }
            continue;
        case 4:
            for (int i = 0; i < 100; i ++){
                if ((abonent[i].name[0] != '\0') && (abonent[i].second_name[0] != '\0') && (abonent[i].tel[0] != '\0')){
                    printf("Абонент номер: %d | %s %s %s\n", i, abonent[i].name, abonent[i].second_name, abonent[i].tel);
                }
            }
            continue;
        case 5:
            break;
        default:
            printf("Выберите от 1 до 5\n");
            continue;
        }
        break;
    }
}