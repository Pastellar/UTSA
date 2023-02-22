/////////////////////////////////////////////////////////////////////////////////////////
/*
 * @file: File: prog.c
 * @author: HAVEN KOTARA
 * @instructions: http://www.cs.utsa.edu/~korkmaz/teaching/resources-os-ug/tk-assign/assign5.html
 *
 * --------------------------------------------------------------------------------------
 * This program implements multithreading to measure the performance of the four basic CPU
 * scheduling algorithms. The program will emulate/simulate the processes whose priority,
 * sequence of CPU burst time(ms), and I/O burst time(ms) will be given in an input file.
 * All scheduling algorithms (except RR) are assumed non-preemptive and will ignore process
 * priorities.
 *
 * @Usage: ./prog -alg [FIFO|SJF|PR|RR] [-quantum integer(ms)] -input [file name]
 */
/////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdbool.h>
#include <math.h>
#include <errno.h> 
#include "helper.h"

/* Function: FileReadThread
 * Usage: pthread_create(&FileRead_t, NULL, FileReadThread, NULL);
 * --------------------------------------
 * This function reads the input file and creates
 * a new PCB process. This process is then added
 * to the ready queue.
 *
 */
void *FileReadThread();

/* Function: CPUSchedulerThread
 * Usage: pthread_create(&CPU_t, NULL, CPUSchedulerThread, NULL);
 * --------------------------------------
 * This function checks the ready queue and,
 * if there is a process, will pick one according
 * to the chosen algorithm. The CPU will sleep
 * for the given burst/quantum time(s). During this
 * time, the CPU will have put this process into 
 * the IO queue, or terminate if it was the last CPU
 * burst. The CPU will then check the ready queue again
 * for another process.
 *
 */
void *CPUSchedulerThread ();

/* Function: IOSystemThread
 * Usage: 
 * --------------------------------------
 * This function checks the IO queue. If there
 * is a process, it will sleep for the given
 * IO burst time. The IO System then puts this
 * process back into the ready queue, and checks
 * the IO queue again until no more processes remain.
 *
 */
void *IOSystemThread();

/* Function: Output
 * Usage: Output();
 * --------------------------------------
 * This function prints the final data to 
 * the terminal.
 *
 */
void Output();

/* Function: Clean
 * Usage: Clean();
 * --------------------------------------
 * This function frees the queues and 
 * destroys the semaphores.
 *
 */
void Clean();


PCB *readyQueue, *ioQueue, *sortedQueue;
sem_t semCPU, semIO;
FILE *inputFile;
char *fileName, *algName;           //Input file name & CPU scheduling algorithm
double util, throughput;            //CPU utilization & throughput
int quantum;                        //Quantum for RR
int cpuWait, ioWait;
int cpuDone, ioDone;
double elapsed = 0.0, waiting = 0.0;
double totalTurnaround = 0.0, totalWaiting = 0.0, totalCpuTime = 0.0;
struct timespec tsSec, tsCurrTime, tsCPUbegin, tsCPUend, tsBegin, tsEnd;
bool cpuBusy = false, ioBusy = false;
bool fileReadDone = false, cpuSchedDone = false, ioSysDone = false;

