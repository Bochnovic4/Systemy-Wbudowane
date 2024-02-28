#include <stdio.h>
#include <stdlib.h>

#define BIT_VALUE(val, no_bit) (val >> no_bit) & 1

int seed = 1;
int generateRandom(int value){
    int sprzezenieVal = 0;
    for (int i =0; i<70; i++){
        sprzezenieVal = sprzezenie(value);
        value = value>>1;
        //value = value + sprzezenieVal * 32;
        value = value | (sprzezenieVal << 5);
        printf("%d \n",value);
    }
}

int sprzezenie(int value){
    return BIT_VALUE(value,0)^BIT_VALUE(value,1)^BIT_VALUE(value,4)^BIT_VALUE(value,5);
}

int generateRandom2(int value){
    int sprzezenieVal = 0;
    for (int i =0; i<70; i++){
        sprzezenieVal = sprzezenie2(value);
        value = value>>1;
        //value = value + sprzezenieVal * 32;
        value = value | (sprzezenieVal << 5);
        printf("%d \n",value);
    }
}

int sprzezenie2(int value){
    return BIT_VALUE(value,0)^BIT_VALUE(value,3)^BIT_VALUE(value,4)^BIT_VALUE(value,5);
}

int main()
{
    int val1 = seed;
    int val2 = seed;
    int sprzezenieVal1 = 0;
    int sprzezenieVal2 = 0;
    int gold = 0;
    for (int i =0; i<70; i++){
        sprzezenieVal1 = sprzezenie(val1);
        sprzezenieVal2 = sprzezenie2(val2);
        val1 = val1>>1;
        val1 = val1 | (sprzezenieVal1 << 5);
        val2 = val2>>1;
        val2 = val2 | (sprzezenieVal2 << 5);
        gold = val1 ^ val2;
        printf("%d %d %d\n",val1, val2, gold);
    }
    return 0;
}
