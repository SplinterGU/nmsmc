/**
 * @file misc.h
 * @brief Miscellaneous utility functions and macros for the No Man's Sky Mod Creator (nmsmc) project.
 *
 * This header file provides miscellaneous utility functions and macros used within the No Man's Sky Mod Creator (nmsmc) project.
 * It includes constants for path lengths, device file paths for different platforms, macros for enabling/disabling console output,
 * and functions for string trimming and executing shell commands.
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

#ifndef __MISC_H
#define __MISC_H

/**
 * Constant for specifying the maximum path length, which is platform-dependent.
 * On Windows, it's defined as MAX_PATH; on Unix-like systems, it's PATH_MAX.
 */
#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif

/**
 * Constants for specifying standard device file paths on different platforms.
 * These constants define paths for the console and null devices for both Windows and Unix-like systems.
 */
#ifdef _WIN32
#define DEVCONSOLE     "CON"
#define DEVNULL        "NUL"
#else
#define DEVCONSOLE     "/dev/tty"
#define DEVNULL        "/dev/null"
#endif

/**
 * Macro for disabling console output by redirecting stdout and stderr to the null device.
 * Use this macro to suppress console output temporarily.
 */
#define DISABLE_CONSOLE     freopen(DEVNULL, "w", stdout); freopen(DEVNULL, "w", stderr);

/**
 * Macro for enabling console output by redirecting stdout and stderr back to the console.
 * Use this macro to restore console output after it has been disabled.
 */
#define ENABLE_CONSOLE      freopen(DEVCONSOLE, "w", stdout); freopen(DEVCONSOLE, "w", stderr);

/**
 * Trim leading and trailing white spaces from a string.
 *
 * This function trims any leading and trailing white spaces (including spaces,
 * tabs, and newline characters) from the given string in-place.
 *
 * @param str The string to trim. It will be modified in-place.
 */
void trim(char *str);

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
int do_command(const char* format, ...);

#endif /* __MISC_H */
