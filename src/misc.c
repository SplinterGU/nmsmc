/**
 * @file misc.c
 * @brief Implementation of miscellaneous utility functions and macros for the No Man's Sky Mod Creator (nmsmc) project.
 *
 * This source file contains the implementation of various miscellaneous utility functions and macros
 * used within the No Man's Sky Mod Creator (nmsmc) project. These functions and macros provide various utilities
 * related to path handling, console output, string manipulation, and shell command execution.
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
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <stdarg.h>

#include "misc.h"

#ifdef _WIN32
#include <process.h>
#include <errno.h>
#include <windows.h>
#endif

/**
 * Trim leading and trailing white spaces from a string.
 *
 * This function trims any leading and trailing white spaces (including spaces,
 * tabs, and newline characters) from the given string in-place.
 *
 * @param str The string to trim. It will be modified in-place.
 */
void trim(char *str) {
    if (!str) return;

    // Find the length of the string
    int length = strlen(str);

    // Remove leading white spaces
    int start = 0;
    while (isspace(str[start])) {
        start++;
    }

    // Remove trailing white spaces
    int end = length - 1;
    while (end >= 0 && isspace(str[end])) {
        str[end] = '\0';
        end--;
    }

    // Shift the remaining characters to the beginning of the string
    if (start) {
        strcpy(str, &str[start]);
    }
}

/**
 * Execute a shell command with formatted arguments and handle signals.
 *
 * This function takes a format string and a variable number of arguments, calculates
 * the required buffer size, allocates memory for the buffer, and formats the string.
 * It then executes the command using the system function and handles signals.
 *
 * @param format    The format string for the shell command.
 * @param ...       Variable arguments to be formatted into the command.
 *
 * @return          The return code of the executed command. Returns -1 on errors.
 */
int do_command(const char* format, ...) {
    va_list args, args_copy;
    va_start(args, format);
    va_copy(args_copy, args);

    // Calculate the necessary buffer size
    int size = vsnprintf(NULL, 0, format, args);

    if (size < 0) {
        va_end(args);
        return -1; // Error in vsnprintf
    }

    // Allocate memory for the buffer and format the string
    char* buffer = (char*)malloc(size + 1);
    if (buffer == NULL) {
        va_end(args);
        return -1; // Error in memory allocation
    }

    // Use vsnprintf with the copied va_list
    vsnprintf(buffer, size + 1, format, args_copy);

    // End both va_lists
    va_end(args);
    va_end(args_copy);

    // Execute the command using system
    int result = system(buffer);
    free(buffer);

#ifndef _WIN32
    // Handle signals if the command was terminated by a signal
    if (WIFSIGNALED(result)) {
        int signalNum = WTERMSIG(result);
        raise(signalNum); // Raise the same signal that terminated the command
    }
#endif

    return result;
}
