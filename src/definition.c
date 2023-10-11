/**
 * @file definition.c
 * @brief Implementation of functions for parsing, processing, and cleaning up mod definitions in the No Man's Sky Mod Creator (nmsmc) project.
 *
 * This source file provides the implementation of functions for parsing mod definitions, processing them to handle MBIN data,
 * input PAK file data, and extra files, and performing necessary cleanup operations.
 * It includes functions to parse definition files, process mod definitions, and release allocated memory.
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

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

#include "common.h"
#include "fs_utils.h"
#include "misc.h"
#include "definition.h"

#ifndef _WIN32
#include "spawn.h"
#endif

xmlDocPtr doc = NULL;

char xpath[32768] = "";

static ModificationData* currentModification = NULL;
static MBINData *currentMbinData = NULL;
static InputPakFileData* currentInputPakFileList = NULL;
static OutputPakFileData* currentOutputPakFile = NULL;
static OutputPakFileData * lastOutputPakFile = NULL;

/**
 * Cleans up memory associated with OutputPakFileData and related data structures.
 *
 * @param outputPakFileList - The list of OutputPakFileData to be cleaned up.
 *
 * This function deallocates memory associated with OutputPakFileData and its related data structures,
 * including InputPakFileData, MBINData, ModificationData, and NameValue structures.
 */
void definition_cleanup(OutputPakFileData* outputPakFileList) {
    void *ptr;
    OutputPakFileData * outputPakFile = outputPakFileList;
    while( outputPakFile ) {
        free(outputPakFile->outputPakFile);
        InputPakFileData * inputPakFile = outputPakFile->inputPakFileList;
        while( inputPakFile ) {
            MBINData * mbinData = inputPakFile->mbinData;
            while( mbinData ) {
                ModificationData * modification = mbinData->modifications;
                while( modification ) {
                    NameValue * namevalue = modification->values;
                    while( namevalue ) {
                        free(namevalue->name);
                        free(namevalue->value);
                        ptr = namevalue->next;
                        free(namevalue);
                        namevalue = ptr;
                    }
                    ptr = modification->next;
                    free(modification);
                    modification = ptr;
                }
                ptr = mbinData->next;
                free(mbinData);
                mbinData = ptr;
            }
            ptr = inputPakFile->next;
            free(inputPakFile->inputPakFile);
            inputPakFile = ptr;
        }
        if ( outputPakFile->extraFileList ) {
            ExtraFile * extraFile = outputPakFile->extraFileList;
            while( extraFile ) {
                free(extraFile->filename);
                ptr = extraFile->next;
                free(extraFile);
                extraFile = ptr;
            }
        }
        ptr = outputPakFile->next;
        free(outputPakFile);
        outputPakFile = ptr;
    }
}

/**
 * Creates a new OutputPakFileData structure with the given file name.
 *
 * @param file - The name of the output pak file.
 * @return A pointer to the newly created OutputPakFileData structure.
 *
 * This function allocates memory for a new OutputPakFileData structure and initializes its values.
 */
static OutputPakFileData* createOutputPakFileData(const char* file) {
    OutputPakFileData* data = (OutputPakFileData*)malloc(sizeof(OutputPakFileData));
    if (!data) return NULL;

    // Initialize the values of OutputPakFileData
    data->outputPakFile = file ? strdup(file) : NULL;
    data->inputPakFileList = NULL;
    data->lastInputPakFileList = NULL;
    data->totalMbinCount = 0;
    data->extraFileList = NULL;
    data->extraFileCount = 0;
    data->next = NULL;

    return data;
}

/**
 * Searches for an InputPakFileData structure with the specified file name.
 *
 * @param inputPakFile - The name of the pak file to search for.
 * @return A pointer to the found InputPakFileData structure, or NULL if not found.
 *
 * This function searches for an InputPakFileData structure within the currentOutputPakFile
 * based on the provided file name.
 */
InputPakFileData * search_input_pak(const char *inputPakFile) {
    InputPakFileData * inputPak = currentOutputPakFile->inputPakFileList;
    while( inputPak ) {
        if (!strcmp(inputPakFile, inputPak->inputPakFile)) {
            return inputPak;
        }
        inputPak = inputPak->next;
    }
    return NULL;
}

