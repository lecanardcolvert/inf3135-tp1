/*
 * canvascii.c
 *
 * This program draws on an ASCII canvas. The different options available must
 * be specified in the arguments when running the program. It is possible to
 * create a new canvas, then draw lines, rectangles, segments and circles, of
 * different colors. The canvas can also be read on standard input. After that,
 * you can print the canvas on the screen. The canvas can be colorized.
 *
 * The maximum size of a canvas is 80 wide * 40 high.
 *
 * The user guide is printed when the program is called without an argument or
 * when there is an input error.
 *
 * @author Alexandre H. Bourdeau (HAMA12128907)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "canvascii.h"

struct Canvas canvas;
char importedCanvas[MAX_HEIGHT][MAX_WIDTH];
bool colorPrint = false;

// TODO Complete this function
/*
 * Prints the manual on the console.
 */
void printUsage() {
    fprintf(stdout, "%s", USAGE);
}

// TODO Complete this function
/* 
 * Prints the error message and the manual, then closes the program
 */
void closeProgram(int errorNo) {
    fprintf(stderr, "%s", ERROR_MSG[errorNo]);
    fprintf(stderr, "%s", USAGE);
}

/*
 * fgetstr() - mimics behavior of fgets(), but 
 * removes new-line character at end of line if it exists
 *
 * @param   *string The string where to store characters
 *          n       Maximum of characters to be read
 *          *STREAM Pointer to FILE where to read stream from (stdin)
 * @returns The string read from the stream, until EOF is encountered
 * @see     fgets
 */
char *fgetstr(char *string, int n, FILE *stream)
{
	char *result;
	result = fgets(string, n, stream);
	if(!result)
		return(result);

	if(string[strlen(string) - 1] == '\n')
		string[strlen(string) - 1] = 0;

	return(string);
}

/*
 * Returns the minimum between two integers
 *
 * @param   int1  The first int to compare
 * @param   int2  The second int to compare
 * @return  An int which is the minimum
 */
int min(int int1, int int2) {
    return (int1 < int2) ? int1 : int2;
}

/*
 * Returns the maximum between two integers
 *
 * @param   int1    The first int to compare
 * @param   int2    The second int to compare
 * @return  An int which is the maximum
 */
int max(int int1, int int2) {
    return (int1 > int2) ? int1 : int2;
}

/*
 * Tells if a number is in the range (inclusive) of two numbers
 *
 * @param   lowerLimit  The lower boundary
 * @param   upperLimit  The upper boundary
 * @return  TRUE if the number is between both inclusive boundaries,
 *          FALSE if not
 */
bool isInRange(int value, int lowerLimit, int upperLimit) {
    return (lowerLimit <= value && value <= upperLimit);
}

/*
 * Validates if a string is a number
 *
 * @param   *str    The input string
 * @returns OK if string is a number,
 *          ERR_BAD_INPUT if not
 */
int validateStrToInt(char *str) {
    int errorNo = OK;
    unsigned int strLength = strlen(str);

    if (str != NULL) {
        for (unsigned int i = 0; i < strLength; i++) {
            if (!isdigit(str[i])) {
                errorNo = ERR_BAD_INPUT;
            }
        }
    } else {
        errorNo = ERR_BAD_INPUT;
    }

    return errorNo;
}
/*
 * Converts a string to an int
 *
 * @param   *str    The input string
 * @returns The converted int if the validation is succesful,
 *          ERR_BAD_INPUT if there is an error
 */
int strToInt(char *str) {
    int errorNo = validateStrToInt(str);
    unsigned int value = 0;

    if (errorNo != ERR_BAD_INPUT) {
        value = atoi(str); 
    }

    return value;
}

// TODO Complete function
/*
 * Parses a string separated by commas into an int array
 *
 * @param   *str    The input string
 * @param   noArgs  The number of tokens to extract
 * @param   *values The int array to store the results
 * @returns OK if no errors,
 *          ERR_BAD_INPUT if there is an error.
 *          The data is saved in *values.
 */
