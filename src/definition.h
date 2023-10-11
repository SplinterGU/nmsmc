/**
 * @file definition.h
 * @brief Header file containing data structures and function declarations for handling mod definitions in the No Man's Sky Mod Creator (nmsmc) project.
 *
 * This header file defines various data structures used for storing mod definitions, including name-value pairs,
 * MBIN modifications, MBIN data, input PAK file data, and extra files.
 * It also provides function declarations for parsing mod definitions, processing them, and performing cleanup.
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

#ifndef __DEFINITION_H
#define __DEFINITION_H

// Structure to store name-value pairs
typedef struct NameValue {
    char* name;
    char* value;
    struct NameValue * next;
} NameValue;

// Structure to store MBIN modifications
typedef struct ModificationData {
    char* xpath;
    NameValue * values;
    NameValue * lastValues;
    struct ModificationData * next;
} ModificationData;

// Structure to store MBIN data
typedef struct MBINData {
    char* mbinFile;
    ModificationData * modifications;
    ModificationData * lastModifications;
    xmlDocPtr xmlData;
    struct MBINData * next;
} MBINData;

// Structure to store Input PAK file data
typedef struct InputPakFileData {
    char* inputPakFile;
    MBINData * mbinData;
    size_t mbinCount;
    MBINData * lastMbinData;
    struct InputPakFileData * next;
} InputPakFileData;

// Structure to store extra files
typedef struct ExtraFile {
    char* filename;
    struct ExtraFile * next;
} ExtraFile;

// Declaration of the structure for Output PAK file data
typedef struct OutputPakFileData {
    char* outputPakFile;
    InputPakFileData * inputPakFileList;
    InputPakFileData * lastInputPakFileList;
    size_t totalMbinCount;
    ExtraFile * extraFileList;
    size_t extraFileCount;
    struct OutputPakFileData * next;
} OutputPakFileData;

// Function declarations
OutputPakFileData* parse_definition(const char* filename, OutputPakFileData* ouputPakFileDataList);
int process_definitions(OutputPakFileData * outputPakFileList);
void definition_cleanup(OutputPakFileData* outputPakFileList);

#endif /* __DEFINITION_H */
