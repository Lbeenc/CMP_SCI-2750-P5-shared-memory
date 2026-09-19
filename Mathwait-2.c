#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 12345

void print_help() {
    printf("Usage: ./mathwait [-h] <num1> <num2> ...\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_help();
        return 1;
    }

    // Check for the help option
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        print_help();
        return 0;
    }

    // Allocate shared memory
    int shmid = shmget(SHM_KEY, 2 * sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("Error in shmget");
        return 1;
    }

    // Attach shared memory
    int *shm = (int *)shmat(shmid, NULL, 0);
    if (shm == (int *)(-1)) {
        perror("Error in shmat");
        return 1;
    }

    // Set initial values in shared memory
    shm[0] = -2;
    shm[1] = -2;

    // Fork the process
    pid_t pid = fork();

    if (pid == -1) {
        perror("Error in fork");
        return 1;
    }

    if (pid == 0) {  // Child process
        // Parse command line arguments and find a pair that sums to 19
        int pair_found = 0;
        for (int i = 1; i < argc; i++) {
            for (int j = i + 1; j < argc; j++) {
                if (atoi(argv[i]) + atoi(argv[j]) == 19) {
                    pair_found = 1;
                    // Write the pair to shared memory
                    shm[0] = atoi(argv[i]);
                    shm[1] = atoi(argv[j]);
                    break;
                }
            }
            if (pair_found) {
                break;
            }
        }

        // If no pair is found, write -1 -1 to shared memory
        if (!pair_found) {
            shm[0] = -1;
            shm[1] = -1;
        }

        // Detach from shared memory
        shmdt(shm);

        exit(0);
    } else {  // Parent process
        // Wait for the child to finish
        wait(NULL);

        // Check shared memory for results
        if (shm[0] == -2 && shm[1] == -2) {
            fprintf(stderr, "Error: Shared memory not updated by child.\n");
        } else if (shm[0] == -1 && shm[1] == -1) {
            printf("No pair found by child.\n");
        } else {
            printf("Pair found by child: %d %d\n", shm[0], shm[1]);
        }

        // Detach from shared memory
        shmdt(shm);

        // Remove shared memory
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}
