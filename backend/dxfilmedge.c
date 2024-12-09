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

// static int pharma_two_calc(int tester, char *d) {
//     /* This code uses the Two Track Pharamacode defined in the document at
//        http://www.laetus.com/laetus.php?request=file&id=69 and using a modified
//        algorithm from the One Track system. This standard accepts integet values
//        from 4 to 64570080. */

//     int counter, h;
//     char inter[17];
//     char *in = inter;

//     do {
//         switch (tester % 3) {
//             case 0:
//                 *in++ = '3';
//                 tester = (tester - 3) / 3;
//                 break;
//             case 1:
//                 *in++ = '1';
//                 tester = (tester - 1) / 3;
//                 break;
//             case 2:
//                 *in++ = '2';
//                 tester = (tester - 2) / 3;
//                 break;
//         }
//     } while (tester != 0);

//     h = in - inter;
//     for (counter = h - 1; counter >= 0; counter--) {
//         *d++ = inter[counter];
//     }
//     *d = '\0';

//     return h;
// }

// INTERNAL int backup_dxfilmedge(struct zint_symbol *symbol, unsigned char source[], int length) {
//     /* Draws the patterns for two track pharmacode */
//     int i;
//     int tester;
//     char height_pattern[200];
//     unsigned int loopey, h;
//     int writer;
//     int error_number = 0;

//     if (length > 8) {
//         return errtxtf(ZINT_ERROR_TOO_LONG, symbol, 354, "Input length %d too long (maximum 8)", length);
//     }
//     if ((i = not_sane(NEON_F, source, length))) {
//         return errtxtf(ZINT_ERROR_INVALID_DATA, symbol, 355,
//                         "Invalid character at position %d in input (digits only)", i);
//     }

//     tester = to_int(source, length);
//     if ((tester < 4) || (tester > 64570080)) {
//         return errtxtf(ZINT_ERROR_INVALID_DATA, symbol, 353, "Input value '%d' out of range (4 to 64570080)", tester);
//     }
//     h = pharma_two_calc(tester, height_pattern);

//     writer = 0;
//     for (loopey = 0; loopey < h; loopey++) {
//         if ((height_pattern[loopey] == '2') || (height_pattern[loopey] == '3')) {
//             set_module(symbol, 0, writer);
//         }
//         if ((height_pattern[loopey] == '1') || (height_pattern[loopey] == '3')) {
//             set_module(symbol, 1, writer);
//         }
//         writer += 2;
//     }
//     symbol->rows = 2;
//     symbol->width = writer - 1;

//     if (symbol->output_options & COMPLIANT_HEIGHT) {
//         /* Laetus Pharmacode Guide 1.4
//            Two-track height min 8mm / 2mm (X max) = 4X (2X per row), standard 8mm / 1mm = 8X,
//            max 12mm / 0.8mm (X min) = 15X */
//         error_number = set_height(symbol, 2.0f, 8.0f, 15.0f, 0 /*no_errtxt*/);
//     } else {
//         (void) set_height(symbol, 0.0f, 10.0f, 0.0f, 1 /*no_errtxt*/);
//     }

//     return 0;
// }

// INTERNAL int dxfilmedge(struct zint_symbol *symbol, unsigned char source[], int length) {
//     int i;
//     int writer = 0;
//     int error_number = 0;
//     const float max_height = 2.0f;

//     // Clock pattern
//     const char clock_pattern[] = "1111101010101010101010101010111";
//     // const char clock_pattern[] = "0000010101010101010101010101000";
//     int clock_length = sizeof(clock_pattern) - 1; // Exclude null terminator

//     const char start_pattern[] = "101010";
//     int start_pattern_length = sizeof(start_pattern) - 1;

//     const char stop_pattern[] = "101";
//     int stop_pattern_length = sizeof(stop_pattern) - 1;

