#include <stdio.h>

void prog1(){
    int N;
    printf("Введите размерность квадратной матрицы: ");
    scanf("%d", &N);
    int matrix[N][N];

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            matrix[i][j] = 1;
        }
    }

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (j > 0) {
                matrix[i][j] += matrix[i][j - 1];
            } else if (i > 0) {
                matrix[i][j] += matrix[i - 1][N - 1];
            }
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void prog2(){
    int N;
    int swap;
    printf("Введите размерность квадратной матрицы: ");
    scanf("%d", &N);
    int matrix[N][N];

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            matrix[i][j] = 1;
        }
    }
    float count = 0;
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (j > 0) {
                matrix[i][j] += matrix[i][j - 1];
            } else if (i > 0) {
                matrix[i][j] += matrix[i - 1][N - 1];
            }
        }
    }

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            count++;
            if (count <= (N*N)/2){
                swap = matrix[i][j];
                matrix[i][j] = matrix[N - 1 - i][N - 1 - j];
                matrix[N - 1 - i][N - 1 - j] = swap;
            }
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void prog3(){
    int N;
    int swap;
    printf("Введите размерность квадратной матрицы: ");
    scanf("%d", &N);
    int matrix[N][N];

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            matrix[i][j] = 2;
        }
    }

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (j < N - 1 - i){
                matrix[i][j] = 0;
            } else if (j > N - 1 - i){
                matrix[i][j] = 1;
            }
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void prog4(){
    int N;
    printf("Введите размерность квадратной матрицы: ");
    scanf("%d", &N);
    int matrix[N][N];

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            matrix[i][j] = 0;
        }
    }

    int count = 1;
    int top = 0;
    int bottom = N - 1;
    int left = 0;
    int right = N - 1;

    while (count <= N * N) {
        for (int j = left; j <= right; j++) {
            matrix[top][j] = count++;
        }
        top++;

        for (int i = top; i <= bottom; i++) {
            matrix[i][right] = count++;
        }
        right--;

        for (int j = right; j >= left; j--) {
            matrix[bottom][j] = count++;
        }
        bottom--;

        for (int i = bottom; i >= top; i--) {
            matrix[i][left] = count++;
        }
        left++;
    }

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(){
    prog1();
    prog2();
    prog3();
    prog4();
    return 0;
}