/**
 * Searches for an MBINData structure with the specified MBIN file name.
 *
 * @param mbinFile - The name of the MBIN file to search for.
 * @return A pointer to the found MBINData structure, or NULL if not found.
 *
 * This function searches for an MBINData structure within the currentInputPakFileList
 * based on the provided MBIN file name.
 */
MBINData * search_mbin(const char *mbinFile) {
    MBINData * mbinData = currentInputPakFileList->mbinData;
    while( mbinData ) {
        if (!strcmp(mbinFile, mbinData->mbinFile)) {
            return mbinData;
        }
        mbinData = mbinData->next;
    }
    return NULL;
}

/**
 * Checks if the current line starts with a specified token.
 *
 * @param x - The token to check for at the beginning of the line.
 * @return 1 if the line starts with the token, 0 otherwise.
 *
 * This macro is used to check if the current line starts with a specified token.
 */
#define CHECK_TOKEN(x)  (!strncmp(line, x, sizeof(x) - 1))

/**
 * Function to parse a text file and populate InputPakFileData.
 *
 * @param filename - The name of the input text file to parse.
 * @param ouputPakFileDataList - The list of OutputPakFileData to which data will be added.
 * @return A pointer to the updated OutputPakFileData list.
 *
 * This function reads and processes a text file line by line. It parses commands and data from the file,
 * including commands like "!include," "!outputPakFile," "!addFile," "!inputPakFile," "!mbinFile," "cd," and
 * assignments of the form "name=value" within a "cd" block. It populates the relevant data structures with the
 * parsed information.
 */
