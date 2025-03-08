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

	char choice;
	do
	{
    	printf("Do you want to close the hotel? Enter Y for Yes and N for No.\n");
    	scanf(" %c", &choice);

    	if (choice == 'Y')
    	{
        	// Inform hotel manager to close
        	shmPtr1[0] = 1;
        	printf("Closing the hotel...\n");
    	}
    	else if (choice == 'N')
    	{
   		 shmPtr1[0]=0;
        	printf("Hotel remains open.\n");
    	}
    	else
    	{
        	printf("Invalid input. Please enter Y or N.\n");
    	}
	} while (choice != 'Y');
    sleep(30);
    
	// Cleanup shared memory
	shmdt(shmPtr1);
	shmctl(shmid1, IPC_RMID, NULL);

	return 0;
}

