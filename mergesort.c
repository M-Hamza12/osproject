#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#define ARR_SIZE 1000

int arr[ARR_SIZE];

void ParseParameters(char *argv[]) {
  for (int i = 0, j = 1; j < ARR_SIZE + 1; i++, j++)
    sscanf(argv[j], "%d", &arr[i]);
}
// Merge two subarrays L and M into arr
void merge(int arr[], int p, int q, int r) {

  // Create L ← A[p..q] and M ← A[q+1..r]
  int n1 = q - p + 1;
  int n2 = r - q;

  int L[n1], M[n2];

  for (int i = 0; i < n1; i++)
    L[i] = arr[p + i];
  for (int j = 0; j < n2; j++)
    M[j] = arr[q + 1 + j];

  // Maintain current index of sub-arrays and main array
  int i, j, k;
  i = 0;
  j = 0;
  k = p;

  // Until we reach either end of either L or M, pick larger among
  // elements L and M and place them in the correct position at A[p..r]
  while (i < n1 && j < n2) {
    if (L[i] <= M[j]) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = M[j];
      j++;
    }
    k++;
  }

  // When we run out of elements in either L or M,
  // pick up the remaining elements and put in A[p..r]
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  while (j < n2) {
    arr[k] = M[j];
    j++;
    k++;
  }
}

// Divide the array into two subarrays, sort them and merge them
void mergeSort(int arr[], int l, int r) {
  if (l < r) {

    // m is the point where the array is divided into two subarrays
    int m = l + (r - l) / 2;

    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);

    // Merge the sorted subarrays
    merge(arr, l, m, r);
  }
}

// Driver program
int main(int argc, char *argv[]) {
  FILE *fp;
  remove("MergeFork.csv");
  fp = fopen("MergeFork.csv", "a");
  fprintf(fp,"time,trial");
  if(fp == NULL)
    printf("ERROR!\n");

  // fprintf(fp,"time,trial\n");
  int k;
  int id = getpid();
  double times[15];
  double timetaken1;
  clock_t start1 = clock();

  fork();
  fork();
  fork();
  if (getpid() == id)
  {
    clock_t end1 = clock();
    timetaken1 = (double)(end1 - start1) / CLOCKS_PER_SEC;
  }

  for (k = 0; k < 10; k++)
  {
    clock_t start = clock();
    ParseParameters(argv);
    mergeSort(arr,0,ARR_SIZE-1);
    clock_t end = clock();

    double timetaken = ((double)(end - start) / CLOCKS_PER_SEC) + timetaken1*8;
    if (getpid() == id)
    {
      if(k==14)
        fprintf(fp,"%f,15", timetaken *8);
      else
        fprintf(fp, "%f,%d\n", (timetaken * 8),(k+1));
      
      printf("Time taken by 8 \x1b[1mPROCESSES\x1b[0m:\x1b[92m %f "
             "seconds\x1b[0m\n\n",
             (timetaken * 8));
    }
  }
  fclose(fp);
}