OutputPakFileData* parse_definition(const char* filename, OutputPakFileData* ouputPakFileDataList) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open the file [%s]\n", filename);
        return NULL;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Remove comments from the line
        char* comment = strchr(line, '#');
        if (comment)
            *comment = '\0';

        // Remove leading and trailing whitespace from the line
        trim(line);
        if (!*line)
            continue;

        // Search for the "!include" token
        if (CHECK_TOKEN("!include")) {
            char* token = strtok(line, " \t\r\n");
            token = strtok(NULL, "\r\n");  // Get the value after the token
            trim(token);
            if (token) {
                // Call parse_definition recursively and add the data to the current list
                ouputPakFileDataList = parse_definition(token, ouputPakFileDataList);
            }
        }
        // Process the "!outputPakFile" token
        else if (CHECK_TOKEN("!outputPakFile")) {
            char* token = strtok(line, " \t\r\n");
            token = strtok(NULL, "\r\n");  // Get the value after the token
            trim(token);
            if (token) {
                // Create a new OutputPakFileData structure and associate it with the current inputPakFileList
                currentOutputPakFile = createOutputPakFileData(token);
                if (!currentOutputPakFile ) {
                    fprintf(stderr, "Error: Memory allocation for OutputPakFileData failed\n");
                    return NULL;
                }
                if (!ouputPakFileDataList)
                    ouputPakFileDataList = currentOutputPakFile;
                else
                    lastOutputPakFile->next = currentOutputPakFile;
                lastOutputPakFile = currentOutputPakFile;

                currentModification = NULL;
                currentMbinData = NULL;
                currentInputPakFileList = NULL;
            }
        }
        // Search for the "!addFile" token
        else if (CHECK_TOKEN("!addFile")) {
            if (!currentOutputPakFile) {
                fprintf(stderr, "Error: Expected !outputPakFile, but got !addFile.\n");
                return NULL;
            }
            char* token = strtok(line, " \t\r\n");
            token = strtok(NULL, "\r\n");  // Get the value after the token
            trim(token);
            if (token) {
                // Create a new InputPakFileData element and add it to the list
                ExtraFile * e = (ExtraFile*)malloc(sizeof(ExtraFile));
                if (!e) {
                    fprintf(stderr, "Error: Memory allocation for !addFile failed\n");
                    return NULL;
                }

                e->filename = strdup(token);
                e->next = currentOutputPakFile->extraFileList;
                currentOutputPakFile->extraFileList = e;

                currentOutputPakFile->extraFileCount++;
            }
        }
        // Search for the "!inputPakFile" token
        else if (CHECK_TOKEN("!inputPakFile")) {
            if (!currentOutputPakFile) {
                fprintf(stderr, "Error: Expected !outputPakFile, but got !inputPakFile.\n");
                return NULL;
            }
            char* token = strtok(line, " \t\r\n");
            token = strtok(NULL, "\r\n");  // Get the value after the token
            trim(token);
            if (token) {
                if (!(currentInputPakFileList = search_input_pak(token))) {
                    // Create a new InputPakFileData element and add it to the list
                    currentInputPakFileList = (InputPakFileData*)malloc(sizeof(InputPakFileData));
                    if (!currentInputPakFileList) {
                        fprintf(stderr, "Error: Memory allocation for InputPakFileData failed\n");
                        return NULL;
                    }

                    // Initialize the values of InputPakFileData
                    currentInputPakFileList->inputPakFile = strdup(token);
                    currentInputPakFileList->mbinData = NULL;
                    currentInputPakFileList->mbinCount = 0;
                    currentInputPakFileList->lastMbinData = NULL;
                    currentInputPakFileList->next = NULL;

                    if (!currentOutputPakFile->inputPakFileList)
                        currentOutputPakFile->inputPakFileList = currentInputPakFileList;
                    else
                        currentOutputPakFile->lastInputPakFileList->next = currentInputPakFileList;
                    currentOutputPakFile->lastInputPakFileList = currentInputPakFileList;
                }
                currentModification = NULL;
                currentMbinData = NULL;
            }
        }
        // Search for the "!mbinFile" token
        else if (CHECK_TOKEN("!mbinFile")) {
            if (!currentInputPakFileList) {
                fprintf(stderr, "Error: Expected !inputPakFile, but got !mbinFile.\n");
                return NULL;
            }
            char* token = strtok(line, " \t\r\n");
            token = strtok(NULL, "\r\n");  // Get the value after the token
            trim(token);
            if (token) {
                if ((currentMbinData = search_mbin(token))) {
                    currentModification = currentMbinData->lastModifications;
                } else {
                    // Add the ModificationData element to currentInputPakFileList
                    currentMbinData = (MBINData*)malloc(sizeof(MBINData));
                    if (!currentMbinData) {
                        fprintf(stderr, "Error: Memory allocation for mbinData failed\n");
                        fclose(file);
                        return NULL;
                    }

                    currentMbinData->mbinFile = strdup(token);
                    currentMbinData->modifications = NULL;
                    currentMbinData->lastModifications = NULL;
                    currentMbinData->xmlData = NULL;
                    currentMbinData->next = NULL;

                    if (!currentInputPakFileList->mbinData)
                        currentInputPakFileList->mbinData = currentMbinData;
                    else
                        currentInputPakFileList->lastMbinData->next = currentMbinData;
                    currentInputPakFileList->lastMbinData = currentMbinData;
                    currentInputPakFileList->mbinCount++;
                    currentOutputPakFile->totalMbinCount++;
                    currentModification = NULL;
                }
            }
        }
        // Search for the "cd" token and add it to currentModification
        else if (CHECK_TOKEN("cd")) {
            if (!currentMbinData) {
                fprintf(stderr, "Error: Expected !mbinFile, but got \"cd\".\n");
                return NULL;
            }
            char* token = strtok(line, " \t\r\n");
            token = strtok(NULL, "\r\n");  // Get the value after the token
            trim(token);
            if (token) {
                // Create a new ModificationData element
                currentModification = (ModificationData*)malloc(sizeof(ModificationData));
                if (!currentModification) {
                    fprintf(stderr, "Error: Memory allocation for ModificationData failed\n");
                    return NULL;
                }

                // Initialize the values of ModificationData
                currentModification->xpath = strdup(token);
                currentModification->values = NULL;
                currentModification->next = NULL;

                if (!currentMbinData->modifications)
                    currentMbinData->modifications = currentModification;
                else
                    currentMbinData->lastModifications->next = currentModification;
                currentMbinData->lastModifications = currentModification;
            }
        }
        // Process lines with "=" and split them into "name=value"
        else {
            if (!currentModification) {
                fprintf(stderr, "Error: Expected \"cd\", but found an assignment expression.\n");
                return NULL;
            }

            // Process lines defining items
            char* name = line;
            char* value = strchr(line, '=');
            if (value) {
                *value = '\0';
                value++;
                trim(value);
                if (!*value) value = NULL;
            }
            trim(name);
            if (!*name) name = NULL;

            if (name || value) {
                // Add the "name=value" pair to currentModification
                NameValue * nv = (NameValue*)malloc(sizeof(NameValue));
                if (!nv) {
                    fprintf(stderr, "Error: Memory allocation for NameValue failed\n");
                    return NULL;
                }

                nv->name = name ? strdup(name) : NULL;
                nv->value = value ? strdup(value) : NULL;
                nv->next = NULL;

                if (!currentModification->values)
                    currentModification->values = nv;
                else
                    currentModification->lastValues->next = nv;
                currentModification->lastValues = nv;
            }
        }
    }

    fclose(file);
    return ouputPakFileDataList;
}

