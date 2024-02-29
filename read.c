#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct HashTable {
    int numElements;       
    int bucketsVacios;     
    int* keys;              
    int** values;          
} HashTable;

HashTable *createHashTable(int size){
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    ht->numElements = size;
    ht->bucketsVacios = size;
    ht->keys = (int *)malloc(size * sizeof(int));
    ht->values = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++){
        ht->keys[i] = -1;
        ht->values[i] = 0;
    }
    return ht;
}

int destruirHash(HashTable **hashTablePP){
    HashTable *hashTableP = *hashTablePP;
    free(hashTableP->keys);
    for (int i = 0; i < hashTableP->numElements; i++){
        if (hashTableP->values[i] != NULL){
            free(hashTableP->values[i]);
        }
    }
    free(hashTableP->values);
    free(hashTableP);
    *hashTablePP = NULL;
    return 0;
}

int insertar(HashTable *hashTableP, int key, int value){
    int index = key % hashTableP->numElements;
    while (hashTableP->keys[index] != -1){
        if (hashTableP->keys[index] == key) {
             *(hashTableP->values[index]) += 1;
            return 0;
        }
        index = (index + 1) % hashTableP->numElements;
    }
    hashTableP->keys[index] = key;
     hashTableP->values[index] = (int *)malloc(sizeof(int));
     *(hashTableP->values[index]) = value;
    hashTableP->bucketsVacios--;
    return 0;
}


double varianza(HashTable *hashTable, int size, float media){
    double varian = 0.0;
    for(int i = 0; i < size; i++){
        double values = 0;
        if(hashTable->keys[i] != -1){
            values = hashTable->keys[i] - media;
            varian += values * values;
        }
    }
    return varian / (size - 1);
}

int main(int argc, char *argv[]){

    HashTable *hashTable = NULL;

    struct timespec start, end;
    double readFileTime, sumTime, varian = 0, desviacionEsta = 0;
    int mostCommonValue, mostCommonValueCount = 0;

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
    hashTable = createHashTable(size);

    if(array == NULL){
        printf("Memory allocation failed\n");
        exit(1);
    }

    long int suma = 0;
    float media = 0;

    clock_gettime(CLOCK_REALTIME, &start);

    for(int i = 0; i < size; i++){
        fscanf(file, "%d", &array[i]);
        suma += array[i];
        insertar(hashTable, array[i], 1);

    }
    clock_gettime(CLOCK_REALTIME, &end);
    int tamanoReal = hashTable->numElements - hashTable->bucketsVacios;

    media = (float)suma / tamanoReal;
    varian = varianza(hashTable, tamanoReal, media);
    desviacionEsta = sqrt(varian);
    sumTime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ 1e9;
    
    mostCommonValue = -9999;
    for(int i = 0; i < tamanoReal; i++){
        if (hashTable->keys[i] != -1){
            if(*(hashTable->values[i]) > mostCommonValueCount){
            mostCommonValueCount = *(hashTable->values[i]);
            mostCommonValue = hashTable->keys[i];
            }
        }
        
    }
    destruirHash(&hashTable);
    fclose(file);
    free(array);

    printf("Read file time: %f\n Sum time: %f\n Total sum: %ld\n The mst common value is: (%d, %d)\n Media %f\n Varianza: %f\n Desviacion Estandar: %f\n", readFileTime, sumTime, suma, mostCommonValue, mostCommonValueCount,media, varian, desviacionEsta);
}