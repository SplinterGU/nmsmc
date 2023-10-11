/**
 * @file fs_utils.h
 * @brief File and directory utility functions for the No Man's Sky Mod Creator (nmsmc) project.
 *
 * This header file provides utility functions for file and directory operations used within
 * the No Man's Sky Mod Creator (nmsmc) project. It includes functions to work with paths,
 * directories, temporary directories, and more.
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

#ifndef __FS_UTILS_H
#define __FS_UTILS_H

#include <sys/types.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define stat _stat
#define mkdir(path, mode) _mkdir(path)
#else
#include <dirent.h>
#include <ctype.h>
#endif

/**
 * Get the system's temporary directory path.
 *
 * This function retrieves the system's temporary directory path.
 * On Windows, it uses the GetTempPathA function to obtain the path,
 * while on Unix/Linux, it checks the TMPDIR and TMP environment variables.
 * If neither is defined, it defaults to /tmp on Unix/Linux.
 *
 * @return const char*: A pointer to a string representing the system's temporary directory path.
 */
const char *tempdir();

/**
 * Get the current working directory.
 *
 * This function returns the path of the current working directory.
 *
 * @return                  A string containing the current working directory.
 */
char *get_current_dir();

#ifdef _WIN32
/**
 * Change the current working directory.
 *
 * This changes the current working directory to the specified path.
 *
 * @param x                 The path of the directory to change to.
 *
 * @return                  0 on success, 1 on error.
 */

#define chdir(x)    (!SetCurrentDirectory(x))
#endif

#ifdef _WIN32
/**
 * Create a unique temporary directory.
 *
 * This function attempts to create a unique temporary directory based on the provided template.
 * The template must contain at least 6 trailing "X" characters, which will be replaced with random characters
 * to create a unique directory name. The function makes multiple attempts (up to 100) to ensure a unique directory.
 *
 * @param template: A pointer to a string serving as the template for the temporary directory name.
 *                  It must contain at least 6 trailing "X" characters.
 *
 * @return char*: A pointer to the string containing the created temporary directory name if successful,
 *               or NULL if a unique directory could not be created after multiple attempts.
 */
char* mkdtemp(char* template);
#endif

/**
 * Recursively create directories in a given path.
 *
 * This function creates directories in the specified path recursively.
 *
 * @param path  The path to create, including any necessary parent directories.
 * @param mode  The permissions mode for the created directories.
 *
 * @return      0 on success, -1 on failure.
 */
int mkpath(const char *path, mode_t mode);

/**
 * Recursively remove a directory and its contents.
 *
 * This function removes a directory and its contents recursively.
 *
 * @param path  The path to the directory to remove.
 *
 * @return      0 on success, -1 on failure.
 */
int removedir(const char *path);

/**
 * Copy a file from source to destination.
 *
 * This function copies a file from the source path to the destination path.
 *
 * @param source    The path to the source file.
 * @param dest      The path to the destination file.
 *
 * @return          1 on success, 0 on failure.
 */
int copy_file(const char* source, const char* dest);

/**
 * Convert a Windows-style path to a Unix-style path.
 *
 * This function takes a Windows-style path and converts it to a Unix-style path
 * by replacing backslashes with forward slashes.
 * If the `resolved_path` parameter is NULL, the function allocates memory for the
 * converted path and returns it.
 *
 * @param path              The Windows-style path to convert.
 * @param converted_path    A pointer to a character buffer to store the converted path,
 *                          or NULL to allocate memory dynamically.
 *
 * @return                  A pointer to the converted Unix-style path.
 *                          If `converted_path` is provided, it will be used; otherwise,
 *                          dynamically allocated memory will be used. Returns NULL on error.
 */

char *path_to_unix(const char* path, char *converted_path);

/**
 * Convert a Unix-style path to a Windows-style path.
 *
 * This function takes a Unix-style path and converts it to a Windows-style path
 * by replacing backslashes with forward slashes.
 * If the `resolved_path` parameter is NULL, the function allocates memory for the
 * converted path and returns it.
 *
 * @param path              The Unix-style path to convert.
 * @param converted_path    A pointer to a character buffer to store the converted path,
 *                          or NULL to allocate memory dynamically.
 *
 * @return                  A pointer to the converted Windows-style path.
 *                          If `converted_path` is provided, it will be used; otherwise,
 *                          dynamically allocated memory will be used. Returns NULL on error.
 */

char *path_to_dos(const char* path, char *converted_path);

#endif /* __FS_UTILS_H */
