#include <stdio.h>

#define BIT_VALUE(val, no_bit) (val >> no_bit) & 1

void delay(){
    long long j = 999999999;
    while(j >0){
        j--;
    }
}

void number_bit(){
    int x = 0;
    while(1){
        for (int i=7; i>=0;i--){
            printf("%d",BIT_VALUE(x,i));
        }
        printf("\r");
        delay();
        x++;
    }
}

int IntToGray(unsigned char input){
        return (input>>1)^input;
}

void Counter(){
    int counter = 0;
    while(1){
        printf("\r");
        for(int i=7; i>=0; i--){
            printf("%d", BIT_VALUE(counter,i));
        }
        printf("    ");
        for(int i=7; i>=0; i--){
            printf("%d", BIT_VALUE(IntToGray(counter),i));
        }
        counter++;
        delay();
    }
}

int fun_bin_value(){
    int value = 0;
    int mask = 0x80;
    int counter = 10;
    while(counter >= 0){
       while(mask != 0){
        if (mask & value){
            printf("1");
        }
        else {
            printf("0");
        }
        mask = mask >> 1;
        }
        printf("\n");
        mask = 0x80;
        counter--;
        value++;
    }
}

int main()
{
    system("color 4A");
    Counter();
    return 0;
}
