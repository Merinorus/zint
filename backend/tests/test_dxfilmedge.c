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

#include "testcommon.h"

// static void test_large(const testCtx *const p_ctx) {
//     int debug = p_ctx->debug;

//     struct item {
//         int symbology;
//         int option_2;
//         char *pattern;
//         int length;
//         int ret;
//         int expected_rows;
//         int expected_width;
//         char *expected_errtxt;
//     };
//     /* s/\/\*[ 0-9]*\*\//\=printf("\/\*%3d*\/", line(".") - line("'<")): */
//     static const struct item data[] = {
//         /*  0*/ { BARCODE_C25STANDARD, -1, "1", 112, 0, 1, 1137, "" },
//         /*  1*/ { BARCODE_C25STANDARD, -1, "1", 113, ZINT_ERROR_TOO_LONG, -1, -1, "Error 301: Input length 113 too long (maximum 112)" },
//         /*  2*/ { BARCODE_C25STANDARD, 1, "1", 112, 0, 1, 1147, "" },
//         /*  3*/ { BARCODE_C25STANDARD, 1, "1", 113, ZINT_ERROR_TOO_LONG, -1, -1, "Error 301: Input length 113 too long (maximum 112)" },
//         /*  4*/ { BARCODE_C25INTER, -1, "1", 125, 0, 1, 1143, "" },
//         /*  5*/ { BARCODE_C25INTER, -1, "1", 126, ZINT_ERROR_TOO_LONG, -1, -1, "Error 309: Input length 126 too long (maximum 125)" },
//         /*  6*/ { BARCODE_C25INTER, 1, "1", 125, 0, 1, 1143, "" },
//         /*  7*/ { BARCODE_C25INTER, 1, "1", 126, ZINT_ERROR_TOO_LONG, -1, -1, "Error 309: Input length 126 too long (maximum 125)" },
//         /*  8*/ { BARCODE_C25IATA, -1, "1", 80, 0, 1, 1129, "" },
//         /*  9*/ { BARCODE_C25IATA, -1, "1", 81, ZINT_ERROR_TOO_LONG, -1, -1, "Error 305: Input length 81 too long (maximum 80)" },
//         /* 10*/ { BARCODE_C25IATA, 1, "1", 80, 0, 1, 1143, "" },
//         /* 11*/ { BARCODE_C25IATA, 1, "1", 81, ZINT_ERROR_TOO_LONG, -1, -1, "Error 305: Input length 81 too long (maximum 80)" },
//         /* 12*/ { BARCODE_C25LOGIC, -1, "1", 113, 0, 1, 1139, "" },
//         /* 13*/ { BARCODE_C25LOGIC, -1, "1", 114, ZINT_ERROR_TOO_LONG, -1, -1, "Error 307: Input length 114 too long (maximum 113)" },
//         /* 14*/ { BARCODE_C25LOGIC, 1, "1", 113, 0, 1, 1149, "" },
//         /* 15*/ { BARCODE_C25LOGIC, 1, "1", 114, ZINT_ERROR_TOO_LONG, -1, -1, "Error 307: Input length 114 too long (maximum 113)" },
//         /* 16*/ { BARCODE_C25IND, -1, "1", 79, 0, 1, 1125, "" },
//         /* 17*/ { BARCODE_C25IND, -1, "1", 80, ZINT_ERROR_TOO_LONG, -1, -1, "Error 303: Input length 80 too long (maximum 79)" },
//         /* 18*/ { BARCODE_C25IND, 1, "1", 79, 0, 1, 1139, "" },
//         /* 19*/ { BARCODE_C25IND, 1, "1", 80, ZINT_ERROR_TOO_LONG, -1, -1, "Error 303: Input length 80 too long (maximum 79)" },
//         /* 20*/ { BARCODE_DPLEIT, -1, "1", 13, 0, 1, 135, "" },
//         /* 21*/ { BARCODE_DPLEIT, -1, "1", 14, ZINT_ERROR_TOO_LONG, -1, -1, "Error 313: Input length 14 too long (maximum 13)" },
//         /* 22*/ { BARCODE_DPIDENT, -1, "1", 11, 0, 1, 117, "" },
//         /* 23*/ { BARCODE_DPIDENT, -1, "1", 12, ZINT_ERROR_TOO_LONG, -1, -1, "Error 315: Input length 12 too long (maximum 11)" },
//         /* 24*/ { BARCODE_ITF14, -1, "1", 13, 0, 1, 135, "" },
//         /* 25*/ { BARCODE_ITF14, -1, "1", 14, ZINT_ERROR_TOO_LONG, -1, -1, "Error 311: Input length 14 too long (maximum 13)" },
//     };
//     const int data_size = ARRAY_SIZE(data);
//     int i, length, ret;
//     struct zint_symbol *symbol = NULL;

