/**
 * @file main.c
 * @brief Main program for the No Man's Sky Mod Creator (nmsmc) project.
 *
 * This source file contains the main program for the No Man's Sky Mod Creator (nmsmc) project.
 * It handles command-line arguments, initializes necessary libraries, and orchestrates the mod creation process.
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
#include <signal.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>

//#include "definition.h"
#include "fs_utils.h"
#include "misc.h"
#include "definition.h"

char tmpdir_template[MAX_PATH];

char* MBINCompiler = NULL;
char* PSAR = NULL;
char* tmpdir = NULL;

OutputPakFileData* outputPakFileList = NULL;

void cleanup() {
    // Clean up resources and reset data structures related to file definitions
    definition_cleanup(outputPakFileList);

    // Free dynamically allocated memory
    free(MBINCompiler);
    free(PSAR);

    // Clean up libxml2 library resources
    xmlCleanupParser();

    // Remove the temporary directory
    if (tmpdir) removedir(tmpdir);
}

void sigintHandler(int signum) {
    printf("Abort request by user, exiting...\n");
    exit(signum);
}

void displayUsage() {
    fprintf(stderr, "Usage: nmsmc <definition_file>\n");
}

void displayHelp() {
    printf("No Man's Sky Mod Creator v1.0 - (c) 2023 Juan José Ponteprino (SplinterGU)\n\n");
    printf("Usage: nmsmc [OPTIONS] <definition_file>\n\n");
    printf("Examples:\n");
    printf("  nmsmc modification.def  - Create a mod using 'modification.def' as the definition file\n");
    printf("  nmsmc -V                - Display the version information\n\n");
    printf("Options:\n");
    printf("  -h, --help        Show this help message and exit\n");
    printf("  -V, --version     Show version information\n\n");
    printf("This software is provided under the terms of the MIT License.\n");
    printf("You may freely use, modify, and distribute this software, subject\n");
    printf("to the conditions and limitations of the MIT License.\n\n");
    printf("For more details, please see the LICENSE file included with this software.\n\n");
    printf("Report bugs to: splintergu@gmail.com\n");
    printf("Home page: <https://github.com/SplinterGU/nmsmc>\n");
}

int main(int argc, char* argv[]) {
    // Handle command line arguments
    if (argc < 2) {
        fprintf(stderr, "nmsmc: missing definition file\n");
        fprintf(stderr, "Try 'nmsmc --help' for more information.\n");
        return 1;
    } else if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        displayHelp();
        return 0;
    } else if (argc == 2 && (strcmp(argv[1], "-V") == 0 || strcmp(argv[1], "--version") == 0)) {
        printf("nmsmc (No Man's Sky Mod Creator) v1.0.0\n");
        printf("Copyright (c) 2023 Juan José Ponteprino\n");
        printf( "License MIT: MIT License <https://opensource.org/licenses/MIT>.\n" );
        printf( "This is open-source software: you are free to change and redistribute it.\n" );
        printf( "There is NO WARRANTY, to the extent permitted by law.\n\n" );
        printf( "Written by Juan José Ponteprino (SplinterGU)\n" );
        printf( "Report bugs/contact to: splintergu@gmail.com\n" );
        printf( "Home page: <https://github.com/SplinterGU/nmsmc>\n" );
        return 0;
    }

    // Initialize the libxml2 library
    xmlInitParser();

    // Register the cleanup function with atexit
    atexit(cleanup);

    // Register the signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, sigintHandler);
 
    char *t = (char *) tempdir();

#ifdef _WIN32
    path_to_unix(NULL, t);
#endif

    char tail = t[strlen(t)-1];

    sprintf(tmpdir_template,"%s%sNMSMC_XXXXXX", t, tail == '/' ? "" : "/");

    if (!(tmpdir = mkdtemp(tmpdir_template))) {
        fprintf(stderr, "Can't create a temporary directory\n");
        return 1;
    }

    // Set default values for MBINCompiler and PSAR
    MBINCompiler = strdup("MBINCompiler");
    PSAR = strdup("psar");

    const char* definitionFile = argv[argc - 1];

    // Process the definition file
    outputPakFileList = parse_definition(definitionFile, NULL);
    process_definitions(outputPakFileList);

    return 0;
}
