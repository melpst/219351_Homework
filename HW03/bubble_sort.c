#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100000

void swap(int *xp, int *yp) {
  int temp = *xp;
  *xp = *yp;
  *yp = temp;
}

// A function to implement bubble sort
void bubbleSort(int arr[], int n) {
  int i, j;

  for (i = 0; i < n-1; i++)      
    for (j = 0; j < n-i-1; j++)
      if (arr[j] > arr[j+1])
	swap(&arr[j], &arr[j+1]);
}

int isSorted(int *a, int size) {
  int i;
  for (i = 0; i < size - 1; i++) {
    if (a[i] > a[i + 1]) {
      return 0;
    }
  }
  return 1;
}

// Function to print an array
void printArray(int arr[], int size)
{
  int i;
  for (i=0; i < size; i++)
    printf("%d ", arr[i]);
  printf("\n");
}


int main(int argc, char** argv) {
	int i, j, n;
	int* A, C;
	clock_t start, end;
	double elapsed_time, t1, t2;

	MPI_Init(NULL, NULL);
	  
	int world_rank;
  	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  	int world_size;
  	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  	if(world_rank == 0) {
  		t1 = MPI_Wtime();

  		printf("N = %d\n", N);

	  	A = (int *)malloc(sizeof(int)*N);

		if (A == NULL) {
			printf("Fail to malloc\n");
			exit(0);
		}
		for (i=N-1; i>=0; i--)
			A[N-1-i] = i;

		if (isSorted(A, N))
		  printf("Array is sorted\n");
		else
		  printf("Array is NOT sorted\n");

		// printArray(A, N);

	  	for (i = 1; i < world_size; i++) {
	  		// printf("%d\n", (N/world_size)*(i-1));
			MPI_Send(A + (N/world_size)*(i), N/world_size, MPI_INT, i, 0, MPI_COMM_WORLD);
		}

		// int* B = (int *)malloc(sizeof(int)*N/world_size);
		// B = A;

		// printf("%d\n", (world_size));
		bubbleSort(A, N/world_size);
		// printArray(A, N/world_size);

		for(j = 0; j < N/world_size; j++) {
	    	// printf("%d %d\n", ((N/world_size)*i)+j, B[j]);
	    	A[(N-(N/world_size))+j] = A[j];
	    }
	}

  	if (world_rank != 0) {
  		int* B = (int *)malloc(sizeof(int)*N/world_size);
	    MPI_Recv(B, N/world_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// printArray(B, N/world_size);

		bubbleSort(B, N/world_size);
		MPI_Send(B, N/world_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
	} 
	
	if(world_rank == 0) {
		for (i = 1; i < world_size; i++) {
			int* B = (int *)malloc(sizeof(int)*N/world_size);
		    MPI_Recv(B, N/world_size, MPI_INT, world_size-i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		    for(j = 0; j < N/world_size; j++) {
		    	// printf("%d %d\n", ((N/world_size)*(i-1))+j, B[j]);
		    	A[((N/world_size)*(i-1))+j] = B[j];
		    }
		}

		printArray(&A[N-10], 10);
		// printArray(A, N);

		if (isSorted(A, N))
		  printf("Array is sorted\n");
		else
		  printf("Array is NOT sorted\n");

		t2 = MPI_Wtime();
		printf( "Elapsed time MPI_Wtime is %f\n", t2 - t1 ); 
	}
	
	MPI_Finalize();
	return 0;
}