/**
 * Get the list of MBIN file names from the InputPakFileData.
 *
 * This function retrieves the names of MBIN files from the provided InputPakFileData
 * and appends them to an existing list of strings. It also updates the count of
 * items in the list. Memory reallocation is handled internally.
 *
 * @param list      The current list of MBIN file names (can be NULL for the initial call).
 * @param count     A pointer to the count of items in the list.
 * @param data      The InputPakFileData containing MBIN file information.
 *
 * @return          A pointer to the updated list of MBIN file names. NULL on error.
 */
char **get_mbin_list(char **list, size_t *count, InputPakFileData *data) {

    if ( !data->mbinCount ) return list;

    // Resize the list to accommodate more entries
    char ** l = realloc(list, sizeof(char *) * ( data->mbinCount + *count + 1 ));
    if (!l) {
        if (list) free(*list);
        return NULL;
    }
    list = l;

    MBINData * mbinData = data->mbinData;
    while( mbinData ) {
        // Add the MBIN file name to the list        
        list[(*count)++] = mbinData->mbinFile;
        mbinData = mbinData->next;
    }
    list[(*count)] = NULL;

    return list;
}

/**
 * Get the complete list of MBIN file names from OutputPakFileData.
 *
 * This function retrieves the names of MBIN files from the provided OutputPakFileData
 * along with its associated InputPakFileData and appends them to an existing list of strings.
 * It also updates the count of items in the list. Memory reallocation is handled internally.
 *
 * @param list      The current list of MBIN file names (can be NULL for the initial call).
 * @param count     A pointer to the count of items in the list.
 * @param data      The OutputPakFileData containing InputPakFileData and MBIN file information.
 * @param exml      A flag indicating whether to convert MBIN file extensions to EXML.
 *
 * @return          A pointer to the updated list of MBIN file names. NULL on error.
 */
char **get_complete_mbin_list(char **list, size_t *count,OutputPakFileData *data, int exml) {
    if ( !data->totalMbinCount ) return list;

    // Resize the list to accommodate more entries
    char ** l = realloc(list, sizeof(char *) * ( data->totalMbinCount + *count + 1 ));
    if (!l) {
        if (list) free(*list);
        return NULL;
    }
    list = l;

    InputPakFileData *i = data->inputPakFileList;
    while( i ) {
        MBINData * mbinData = i->mbinData;
        while( mbinData ) {
            char *l;
            if ( exml ) {
                // Convert MBIN file extension to EXML
                l = strdup(mbinData->mbinFile);
                char* e = strstr(l, ".MBIN");
                while (e) {
                    memcpy(e, ".EXML", 5);
                    e = strstr(e + 5, ".MBIN");
                }
            } else {
                l = mbinData->mbinFile;
            }

            // Add the modified MBIN file name to the list
            list[(*count)++] = l;
            mbinData = mbinData->next;
        }
        i = i->next;
    }

    list[(*count)] = NULL;

    return list;
}