//     // Mock data for the second row
//     const char data[]     = "1001111000101111110000";
//     int data_length = sizeof(data) - 1; // Exclude null terminator

//     // Ensure both rows have the same width
//     if (data_length != clock_length - start_pattern_length - stop_pattern_length) {
//         return ZINT_ERROR_INVALID_DATA; // Return an error if data lengths don't match
//     }

//     // First row: Clock pattern
//     for (i = 0; i < clock_length; i++) {
//         if (clock_pattern[i] == '1') {
//             set_module(symbol, 0, writer); // Black bar
//         } else if (clock_pattern[i] == '0') {
//             unset_module(symbol, 0, writer); // White bar
//         }
//         writer++;
//     }

//     // Reset writer for the second row
//     writer = 0;

//     // Second row: start pattern
//     for (i = 0; i < start_pattern_length; i++) {
//         if (start_pattern[i] == '1') {
//             set_module(symbol, 1, writer); // Black bar for second row
//         } else if (start_pattern[i] == '0') {
//             unset_module(symbol, 1, writer); // White bar for second row
//         }
//         writer++;
//     }

//     // Second row: Mock data
//     for (i = 0; i < data_length; i++) {
//         if (data[i] == '1') {
//             set_module(symbol, 1, writer); // Black bar for second row
//         } else if (data[i] == '0') {
//             unset_module(symbol, 1, writer); // White bar for second row
//         }
//         writer++;
//     }

//     // Second row: stop pattern
//     for (i = 0; i < stop_pattern_length; i++) {
//         if (stop_pattern[i] == '1') {
//             set_module(symbol, 1, writer); // Black bar for second row
//         } else if (stop_pattern[i] == '0') {
//             unset_module(symbol, 1, writer); // White bar for second row
//         }
//         writer++;
//     }

//     // Set the number of rows and total width
//     symbol->rows = 2;          // Two rows: clock + mock data
//     symbol->width = clock_length; // Total width is the width of a single row

//     if (symbol->output_options & COMPLIANT_HEIGHT) {
//         const float default_height = 1.4545441f; /* 0.625 / 0.01375 */
//         error_number = set_height(symbol, 6.25f, default_height, max_height, 0 /*no_errtxt*/);
//     }


//     return error_number; // Return 0 for success
// }

int parse_dx_code(const char *source, int *dx_code_1, int *dx_code_2, int *frame_number, bool *half_frame_flag) {
    const char *ptr = source; // Pointer to traverse the string
    char *end_ptr;           // Pointer for strtol to indicate where parsing stopped
    
    *frame_number = -1; // No frame number present
    *half_frame_flag = false;

    // Parse dx_code_1
    *dx_code_1 = strtol(ptr, &end_ptr, 10);
    if (*dx_code_1 < 0 || *dx_code_1 > 127 || *end_ptr != '-') {
        fprintf(stderr, "Invalid dx_code_1 or missing '-': %s\n", source);
        return -1;
    }
    ptr = end_ptr + 1; // Move past the '-'

    // Parse dx_code_2
    *dx_code_2 = strtol(ptr, &end_ptr, 10);
    if (*dx_code_2 < 0 || *dx_code_2 > 15 || (*end_ptr != '/' && *end_ptr != '\0')) {
        fprintf(stderr, "Invalid dx_code_2 or unexpected character: %s\n", source);
        return -1;
    }
    ptr = end_ptr; // Move past the number

    // Parse optional frame_number and half_frame_flag
    if (*ptr == '/') {
        ptr++; // Move past the '/'
        *frame_number = strtol(ptr, &end_ptr, 10);
        if (*frame_number < 0 || *frame_number > 63) {
            fprintf(stderr, "Invalid frame_number: %s\n", source);
            return -1;
        }
        *half_frame_flag = (*end_ptr == 'A');
        if (*half_frame_flag) {
            end_ptr++; // Move past 'A'
        }
        ptr = end_ptr; // Update pointer
    }

    // Ensure we processed the whole string
    if (*ptr != '\0') {
        fprintf(stderr, "Unexpected characters at the end: %s\n", source);
        return -1;
    }

    return 0; // Success
}

