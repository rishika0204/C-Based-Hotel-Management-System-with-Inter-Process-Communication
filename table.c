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



void read_menu() {
	FILE *file;
	char line[100];

	// Open the file in read mode
	file = fopen("menu.txt", "r");

	// Check if the file is opened successfully
	if (file == NULL) {
    	printf("Error opening file.\n");
    	exit(EXIT_FAILURE);
	}

	// Read and print the menu
	while (fgets(line, sizeof(line), file) != NULL) {
    	printf("%s", line);
	}

	// Close the file
	fclose(file);
}

    
    

#define BUF_SIZE 500

int main() {

    
    
	int num_customers;
	int table_number;
	int total_bill;
    
   	 printf("\nEnter Table Number:\n");
	scanf("%d", &table_number);
	char id = table_number;
    

    struct shmid_ds b1;
    key_t key;
    key = ftok("table.c", id );

	if (key == -1) {
    	perror("ftok");
    	exit(EXIT_FAILURE);
	}
    int shmid;
    int *shmPtr;
    shmid= shmget(key, BUF_SIZE, 0666 | IPC_CREAT);
    
    if(shmid==-1)
    {
   	 perror("Error in shmget\n");
   	 return 1;
    }
    
    shmPtr= shmat(shmid, NULL, 0);
    if (shmPtr==(void*)-1)
    {
   	 perror("Error in shmPtr in attaching\n");
   	 return 1;
    }
    

    

	if (table_number > 10) {
    	printf("No Tables available");
    	exit(EXIT_FAILURE);
	}

	// Get the number of customers
	printf("\nEnter Number of Customers at Table (1-5): ");
	scanf("%d", &num_customers);
do{
	// Validate the number of customers
	if (num_customers < 1 || num_customers > 5) {
    	printf("Invalid number of customers. Exiting.\n");
    	exit(EXIT_FAILURE);
	}

	// Create pipes for communication with customers
	int t_order[5][100];  // Assuming a maximum of 100 inputs for each customer
	int indexes[5];   	// To store the actual number of elements entered by each customer
    
    
    pid_t pid;

    int fd_order[2];
	for (int i = 0; i < num_customers; i++) {
    	int order[100];
	int index = 0;

    	if (pipe(fd_order) == -1) {
        	perror("Error creating pipes");
        	exit(EXIT_FAILURE);
    	}

   	 
   	 

    	pid = fork();
   	 

    	if (pid < 0) {
        	perror("Fork Failed");
        	exit(EXIT_FAILURE);
    	}

    	if (pid == 0) { //child process
   	 
    	// Read the menu for each customer
        	read_menu();

        	int choice;
        	do {
            	printf("\nEnter the serial number(s) of the item(s) to order from the menu. Enter -1 when done: \n");
            	printf("Enter a number: ");
            	scanf("%d", &choice);

            	if (choice >= 1) {
                	order[index++] = choice;
            	} else if (choice == -1) {
                	printf("Moving to next customer as %d is entered.\n", choice);
                	break;
            	}

        	} while (choice != -1);

       	 
        	close(fd_order[0]);
        	write(fd_order[1], &index, sizeof(int));
        	write(fd_order[1], order, sizeof(int) * index);
        	close(fd_order[1]);
       	 
        	exit(EXIT_SUCCESS);
     	 
    	}
         	else {
        	// Parent process
        	waitpid(pid, NULL, 0); // Wait for the child

        	close(fd_order[1]);
        	read(fd_order[0], &indexes[i], sizeof(int));
        	read(fd_order[0], t_order[i], sizeof(int) * 100);
        	close(fd_order[0]);
       	 
    	}
  	 
	}
    
	key_t key1;
    key1 = ftok("waiter.c", id );

	if (key1 == -1) {
    	perror("ftok");
    	exit(EXIT_FAILURE);
	}
    
	int shmid1;
    int *shmPtr1;
    shmid1= shmget(key1, BUF_SIZE, 0666);
    
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
	//printf("\n%d", pid);
    int counter=0;
    do {
   	 if(counter>0)
   	 {
   	 for (int i = 0; i < num_customers; i++)
   	 {
   		 int order[100];
   		 int index = 0;
   		 if (pid == 0) { //child process
   	 
    	// Read the menu for each customer
        	read_menu();

        	int choice;
        	do {
            	printf("\nEnter the serial number(s) of the item(s) to order from the menu. Enter -1 when done: \n");
            	printf("Enter a number: ");
            	scanf("%d", &choice);

            	if (choice >= 1) {
                	order[index++] = choice;
            	} else if (choice == -1) {
                	printf("Moving to next customer as %d is entered.\n", choice);
                	break;
            	}

        	} while (choice != -1);

       	 
        	close(fd_order[0]);
        	write(fd_order[1], &index, sizeof(int));
        	write(fd_order[1], order, sizeof(int) * index);
        	close(fd_order[1]);
        	printf("Error");
        	exit(EXIT_SUCCESS);
     	 
    	}
         	else {
        	// Parent process
        	wait(NULL); // Wait for the child

        	close(fd_order[1]);
        	read(fd_order[0], &indexes[i], sizeof(int));
        	read(fd_order[0], t_order[i], sizeof(int) * 100);
        	close(fd_order[0]);
        	printf("Error1");
    	}
    	}
    	}
  	 
    
   	 counter+=1;
    //printf("\n%d",counter);
	//printf("\nArrays entered:\n");
	//for (int i = 0; i < num_customers; i++) {
    	//printf("Customer %d: ", i + 1);
    	//for (int j = 0; j < indexes[i]; j++) {
        //	printf("%d ", t_order[i][j]);
    	//}
    	//printf("\n");
	//}

    int total_items = 0;
	for (int i = 0; i < num_customers; i++) {
    	total_items += indexes[i];
	}

	// Create a one-dimensional array to store the elements
	int table_order[total_items];
	int current_index = 0;

	// Copy the elements
	for (int i = 0; i < num_customers; i++) {
    	for (int j = 0; j < indexes[i]; j++) {
        	table_order[current_index++] = t_order[i][j];
    	}
	}
	shmPtr[0]=total_items;
	for(int i=0; i<total_items; i++)
	{
   	 shmPtr[i+1]=table_order[i];
	}
    
    sleep(10);
    } while(shmPtr1[1]==0);
    
    sleep(10);
    total_bill= shmPtr1[0];
    printf("The total bill amount is %d INR", total_bill);
    
  
    printf("\nEnter Number of Customers at Table (1-5): ");
	scanf("%d", &num_customers);
	
	if(num_customers == -1)
	{
		shmPtr1[2]=0;
	}
	else
	{
		shmPtr1[2]=1;
	}
    shmdt(shmPtr1);
    }while(num_customers != -1);
    
    shmdt(shmPtr);
	shmctl(shmid, IPC_RMID, NULL);
    
	//shmdt(shmPtr1);
    
    
    
	return 0;
}


