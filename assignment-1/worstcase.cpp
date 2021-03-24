#include <stdio.h>

int main(){
    int iter = 19512195;
    for(int i = 0; i< iter ;i++){
        printf("a");
        for(int j = 0; j<1023; j++){
            printf("k");
        }
        printf("\n");
    }
    for(int j = 0; j<1024; j++){
        printf("k");
    }
    printf("\n");
}