/**
 * @file spawn.c
 * @brief Implementation of process spawning and execution utility functions for the No Man's Sky Mod Creator (nmsmc) project.
 *
 * This source file contains the implementation of process spawning and execution utility functions
 * used within the No Man's Sky Mod Creator (nmsmc) project. These functions provide the ability to spawn child processes,
 * manage their execution modes, and retrieve their exit statuses.
 *
 * This file is part of the No Man's Sky Mod Creator (nmsmc) project.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Juan José Ponteprino
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @author Juan José Ponteprino
 * @date October 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include "spawn.h"

/**
 * Spawns a new process using the provided path.
 *
 * @param mode The mode of execution (P_OVERLAY, P_NOWAIT, or P_WAIT).
 * @param path The path to the executable.
 * @param argv An array of strings representing command-line arguments.
 * @return Returns the exit status of the child process if in P_WAIT mode, or the PID of the child process.
 *         Returns -1 in case of an error.
 */
int spawnv(int mode, const char* path, char* const argv[]) {
    // Check if the mode is not P_OVERLAY
    if (mode != P_OVERLAY) {
        pid_t pid = fork(); // Create a new process using fork()

        if (pid == -1) {
            perror("Error creating the process");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // This code runs in the child process
            execv(path, argv); // Execute the program with arguments
            perror("Error executing the program");
            exit(EXIT_FAILURE);
        } else {
            // This code runs in the parent process
            if (mode == P_WAIT) {
                int status;
                waitpid(pid, &status, 0); // Wait for the child process to finish
                if (WIFEXITED(status)) {
                    return WEXITSTATUS(status); // Return the exit status of the child process
                } else {
                    perror("Error in the child process");
                    return -1;
                }
            }
            // In other modes, continue executing the parent process and return the PID of the child.
            return pid;
        }
    } else {
        // P_OVERLAY: Replace the parent process with the child process using execv.
        execv(path, argv); // Execute the program with arguments
        perror("Error executing the program");
        exit(EXIT_FAILURE);
    }
}

/**
 * Spawns a new process using the PATH environment variable.
 *
 * @param mode The mode of execution (P_OVERLAY, P_NOWAIT, or P_WAIT).
 * @param file The name of the executable (resolved using the PATH environment variable).
 * @param argv An array of strings representing command-line arguments.
 * @return Returns the exit status of the child process if in P_WAIT mode, or the PID of the child process.
 *         Returns -1 in case of an error.
 */
int spawnvp(int mode, const char* file, char* const argv[]) {
    // Check if the mode is not P_OVERLAY
    if (mode != P_OVERLAY) {
        pid_t pid = fork(); // Create a new process using fork()

        if (pid == -1) {
            perror("Error creating the process");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // This code runs in the child process
            execvp(file, argv); // Execute the program with arguments using PATH
            perror("Error executing the program");
            exit(EXIT_FAILURE);
        } else {
            // This code runs in the parent process
            if (mode == P_WAIT) {
                int status;
                waitpid(pid, &status, 0); // Wait for the child process to finish
                if (WIFEXITED(status)) {
                    return WEXITSTATUS(status); // Return the exit status of the child process
                } else {
                    perror("Error in the child process");
                    return -1;
                }
            }
            // In other modes, continue executing the parent process and return the PID of the child.
            return pid;
        }
    } else {
        // P_OVERLAY: Replace the parent process with the child process using execvp.
        execvp(file, argv); // Execute the program with arguments using PATH
        perror("Error executing the program");
        exit(EXIT_FAILURE);
    }
}
