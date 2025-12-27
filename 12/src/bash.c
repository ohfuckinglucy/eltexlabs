#include "bash.h"

int main() {
    char input[BUFFER_SIZE];

    while(1){
        printf("sh> ");
        if (!fgets(input, sizeof(input), stdin)) {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (input[0] == '\0') continue;

        if (strcmp(input, "exit") == 0) {
            break;
        }

        char input_copy[BUFFER_SIZE];
        strncpy(input_copy, input, BUFFER_SIZE - 1);
        input_copy[BUFFER_SIZE - 1] = '\0';

        char *parts[PARTS_COUNT];
        int n_parts = 0;
        char *part = strtok(input_copy, "|");

        while (part && n_parts < PARTS_COUNT){
            parts[n_parts++] = trim(part);
            part = strtok(NULL, "|");
        }

        if (n_parts == 1){
            run_single_command(parts[0]);
        } else {
            run_pipeline(parts, n_parts);
        }
    }

    return 0;
}