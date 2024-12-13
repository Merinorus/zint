/* dxfilmedge.c - Handles DX Film Edge symbology */
/*
    libzint - the open source barcode library
    Copyright (C) 2024-2025 Antoine Merino <antoine.merino.dev@gmail.com>

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Neither the name of the project nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
 */
/* SPDX-License-Identifier: BSD-3-Clause */

/* DX Film Edge Barcode is used on 35mm and APS films:
 * https://en.wikipedia.org/wiki/DX_encoding
 *
 * A little information about decoding this symbology can be found at
 * https://www.merinorus.com/blog/identifying-manufacturer-35-mm-films/
 *
 * Partial specification and history can be found on this Kodak patent:
 * https://patents.google.com/patent/US4965628A/en
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "common.h"

#define MAX_DX_INFO_LENGTH 7 // Max length of the DX info part. Include the \0. Eg: "018500\0", "150-10\0"
#define MAX_FRAME_INFO_LENGTH 4 // Max length of the frame info part. Eg: "00A\0", "23A\0"
#define DEBUG_STR_LEN 20

/* Utility to convert integer to binary string with fixed width */
void int_to_binary(int value, int width, char *output) {
    int i = 0;
    for (i = 0; i < width; i++) {
        output[width - 1 - i] = (value & (1 << i)) ? '1' : '0';
    }
    output[width] = '\0';
}

// Function to convert a string to uppercase
void str_to_uppercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}


// /* Warning and error conditions (API return values) */
// #define ZINT_WARN_HRT_TRUNCATED     1   /* Human Readable Text was truncated (max 199 bytes) */
// #define ZINT_WARN_INVALID_OPTION    2   /* Invalid option given but overridden by Zint */
// #define ZINT_WARN_USES_ECI          3   /* Automatic ECI inserted by Zint */
// #define ZINT_WARN_NONCOMPLIANT      4   /* Symbol created not compliant with standards */
// #define ZINT_ERROR                  5   /* Warn/error marker, not returned */

// #define ZINT_ERROR_INVALID_CHECK    7   /* Input check digit incorrect */
// #define ZINT_ERROR_INVALID_OPTION   8   /* Incorrect option given */
// #define ZINT_ERROR_ENCODING_PROBLEM 9   /* Internal error (should not happen) */
// #define ZINT_ERROR_FILE_ACCESS      10  /* Error opening output file */
// #define ZINT_ERROR_MEMORY           11  /* Memory allocation (malloc) failure */
// #define ZINT_ERROR_FILE_WRITE       12  /* Error writing to output file */
// #define ZINT_ERROR_USES_ECI         13  /* Error counterpart of warning if WARN_FAIL_ALL set (see below) */
// #define ZINT_ERROR_NONCOMPLIANT     14  /* Error counterpart of warning if WARN_FAIL_ALL set */
// #define ZINT_ERROR_HRT_TRUNCATED    15  /* Error counterpart of warning if WARN_FAIL_ALL set */






// #define ZINT_ERROR_TOO_LONG         5   /* Input data wrong length */
// #define ZINT_ERROR_INVALID_DATA     6   /* Input data incorrect */



// /* Set `symbol->errtxt` to "err_id: msg" with restricted subset of `printf()` formatting, returning `error_number`.
//    If `err_id` is -1, the "err_id: " prefix is omitted. Only the following specifiers are supported: "c", "d", "f",
//    "g" and "s", with no modifiers apart from "<n>$" numbering for l10n ("<n>" 1-9), in which case all specifiers must
//    be numbered, "%s" with length precisions: "%.*s", "%<n+1>$.*<n>$s", "%.<p>s" and "%<n>$.<p>s", and "%d" with
//    zero-padded minimum field lengths: "%0<m>d" or %<n>$0<m>d" ("<m>" 1-99) */
// INTERNAL int errtxtf(const int error_number, struct zint_symbol *symbol, const int err_id, const char *fmt, ...) {
//     const int max_len = (int) sizeof(symbol->errtxt) - 1;
//     int p = errtxt_id_str(symbol->errtxt, err_id);
//     const char *f;
//     int i;
//     int arg_cnt = 0;
//     int have_num_arg = 0, have_unnum_arg = 0;
//     va_list ap;
//     int idxs[9] = {0}; /* Argument order */
//     char specs[9] = {0}; /* Format specifiers */
//     const char *ss[9] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }; /* "%s" */
//     int slens[9] = {0}; /* "%s" length precisions */
//     int have_slens[9] = {0}; /* Bools for if "%s" has length precision */
//     char dpads[9][3] = {{0}}; /* 2-digit minimum field length */
//     char dfgs[9][100] = {{0}}; /* "%d", "%f" and "%g", allowing for padding up to 99 */
//     int cs[9] = {0}; /* "%c" */


    // if (length > 6) {
    //     return errtxtf(ZINT_ERROR_TOO_LONG, symbol, 484, "Input length %d too long (maximum 6)", length);
    // }
    // if ((i = not_sane(NEON_F, source, length))) {
    //     return errtxtf(ZINT_ERROR_INVALID_DATA, symbol, 485,
    //                     "Invalid character at position %d in input (digits only)", i);
    // }








