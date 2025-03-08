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

int readMenuPrices(const char* filename, int** prices) {
	FILE* file = fopen(filename, "r");

	if (file == NULL) {
    	perror("Error opening file");
    	return -1;  // Return -1 to indicate failure
	}

	int price;
	int count = 0;

	// Count the number of lines in the file to determine the array size
	while (fscanf(file, "%*d. %*s %*s %d INR", &price) == 1) {
    	count++;
	}

	// Allocate memory for the array of prices
	*prices = (int*)malloc(count * sizeof(int));
	if (*prices == NULL) {
    	perror("Memory allocation error");
    	fclose(file);
    	return -1;  // Return -1 to indicate failure
	}

	// Reset file pointer to the beginning of the file
	fseek(file, 0, SEEK_SET);

	// Read prices into the array
	for (int i = 0; i < count; i++) {
    	fscanf(file, "%*d. %*s %*s %d INR", &(*prices)[i]);
	}

	fclose(file);

	return count;  // Return the number of items in the menu
}



int main()
{

const char* menuFile = "menu.txt";
	int* prices;
	int numItems = readMenuPrices(menuFile, &prices);

	if (numItems == -1) {
    	printf("Failed to read menu prices.\n");
    	return EXIT_FAILURE;
	}

	//printf("Menu prices array:\n");
	//for (int i = 0; i < numItems; i++) {
	//	printf("%d ", prices[i]);
	//}
	//printf("\n");
	//printf("%d", numItems);

    

    int waiter_ID;
    
    int total_items;
    
    printf("\nEnter Waiter ID:\n");
    scanf("%d", &waiter_ID);
    char id = waiter_ID;
    struct shmid_ds b1;
    key_t key;
    key = ftok("table.c", id );

	if (key == -1) {
    	perror("ftok");
    	exit(EXIT_FAILURE);
	}
    int shmid;
    int *shmPtr;
    shmid= shmget(key, BUF_SIZE, 0666);
    
    if(shmid==-1)
    {
   	 perror("Error in shmget\n");
   	 return 1;
    }
    
    shmPtr= shmat(shmid, NULL, 0);
    if (shmPtr==(void*)-1)
    {
   	 perror("Error in shmPtr in aataching\n");
   	 return 1;
    }
    
    key_t key1;
    key1 = ftok("waiter.c", id );

	if (key1 == -1) {
    	perror("ftok");
    	exit(EXIT_FAILURE);
	}
    
	int shmid1;
    int *shmPtr1;
    shmid1= shmget(key1, BUF_SIZE, 0666 | IPC_CREAT);
    
    if(shmid1==-1)
    {
   	 perror("Error in shmget\n");
   	 return 1;
    }
    
    shmPtr1= shmat(shmid1, NULL, 0);
    if (shmPtr1==(void*)-1)
    {
   	 perror("Error in shmPtr in aataching\n");
   	 return 1;
    }
    sleep(10);
    int status;
    do{
    int total_bill=0;
    do {
    total_items=shmPtr[0];
    int table_order[total_items];
    for(int i=0; i<total_items; i++)
	{
   	 table_order[i]= shmPtr[i+1];
   	 //printf("%d\n", table_order[i]);
   	 
   	 if(table_order[i]<1 | table_order[i]> numItems)
   	 {
   		 shmPtr1[1]= 0; // 0 means order is wrong and table.c has to take the whole order again
   	 }
   	 else
   	 {
   		 total_bill+= prices[table_order[i]-1];
   		 shmPtr1[1]=1;  // 1 means order was correct
   	 }
   	 //printf("\n%d", shmPtr1[1]);
	} sleep(10);
	} while(shmPtr1[1]==0);
    
	printf("\nBill Amount for Table %d: %d INR\n ", waiter_ID, total_bill);
    
    
    
    
    shmPtr1[0]= total_bill;
    
    sleep(2);
    key_t key2;
    key2 = ftok("hotelmanager.c", id );

	if (key2 == -1) {
    	perror("ftok");
    	exit(EXIT_FAILURE);
	}
    
	int shmid2;
    int *shmPtr2;
    shmid2= shmget(key2, BUF_SIZE, 0666);
    
    if(shmid2==-1)
    {
   	 perror("Error in shmget\n");
   	 return 1;
    }
    
    shmPtr2= shmat(shmid2, NULL, 0);
    if (shmPtr2==(void*)-1)
    {
   	 perror("Error in shmPtr in aataching\n");
   	 return 1;
    }
    
    shmPtr2[0]= total_bill;
    sleep(5);
    status = shmPtr1[2];
    //printf("status: %d", status);
    shmdt(shmPtr2);
    sleep(10);
    }while(status == 1);
    
    
    shmdt(shmPtr);
    
    
	shmdt(shmPtr1);
	shmctl(shmid1, IPC_RMID, NULL);
    
	//shmdt(shmPtr2);
    
	sleep(20);
	// Free allocated memory
	free(prices);
    
	return 0;
}