//     char data_buf[4096];

//     testStartSymbol("test_large", &symbol);

//     for (i = 0; i < data_size; i++) {

//         if (testContinue(p_ctx, i)) continue;

//         symbol = ZBarcode_Create();
//         assert_nonnull(symbol, "Symbol not created\n");

//         testUtilStrCpyRepeat(data_buf, data[i].pattern, data[i].length);
//         assert_equal(data[i].length, (int) strlen(data_buf), "i:%d length %d != strlen(data_buf) %d\n", i, data[i].length, (int) strlen(data_buf));

//         length = testUtilSetSymbol(symbol, data[i].symbology, -1 /*input_mode*/, -1 /*eci*/, -1 /*option_1*/, data[i].option_2, -1, -1 /*output_options*/, data_buf, data[i].length, debug);

//         ret = ZBarcode_Encode(symbol, (unsigned char *) data_buf, length);
//         assert_equal(ret, data[i].ret, "i:%d ZBarcode_Encode ret %d != %d (%s)\n", i, ret, data[i].ret, symbol->errtxt);
//         assert_equal(symbol->errtxt[0] == '\0', ret == 0, "i:%d symbol->errtxt not %s (%s)\n", i, ret ? "set" : "empty", symbol->errtxt);
//         assert_zero(strcmp(symbol->errtxt, data[i].expected_errtxt), "i:%d strcmp(%s, %s) != 0\n", i, symbol->errtxt, data[i].expected_errtxt);

//         if (ret < ZINT_ERROR) {
//             assert_equal(symbol->rows, data[i].expected_rows, "i:%d symbol->rows %d != %d\n", i, symbol->rows, data[i].expected_rows);
//             assert_equal(symbol->width, data[i].expected_width, "i:%d symbol->width %d != %d\n", i, symbol->width, data[i].expected_width);
//         }

//         ZBarcode_Delete(symbol);
//     }

//     testFinish();
// }

// static void test_hrt(const testCtx *const p_ctx) {
//     int debug = p_ctx->debug;