INTERNAL int dxfilmedge(struct zint_symbol *symbol, unsigned char source[], int length) {
    int i;
    int writer = 0;
    int error_number = 0;

    const float bar_height_ratio = 2.0f; // Each bar is twice as tall as it is wide
    const int bar_width = 1; // Fixed bar width (module width of 1 unit)
    const int num_rows = 2;  // Total number of rows (clock + mock data)

    int dx_code_1, dx_code_2, frame_number;
    bool half_frame_flag = 0;

    if (parse_dx_code(source, &dx_code_1, &dx_code_2, &frame_number, &half_frame_flag) == 0) {
        printf("  dx_code_1: %d\n", dx_code_1);
        printf("  dx_code_2: %d\n", dx_code_2);
        if (frame_number >= 0){
            printf("  frame_number: %d\n", frame_number);
            printf("  half_frame_flag: %s\n\n", half_frame_flag ? "true" : "false");
        }
    } else {
        printf("  Failed to parse.\n\n");
    }

    // Clock pattern: two formats exist, with or without frame number information.
    const char long_clock_pattern[] = "1111101010101010101010101010111";
    const char short_clock_pattern[] = "11111010101010101010111";

    const char *clock_pattern;
    int clock_length;
    if (frame_number >= 0){
        clock_pattern = long_clock_pattern;
        clock_length = sizeof(long_clock_pattern) -1;
    }
    else{
        clock_pattern = short_clock_pattern;
        clock_length = sizeof(short_clock_pattern) -1;
    }
    printf("  clock pattern: %s\n", clock_pattern);
    printf("  clock length: %d\n", clock_length);
    // clock_length = sizeof(clock_pattern) - 1;
    


    // Start and stop patterns
    const char start_pattern[] = "101010";
    int start_pattern_length = sizeof(start_pattern) - 1;
    const char stop_pattern[] = "101";
    int stop_pattern_length = sizeof(stop_pattern) - 1;

    const int dx_code_1_length = 7;
    const int dx_code_2_length = 4;
    const int frame_number_length = 6;
    // Mock data for the second row
    const char data[] = "1001111000101111110000";
    int data_length = sizeof(data) - 1; // Exclude null terminator

    // Ensure mock data width matches the clock row width
    if (data_length != clock_length - start_pattern_length - stop_pattern_length) {
        return ZINT_ERROR_INVALID_DATA; // Return an error if data lengths don't match
    }

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

    // Second row: Start pattern
    for (i = 0; i < start_pattern_length; i++) {
        if (start_pattern[i] == '1') {
            set_module(symbol, 1, writer); // Black bar for second row
        } else if (start_pattern[i] == '0') {
            unset_module(symbol, 1, writer); // White bar for second row
        }
        writer++;
    }

    // Second row: Mock data
    for (i = 0; i < data_length; i++) {
        if (data[i] == '1') {
            set_module(symbol, 1, writer); // Black bar for second row
        } else if (data[i] == '0') {
            unset_module(symbol, 1, writer); // White bar for second row
        }
        writer++;
    }

    // Second row: Stop pattern
    for (i = 0; i < stop_pattern_length; i++) {
        if (stop_pattern[i] == '1') {
            set_module(symbol, 1, writer); // Black bar for second row
        } else if (stop_pattern[i] == '0') {
            unset_module(symbol, 1, writer); // White bar for second row
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
        error_number = set_height(symbol, min_row_height, default_height, max_height, 0 /*no_errtxt*/);


        // const float default_height = 4.0f; // Default height-to-width ratio
        // error_number = set_height(symbol, default_height, default_height, bar_height_ratio, 0 /*no_errtxt*/);
    }

    return error_number;
}