/**
 * Extract MBIN files from a PAK archive.
 *
 * @param destdir - The destination directory to extract the MBIN files to.
 * @param data - The InputPakFileData containing MBIN data.
 * @return 0 if extraction is successful, 1 otherwise.
 *
 * This function extracts MBIN files from a PAK archive using PSAR utility.
 * It also compiles the extracted MBIN files to XML using MBINCompiler.
 * Finally, it stores the XML data in the MBINData structures.
 */
int get_input_files(const char *destdir, InputPakFileData *data) {
    printf("open %s\n", data->inputPakFile);

    char ** argv = malloc( 6 * sizeof( char * ) );
    size_t argc = 0;

    argv[argc++] = PSAR;
    argv[argc++] = "-yxf";
    argv[argc++] = data->inputPakFile;
    argv[argc++] = "-t";
    argv[argc++] = (char *) destdir;
    argv[argc] = NULL;

    argv = get_mbin_list(argv, &argc, data);

    DISABLE_CONSOLE

    int result = spawnvp(P_WAIT, PSAR, argv);

    ENABLE_CONSOLE

    free(argv);
    if (result) {
        fprintf(stderr, "Error extracting MBINs from file: %s\n", data->inputPakFile);
        return 1;
    }

    argv = malloc( 5 * sizeof( char * ) );
    argc = 0;
    argv[argc++] = MBINCompiler;
    argv[argc++] = "-y";
    argv[argc++] = "-q";
    argv[argc++] = "--no-version";
    argv[argc] = NULL;

    argv = get_mbin_list(argv, &argc, data);

    // Build the MBINCompiler command to compile the MBIN file to XML

    char *current_dir = get_current_dir();
    chdir(destdir);

    DISABLE_CONSOLE

    result = spawnvp(P_WAIT, MBINCompiler, argv);

    ENABLE_CONSOLE

    free(argv);

    if (result) {
        chdir(current_dir);
        free(current_dir);

        fprintf(stderr, "Error converting MBINs from EXML: %s\n", data->inputPakFile);
        return 1;
    }

    // Temporary variables to store file names
    char filename[MAX_PATH];

    MBINData * mbinData = data->mbinData;
    while( mbinData ) {
        // Compile the MBIN file to XML
        strcpy(filename, mbinData->mbinFile);
        char* e = strstr(filename, ".MBIN");
        if (e) strcpy(e, ".EXML");
        mbinData->xmlData = xmlReadFile(filename, NULL, 0);
        mbinData = mbinData->next;
    }

    chdir(current_dir);
    free(current_dir);

    return 0; // Success in extracting MBIN files
}

/**
 * Add files to a PAK archive.
 *
 * @param sourcedir - The source directory containing files to add to the PAK archive.
 * @param pakData - The OutputPakFileData containing the PAK archive information.
 * @return 0 if adding files is successful, 1 otherwise.
 *
 * This function adds files to a PAK archive using PSAR utility.
 * It also compiles XML files to MBIN using MBINCompiler.
 * Additionally, it handles adding extra files to the PAK archive.
 */