//     struct item {
//         int symbology;
//         int option_2;
//         char *data;
//         char *expected;
//     };
//     /* s/\/\*[ 0-9]*\*\//\=printf("\/\*%3d*\/", line(".") - line("'<")): */
//     static const struct item data[] = {
//         /*  0*/ { BARCODE_C25STANDARD, -1, "123456789", "123456789" },
//         /*  1*/ { BARCODE_C25STANDARD, 1, "123456789", "1234567895" },
//         /*  2*/ { BARCODE_C25STANDARD, 2, "123456789", "123456789" }, /* Suppresses printing of check digit */
//         /*  3*/ { BARCODE_C25INTER, -1, "123456789", "0123456789" }, /* Adds leading zero if odd */
//         /*  4*/ { BARCODE_C25INTER, 1, "123456789", "1234567895" }, /* Unless check digit added when it becomes even */
//         /*  5*/ { BARCODE_C25INTER, 2, "123456789", "123456789" },
//         /*  6*/ { BARCODE_C25INTER, -1, "1234567890", "1234567890" }, /* No leading zero if even */
//         /*  7*/ { BARCODE_C25INTER, 1, "1234567890", "012345678905" }, /* Unless check digit added when it becomes odd */
//         /*  8*/ { BARCODE_C25INTER, 2, "1234567890", "01234567890" },
//         /*  9*/ { BARCODE_C25IATA, -1, "123456789", "123456789" },
//         /* 10*/ { BARCODE_C25IATA, 1, "123456789", "1234567895" },
//         /* 11*/ { BARCODE_C25IATA, 2, "123456789", "123456789" },
//         /* 12*/ { BARCODE_C25LOGIC, -1, "123456789", "123456789" },
//         /* 13*/ { BARCODE_C25LOGIC, 1, "123456789", "1234567895" },
//         /* 14*/ { BARCODE_C25LOGIC, 2, "123456789", "123456789" },
//         /* 15*/ { BARCODE_C25IND, -1, "123456789", "123456789" },
//         /* 16*/ { BARCODE_C25IND, 1, "123456789", "1234567895" },
//         /* 17*/ { BARCODE_C25IND, 2, "123456789", "123456789" },
//         /* 18*/ { BARCODE_DPLEIT, -1, "123456789", "00001.234.567.890" }, /* Leading zeroes added to make 13 + appended checksum */
//         /* 19*/ { BARCODE_DPLEIT, -1, "1234567890123", "12345.678.901.236" },
//         /* 20*/ { BARCODE_DPIDENT, -1, "123456789", "00.12 3.456.789 0" }, /* Leading zeroes added to make 11 + appended checksum */
//         /* 21*/ { BARCODE_DPIDENT, -1, "12345678901", "12.34 5.678.901 6" },
//         /* 22*/ { BARCODE_ITF14, -1, "123456789", "00001234567895" }, /* Leading zeroes added to make 13 + appended checksum */
//         /* 23*/ { BARCODE_ITF14, -1, "1234567890123", "12345678901231" },
//     };
//     const int data_size = ARRAY_SIZE(data);
//     int i, length, ret;
//     struct zint_symbol *symbol = NULL;

//     testStartSymbol("test_hrt", &symbol);

//     for (i = 0; i < data_size; i++) {

//         if (testContinue(p_ctx, i)) continue;

//         symbol = ZBarcode_Create();
//         assert_nonnull(symbol, "Symbol not created\n");

//         length = testUtilSetSymbol(symbol, data[i].symbology, -1 /*input_mode*/, -1 /*eci*/, -1 /*option_1*/, data[i].option_2, -1, -1 /*output_options*/, data[i].data, -1, debug);

//         ret = ZBarcode_Encode(symbol, (unsigned char *) data[i].data, length);
//         assert_zero(ret, "i:%d ZBarcode_Encode ret %d != 0 %s\n", i, ret, symbol->errtxt);

//         assert_zero(strcmp((char *) symbol->text, data[i].expected), "i:%d strcmp(%s, %s) != 0\n", i, symbol->text, data[i].expected);

//         ZBarcode_Delete(symbol);
//     }

//     testFinish();
// }

// static void test_input(const testCtx *const p_ctx) {
//     int debug = p_ctx->debug;

//     struct item {
//         int symbology;
//         int input_mode;
//         char *data;
//         int ret;
//         int expected_rows;
//         int expected_width;
//         char *expected_errtxt;
//     };
//     /* s/\/\*[ 0-9]*\*\//\=printf("\/\*%3d*\/", line(".") - line("'<")): */
//     static const struct item data[] = {
//         /*  0*/ { BARCODE_C25STANDARD, -1, "A", ZINT_ERROR_INVALID_DATA, -1, -1, "Error 302: Invalid character at position 1 in input (digits only)" },
//         /*  1*/ { BARCODE_C25STANDARD, -1, "1234A6", ZINT_ERROR_INVALID_DATA, -1, -1, "Error 302: Invalid character at position 5 in input (digits only)" },
//         /*  2*/ { BARCODE_C25STANDARD, ESCAPE_MODE, "\\d049234A6", ZINT_ERROR_INVALID_DATA, -1, -1, "Error 302: Invalid character at position 5 in input (digits only)" }, /* TODO (maybe): position doesn't account for escapes */
//         /*  3*/ { BARCODE_C25INTER, -1, "A", ZINT_ERROR_INVALID_DATA, -1, -1, "Error 310: Invalid character at position 1 in input (digits only)" },
//         /*  4*/ { BARCODE_C25IATA, -1, "A", ZINT_ERROR_INVALID_DATA, -1, -1, "Error 306: Invalid character at position 1 in input (digits only)" },
//         /*  5*/ { BARCODE_C25LOGIC, -1, "A", ZINT_ERROR_INVALID_DATA, -1, -1, "Error 308: Invalid character at position 1 in input (digits only)" },
//         /*  6*/ { BARCODE_C25IND, -1, "A", ZINT_ERROR_INVALID_DATA, -1, -1, "Error 304: Invalid character at position 1 in input (digits only)" },
//         /*  7*/ { BARCODE_DPLEIT, -1, "A", ZINT_ERROR_INVALID_DATA, -1, -1, "Error 314: Invalid character at position 1 in input (digits only)" },
//         /*  8*/ { BARCODE_DPIDENT, -1, "A", ZINT_ERROR_INVALID_DATA, -1, -1, "Error 316: Invalid character at position 1 in input (digits only)" },
//         /*  9*/ { BARCODE_DPIDENT, -1, "1234567890A", ZINT_ERROR_INVALID_DATA, -1, -1, "Error 316: Invalid character at position 11 in input (digits only)" },
//         /* 10*/ { BARCODE_ITF14, -1, "A", ZINT_ERROR_INVALID_DATA, -1, -1, "Error 312: Invalid character at position 1 in input (digits only)" },
//     };
//     const int data_size = ARRAY_SIZE(data);
//     int i, length, ret;
//     struct zint_symbol *symbol = NULL;

