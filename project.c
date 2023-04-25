#include <pthread.h>
#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#define ARR_SIZE 1000
int arr[ARR_SIZE];
void populate(){
    srand(time(0));
    int i ;
    for(i=0;i<ARR_SIZE;i++){
        arr[i] = rand()%10000;
    }
}
void BubbleSort()
{
	int temp = 0;
	int i, j;
	int temparr[ARR_SIZE];

	//temp arr here so each thread dosent recieve an "ALREADY SORTED" array
	//and it dosent disturb the global array
	for (i = 0; i < ARR_SIZE; i++)
	{
		temparr[i] = arr[i];
	}

	for (i = 0; i < ARR_SIZE - 1; i++)
	{
		for (j = 0; j < ARR_SIZE - i - 1; j++)
		{
			if (temparr[j] > temparr[j + 1])
			{
				temp = temparr[j];
				temparr[j] = temparr[j + 1];
				temparr[j + 1] = temp;
			}
		}
	}
}

void SelectionSort()
{
	int n = ARR_SIZE;
	int temp = 0;
	int i, j, min_idx;
	int temparr[ARR_SIZE];

	for (i = 0; i < ARR_SIZE; i++)
	{
		temparr[i] = arr[i];
	}

	for (i = 0; i < n - 1; i++)
	{
		
		min_idx = i;
		for (j = i + 1; j < n; j++)
		{
			if (temparr[j] < temparr[min_idx])
			{
				min_idx = j;
			}
		}

		temp = temparr[min_idx];
		temparr[min_idx] = temparr[i];
		temparr[i] = temp;
	}
}

int ShellSort()
{
	int n = ARR_SIZE;
	int temp = 0;
	int i, j, key;
	int temparr[ARR_SIZE];

	for (i = 0; i < ARR_SIZE; i++)
	{
		temparr[i] = arr[i];
	}

	for (int gap = n / 2; gap > 0; gap /= 2)
	{

		for (int i = gap; i < n; i += 1)
		{

			int temp = temparr[i];

			int j;
			for (j = i; j >= gap && temparr[j - gap] > temp; j -= gap)
				temparr[j] = temparr[j - gap];

			temparr[j] = temp;
		}
	}
	return 0;
}

void InsertionSort()
{
	int n = ARR_SIZE;
	int temp = 0;
	int i, j, key;
	int temparr[ARR_SIZE];

	for (i = 0; i < ARR_SIZE; i++)
	{
		temparr[i] = arr[i];
	}

	for (i = 1; i < n; i++)
	{
		key = temparr[i];
		j = i - 1;


		while (j >= 0 && temparr[j] > key)
		{
			temparr[j + 1] = temparr[j];
			j = j - 1;
		}
		temparr[j + 1] = key;
	}
}

int getNextGap(int gap)
{
	gap = (gap * 10) / 13;

	if (gap < 1)
		return 1;
	return gap;
}

void CombSort()
{
	int n = ARR_SIZE;

	int gap = n;
	int i, temparr[ARR_SIZE], temp;

	for (i = 0; i < ARR_SIZE; i++)
	{
		temparr[i] = arr[i];
	}

	int swapped = 1;

	while (gap != 1 || swapped == 1)
	{

		gap = getNextGap(gap);

		swapped = 0;

		for (i = 0; i < n - gap; i++)
		{
			if (temparr[i] > temparr[i + gap])
			{

				temp = temparr[i];
				temparr[i] = temparr[i + gap];
				temparr[i + gap] = temp;

				swapped = 1;
			}
		}
	}
}

int main(){
    FILE *fp;

    // Create a dynamic string array to pass data to other files
	char **strArr = calloc(ARR_SIZE + 2, sizeof(char *));
	for (int i = 0; i < ARR_SIZE + 2; i++)
		strArr[i] = calloc(50, sizeof(char));	//strArr contains our filename, array and NULL
		//these are needed when passing parameters to execvp later on

	// Convert the data to string and populate the string array
	for (int i = 1; i < ARR_SIZE + 1; i++)
		sprintf(strArr[i], "%d", arr[i]);

    clock_t start,end;
    pthread_t thread[10];
    double totalTime = 0;
    int k ;
    void (*fun_ptr)() = &BubbleSort; // [pointer function]
    fp = fopen("BubbleThread.csv", "w");
		fprintf(fp,"trial,time\n");
    for(k = 0 ; k < 10 ; k++){ // each algo will run 10 times having 10 thread in each iter.
         int i;
         start = clock();
         for(i=0;i<10;i++)
            pthread_create(&thread[i],NULL,(void*)fun_ptr,NULL);

         for(i=0;i<10;i++)
            pthread_join(thread[i],NULL);
        end = clock();
        double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        totalTime += cpu_time_used;
        printf("The time is %f \n",cpu_time_used);
        if(k == 9)
            {
                printf("The average time is %fs\n",totalTime/10);
                totalTime = 0;
            }
        fprintf(fp, "%d,%f\n",k+1, cpu_time_used);
    }
    strcpy(strArr[0], "./bubble");
    int j;
    for(j=0;j<10;j++)
    if (execvp("./bubble", strArr) == -1)
			printf("Error!\n");
     
}
    //  clock_t start, end;
    //  double cpu_time_used;
     
    //  start = clock();
    //  int i =0;
    //  for(i=0;i<100000;i++);
    //  end = clock();
    //  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    //  printf("%f",cpu_time_used);