// Allowed formats: DX Codes parts 1 and 2 XXX-YY, 4-digit DX Extract or 6-digit full DX code.

// /* Main parsing function */
int parse_dx_code(struct zint_symbol *symbol, const unsigned char *source, char *binary_output, int *output_length, bool *has_frame_info) {

    int i;
    int parity_bit = 0;
    int dx_extract = -1;
    int dx_code_1 = -1, dx_code_2 = -1, frame_number = -1;
    char binary_dx_code_1[8], binary_dx_code_2[5], binary_frame_number[7];
    char half_frame_flag = '\0', special_frame_number='\0';
    *has_frame_info = false; // Default to no frame info

    // TODO: check char array size
    unsigned char dx_info[MAX_DX_INFO_LENGTH] = "\0";
    unsigned char frame_info[MAX_FRAME_INFO_LENGTH] = "\0";
    unsigned char *detected_char = strchr(source, ' ');
    // Forbid spaces in the first part, because it could lead to mistakenly wrong barcodes
    if (detected_char){
        return errtxtf(ZINT_ERROR_INVALID_DATA, symbol, 1011, "Forbidden space at position %d (input: \"%s\").", detected_char - source + 1, source);
    }
    // Detect if there is the '/' separator, which indicates the frame number is present.
    detected_char = strchr(source, '/');
    if (detected_char){
        // Extract the part before "/" into dx_info    
        size_t dx_length = detected_char - source;
        if (dx_length >= MAX_DX_INFO_LENGTH){
            return errtxt(ZINT_ERROR_TOO_LONG, symbol, 1001, "DX part is too long.");
        }
        strncpy(dx_info, source, dx_length);
        dx_info[dx_length] = '\0';
        // Extract the part after "/" into frame_info
        const unsigned char *frame_start = detected_char + 1;
        if (strlen(frame_start) >= MAX_FRAME_INFO_LENGTH) {
            return errtxt(ZINT_ERROR_TOO_LONG, symbol, 1002, "Frame number part is too long.");
        }
        strcpy(frame_info, frame_start);
        *has_frame_info = true;
    }
    else{
        // No "/" found, store the entire input in dx_info
        if (strlen(source) >= MAX_DX_INFO_LENGTH) {
            return errtxt(ZINT_ERROR_TOO_LONG, symbol, 1003, "DX part is too long.");
        }
        strcpy(dx_info, source);
    }
    if (ZINT_DEBUG_PRINT) printf("\nDX info part: \"%s\", Frame info part: \"%s\"\n", dx_info, frame_info);
    // Parse the dx_info
    if (strchr(dx_info, '-')){
        // DX code parts 1 and 2 are given directly, separated by a '-'
        if (ZINT_DEBUG_PRINT) printf("DX code 1 and 2 are separated by a dash \'-\'\n");
        if (sscanf(dx_info, "%d-%d", &dx_code_1, &dx_code_2) < 2){
            return errtxt(ZINT_ERROR_INVALID_DATA, symbol, 1004, "Wrong format for DX parts 1 and 2. Expected format: XXX-XX (digits).");
        }
        if (dx_code_1 <= 0 || dx_code_1 > 127){
            return errtxt(ZINT_ERROR_INVALID_DATA, symbol, 1007, "DX part 1 must be between 1 and 127.");
        }
        if (dx_code_2 < 0 || dx_code_2 > 15){
            return errtxt(ZINT_ERROR_INVALID_DATA, symbol, 1008, "DX part 2 must be between 0 and 15.");
        }
    }
    else{
        // DX format is either 4 digits (DX Extract) or 6 digits (DX Full)
        if (ZINT_DEBUG_PRINT) printf("No \'-\' separator, computing from DX Extract (4 digits) or DX Full (6 digits)\n");
        if (strlen(dx_info) > 6){
            return errtxt(ZINT_ERROR_TOO_LONG, symbol, 1005, "DX number is too long. Expected: 4 digits (DX extract) or 6 digits (DX full).");
        }
        else if (strlen(dx_info) == 5){
            return errtxt(ZINT_ERROR_INVALID_DATA, symbol, 1006, "DX number is incorrect. Expected: 4 digits (DX extract) or 6 digits (DX full).");
        }
        if (strlen(dx_info) == 6){
            if (ZINT_DEBUG_PRINT) printf("DX full format detected: %s. Removing the first and the last characters.\n", dx_info);
            // Convert DX Full to DX Extract (remove first and last character)
            for (i=0; i <= 3; ++i){
                dx_info[i] = dx_info[i+1];
            }
            dx_info[4] = '\0';
        }
        // Compute the DX parts 1 and 2 from the DX extract
        if (sscanf(dx_info, "%d", &dx_extract) < 1){
            return errtxt(ZINT_ERROR_INVALID_DATA, symbol, 1006, "DX number is incorrect. Expected: 4 digits (DX extract) or 6 digits (DX full).");
        }
        if (dx_extract <= 0 || dx_extract > 2047){
            return errtxtf(ZINT_ERROR_INVALID_DATA, symbol, 1010, "DX extract is %d. Allowed values are between 0 and 2047.", dx_extract);
        }
        if (ZINT_DEBUG_PRINT) printf("Computed DX extract: %04d\n", dx_extract);
        dx_code_1 = dx_extract / 16;
        dx_code_2 = dx_extract % 16;
    }

    /* Convert components to binary strings */
    int_to_binary(dx_code_1, 7, binary_dx_code_1);  // 7 bits for dx_code_1
    int_to_binary(dx_code_2, 4, binary_dx_code_2);  // 4 bits for dx_code_2

    if (ZINT_DEBUG_PRINT) printf("%-*s%d\t-> %s\n", DEBUG_STR_LEN, "DX code 1:", dx_code_1, binary_dx_code_1);
    if (ZINT_DEBUG_PRINT) printf("%-*s%d\t-> %s\n", DEBUG_STR_LEN, "DX code 2:", dx_code_2, binary_dx_code_2);

    if (*has_frame_info) {
        str_to_uppercase(frame_info);

        // Map special values
        if (strcmp(frame_info, "S") == 0 || strcmp(frame_info, "X") == 0) {
            strcpy(frame_info, "62");
        } else if (strcmp(frame_info, "SA") == 0 || strcmp(frame_info, "XA") == 0) {
            strcpy(frame_info, "62A");
        } else if (strcmp(frame_info, "K") == 0 || strcmp(frame_info, "00") == 0) {
            strcpy(frame_info, "63");
        } else if (strcmp(frame_info, "KA") == 0 || strcmp(frame_info, "00A") == 0) {
            strcpy(frame_info, "63A");
        } else if (strcmp(frame_info, "F") == 0) {
            strcpy(frame_info, "0");
        } else if (strcmp(frame_info, "FA") == 0) {
            strcpy(frame_info, "0A");
        }

        // if (*has_frame_info && frame_number >= 0) {
        if (sscanf(frame_info, "%d%c", &frame_number, &half_frame_flag) < 1){
            return errtxt(ZINT_ERROR_INVALID_DATA, symbol, 1008, "DX part 2 must be between 0 and 15.");
            return -1; // invalid format
        }
        int_to_binary(frame_number, 6, binary_frame_number); // 6 bits for frame_number
        if (frame_number < 0 || frame_number > 63){
            return errtxt(ZINT_ERROR_INVALID_DATA, symbol, 1009, "Frame number should be between 0 and 63.");
        }
        if (ZINT_DEBUG_PRINT) printf("%-*s%d\t-> %s\n", DEBUG_STR_LEN, "Frame number:", frame_number, binary_frame_number);
    }



    /* Build the binary output */
    strcpy(binary_output, "101010"); // Start pattern
    strcat(binary_output, binary_dx_code_1);
    strcat(binary_output, "0"); // separator
    strcat(binary_output, binary_dx_code_2);
    if (*has_frame_info) {
        strcat(binary_output, binary_frame_number);
        if (half_frame_flag == 'A' || half_frame_flag == 'a') {
            if (ZINT_DEBUG_PRINT) printf("%-*s\'%c\'\t-> 1\n", DEBUG_STR_LEN, "Half frame flag:", half_frame_flag);
            strcat(binary_output, "1"); // Half-frame flag is 1 for 'A'
        } else {
            if (ZINT_DEBUG_PRINT) printf("%-*s\'%c\'\t-> 0\n", DEBUG_STR_LEN, "Half frame flag:", half_frame_flag);
            strcat(binary_output, "0"); // Default is 0
        }
        strcat(binary_output, "0"); // separator
    }
    

    // Compute parity bit
    for (i = 6; binary_output[i] != '\0'; i++) {
        if (binary_output[i] == '1') {
            parity_bit++;
        }
    }

    parity_bit %= 2;
    // if (ZINT_DEBUG_PRINT) printf("%-*s\'%c\'\t-> 0\n", DEBUG_STR_LEN, "Half frame flag:", half_frame_flag);
    if (ZINT_DEBUG_PRINT) printf("%-*s%s\t-> %d\n", DEBUG_STR_LEN, "Parity bit:", parity_bit?"yes":"no", parity_bit);
    if (parity_bit){
        strcat(binary_output, "1");
    }
    else{
        strcat(binary_output, "0");
    }

    strcat(binary_output, "0101"); // Stop pattern

    *output_length = strlen(binary_output);
    return 0; // Success
}



