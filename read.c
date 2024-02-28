#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){

    struct timespec start, end;
    double readFileTime, sumTime;
    int mostCommonValue = 0;

    FILE *file;
    int size;  
    file = fopen(argv[1], "r");

    if(file == NULL){
        printf("File not found\n");
        exit(1);
    }else{
        printf("File %s open\n", argv[1]);
    }

    int *array = NULL;

    clock_gettime(CLOCK_REALTIME, &start);

    fscanf(file, "%d", &size);

    clock_gettime(CLOCK_REALTIME, &end);
    readFileTime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ 1e9;

    array = (int *)malloc(size * sizeof(int));

    if(array == NULL){
        printf("Memory allocation failed\n");
        exit(1);
    }

    long int suma = 0;

    clock_gettime(CLOCK_REALTIME, &start);

    int mayor = -99999, count = 0;
    for(int i = 0; i < size; i++){
        fscanf(file, "%d", &array[i]);
        suma += array[i];
        for (int j = 1; j < size; j++){
            if (array[i] == array[j]){
                count++;
            }     
        }
        if (count > mayor){
            mayor = count;
            mostCommonValue = array[i];
        }       
        count = 0;
    }
    clock_gettime(CLOCK_REALTIME, &end);
    sumTime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ 1e9;

    fclose(file);
    free(array);

    printf("Read file time: %f\n Sum time: %f\n Total sum: %ld\n The mst common value is: %d\n", readFileTime, sumTime, suma, mostCommonValue);
}