int parseIntArgs(char *str, int noArgs, int *values) {
    const char *SEPARATOR = ",";
    char *token;
    int errorNo = OK;
    int i = 0;
   
    if ((str != NULL) || (values != NULL)) {
        if (noArgs > 0 && strchr(str, *SEPARATOR) != NULL) {
            // Parse first token first
            token = strtok(str, SEPARATOR);
            if (strToInt(token) != -1) {
                values[i] = strToInt(token);
                i++;
            }

            // Parse next tokens
            while ((token != NULL) && (i < noArgs)) {
                token = strtok(NULL, SEPARATOR);
                if (strToInt(token) != -1) {
                    values[i] = strToInt(token);
                    i++;
                }
            }
        }
    } else {
        errorNo = ERR_BAD_INPUT;
    }

    return errorNo;
}

/*
 * Fills a new canvas with the empty pixel
 */
void fillCanvas() {
    for (unsigned int i = 0; i < canvas.height; i++) {
        for (unsigned int j = 0; j < canvas.width; j++) {
            canvas.pixels[i][j] = EMPTY_PIXEL;
        }
    }
}

/*
 * Validates the size of a new canvas
 *
 * @param   height  The height
 * @param   width   The width
 * @returns OK if no error,
 *          ERR_CANVAS_TOO_HIGH if too high,
 *          ERR_CANVAS_TOO_WIDE if too wide,
 *          ERR_WITH_VALUE if too small
 */
int validateCanvasSize(int height, int width) {
    int errorNo = OK;

    if (height > MAX_HEIGHT) {
        errorNo = ERR_CANVAS_TOO_HIGH;
    } else if (width > MAX_WIDTH) {
        errorNo = ERR_CANVAS_TOO_WIDE;
    } else if ((width < MIN_WIDTH) || (height < MIN_HEIGHT)) {
        errorNo = ERR_WITH_VALUE;
    }

    return errorNo;
}

// TODO Complete function
/* 
 * Creates a new empty canvas and fills it with empty pixels
 *
 * @param   height  The height
 * @param   width   The width
 * @returns see function validateCanvasSize
 */
int newCanvas(int height, int width) {
    int errorNo = validateCanvasSize(height, width);

    if (errorNo == OK) {
        canvas.height = height;
        canvas.width = width;
        canvas.pen = 7;
        // TODO Change fillCanvas(); position
        // fillCanvas();
    }

    return errorNo;
}

/*
 * Validates the value of a pixel.
 * Must be either a valid pen or the empty pixel
 *
 * @param   pixel   The pixel to validate
 * @returns OK if valid,
 *          ERR_WRONG_PIXEL if invalid
 */
int validatePixel(char pixel) {
    int errorNo = OK;

    if ((validatePen(pixel) != OK) && (pixel != EMPTY_PIXEL)) {
        errorNo = ERR_WRONG_PIXEL;
    }

    return errorNo;
}

/* 
 * Validates the value of pixels of a string
 *
 * @param   *str    The line to validate
 * @returns OK if all pixels are valid,
 *          ERR_WRONG_PIXEL if one pixel is invalid.
 * @see     validatePixel
 */
int validatePixelLine(char *str) {
    int errorNo = OK;

    for (unsigned int i = 0; i < strlen(str); i++) {
        char pixel = str[i];

        if ((errorNo = validatePixel(pixel)) != OK) {
            break;
        }
    }

    return errorNo;
}

/*
 * Imports an canvas from stdin input
 *
 * @param   Reads from stdin
 * @returns ERR_CANVAS_TOO_HIGH if read canvas is too high,
 *          ERR_CANVAS_TOO_WIDE if read canvas is too wide,
 *          ERR_CANVAS_NON_RECTANGULAR if canvas rows aren't same length,
 *          ERR_WRONG_PIXEL if a pixel is invalid
 *  @see    validatePixelLine
 */
