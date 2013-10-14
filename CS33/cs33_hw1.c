#include <stdio.h>
typedef unsigned char *byte_pointer;

int main() {
    int large = 0x00004142;
    unsigned char *small = &large;
    //Hex of 33154 is 0x00004142
    
    if (*small == 0x00) {
        printf("%s","big\n");
    }
    if (*small == 0x42) {
        printf("%s","small\n");
    }
    //printf("%c",*small);
    //printf("%.2x\n",large);
    return 0;
}
