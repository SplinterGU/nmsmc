/**
 * @file spawn.h
 * @brief Process spawning and execution utility functions for the No Man's Sky Mod Creator (nmsmc) project.
 *
 * This header file provides process spawning and execution utility functions for use within the No Man's Sky Mod Creator (nmsmc) project.
 * It includes constants for specifying process execution modes and functions for spawning and managing child processes.
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

#ifndef __SPAWN_H
#define __SPAWN_H

/**
 * Constant for specifying that the spawned process should be waited for to complete (synchronous execution).
 * This means the parent process will wait for the child process to finish before continuing.
 */
#define P_WAIT 0

/**
 * Constant for specifying that the spawned process should run asynchronously (asynchronously execution).
 * This means the parent process and child process run concurrently, and the parent doesn't wait for the child to finish.
 */
#define P_NOWAIT 1

/**
 * Constant for specifying that the spawned process should replace the current process (overlay execution).
 * This means the current process is replaced by the child process, and the parent process effectively ends.
 */
#define P_OVERLAY 2

/**
 * Constant for specifying that the spawned process should run asynchronously (asynchronously execution),
 * and the child process is detached from the parent, meaning it can continue running independently.
 */
#define P_NOWAITO 3

/**
 * Constant for specifying that the spawned process should run asynchronously (asynchronously execution),
 * and the parent process is detached from the child, meaning it can continue running independently.
 */
#define P_DETACH 4

/**
 * Spawns a new process using the provided path.
 *
 * @param mode The mode of execution (P_OVERLAY, P_NOWAIT, or P_WAIT).
 * @param path The path to the executable.
 * @param argv An array of strings representing command-line arguments.
 * @return Returns the exit status of the child process if in P_WAIT mode, or the PID of the child process.
 *         Returns -1 in case of an error.
 */
int spawnv(int mode, const char* path, char* const argv[]);

/**
 * Spawns a new process using the PATH environment variable.
 *
 * @param mode The mode of execution (P_OVERLAY, P_NOWAIT, or P_WAIT).
 * @param file The name of the executable (resolved using the PATH environment variable).
 * @param argv An array of strings representing command-line arguments.
 * @return Returns the exit status of the child process if in P_WAIT mode, or the PID of the child process.
 *         Returns -1 in case of an error.
 */
int spawnvp(int mode, const char* file, char* const argv[]);

#endif /* __SPAWN_H */