int importCanvas() {
    int errorNo = OK;
    unsigned int row = 0;
    char buffer[MAX_WIDTH + 2];

    while (fgetstr(buffer, MAX_WIDTH + 2, stdin) != NULL) {
        unsigned int bufferLength = strlen(buffer);
        unsigned int previousRowLength = strlen(importedCanvas[row - 1]);
        if (row > MAX_HEIGHT) {
            errorNo = ERR_CANVAS_TOO_HIGH;
            break;
        }
        if (bufferLength > MAX_WIDTH) {
            errorNo = ERR_CANVAS_TOO_WIDE;
            break;
        }
        if ((row > 0) && (bufferLength != previousRowLength)) {
            errorNo = ERR_CANVAS_NON_RECTANGULAR;
            break;
        }
        if ((errorNo = validatePixelLine(buffer)) != OK) {
            break;
        }
        strcpy(importedCanvas[row], buffer);
        row++;
    }

      return errorNo;
}

/*
 * Prints the canvas on console (stdout)
 */
void printCanvas() {
    for (unsigned int i = 0; i < canvas.height; i++) {
        for (unsigned int j = 0; j < canvas.width; j++) {
            fprintf(stdout, "%c", canvas.pixels[i][j]);
        }
        fprintf(stdout, "\n");
    }
}

/* Validates the value of the pen
 *
 * @param   pen The pen to validate
 * @returns OK if pen is valid,
 *          ERR_WITH_VALUE if the pen is invalid
 */
int validatePen(char pen) {
    int errorNo = OK;

    if (strchr(PEN_LIST, pen) == NULL) {
        errorNo = ERR_WITH_VALUE;
    } 

    return errorNo;
}

/*
 * Sets a new pen
 *
 * @param   pen The pen to set
 * @returns OK if the pen is valid,
 *          ERR_WITH_VALUE if the pen is invalid
 * @see     validatePen
 */
int setPen(char pen) {
    int errorNo = validatePen(pen);

    if (errorNo == OK) {
        canvas.pen = pen;
    }

    return errorNo;
}

/*
 * Draws a single pixel on canvas if coordinates are inside bounds of canvas
 *
 * @param   row,col The coordinates starting from top left
 */
void drawPixel(int row, int col) {
    if (isInRange(row, 0, canvas.height)) {
        if (isInRange(col, 0, canvas.width)) {
            canvas.pixels[row][col] = canvas.pen;
        }
    }
}

/*
 * Validates a row number based on canvas height
 *
 * @param   row The row to validate
 * @returns OK if the row is inside bounds of canvas,
 *          ERR_WITH_VALUE if the row is out of bounds
 */
int validateRow(int row) {
    int errorNo = OK;

    if (!isInRange(row, 0, canvas.height)) {
        errorNo = ERR_WITH_VALUE;
    }

    return errorNo;
}

/*
 * Draws a horizontal line on the width of the canvas
 *
 * @param   row The row to draw on
 * @returns OK if the row is inside bounds of canvas,
 *          ERR_WITH_VALUE if the row is out of bounds
 * @see     validateRow
 */
int drawFullHLine(int row) {
    int errorNo = validateRow(row);

    if (errorNo == OK) {
        for (unsigned int i = 0; i < canvas.width; i++) {
            drawPixel(row, i);
        }
    }

    return errorNo;
}

/*
 * Draws a horizontal line from a column to another
 *
 * @param   row         The row to draw on
 *          colBegin    The column to begin drawing
 *          colEnd      The column where to stop drawing
 * @returns OK if row is inside bounds of canvas,
 *          ERR_WITH_VALUE if row is out of bounds
 * @see     validateRow
 */
void drawHLine(int row, int colBegin, int colEnd) {
    int errorNo = validateRow(row);
    colEnd++;

    if (errorNo == OK) {
        for (int i = colBegin; i < colEnd; i++) {
            drawPixel(row, i);
        }
    }
}

/*
 * Validates a column number based on canvas size
 *
 * @param   col The col to draw on
 * @returns OK if col is inside bounds of canvas,
 *          ERR_WITH_VALUE if col is out of bounds
 */
int validateColumn(int col) {
    int errorNo = OK;

    if (!isInRange(col, 0, canvas.width)) {
        errorNo = ERR_WITH_VALUE;
    }
 
    return errorNo;
}