//     testStartSymbol("test_input", &symbol);

//     for (i = 0; i < data_size; i++) {

//         if (testContinue(p_ctx, i)) continue;

//         symbol = ZBarcode_Create();
//         assert_nonnull(symbol, "Symbol not created\n");

//         length = testUtilSetSymbol(symbol, data[i].symbology, data[i].input_mode, -1 /*eci*/, -1 /*option_1*/, -1 /*option_2*/, -1, -1 /*output_options*/, data[i].data, -1, debug);

//         ret = ZBarcode_Encode(symbol, (unsigned char *) data[i].data, length);
//         assert_equal(ret, data[i].ret, "i:%d ZBarcode_Encode ret %d != %d (%s)\n", i, ret, data[i].ret, symbol->errtxt);
//         assert_equal(symbol->errtxt[0] == '\0', ret == 0, "i:%d symbol->errtxt not %s (%s)\n", i, ret ? "set" : "empty", symbol->errtxt);
//         assert_zero(strcmp(symbol->errtxt, data[i].expected_errtxt), "i:%d strcmp(%s, %s) != 0\n", i, symbol->errtxt, data[i].expected_errtxt);

//         if (ret < ZINT_ERROR) {
//             assert_equal(symbol->rows, data[i].expected_rows, "i:%d symbol->rows %d != %d\n", i, symbol->rows, data[i].expected_rows);
//             assert_equal(symbol->width, data[i].expected_width, "i:%d symbol->width %d != %d\n", i, symbol->width, data[i].expected_width);
//         }

//         ZBarcode_Delete(symbol);
//     }

//     testFinish();
// }

