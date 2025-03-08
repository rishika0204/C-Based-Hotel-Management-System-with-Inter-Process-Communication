# C-Based Hotel Management System with Inter-Process Communication

## Overview

This project is a **C-based hotel management system** that simulates real-time hotel operations using **inter-process communication (IPC)**. The system uses shared memory and semaphores to facilitate real-time data sharing and synchronization across multiple modules, including administration, table management, and waiter services. The system is designed to manage multiple tables, track earnings, handle customer orders, and calculate total bills efficiently.

## Features

- **Table Management**: Each table has its own process, which manages customer orders and interactions in real time.
- **Customer Order Handling**: Orders are taken via child processes that communicate with the parent process to manage the orders.
- **Real-Time Data Exchange**: Shared memory is used to facilitate data exchange between the administrator, waiter, and table management processes.
- **Earnings and Profit Calculation**: The system calculates the earnings from each table, total wages for waiters, and hotel profit, then writes this information to a file.
- **Hotel Shutdown**: The system allows for the closing of the hotel, notifying all relevant processes to terminate.

## Technologies Used

- **Programming Language**: C
- **Inter-Process Communication (IPC)**: Shared memory, semaphores
- **System Tools**: System V shared memory and message queues

## How It Works

### Components

1. **Admin Process**:
   - Initiates and manages the hotel closing process.
   - Uses shared memory to communicate the shutdown command to other modules.
   
2. **Hotel Manager Process**:
   - Tracks the total earnings and waiter wages.
   - Communicates with each table’s process to retrieve the total earnings.
   
3. **Table Process**:
   - Manages customer orders using pipes to communicate with waiter processes.
   - Updates shared memory with total earnings for the table.
   
4. **Waiter Process**:
   - Retrieves the table's order, verifies it, and calculates the total bill.
   - Updates shared memory with the final bill amount.

### Flow

- The admin can close the hotel, which signals all processes to stop accepting new orders and finalize calculations.
- Waiters take orders and validate them, communicating with the table processes.
- The hotel manager processes the earnings and profits, and writes the results to a file.

## Setup and Usage

### Prerequisites

- A Unix-like environment (Linux, macOS)
- GCC compiler

### Steps to Run

1. Clone or download the repository.
2. Open a terminal and navigate to the project directory.
3. Compile the source files:
   ```bash
   gcc -o hotelmanager hotelmanager.c
   gcc -o admin admin.c
   gcc -o waiter waiter.c
   gcc -o table table.c
   ```
4. Run the processes in separate terminal windows:
   - Start the **Admin Process**:
     ```bash
     ./admin
     ```
   - Start the **Hotel Manager Process**:
     ```bash
     ./hotelmanager
     ```
   - Start the **Waiter Process** (repeat for multiple waiters):
     ```bash
     ./waiter
     ```
   - Start the **Table Processes** (repeat for each table):
     ```bash
     ./table
     ```

5. Follow the prompts in each terminal to simulate the operations.

### Example Workflow

1. The **Admin Process** can close the hotel by entering 'Y' when prompted.
2. The **Hotel Manager Process** calculates the total earnings, wages, and profit and writes the results to `earnings.txt`.
3. The **Table Processes** handle customer orders, while the **Waiter Processes** validate and calculate the total bill.

## Contributing

Feel free to fork the repository and submit pull requests. Suggestions and improvements are always welcome!