/////////////////////////////////////////////////////////////////////////////////////////
int main (int argc, char* argv[])
{
    if (argc < 5 || argc > 7)   //check if correct # of args were inputted
    {
        perror("Error");
        printf("Value of errno: %d\n", errno);
        Usage();
        exit(1);
    }

    //------------------------------------------    //initialize data
    pthread_t fileRead_t;
    pthread_t CPU_t;
    pthread_t IO_t;
    readyQueue = (PCB*)malloc(sizeof(PCB));
    ioQueue = (PCB*)malloc(sizeof(PCB));
    sortedQueue = (PCB*)malloc(sizeof(PCB));
    tsSec.tv_sec = 1;
    sem_init(&semCPU, 0, 0);
    sem_init(&semIO, 0, 0);
    //------------------------------------------



    if (argc == 5)                                                  //if alg is not RR
    {
        algName = argv[2];
        fileName = argv[4];
        pthread_create(&fileRead_t, NULL, FileReadThread, NULL);    //create the file read thread
    }

    else if (argc == 7)                                             //if alg is RR
    {
        algName = argv[2];
        quantum = atoi(argv[4]);
        fileName = argv[6];
        pthread_create(&fileRead_t, NULL, FileReadThread, NULL);    //create the file read thread
    }

    else                                                            //if incorrect usage
    {
        printf("Error: Incorrect usage!\n");
        Usage();
        free(readyQueue);
        free(ioQueue);
        exit(1);
    }

    clock_gettime(CLOCK_MONOTONIC, &tsBegin);                       //get the start time
    pthread_create(&CPU_t, NULL, CPUSchedulerThread, NULL);         //create the CPU & IO threads for simulation
    pthread_create(&IO_t, NULL, IOSystemThread, NULL);

    pthread_join(fileRead_t, NULL);                                 //join the threads
    pthread_join(CPU_t, NULL);
    pthread_join(IO_t, NULL);
    clock_gettime(CLOCK_MONOTONIC, &tsEnd);                        //get the end time

    Output();                                                       //print the data
    // printf("sorted: ");                                          //Left this here in case you want to see the sorted order of the processes
    // PrintQueue(sortedQueue);
    fclose(inputFile);                                              //close the file
    Clean();                                                        //free memory

    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////


/* Function: FileReadThread
 * Usage: 
 * --------------------------------------
 * This function reads the input file and creates
 * a new PCB process. This process is then added
 * to the ready queue.
 *
 */
void *FileReadThread()
{
    char *type = (char*)malloc(sizeof(8));                          //variable to hold the type of process
    PCB *newPCB;                                                    //create a new PCB node
    int PID = 0, PPR = 0, numRemaining, burst, i;
    inputFile = fopen(fileName, "r");                               //open the file

    if (inputFile == NULL)                                          //check if file exists
    {
        PrintError();
        Clean();
        exit(1);
    }

    while (!feof(inputFile))                                        //loop until end of file
    {
        fscanf(inputFile,"%s", type);                               //get the type of process

        if (strncmp(type, "proc", 5) == 0)                          //if the input is proc
        {
            ++PID;                                                  //increment the current PID
            fscanf(inputFile, "%d %d", &PPR, &numRemaining);        //get the priority & number of remaining bursts                                   
            
            // printf("PID: %d\n", PID);
            //----------------------------------------------------  //initialize data for the newPCB node
            newPCB = (PCB *)malloc( sizeof(PCB));
            newPCB->procID = PID;
            newPCB->procPR = PPR;
            newPCB->cpuIndex = 0;
            newPCB->ioIndex = 0;
            newPCB->numCpuBurst = numRemaining / 2 + 1;
            newPCB->numIoBurst = numRemaining / 2;
            newPCB->cpuBurst = (int*)malloc(sizeof(int) * numRemaining / 2 + 1);
            newPCB->ioBurst = (int*)malloc(sizeof(int) * numRemaining / 2);
            //----------------------------------------------------
            
            clock_gettime(CLOCK_MONOTONIC, &newPCB->tsBegin);       //get start time for newPCB
    
            for (i = 0; i < numRemaining; i++)                      //assign bursts to the list
            {
                fscanf(inputFile,"%d", &burst);

                if (i % 2 == 0)                                     //CPU bursts
                {
                    newPCB->cpuBurst[newPCB->cpuIndex] = burst;
                    newPCB->cpuIndex++;  
                }

                else                                                //IO bursts
                {
                    newPCB->ioBurst[newPCB->ioIndex] = burst;
                    newPCB->ioIndex++;
                } 
            }
            newPCB->cpuIndex = 0;                                   //initialize the rest of the ready queue
            newPCB->ioIndex = 0;
            newPCB->numCpuBurst = (int)(numRemaining / 2 + 1);
            newPCB->numIoBurst = (int)(numRemaining / 2);
            clock_gettime(CLOCK_MONOTONIC, &newPCB->queueEnterClock); 
            Enlist(readyQueue, newPCB);
            sem_post(&semCPU);                                      //unlock the CPU
        }

        if (strncmp(type, "sleep", 6) == 0)                         //let this thread usleep for the given ms
        {
            int sleep = 0;
            fscanf(inputFile,"%d", &sleep);
            usleep(sleep * 1000);
        }

        if (strncmp(type, "stop", 5) == 0)                          //let this thread break
        {
            break;
        }
    }

    free(type);
    fileReadDone = true;                                            //fileRead is complete
    pthread_exit(0);
}


/* Function: CPUSchedulerThread
 * Usage: 
 * --------------------------------------
 * This function checks the ready queue and,
 * if there is a process, will pick one according
 * to the chosen algorithm. The CPU will sleep
 * for the given burst/quantum time(s). During this
 * time, the CPU will have put this process into 
 * the IO queue, or terminate if it was the last CPU
 * burst. The CPU will then check the ready queue again
 * for another process.
 *
 */
void *CPUSchedulerThread ()
{
    PCB *node;                                                          //create a new PCB node
    
    while (true)
    {                                                                   //loop until queues are empty, !ioBusy, and fileRead is done        
        if (QueueIsEmpty(readyQueue) && QueueIsEmpty(ioQueue) && cpuBusy == false && ioBusy == false && fileReadDone == true)
        {
            break;
        }

        if (strcmp(algName, "FIFO") == 0)
        {
            int res = sem_timedwait(&semCPU, &tsSec);                  //waiting on cpu
            if (res == -1 && errno == ETIMEDOUT) continue;
            
            cpuBusy = true;                                             //cpu is now busy
            clock_gettime(CLOCK_MONOTONIC, &tsCPUbegin);                //get clock enter time
            node = Delist(readyQueue);                                  //get the head of the ready queue
            
            clock_gettime(CLOCK_MONOTONIC, &tsCurrTime);
            elapsed = tsCurrTime.tv_sec - node->queueEnterClock.tv_sec;
            elapsed += (tsCurrTime.tv_nsec - node->queueEnterClock.tv_nsec) / 1000000000.0;
            node->totalWait += elapsed * 1000;

            usleep(node->cpuBurst[node->cpuIndex] * 1000);              //sleep
            node->cpuIndex++;

            if (node->cpuIndex >= node->numCpuBurst)                    //if the last CPU burst is reached
            {
                clock_gettime(CLOCK_MONOTONIC, &node->tsEnd);           //get CPU end time
                elapsed = node->tsEnd.tv_sec - node->tsBegin.tv_sec;
                elapsed += (node->tsEnd.tv_nsec - node->tsBegin.tv_nsec) / 1000000000.0;
                totalTurnaround += (elapsed * 1000);                      //get total turnaround time
                totalWaiting += node->totalWait;                        //get total wait time

                clock_gettime(CLOCK_MONOTONIC, &tsCPUend);              //get clock exit time
                elapsed = tsCPUend.tv_sec - tsCPUbegin.tv_sec;
                // printf("elapsed: %lf\n", elapsed * 1000);            //tsCPUend and tsCPUbegin don't seem to be getting any times when clock_gettime is called
                elapsed += (tsCPUend.tv_nsec - tsCPUbegin.tv_nsec) / 1000000000.0;
                // printf("elapsed2: %lf\n", elapsed * 1000);
                totalCpuTime += elapsed * 1000;                         //get the total cpu time

                Enlist(sortedQueue, node);                              //stick the node in the sorted queue
                cpuBusy = false;                                        //CPU no longer busy
            }

            else                                                        //if an IO burst is reached
            {
                Enlist(ioQueue, node);                                  //enlist IO bursts into the IO queue
                cpuBusy = false;                                        //CPU no longer busy
                sem_post(&semIO);                                       //unlock the IO
            }
        }

        else if (strcmp(algName, "SJF") == 0)
        {
            PCB *node, *SJFnode;
            int index, i;
            int res = sem_timedwait(&semCPU, &tsSec);                   //waiting on cpu
            if (res == -1 && errno == ETIMEDOUT) continue;
            cpuBusy = true;                                             //cpu is now busy
            clock_gettime(CLOCK_MONOTONIC, &tsCPUbegin);                //get clock enter time
            

            if (!QueueIsEmpty(readyQueue))                              //parse through ready queue until empty
            {
                node = GetNodeAtIndex(readyQueue, 0);                   //get the current shortest job
                SJFnode = node;                                         //assign to the SJF node

                for (i = 0; i < QueueSize(readyQueue); i++)             //parse through queue to find the index of the SJF
                {
                    index = 0;                                          //set index to the head index
                    node = GetNodeAtIndex(readyQueue, i);               //get the index of the SJF

                    if (node->cpuBurst < SJFnode->cpuBurst)                       //if a new SJF is found
                    {
                        index = i;                                      //update the index
                    }
                }
            }

            SJFnode = GetNodeAtIndex(readyQueue, index);                //get the SJF
            DelistNode(readyQueue, SJFnode);                            //remove it from the ready queue

            clock_gettime(CLOCK_MONOTONIC, &tsCurrTime);
            elapsed = tsCurrTime.tv_sec - SJFnode->queueEnterClock.tv_sec;
            elapsed += (tsCurrTime.tv_nsec - SJFnode->queueEnterClock.tv_nsec) / 1000000000.0;
            SJFnode->totalWait += elapsed * 1000;

            usleep(SJFnode->cpuBurst[SJFnode->cpuIndex] * 1000);        //sleep
            SJFnode->cpuIndex++;

            if (SJFnode->cpuIndex >= SJFnode->numCpuBurst)              //if the last CPU burst is reached
            {
                clock_gettime(CLOCK_MONOTONIC, &SJFnode->tsEnd);           //get CPU end time
                elapsed = SJFnode->tsEnd.tv_sec - SJFnode->tsBegin.tv_sec;
                elapsed += (SJFnode->tsEnd.tv_nsec - SJFnode->tsBegin.tv_nsec) / 1000000000.0;
                totalTurnaround += (elapsed * 1000);                      //get total turnaround time
                totalWaiting += SJFnode->totalWait;                        //get total wait time

                clock_gettime(CLOCK_MONOTONIC, &tsCPUend);              //get clock exit time
                elapsed = tsCPUend.tv_sec - tsCPUbegin.tv_sec;
                elapsed += (tsCPUend.tv_nsec - tsCPUbegin.tv_nsec) / 1000000000.0;
                totalCpuTime += elapsed * 1000;                         //get the total cpu time
                Enlist(sortedQueue, SJFnode);                           //add SJF node to the sorted queue
                cpuBusy = false;                                        //CPU is no longer busy
            }

            else                                                        //if an IO burst is reached
            {
                Enlist(ioQueue, SJFnode);                               //add IO burst to the IO queue
                cpuBusy = false;                                        //CPU is no longer busy
                sem_post(&semIO);                                       //unlock the IO thread
            }
        }

        else if (strcmp(algName, "PR") == 0)
        {
            PCB *node, *PRnode;
            int index, i;
            int res = sem_timedwait(&semCPU, &tsSec);                   //waiting on cpu
            if (res == -1 && errno == ETIMEDOUT) continue;
            cpuBusy = true;                                             //cpu is now busy
            clock_gettime(CLOCK_MONOTONIC, &tsCPUbegin);                //get clock enter time

            if (!QueueIsEmpty(readyQueue))                              //parse through ready queue until empty
            {
                node = GetNodeAtIndex(readyQueue, 0);                   //get the minimum burst 
                PRnode = node;                                          //assign the current minimun to the PRnode

                for (i = 0; i < QueueSize(readyQueue); i++)
                {
                    index = 0;
                    node = GetNodeAtIndex(readyQueue, i);

                    if (node->procPR > PRnode->procPR)                  //if a higher priority is found
                    {
                        index = i;                                      //update the index
                    }
                }
            }

            PRnode = GetNodeAtIndex(readyQueue, index);                 //get the highest priority node
            DelistNode(readyQueue, PRnode);                             //remove it from the ready queue

            clock_gettime(CLOCK_MONOTONIC, &tsCurrTime);
            elapsed = tsCurrTime.tv_sec - PRnode->queueEnterClock.tv_sec;
            elapsed += (tsCurrTime.tv_nsec - PRnode->queueEnterClock.tv_nsec) / 1000000000.0;
            PRnode->totalWait += elapsed * 1000;

            usleep(PRnode->cpuBurst[PRnode->cpuIndex] * 1000);          //sleep
            PRnode->cpuIndex++;

            if (PRnode->cpuIndex > PRnode->numCpuBurst)                 //if the last node is reached
            {
                clock_gettime(CLOCK_MONOTONIC, &PRnode->tsEnd);           //get CPU end time
                elapsed = PRnode->tsEnd.tv_sec - PRnode->tsBegin.tv_sec;
                elapsed += (PRnode->tsEnd.tv_nsec - PRnode->tsBegin.tv_nsec) / 1000000000.0;
                totalTurnaround += (elapsed * 1000);                      //get total turnaround time
                totalWaiting += PRnode->totalWait;                        //get total wait time

                clock_gettime(CLOCK_MONOTONIC, &tsCPUend);              //get clock exit time
                elapsed = tsCPUend.tv_sec - tsCPUbegin.tv_sec;
                elapsed += (tsCPUend.tv_nsec - tsCPUbegin.tv_nsec) / 1000000000.0;
                totalCpuTime += elapsed * 1000;                         //get the total cpu time
                Enlist(sortedQueue, PRnode);                            //add the PR node to the sorted queue
                cpuBusy = false;                                        //CPU is no longer busy
            }

            else                                                        //if an IO node is reached
            {
                Enlist(ioQueue, PRnode);                                //add node to the IO queue
                cpuBusy = false;                                        //CPU is no longer busy
                sem_post(&semIO);                                       //unlock the IO thread
            }
        }

        else if (strcmp(algName, "RR") == 0)
        {
            PCB *RRnode;
            int res = sem_timedwait(&semCPU, &tsSec);                   //waiting on cpu
            if (res == -1 && errno == ETIMEDOUT) continue;
            cpuBusy = true;                                             //cpu is now busy
            clock_gettime(CLOCK_MONOTONIC, &tsCPUbegin);                //get clock enter time

            RRnode = Delist(readyQueue);

            clock_gettime(CLOCK_MONOTONIC, &tsCurrTime);
            elapsed = tsCurrTime.tv_sec - RRnode->queueEnterClock.tv_sec;
            elapsed += (tsCurrTime.tv_nsec - RRnode->queueEnterClock.tv_nsec) / 1000000000.0;
            RRnode->totalWait += elapsed * 1000;

            usleep(RRnode->cpuBurst[RRnode->cpuIndex] *1000);           //sleep

            if (RRnode->cpuIndex >= RRnode->numCpuBurst)                //if the last CPU burst is reached
            {
                clock_gettime(CLOCK_MONOTONIC, &RRnode->tsEnd);           //get CPU end time
                elapsed = RRnode->tsEnd.tv_sec - RRnode->tsBegin.tv_sec;
                elapsed += (RRnode->tsEnd.tv_nsec - RRnode->tsBegin.tv_nsec) / 1000000000.0;
                totalTurnaround += (elapsed * 1000);                      //get total turnaround time
                totalWaiting += RRnode->totalWait;                        //get total wait time

                clock_gettime(CLOCK_MONOTONIC, &tsCPUend);              //get clock exit time
                elapsed = tsCPUend.tv_sec - tsCPUbegin.tv_sec;
                elapsed += (tsCPUend.tv_nsec - tsCPUbegin.tv_nsec) / 1000000000.0;
                totalCpuTime += elapsed * 1000;                         //get the total cpu time
                Enlist(sortedQueue, RRnode);                             //add node to the sorted queue
                cpuBusy = false;                                         //CPU is no longer busy
            }

            else
            {
                if (RRnode->cpuBurst[RRnode->cpuIndex] <= quantum)      //if the node is RR
                {
                    usleep(RRnode->cpuBurst[RRnode->cpuIndex] * 1000);  //sleep
                    RRnode->cpuIndex++;                                 
                    Enlist(ioQueue, RRnode);                            //add node to the IO queue
                    sem_post(&semIO);                                   //unlock the IO thread
                }

                else                                                    //if the node isn't RR
                {
                    RRnode->cpuBurst[RRnode->cpuIndex] = RRnode->cpuBurst[RRnode->cpuIndex] - quantum;
                    clock_gettime(CLOCK_MONOTONIC, &RRnode->tsEnd);           //get CPU end time
                    elapsed = RRnode->tsEnd.tv_sec - RRnode->tsBegin.tv_sec;
                    elapsed += (RRnode->tsEnd.tv_nsec - RRnode->tsBegin.tv_nsec) / 1000000000.0;
                    totalTurnaround += (elapsed * 1000);                      //get total turnaround time
                    totalWaiting += RRnode->totalWait;                        //get total wait time

                    clock_gettime(CLOCK_MONOTONIC, &tsCPUend);              //get clock exit time
                    elapsed = tsCPUend.tv_sec - tsCPUbegin.tv_sec;
                    elapsed += (tsCPUend.tv_nsec - tsCPUbegin.tv_nsec) / 1000000000.0;
                    totalCpuTime += elapsed * 1000;                         //get the total cpu time

                    cpuBusy = false;                                    //CPU is no longer busy
                    Enlist(readyQueue, RRnode);                         //add the node to the ready queue
                    sem_post(&semCPU);                                  //unlock CPU
                }
            } 
        }

        else                                                            //if the algorithm is incorrect
        {
            printf("Error: Incorrect algorithm entered\n");
            Usage();
            exit(1);
        }
    }
    cpuSchedDone = true;                                                //CPU thread done
    pthread_exit(0);                                                
}


/* Function: IOSystemThread
 * Usage: 
 * --------------------------------------
 * This function checks the IO queue. If there
 * is a process, it will sleep for the given
 * IO burst time. The IO System then puts this
 * process back into the ready queue, and checks
 * the IO queue again until no more processes remain.
 *
 */
void *IOSystemThread()
{
    PCB *node;                                                          //create a new PCB node

    while (true)
    {
        if (QueueIsEmpty(readyQueue) && QueueIsEmpty(ioQueue) && cpuBusy == false && ioBusy == false && fileReadDone == true)
        {
            break;
        }

        while (QueueIsEmpty(ioQueue))
        {
            if (QueueIsEmpty(readyQueue) && cpuBusy == false && ioBusy == false && fileReadDone == true)
            {                                                           //if CPU thread completes & fileRead thread is done   
                ioDone = true;
                pthread_exit(0);
            }
            usleep(1000);                                               //sleep
        }
        
        int res = sem_timedwait(&semIO, &tsSec);                        //lock IO for specified amount of time
        if (res == -1 && errno == ETIMEDOUT) continue;
        ioBusy = true;                                                  //IO is now busy

        node = Delist(ioQueue);                                         //get the head of the IO queue
        usleep(node->ioBurst[node->ioIndex] * 1000);                    //sleep
        node->ioIndex++;

        Enlist(readyQueue, node);                                       //insert the node into the ready queue
        ioBusy = false;                                                 //IO is no longer busy
        sem_post(&semCPU);                                              //unlock the cpu
    }
    ioDone = true;                                                      //IO is done!
    pthread_exit(0);
}


/* Function: Output
 * Usage: Output();
 * --------------------------------------
 * This function prints the final data to 
 * the terminal.
 *
 */
void Output()
{
    int numProcesses = QueueSize(sortedQueue);

    waiting = tsEnd.tv_sec - tsBegin.tv_sec;
    waiting += (tsEnd.tv_nsec - tsBegin.tv_nsec) / 1000000000.0;

    if ((strcmp(algName, "FIFO") == 0) || (strcmp(algName, "SJF") == 0) || (strcmp(algName, "PR") == 0))
    {                                                                   //if not RR
        printf ("-------------------------------------------------------\n");
        printf("Input File Name\t\t\t: %s\n", fileName);
        printf("CPU Scheduling Alg\t\t: %s\n", algName);
        printf("CPU Utilization\t\t\t: %.2f%%\n", (totalCpuTime / waiting) * 100);
        printf("Throughput\t\t\t: %.4f processes / ms\n", waiting / totalWaiting);
        printf("Avg. Turnaround time\t\t: %.2fms\n", totalTurnaround / numProcesses);
        printf("Avg. Waiting Time In R Queue\t: %.2fms\n", (double)totalWaiting / numProcesses); 
        printf ("-------------------------------------------------------\n");
    }

    else if ((strcmp(algName, "RR") == 0))                              //if RR
    {
        printf ("-------------------------------------------------------\n");
        printf("Input File Name\t\t\t: %s\n", fileName);
        printf("CPU Scheduling Alg\t\t: %s quantum: %d\n", algName, quantum);
        printf("CPU Utilization\t\t\t: %.2f%%\n", ((double)totalCpuTime / elapsed) * 100);
        printf("Throughput\t\t\t: %.4f processes / ms\n", elapsed / totalWaiting);
        printf("Avg. Turnaround time\t\t: %.2fms\n", totalTurnaround / numProcesses);
        printf("Avg. Waiting Time In R Queue\t: %.2fms\n", (double)totalWaiting / numProcesses);
        printf ("-------------------------------------------------------\n");
    }
}


/* Function: Clean
 * Usage: Clean();
 * --------------------------------------
 * This function frees the queues and 
 * destroys the semaphores.
 *
 */
void Clean()
{
    FreeQueue(readyQueue);
    FreeQueue(ioQueue);
    FreeQueue(sortedQueue);
    sem_destroy(&semCPU);
    sem_destroy(&semIO);
}