static void test_encode(const testCtx *const p_ctx) {
    int debug = p_ctx->debug;

    struct item {
        int symbology;
        int option_2;
        char *data;
        int ret;

        int expected_rows;
        int expected_width;
        char *comment;
        char *expected;
    };
    /* BARCODE_ITF14 examples verified manually against GS1 General Specifications 21.0.1 */
    static const struct item data[] = {
        /* DX code 1: 79, DX code 2: 7. DX Extract = 1271. DX Full: X1271X (X is any digit)*/
        /*  0*/ { BARCODE_DXFILMEDGE, -1, "79-7", 0, 2, 23, "",
                    "1111101010101010101011110101010011110011100101"
                },
        /*  1*/ { BARCODE_DXFILMEDGE, -1, "1271", 0, 2, 23, "",
                    "1111101010101010101011110101010011110011100101"
                },
        /*  2*/ { BARCODE_DXFILMEDGE, -1, "012710", 0, 2, 23, "",
                    "1111101010101010101011110101010011110011100101"
                },
        /* First digit and last digit should not impact the result ignored */
        /*  3*/ { BARCODE_DXFILMEDGE, -1, "112712", 0, 2, 23, "",
                    "1111101010101010101011110101010011110011100101"
                },
        /*  ?*/ { BARCODE_DXFILMEDGE, -1, "1-0", 0, 2, 23, "",
                    "1111101010101010101011110101000000010000010101"
                },
        /*  ?*/ { BARCODE_DXFILMEDGE, -1, "79-7/1", 0, 2, 31, "",
                    "11111010101010101010101010101111010101001111001110000010010101"
                },
        /*  ?*/ { BARCODE_DXFILMEDGE, -1, "1271/1", 0, 2, 31, "",
                    "11111010101010101010101010101111010101001111001110000010010101"
                },
        /*  ?*/ { BARCODE_DXFILMEDGE, -1, "912718/1", 0, 2, 31, "",
                    "11111010101010101010101010101111010101001111001110000010010101"
                },
        /*  ?*/ { BARCODE_DXFILMEDGE, -1, "79-7/1A", 0, 2, 31, "",
                    "11111010101010101010101010101111010101001111001110000011000101"
                },
        /*  ?*/ { BARCODE_DXFILMEDGE, -1, "1271/1a", 0, 2, 31, "",
                    "11111010101010101010101010101111010101001111001110000011000101"
                },
        /*  ?*/ { BARCODE_DXFILMEDGE, -1, "212715/1A", 0, 2, 31, "",
                    "11111010101010101010101010101111010101001111001110000011000101"
                },

        /* Tests to do:
        - Should raise error on DX Code 1 <= 0 and > 127 (input : "1-0", dx extracts "1" to "15")
        - Should raise error on DX code 2 < 0 and > 15
        - Should raise error on frame number > 63
        - Should raise error on frame number unknown? eg: "000", "H"...
        - Should not display anything on error
        - Hints on output ? Type of DX code (dx codes 1 and 2 or dx extract...)
        - Show allowed formats
         */
    };
    const int data_size = ARRAY_SIZE(data);
    int i, length, ret;
    struct zint_symbol *symbol = NULL;

    char escaped[1024];
    char cmp_buf[4096];
    char cmp_msg[1024];

    int do_bwipp = (debug & ZINT_DEBUG_TEST_BWIPP) && testUtilHaveGhostscript(); /* Only do BWIPP test if asked, too slow otherwise */
    int do_zxingcpp = (debug & ZINT_DEBUG_TEST_ZXINGCPP) && testUtilHaveZXingCPPDecoder(); /* Only do ZXing-C++ test if asked, too slow otherwise */

    testStartSymbol("test_encode", &symbol);

    for (i = 0; i < data_size; i++) {

        if (testContinue(p_ctx, i)) continue;

        symbol = ZBarcode_Create();
        assert_nonnull(symbol, "Symbol not created\n");

        length = testUtilSetSymbol(symbol, data[i].symbology, -1 /*input_mode*/, -1 /*eci*/, -1 /*option_1*/, data[i].option_2, -1, -1 /*output_options*/, data[i].data, -1, debug);

        ret = ZBarcode_Encode(symbol, (unsigned char *) data[i].data, length);
        assert_equal(ret, data[i].ret, "i:%d ZBarcode_Encode ret %d != %d (%s)\n", i, ret, data[i].ret, symbol->errtxt);

        if (p_ctx->generate) {
            printf("        /*%3d*/ { %s, %d, \"%s\", %s, %d, %d, \"%s\",\n",
                    i, testUtilBarcodeName(data[i].symbology), data[i].option_2, testUtilEscape(data[i].data, length, escaped, sizeof(escaped)),
                    testUtilErrorName(data[i].ret), symbol->rows, symbol->width, data[i].comment);
            testUtilModulesPrint(symbol, "                    ", "\n");
            printf("                },\n");
        } else {
            if (ret < ZINT_ERROR) {
                int width, row;

                assert_equal(symbol->rows, data[i].expected_rows, "i:%d symbol->rows %d != %d (%s)\n", i, symbol->rows, data[i].expected_rows, data[i].data);
                assert_equal(symbol->width, data[i].expected_width, "i:%d symbol->width %d != %d (%s)\n", i, symbol->width, data[i].expected_width, data[i].data);

                ret = testUtilModulesCmp(symbol, data[i].expected, &width, &row);
                assert_zero(ret, "i:%d testUtilModulesCmp ret %d != 0 width %d row %d (%s)\n", i, ret, width, row, data[i].data);

                if (do_bwipp && testUtilCanBwipp(i, symbol, -1, data[i].option_2, -1, debug)) {
                    ret = testUtilBwipp(i, symbol, -1, data[i].option_2, -1, data[i].data, length, NULL, cmp_buf, sizeof(cmp_buf), NULL);
                    assert_zero(ret, "i:%d %s testUtilBwipp ret %d != 0\n", i, testUtilBarcodeName(symbol->symbology), ret);

                    ret = testUtilBwippCmp(symbol, cmp_msg, cmp_buf, data[i].expected);
                    assert_zero(ret, "i:%d %s testUtilBwippCmp %d != 0 %s\n  actual: %s\nexpected: %s\n",
                                   i, testUtilBarcodeName(symbol->symbology), ret, cmp_msg, cmp_buf, data[i].expected);
                }
                if (do_zxingcpp && testUtilCanZXingCPP(i, symbol, data[i].data, length, debug)) {
                    int cmp_len, ret_len;
                    char modules_dump[8192 + 1];
                    assert_notequal(testUtilModulesDump(symbol, modules_dump, sizeof(modules_dump)), -1, "i:%d testUtilModulesDump == -1\n", i);
                    ret = testUtilZXingCPP(i, symbol, data[i].data, length, modules_dump, cmp_buf, sizeof(cmp_buf), &cmp_len);
                    assert_zero(ret, "i:%d %s testUtilZXingCPP ret %d != 0\n", i, testUtilBarcodeName(symbol->symbology), ret);

                    ret = testUtilZXingCPPCmp(symbol, cmp_msg, cmp_buf, cmp_len, data[i].data, length, NULL /*primary*/, escaped, &ret_len);
                    assert_zero(ret, "i:%d %s testUtilZXingCPPCmp %d != 0 %s\n  actual: %.*s\nexpected: %.*s\n",
                                   i, testUtilBarcodeName(symbol->symbology), ret, cmp_msg, cmp_len, cmp_buf, ret_len, escaped);
                }
            }
        }

        ZBarcode_Delete(symbol);
    }

    testFinish();
}

