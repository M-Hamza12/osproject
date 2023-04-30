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

int ShellSort() {
  int n = ARR_SIZE;
  int temp = 0;
  int i, j, key;

  for (int gap = n / 2; gap > 0; gap /= 2) {

    for (int i = gap; i < n; i += 1) {
      int temp = arr[i];
      int j;
      for (j = i; j >= gap && arr[j - gap] > temp; j -= gap)
        arr[j] = arr[j - gap];

      arr[j] = temp;
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {
  FILE *fp;
  remove("ShellFork.csv");
  fp = fopen("ShellFork.csv", "a");
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
    ShellSort();
    clock_t end = clock();

    double timetaken = ((double)(end - start) / CLOCKS_PER_SEC) + timetaken1;
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