// Helper function to extract dx_code_1 and dx_code_2 from combined code
void extract_dx_codes(int combined_code, int *dx_code_1, int *dx_code_2) {
    if (combined_code < 10000) {
        // Short DX extract (4 digits or fewer)
        *dx_code_1 = combined_code / 16;
        *dx_code_2 = combined_code % 16;
    } else if (combined_code >= 100000) {
        // Full DX code (6 digits)
        *dx_code_1 = (combined_code / 1000) % 100;
        *dx_code_2 = (combined_code / 10) % 10;
    } else {
        *dx_code_1 = *dx_code_2 = -1; // Invalid combined code
    }
}

INTERNAL int dxfilmedge(struct zint_symbol *symbol, unsigned char source[], int length) {
    int i;
    int writer = 0;
    int error_number = 0;

    const int bar_width = 1; // Fixed bar width (module width of 1 unit)
    const int num_rows = 2;  // Total number of rows (clock + mock data)


    //const char *source, char *binary_output, int *output_length, bool *has_frame_info
    char char_data[32];
    int data_length;
    bool has_frame_info;
    int parse_result = parse_dx_code(symbol, source, char_data, &data_length, &has_frame_info);
    if (parse_result != 0){
        if (ZINT_DEBUG_PRINT) printf("Error %s\n\n", symbol->errtxt);
        return parse_result;
    }

    // Clock pattern: two formats exist, with or without frame number information.
    const char long_clock_pattern[] = "1111101010101010101010101010111";
    const char short_clock_pattern[] = "11111010101010101010111";

    const char *clock_pattern;
    int clock_length;
    if (has_frame_info){
        clock_pattern = long_clock_pattern;
        clock_length = sizeof(long_clock_pattern) -1;
    }
    else{
        clock_pattern = short_clock_pattern;
        clock_length = sizeof(short_clock_pattern) -1;
    }
    // printf("  clock pattern: %s\n", clock_pattern);
    // printf("  clock length: %d\n", clock_length);

    // printf("  data: %s\n", char_data);
  
    // First row: Clock pattern

    for (i = 0; i < clock_length; i++) {
        if (clock_pattern[i] == '1') {
            set_module(symbol, 0, writer); // Black bar
        } else if (clock_pattern[i] == '0') {
            unset_module(symbol, 0, writer); // White bar
        }
        writer++;
    }

    // Reset writer for the second row
    writer = 0;

    // Second row
    for (i = 0; i < clock_length; i++) {
        if (char_data[i] == '1') {
            set_module(symbol, 1, writer); // Black bar
        } else if (char_data[i] == '0') {
            unset_module(symbol, 1, writer); // White bar
        }
        writer++;
    }

    // Set dimensions
    symbol->rows = num_rows;                // Two rows: clock + mock data
    symbol->width = clock_length * bar_width; // Total width
    // symbol->height = (int)(symbol->width * bar_height_ratio); // Set height based on ratio

    if (symbol->output_options & COMPLIANT_HEIGHT) {

        // AFAIK There is no standard on minimum and maximum height,
        // So the margins are given close to the measurements.
        const float min_row_height = 2.1f;
        const float default_height = 5.08f; // Measured height on 35mm films. Seems like a bar is 1 * 2.54mm.
        const float max_height = 8.0f;

        // Calculate the height based on the width-to-height ratio
        // float calculated_height = (float)symbol->width * bar_height_ratio;
        float calculated_height = default_height;
        // Use set_height to enforce compliance with min/max height
        error_number = set_height(symbol, min_row_height, calculated_height, max_height, 0 /*no_errtxt*/);


        // const float default_height = 4.0f; // Default height-to-width ratio
        // error_number = set_height(symbol, default_height, default_height, bar_height_ratio, 0 /*no_errtxt*/);
    }

    return error_number;
}
