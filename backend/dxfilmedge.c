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
#include "common.h"


/* Utility to convert integer to binary string with fixed width */
void int_to_binary(int value, int width, char *output) {
    int i = 0;
    for (i = 0; i < width; i++) {
        output[width - 1 - i] = (value & (1 << i)) ? '1' : '0';
    }
    output[width] = '\0';
}

/* Main parsing function */
int parse_dx_code(const unsigned char *source, char *binary_output, int *output_length, bool *has_frame_info) {

    int i;
    int parity_bit = 0;
    int dx_code_1 = -1, dx_code_2 = -1, frame_number = -1;
    char binary_dx_code_1[8], binary_dx_code_2[5], binary_frame_number[7];
    char half_frame_flag = '\0';
    *has_frame_info = false; // Default to no frame info

    /* Parse the input string */
    if (strchr(source, '/')) {
        /* Format: 125-4/2A (with frame info) */
        if (sscanf(source, "%d-%d/%d%c", &dx_code_1, &dx_code_2, &frame_number, &half_frame_flag) < 3) {
            return -1; // Invalid format
        }
        *has_frame_info = true;
    } else {
        /* Format: 125-4 (no frame info) */
        if (sscanf(source, "%d-%d", &dx_code_1, &dx_code_2) != 2) {
            return -1; // Invalid format
        }
    }

    /* Convert components to binary strings */
    int_to_binary(dx_code_1, 7, binary_dx_code_1);  // 7 bits for dx_code_1
    int_to_binary(dx_code_2, 4, binary_dx_code_2);  // 4 bits for dx_code_2
    if (*has_frame_info && frame_number >= 0) {
        int_to_binary(frame_number, 6, binary_frame_number); // 6 bits for frame_number
        printf("frame number: %d\n", frame_number);
        printf("binary format: %s\n", binary_frame_number);
    }



    /* Build the binary output */
    strcpy(binary_output, "101010"); // Start pattern
    strcat(binary_output, binary_dx_code_1);
    strcat(binary_output, "0"); // separator
    strcat(binary_output, binary_dx_code_2);
    if (*has_frame_info) {
        strcat(binary_output, binary_frame_number);
        if (half_frame_flag == 'A') {
            strcat(binary_output, "1"); // Half-frame flag is 1 for 'A'
        } else {
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
    printf("parity bit: %d\n", parity_bit);
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
    if (parse_dx_code(source, char_data, &data_length, &has_frame_info) == 0){
        printf("  Input parsed.\n");
    }
    else{
        printf("  Failed to parse.\n\n");
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
    printf("  clock pattern: %s\n", clock_pattern);
    printf("  clock length: %d\n", clock_length);

    printf("  data: %s\n", char_data);
  
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


        const float min_row_height = 2.0f;  // Minimum height defined by standard
        const float default_height = 4.5f; // Example default height compliant with the standard
        const float max_height = 8.0f; // Maximum height allowed by standard

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