int save_pak(const char* sourcedir, OutputPakFileData * pakData) {
    if ( pakData->totalMbinCount ) {
        char ** argv = malloc( 4 * sizeof( char * ) );
        size_t argc = 0;

        argv[argc++] = MBINCompiler;
        argv[argc++] = "-y";
        argv[argc++] = "-q";
        argv[argc] = NULL;
        size_t argc_mbins = argc;

        argv = get_complete_mbin_list(argv, &argc, pakData, 1);

        char *current_dir = get_current_dir();
        chdir(sourcedir);

        freopen(DEVNULL, "w", stdout);
        freopen(DEVNULL, "w", stderr);

        int result = spawnvp(P_WAIT, MBINCompiler, argv);

        freopen(DEVCONSOLE, "w", stdout);
        freopen(DEVCONSOLE, "w", stderr);

        char ** p = &argv[argc_mbins];
        while (*p) {
            free(*p);
            p++;
        }
        free(argv);

        chdir(current_dir);
        free(current_dir);

        if (result) {
            fprintf(stderr, "Error compiling XML files to MBIN\n");
            return 1;
        }
    }

    char ** argv = malloc( 6 * sizeof( char * ) );
    size_t argc = 0;

    argv[argc++] = PSAR;
    argv[argc++] = "-yrczf";
    argv[argc++] = pakData->outputPakFile;
    argv[argc++] = "-s";
    argv[argc++] = (char *) sourcedir;
    argv[argc] = NULL;

    argv = get_complete_mbin_list(argv, &argc, pakData, 0);

    if ( pakData->extraFileCount ) {
        argv = realloc( argv, sizeof(char *) * ( argc + pakData->extraFileCount + 1 ) );

        char tmpFilePath[MAX_PATH];
        ExtraFile * extraFile = pakData->extraFileList;
        while( extraFile ) {
            printf("add %s\n", extraFile->filename);
            snprintf(tmpFilePath, sizeof(tmpFilePath), "%s/%s", sourcedir, extraFile->filename);
            if (!copy_file(extraFile->filename, tmpFilePath)) return 0;

            argv[argc++] = extraFile->filename;
            extraFile = extraFile->next;
        }
        argv[argc] = NULL;
    }

    printf("save %s\n\n", pakData->outputPakFile);

    DISABLE_CONSOLE

    int result = spawnvp(P_WAIT, PSAR, argv);

    ENABLE_CONSOLE

    // Execute PSAR to compress the files
    free(argv);

    if (result) {
        fprintf(stderr, "Error creating PAK archive: %s\n", pakData->outputPakFile);
        return 1;
    }

    return 0; // Success in creating the PAK archive
}

/**
 * Set the XPath context for XML operations.
 *
 * @param in - The input string defining the XPath context.
 *
 * This function sets the XPath context for XML operations. It takes an input string
 * that defines the XPath context and constructs the appropriate XPath expression
 * based on the input.
 */
void set_xpath(char* in) {
    if (!in) {
        xpath[0] = '\0';
        return;
    }

    char* newxpath;
    if (in[0] == '/') {
        strcpy(xpath, "/Data");
        if (in[1])
            strcat(xpath, "/");
    } else {
        if (xpath[0])
            strcat(xpath, "/");
    }

    newxpath = &xpath[strlen(xpath)];

    char* token = strtok(in, "/");
    while (token) {
        char* p;
        if (token[0] == '*') {
            strcat(newxpath, "*");
        } else if (strcmp(token, "..") == 0) {
            strcat(newxpath, "..");
        } else if ((p = strstr(token, "="))) {
            char d = '\0';
            if (p > token && p[-1] == '[') {
                d = '[';
                p[-1] = '\0';
            }
            p[0] = '\0';
            p++;
            strcat(newxpath, "Property[");
            if (token[0] && token[0] != '*') {
                strcat(newxpath, "@name='");
                strcat(newxpath, token);
                strcat(newxpath, "' and ");
            }
            char* p1 = NULL;
            if (d == '[') {
                p1 = strchr(p, ']');
                if (!p1) {
                    printf("error, missing ]\n");
                    exit(EXIT_FAILURE);
                }
                p1[0] = '\0';
            }
            strcat(newxpath, "@value='");
            strcat(newxpath, p);
            strcat(newxpath, "']");
            if (p1 && p1[1] != '\0') {
                printf("error, extra data after ]\n");
                exit(EXIT_FAILURE);
            }
        } else {
            strcat(newxpath, "Property[@name='");
            strcat(newxpath, token);
            strcat(newxpath, "']");
        }

        token = strtok(NULL, "/");
        if (token)
            strcat(newxpath, "/");
    }
}

/**
 * Set the value of an item in the XML document using XPath.
 *
 * @param name - The name of the item.
 * @param value - The value to set for the item.
 *
 * This function sets the value of an item in the XML document using XPath. It takes
 * the name and value of the item and updates the XML document accordingly. If the
 * item does not exist, it creates a new node and sets its value.
 */