/*
 * Draws a vertical line on the height on the canvas
 *
 * @param   col The col to draw on
 * @returns OK if col is inside bounds of canvas,
 *          ERR_WITH_VALUE if col is out of bounds
 * @see     validateColumn
 */
int drawFullVLine(int col) {
    int errorNo = validateColumn(col);

    if (errorNo == OK) {
        for (unsigned int i = 0; i < canvas.height; i++) {
            drawPixel(i, col);
        }
    }

    return errorNo;
}

/*
 * Draws a vertical line from a row to another
 *
 * @param   col         The col to draw on
 *          rowBegin    The row to begin drawing
 *          rowEnd      The row to end drawing
 * @returns OK if col is inside bounds of canvas,
 *          ERR_WITH_VALUE if col is out of bounds
 * @see     validateColumn
 */
void drawVLine(int col, int rowBegin, int rowEnd) {
    int errorNo = validateColumn(col);
    rowEnd++;

    if (errorNo == OK) {
        for (int i = rowBegin; i < rowEnd; i++) {
            drawPixel(i, col);
        }
    }
}

/*
 * Draws a rectangle of (height * width) size starting from (row, col)
 *
 * @param   row, col        The coordinates to start drawing, from top left
 *          height          The height
 *          width           The width
 * @returns OK if height and width are greater than or equal 0,
 *          ERR_WITH_VALUE if height or width is negative
 */
int drawRectangle(int row, int col, int height, int width) {
    int errorNo = OK;
   
    if (height > 0 && width > 0) {
        int rowEnd = row + height - 1;
        int colEnd = col + width - 1;
        drawVLine(col, row, rowEnd);
        drawVLine(colEnd, row, rowEnd);
        drawHLine(row, col, colEnd);
        drawHLine(rowEnd, col, colEnd);
    } else {
        errorNo = ERR_WITH_VALUE;
    }
    
    return errorNo;
}

/*
 * Draws a segment from a point to another
 *
 * @param   row1, col1  The coordinates to start drawing, from top left
 *          row2, col2  The coordinates to stop drawing
 */
void drawSegment (int row1, int col1, int row2, int col2) {
    int dx = abs(row2 - row1);
    int sx = (row1 < row2) ? 1 : -1;
    int dy = -abs(col2 - col1);
    int sy = (col1 < col2) ? 1 : -1;
    int err = dx + dy;

    while (true) {
        drawPixel(row1, col1);
        if ((row1 == row2) && (col1 == col2)) break;
        int err2 = 2 * err;
        if (err2 >= dy) {
            err += dy;
            row1 += sx;
        }
        if (err2 <= dx) {
            err += dx;
            col1 += sy;
        }
    }
}

/*
 * Draws a circle
 *
 * @param   int, col    The coordinates of the center of the circle
 *          radius      The radius, must be greater than or equal 0
 * @returns OK if radius is greater than or equal 0,
 *          ERR_WITH_VALUE if radius is negative
 */
int drawCircle (int row, int col, int radius) {
    int errorNo = OK;

    if (radius >= 0) {
        int f = 1 - radius;
        int ddF_x = 0;
        int ddF_y = -2 * radius;
        int x = 0;
        int y = radius;
 
        drawPixel(row, col + radius);
        drawPixel(row, col - radius);
        drawPixel(row + radius, col);
        drawPixel(row - radius, col);
 
        while (x < y) {
            if (f >= 0) {
                y--;
                ddF_y += 2;
                f += ddF_y;
            }
            x++;
            ddF_x += 2;
            f += ddF_x + 1;    
            drawPixel(row + x, col + y);
            drawPixel(row - x, col + y);
            drawPixel(row + x, col - y);
            drawPixel(row - x, col - y);
            drawPixel(row + y, col + x);
            drawPixel(row - y, col + x);
            drawPixel(row + y, col - x);
            drawPixel(row - y, col - x);
        }
    } else {
        errorNo = ERR_WITH_VALUE;
    }

    return errorNo;
}

// TODO Complete function
/*
 * Parses the options and calls the functions
 *
 * @param   argc    The number of options
 *          *argv   The options
 *
 * @returns //TODO
 */