// #include <time.h>

// #define TEST_PERF_ITER_MILLES   5
// #define TEST_PERF_ITERATIONS    (TEST_PERF_ITER_MILLES * 1000)
// #define TEST_PERF_TIME(arg) ((arg) * 1000.0 / CLOCKS_PER_SEC)

// /* Not a real test, just performance indicator */
// static void test_perf(const testCtx *const p_ctx) {
//     int debug = p_ctx->debug;

//     struct item {
//         int symbology;
//         int option_2;
//         char *data;
//         int ret;

//         int expected_rows;
//         int expected_width;
//         char *comment;
//     };
//     static const struct item data[] = {
//         /*  0*/ { BARCODE_C25INTER, -1, "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890", 0, 1, 819, "C25INTER 90" },
//         /*  1*/ { BARCODE_C25INTER, -1, "1234567890", 0, 1, 99, "C25INTER 10" },
//         /*  2*/ { BARCODE_C25STANDARD, -1, "12345678901234567890123456789012345678901234567890123456789012345678901234567890", 0, 1, 817, "C25STANDARD 80" },
//         /*  3*/ { BARCODE_C25STANDARD, -1, "1234567890", 0, 1, 117, "C25STANDARD 10" },
//     };
//     const int data_size = ARRAY_SIZE(data);
//     int i, length, ret;
//     struct zint_symbol *symbol;

//     clock_t start;
//     clock_t total_create = 0, total_encode = 0, total_buffer = 0, total_buf_inter = 0, total_print = 0;
//     clock_t diff_create, diff_encode, diff_buffer, diff_buf_inter, diff_print;
//     int comment_max = 0;

//     if (!(debug & ZINT_DEBUG_TEST_PERFORMANCE)) { /* -d 256 */
//         return;
//     }

//     for (i = 0; i < data_size; i++) if ((int) strlen(data[i].comment) > comment_max) comment_max = (int) strlen(data[i].comment);

//     printf("Iterations %d\n", TEST_PERF_ITERATIONS);