void set_item(char* name, char* value) {
    if (!name && !value)
        return;

    // Initialize XPath context
    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    if (context == NULL) {
        fprintf(stderr, "Error creating XPath context.\n");
        xmlFreeDoc(doc);
        return;
    }

    // Evaluate the XPath expression
    xmlXPathObjectPtr result = xmlXPathEvalExpression(BAD_CAST xpath, context);
    if (result != NULL) {
        // The XPath exists in the XML document or has been created
        // If it doesn't exist, create a new node and set its value
        if (result->nodesetval) {
            char* search_attr = NULL;
            char* search_value = NULL;

            if (name) {
                search_attr = "name";
                search_value = name;
            } else {
                search_attr = "value";
                search_value = value;
            }

            for (int i = 0; i < result->nodesetval->nodeNr; i++) {
                xmlNodePtr node = result->nodesetval->nodeTab[i];
                xmlNodePtr child = node->children;
                int found = 0;
                while (child) {
                    // Search for the "value" attribute
                    xmlChar* attr = xmlGetProp(child, BAD_CAST search_attr);
                    if (attr) {
                        if (!xmlStrcmp(attr, BAD_CAST search_value)) {
                            xmlAttrPtr attrValue = xmlHasProp(child, BAD_CAST "value");
                            if (attrValue) {
                                // Update the content of the "value" attribute
                                xmlNodeSetContent(attrValue->children, BAD_CAST value);
                            } else {
                                // The "value" attribute does not exist, create a new attribute and set its value
                                xmlNewProp(child, BAD_CAST "value", BAD_CAST value);
                            }
                            xmlFree(attr);
                            found = 1;
                            break;
                        }
                        xmlFree(attr);
                    }
                    child = child->next;
                }
                if (!found) {
                    // Create a new node at the specified XPath location
                    xmlNodePtr newNode = xmlNewNode(NULL, BAD_CAST "Property");
                    if (name)
                        xmlNewProp(newNode, BAD_CAST "name", BAD_CAST name);
                    if (value)
                        xmlNewProp(newNode, BAD_CAST "value", BAD_CAST value);
                    xmlAddChild(node, newNode);
                }
            }
        }
    } else {
        printf("XPath not found: [%s]\n", xpath);
    }

    xmlXPathFreeObject(result);
    xmlXPathFreeContext(context);
}


/**
 * Process definitions and modify XML files within PAK archives.
 *
 * @param outputPakFileList - The list of OutputPakFileData structures to process.
 * @return 0 on success, 1 on failure.
 *
 * This function processes definitions and modifies XML files within PAK archives. It iterates
 * through the provided list of OutputPakFileData structures, extracts MBIN files, applies
 * modifications specified in the XML data, and saves the modified files back to the archive.
 */
int process_definitions(OutputPakFileData * outputPakFileList) {
    // Iterate through the list of OutputPakFileData structures
    OutputPakFileData * outputPakFile = outputPakFileList;
    while( outputPakFile ) {
        // Iterate through the input PAK files within each OutputPakFileData
        InputPakFileData * inputPakFile = outputPakFile->inputPakFileList;
        while( inputPakFile ) {
            // Extract MBIN files from the input PAK file
            if ( get_input_files(tmpdir, inputPakFile) ) return 1;
            
            // Iterate through the MBIN files
            MBINData * mbinData = inputPakFile->mbinData;
            while( mbinData ) {
                // Iterate through modifications for each MBIN file
                ModificationData * modification = mbinData->modifications;
                doc = mbinData->xmlData;
                printf("process %s\n", mbinData->mbinFile);
                while( modification ) {
                    // Set the XPath context for modification
                    set_xpath(modification->xpath);
                    
                    // Iterate through name-value pairs for the modification
                    NameValue * namevalue = modification->values;
                    while( namevalue ) {
                        // Set the value of an item in the XML document using XPath
                        set_item(namevalue->name, namevalue->value);
                        namevalue = namevalue->next;
                    }
                    modification = modification->next;
                }
                // Save the modified XML file
                char filename[MAX_PATH];
                sprintf(filename, "%s/%s", tmpdir, mbinData->mbinFile);
                char *e = strstr(filename, ".MBIN");
                if ( e ) strcpy( e, ".EXML");
                xmlSaveFormatFile(filename, mbinData->xmlData, 0);
                mbinData = mbinData->next;
            }
            inputPakFile = inputPakFile->next;
        }

        // Save the modified PAK archive
        if ( save_pak(tmpdir, outputPakFile)) return 1;

        outputPakFile = outputPakFile->next;
    }
    return 0; // Success
}