int parseOptions(int argc, char *argv[]) {
    int errorNo = OK;

    for (int i = 1; i < argc; i++) {
        //printf("ARGV[I] = %s\n", argv[i]);
        //printf("STRLEN ARGV = %li\n", strlen(argv[i]));

        char opt[strlen(argv[i])];
        strcpy(opt, argv[i]);
        
        if (argv[i+1] != NULL) {
            //printf("ARGV[I+1] = %s\n", argv[i+1]);
            //printf("STRLEN ARGV+1 = %li\n", strlen(argv[i+1]));

            char optArgs[strlen(argv[i+1])];
            strcpy(optArgs, argv[i+1]);
            //int argsValues[3];
            //parseIntArgs(optArgs, 3, argsValues);

            //printf("ARGSV0 = %d, ARGSV1 = %d, ARGSV2 = %d\n====\n",
            //        argsValues[0], argsValues[1], argsValues[2]);
        }
        
        if ((i == 1) && (strcmp(opt, OPT_NEW_CANVAS) == 0)) {
            // NEW CANVAS
            printf("NEW CANVAS !\n");
            printf("NEXT ARG = %s\n", argv[i + 1]);
        } else if (strcmp(opt, OPT_PRINT_CANVAS) == 0) {
            // PRINT CANVAS OK
            printf("PRINT CANVAS !\n");
            printCanvas();
            break;
        } else if (strcmp(opt, OPT_ENABLE_COLORS) == 0) {
            // COLORED INPUT OK
            printf("ENABLE COLORS !\n");
            colorPrint = true;
        } else if (strcmp(opt, OPT_SET_PEN) == 0) {
            printf("SET PEN !\n");
            // SET PEN
        } else if (strcmp(opt, OPT_DRAW_HLINE) == 0) {
            // DRAW HORIZONTAL LINE
        } else if (strcmp(opt, OPT_DRAW_VLINE) == 0) {
            // DRAW VERTICAL LINE
        } else if (strcmp(opt, OPT_DRAW_RECT) == 0) {
            // DRAW RECTANGLE
        } else if (strcmp(opt, OPT_DRAW_SEGMENT) == 0) {
            // DRAW SEGMENT
        } else if (strcmp(opt, OPT_DRAW_CIRCLE) == 0) {
            // DRAW CIRCLE
        } else {
            // UNRECOGNIZED OPTION
            errorNo = ERR_UNRECOGNIZED_OPTION;
            break;
        }
    }

    return errorNo;
}

// TODO Complete function
int main(int argc, char *argv[]) {
    
    //fprintf(stdout, "NEWCANVAS(11,11) = %d\n", newCanvas(11,11));
    //fprintf(stdout, "SETPEN(1) = %d\n", setPen('1'));
    //fprintf(stdout, "DRAWPIXEL(0,0) = %d\n", drawPixel(0,0));
    //fprintf(stdout, "DRAWFULLHLINE(2) = %d\n", drawFullHLine(2));
    //fprintf(stdout, "DRAWHLINE(4,1,5) = %d\n", drawHLine(4, 1, 5));
    //fprintf(stdout, "SETPEN(7) = %d\n", setPen('7'));
    //fprintf(stdout, "DRAWFULLVLINE(26) = %d\n", drawFullVLine(26));
    //fprintf(stdout, "DRAWVLINE(28,2,8) = %d\n", drawVLine(28, 2, 8));
    //fprintf(stdout, "SETPEN(5) = %d\n", setPen('5'));
    //fprintf(stdout, "DRAWRECT(1,1,15,15) = %d\n", drawRectangle(1, 1, 15, 15));
    //fprintf(stdout, "DRAWSEG(1,1,9,15) = %d\n", drawSegment(-2,6,6,-2));
    //fprintf(stdout, "DRAWCIRCLE(5,5,5) = %d\n", drawCircle(-1,-1,5));
    //printCanvas();
    //fprintf(stdout, "IMPORTCANVAS = %d\n", importCanvas());

    if (argc == 1) {
        printUsage();
    } else if (argc > 1) {
        parseOptions(argc, argv);
    }

    return 0;
}
