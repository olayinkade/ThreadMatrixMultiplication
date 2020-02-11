// reminder - compile with -lpthread
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

struct Index {
    int rows;
    int cols;
    int result;
};

int matrix1 [4][5]= {
    {1, 2, 3, 4 , 5},
    {6, 7, 8, 9, 10},
    {11, 12, 13, 14, 15},
    {16, 17, 18, 19, 20}
};

int matrix2 [5][2] = {
    {1, 2},
    {3, 4},
    {5, 6},
    {7, 8},
    {9, 10}
};


void* threading_task(void* ignored)
{
    struct Index* d = (struct Index*) ignored;
    int result = 0;
    int size = sizeof(matrix1[d->rows])/sizeof(int);
    int colsMat[size];

    for (int i = 0; i < size; i++) {
      result += matrix2[i][d->cols] * matrix1[d->rows][i];

    }
    d->result = result;
    pthread_exit(d);

}



int main()
{
    struct Index* d;
    // # of columns of 1st array == # of rows in 2nd array
    assert(sizeof(matrix1[0])/sizeof(int) == sizeof(matrix2)/sizeof(matrix2[0]));
    int resultantRow = sizeof(matrix1)/sizeof(matrix1[0]) ;
    int resultantCols = sizeof(matrix2[0])/sizeof(int);
    int nOfThreads = resultantRow * resultantCols;
    pthread_t threads[nOfThreads];
    int resultant[resultantRow][resultantCols];
    int currThread = 0;

    for (int i = 0; i < resultantRow; i++){
      for (int j = 0; j < resultantCols; j++){

        // init the thread
        struct Index* de =  (struct Index*) malloc(sizeof(struct Index));
        de->rows = i;
        de->cols = j;

        pthread_create( &threads[currThread], NULL, threading_task, de);
        currThread++;
      }
    }
    

    for (int i = 0; i < nOfThreads; i++){
      pthread_join( threads[i], (void**)&d);
      resultant[d->rows][d->cols] = d ->result;
    }

    for (int i = 0; i < resultantRow; i++){
      for (int j = 0; j < resultantCols; j++){
        printf("%d ", resultant[i][j] );
      }
      printf("\n");
    }

    // back to the mainline
    printf("done\n");


}