//     for (i = 0; i < data_size; i++) {
//         int j;

//         if (testContinue(p_ctx, i)) continue;

//         diff_create = diff_encode = diff_buffer = diff_buf_inter = diff_print = 0;

//         for (j = 0; j < TEST_PERF_ITERATIONS; j++) {
//             start = clock();
//             symbol = ZBarcode_Create();
//             diff_create += clock() - start;
//             assert_nonnull(symbol, "Symbol not created\n");

//             length = testUtilSetSymbol(symbol, data[i].symbology, DATA_MODE, -1 /*eci*/, -1 /*option_1*/, data[i].option_2, -1, -1 /*output_options*/, data[i].data, -1, debug);

//             start = clock();
//             ret = ZBarcode_Encode(symbol, (unsigned char *) data[i].data, length);
//             diff_encode += clock() - start;
//             assert_equal(ret, data[i].ret, "i:%d ZBarcode_Encode ret %d != %d (%s)\n", i, ret, data[i].ret, symbol->errtxt);

//             assert_equal(symbol->rows, data[i].expected_rows, "i:%d symbol->rows %d != %d (%s)\n", i, symbol->rows, data[i].expected_rows, data[i].data);
//             assert_equal(symbol->width, data[i].expected_width, "i:%d symbol->width %d != %d (%s)\n", i, symbol->width, data[i].expected_width, data[i].data);

//             start = clock();
//             ret = ZBarcode_Buffer(symbol, 0 /*rotate_angle*/);
//             diff_buffer += clock() - start;
//             assert_zero(ret, "i:%d ZBarcode_Buffer ret %d != 0 (%s)\n", i, ret, symbol->errtxt);

//             symbol->output_options |= OUT_BUFFER_INTERMEDIATE;
//             start = clock();
//             ret = ZBarcode_Buffer(symbol, 0 /*rotate_angle*/);
//             diff_buf_inter += clock() - start;
//             assert_zero(ret, "i:%d ZBarcode_Buffer OUT_BUFFER_INTERMEDIATE ret %d != 0 (%s)\n", i, ret, symbol->errtxt);
//             symbol->output_options &= ~OUT_BUFFER_INTERMEDIATE; /* Undo */

//             start = clock();
//             ret = ZBarcode_Print(symbol, 0 /*rotate_angle*/);
//             diff_print += clock() - start;
//             assert_zero(ret, "i:%d ZBarcode_Print ret %d != 0 (%s)\n", i, ret, symbol->errtxt);
//             assert_zero(testUtilRemove(symbol->outfile), "i:%d testUtilRemove(%s) != 0\n", i, symbol->outfile);

//             ZBarcode_Delete(symbol);
//         }

//         printf("%*s: encode % 8gms, buffer % 8gms, buf_inter % 8gms, print % 8gms, create % 8gms\n", comment_max, data[i].comment,
//                 TEST_PERF_TIME(diff_encode), TEST_PERF_TIME(diff_buffer), TEST_PERF_TIME(diff_buf_inter), TEST_PERF_TIME(diff_print), TEST_PERF_TIME(diff_create));

//         total_create += diff_create;
//         total_encode += diff_encode;
//         total_buffer += diff_buffer;
//         total_buf_inter += diff_buf_inter;
//         total_print += diff_print;
//     }
//     if (p_ctx->index == -1) {
//         printf("%*s: encode % 8gms, buffer % 8gms, buf_inter % 8gms, print % 8gms, create % 8gms\n", comment_max, "totals",
//                 TEST_PERF_TIME(total_encode), TEST_PERF_TIME(total_buffer), TEST_PERF_TIME(total_buf_inter), TEST_PERF_TIME(total_print), TEST_PERF_TIME(total_create));
//     }
// }

int main(int argc, char *argv[]) {

    testFunction funcs[] = { /* name, func */
        // { "test_large", test_large },
        // { "test_hrt", test_hrt },
        // { "test_input", test_input },
        { "test_encode", test_encode },
        // { "test_perf", test_perf },
    };

    testRun(argc, argv, funcs, ARRAY_SIZE(funcs));

    testReport();

    return 0;
}

/* vim: set ts=4 sw=4 et : */
