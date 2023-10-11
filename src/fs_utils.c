/**
 * @file fs_utils.c
 * @brief Implementation of file and directory utility functions for the No Man's Sky Mod Creator (nmsmc) project.
 *
 * This source file contains the implementation of various utility functions for file and directory operations
 * used within the No Man's Sky Mod Creator (nmsmc) project. These functions provide functionality for handling
 * paths, directories, temporary directories, and more.
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
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define stat _stat
#define mkdir(path, mode) _mkdir(path)
#define snprintf _snprintf
#endif

#include "fs_utils.h"

#define MAX_SIZE 4096

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
const char *tempdir() {
#ifdef _WIN32    
    static char tmpdir[MAX_PATH];
    DWORD length = GetTempPathA(MAX_PATH, tmpdir);
#else
    const char* tmpdir = getenv("TMPDIR");
    if (tmpdir == NULL) {
        tmpdir = getenv("TMP");
        if (tmpdir == NULL) {
            tmpdir = "/tmp"; // Default temporary directory on Linux
        }
    }
#endif
    return tmpdir;
}

/**
 * Get the current working directory.
 *
 * This function returns the path of the current working directory.
 *
 * @return  A string containing the current working directory.
 */

char *get_current_dir() {
#ifdef _WIN32
    DWORD size = GetCurrentDirectory(0, NULL);
    if ( !size ) return NULL;
    char *path = malloc(size);
    GetCurrentDirectory(size, path);
#else
    char *path = getcwd(NULL, 0);
#endif
    return path;
}

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

char *path_to_unix(const char* path, char *converted_path) {
    if (!path && !converted_path) return NULL;

    if (!converted_path) {
        converted_path = strdup(path);
        if (!converted_path) {
            return NULL; // Memory allocation error
        }
    } else {
        if ( path && path != converted_path ) strcpy(converted_path, path);
    }

    // Replace backslashes with forward slashes
    char *p = converted_path;
    while ((p = strchr(p, '\\'))) {
        *p = '/';
        p++;
    }

    return converted_path;
}


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

char *path_to_dos(const char* path, char *converted_path) {
    if (!path && !converted_path) return NULL;

    if (!converted_path) {
        converted_path = strdup(path);
        if (!converted_path) {
            return NULL; // Memory allocation error
        }
    } else {
        if ( path && path != converted_path ) strcpy(converted_path, path);
    }

    // Replace backslashes with forward slashes
    char *p = converted_path;
    while ((p = strchr(p, '/'))) {
        *p = '\\';
        p++;
    }

    return converted_path;
}

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
int mkpath(const char *path, mode_t mode) {
    char *q, *r;
    int rv = -1;

    if ( !path[0]
        || strcmp(path, ".") == 0
        || strcmp(path, "/") == 0 
#ifdef _WIN32
        || ( strlen(path) == 2 && path[1] == ':' ) 
        || ( strlen(path) > 2 && strcmp(path + strlen(path) - 2, ":.") == 0 ) 
        || ( strlen(path) > 2 && ( strcmp(path + strlen(path) - 2, ":/") == 0 ) || strcmp(path + strlen(path) - 2, ":\\") == 0 ) 
#endif
        ) return 0;

    if (!(q = strdup(path))) return -1;

    if (!(r = dirname(q))) {
        free(q);
        return -1;
    }

    if ((mkpath(r, mode) == -1) && (errno != EEXIST)) {
        free(q);
        return -1;
    }

    if ((mkdir(path, mode) == -1) && (errno != EEXIST)) {
        rv = -1;
    } else {
        rv = 0;
    }

    free(q);

    return (rv);
}

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
char* mkdtemp(char* template) {
    if (template == NULL || strlen(template) < 6 || strcmp(&template[strlen(template) - 6], "XXXXXX")) {
        return NULL;
    }

    char * template_mask = &template[strlen(template) - 6];
    int attempt = 0;
    while (attempt < 100) {
        for (int i = 0; i < 6; i++ ) template_mask[i] = 'a' + (rand() % 26);
        if (!mkpath(template, 0700)) return template;
        attempt++;
    }

    return NULL;
}
#endif

/**
 * Recursively remove a directory and its contents.
 *
 * This function removes a directory and its contents recursively.
 *
 * @param path  The path to the directory to remove.
 *
 * @return      0 on success, -1 on failure.
 */
int removedir(const char *path) {
#ifdef _WIN32
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    char buf[MAX_PATH];
    strcat(strcpy(buf, path), "/*");

    hFind = FindFirstFile(buf, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return -1;
    }

    int r = 0;

    do {
        if (!strcmp(findFileData.cFileName, ".") || !strcmp(findFileData.cFileName, "..")) {
            continue;
        }

        snprintf(buf, sizeof(buf), "%s/%s", path, findFileData.cFileName);

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            r = removedir(buf);
        } else {
            if (!DeleteFile(buf)) {
                r = -1;
            }
        }

    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

    if (!r) {
        if (!RemoveDirectory(path)) {
            r = -1;
        }
    }
#else
    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;

    if (d) {
        struct dirent *p;

        r = 0;

        while (!r && (p = readdir(d))) {
            int r2 = -1;
            char *buf;
            size_t len;

            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) {
                continue;
            }

            len = path_len + strlen(p->d_name) + 2;
            buf = malloc(len);

            if (buf) {
                struct stat statbuf;

                snprintf(buf, len, "%s/%s", path, p->d_name);

                if (!stat(buf, &statbuf)) {
                    if (S_ISDIR(statbuf.st_mode)) {
                        r2 = removedir(buf);
                    } else {
                        r2 = unlink(buf);
                    }
                }

                free(buf);
            }

            r = r2;
        }

        closedir(d);
    }

    if (!r) {
        r = rmdir(path);
    }
#endif
    return r;
}

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
int copy_file(const char* source, const char* dest) {
    FILE* srcFile = fopen(source, "rb");
    if (srcFile == NULL) {
        return 0; // Failed to open the source file
    }

    char *d = strdup(dest);
    char *p = strrchr(d,'/');
    if ( p ) p[0] = '\0';
    mkpath( d, 0700 );
    free(d);

    FILE* destFile = fopen(dest, "wb");
    if (destFile == NULL) {
        fclose(srcFile);
        return 0; // Failed to open the destination file
    }

    char buffer[MAX_SIZE];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), srcFile)) > 0) {
        fwrite(buffer, 1, bytesRead, destFile);
    }

    fclose(srcFile);
    fclose(destFile);

    return 1; // File copy successful
}
