#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define BUF_SIZE 500

int main()
{
	int total_tables;
	printf("Enter the Total Number of Tables at the Hotel:\n");
	scanf("%d", &total_tables);

	int total_earning[total_tables];
	long int num_processes[total_tables];
	double hotel_earning=0;
    double total_wages;
    double profit;
    int *shmPt[total_tables];
	for (int i = 0; i < total_tables; i++)
	{
   	 struct shmid_ds buf;
    	char id = i + 1;
    	key_t key2;
    	key2 = ftok("hotelmanager.c", id);

    	if (key2 == -1)
    	{
        	perror("ftok");
        	exit(EXIT_FAILURE);
    	}

    	int shmid2;
    	int *shmPtr2;
    	shmid2 = shmget(key2, BUF_SIZE, 0666 | IPC_CREAT);

    	if (shmid2 == -1)
    	{
        	perror("Error in shmget\n");
        	return 1;
    	}

	shmPtr2 = shmat(shmid2, NULL, 0);
    	if (shmPtr2 == (void *)-1)
    	{
        	perror("Error in shmPtr in attaching\n");
        	return 1;
    	}
    	
    	sleep(5);
   	 shmPt[i]= shmPtr2;
   	 //if(shmctl(shmid2, IPC_STAT, &buf) != -1)
   	 //{
   	//	 if(buf.shm_nattch != 1)
   	//	 {
   	//		 i--;
   	//		 continue;
   	//	 }
   	// }
   	// else
   	// {
   	// printf("Error in reading number of processes");
   	// }
   	 
   	 
   	 
   	 sleep(30);
   	 
    	total_earning[i] = shmPtr2[0];
   	 
   	 
   	 
    	//printf("\nEarning from Table %d: %d INR\n", i + 1, total_earning[i]);
   	 
   	 
   	 
	}

	// Write earnings to the file
	FILE *file = fopen("earnings.txt", "w");
	if (file == NULL)
	{
    	perror("Error opening file earnings.txt");
    	exit(EXIT_FAILURE);
	}

	for (int i = 0; i < total_tables; i++)
	{
    	fprintf(file, "Earning from Table %d: %d INR\n", i + 1, total_earning[i]);
	}

	fclose(file);
    
	sleep(45);
    
	for (int i = 0; i < total_tables; i++)
	{
    	char id = i + 1;
    	struct shmid_ds buf;
    	key_t key2;
    	key2 = ftok("hotelmanager.c", id);
   	 
    	if (key2 == -1)
    	{
        	perror("ftok");
        	exit(EXIT_FAILURE);
    	}

    	int shmid2;
   	 
    	shmid2 = shmget(key2, BUF_SIZE, 0666);

    	if (shmid2 == -1)
    	{
        	perror("Error in shmget\n");
        	return 1;
    	}
   	 
   	 
   	 
    	if(shmctl(shmid2, IPC_STAT, &buf) != -1)
   	 {
   		 num_processes[i]= buf.shm_nattch;
   		 //printf("No. of processes: %ld", num_processes[i]);
   	 }
   	 else{
   	 printf("Error in reading number of processes");
   	 }
   	 
   	 shmdt(shmPt[i]);
   	 shmctl(shmid2, IPC_RMID, NULL);
    }
   	 
    
	sleep(2);
	key_t key1;
	key1 = ftok("admin.c", 'A');

	if (key1 == -1)
	{
    	perror("ftok");
    	exit(EXIT_FAILURE);
	}

	int shmid1;
	int *shmPtr1;
	shmid1 = shmget(key1, BUF_SIZE, 0666 | IPC_CREAT);

	if (shmid1 == -1)
	{
    	perror("Error in shmget\n");
    	return 1;
	}

	shmPtr1 = shmat(shmid1, NULL, 0);
	if (shmPtr1 == (void *)-1)
	{
    	perror("Error in shmPtr in attaching\n");
    	return 1;
	}
	
    	
    	
	do
	{
	//printf("Value of Y/N: %d", shmPtr1[0]);
	if(shmPtr1[0]==1)
	{
   	 int c=0;
   	 do
   	 {
   		 
   		 for (int i = 0; i < total_tables; i++)
   		 {
   			 if(num_processes[i]== 1)
   			 {
   				 //printf("%ld\n", num_processes[i]);
   				 c++;
   			 }
   			 else
   			 {
   				 break;
   			 }
   		 }
   		 
   	 } while(c!= total_tables);
   	 
   	 for (int i = 0; i < total_tables; i++)
   	 {
   		 hotel_earning+= total_earning[i];
   	 }
   	 
   	 total_wages= 0.4* hotel_earning;
   	 profit= hotel_earning-total_wages;
   	 
   	 printf("\nTotal Earnings of the hotel: %lf\n", hotel_earning);
   	 printf("Total Wages of Waiters: %lf\n", total_wages);
   	 printf("Total Profit: %lf\n", profit);
   	 
   	 FILE *file = fopen("earnings.txt", "a+");  // Open the file in append mode

   	 if (file == NULL)
   	 {
   		 perror("Error opening file earnings.txt");
   		 exit(EXIT_FAILURE);
   	 }

   	 fprintf(file, "\n\nTotal Earnings of the Hotel: %lf INR\n",    	 hotel_earning);
   	 fprintf(file, "Total Wages of Waiters: %lf INR\n", total_wages);
   	 fprintf(file, "Total Profit: %lf INR\n", profit);

   	 fclose(file);  // Close the file after writing

   	 
   	 
   	 // Display termination message
   	 printf("\n\nThank you for visiting the Hotel!\n");
   	 return 0;
   	 }
	} while(shmPtr1[0]!=1);
    
	shmdt(shmPtr1);
    
	return 0;   
}


