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
bool colorPrint = false;

/*
 * Prints the manual on the console.
 *
 */
void printUsage() {
    fprintf(stdout, "%s", USAGE);
}

/* 
 * Terminates the program
 * Prints the error message if there is one specified
 * Also prints the detail of an error if there is one
 *
 * @param   errorNo Error number
 *          *detail Details about the error
 */
void closeProgram(int errorNo, char *detail) {
    if (errorNo == OK) {
        printCanvas();
    } else {
        fprintf(stderr, "%s", ERROR_MSG[errorNo]);
        if (detail != NULL) {
            fprintf(stderr, "%s", detail);
        }
        fprintf(stderr, "\n%s", USAGE);

    }
    exit(errorNo);
}

/*
 * Returns the minimum between two integers
 *
 * @param   int1  First int to compare
 *          int2  Second int to compare
 * @returns An int which is the minimum
 */
int min(int int1, int int2) {
    return (int1 < int2) ? int1 : int2;
}

/*
 * Returns the maximum between two integers
 *
 * @param   int1    First int to compare
 *          int2    Second int to compare
 * @returns An int which is the maximum
 */
int max(int int1, int int2) {
    return (int1 > int2) ? int1 : int2;
}

/*
 * Tells if a number is in the range (inclusive) of two numbers
 *
 * @param   lowerLimit  Lower boundary
 *          upperLimit  Upper boundary
 * @returns TRUE if the number is between both inclusive boundaries,
 *          FALSE if not
 */
bool isInRange(int value, int lowerLimit, int upperLimit) {
    return (lowerLimit <= value && value <= upperLimit);
}

/*
 * Validates if a string is a number
 *
 * @param   *str    Input string
 * @returns OK if string is a number,
 *          ERR_BAD_INPUT if not
 */
int validateStrToInt(char *str) {
    int errorNo = OK;

    if (str != NULL) {
        unsigned int strLength = strlen(str);

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
 * @param   *str    Input string
 * @returns Converted int if the validation is succesful,
 *          ERR_BAD_INPUT if there is an error
 * @see     validateStrToInt
 */
int strToInt(char *str) {
    int errorNo = validateStrToInt(str);
    unsigned int value = 0;

    if (errorNo != ERR_BAD_INPUT) {
        value = atoi(str); 
    }

    return value;
}

/*
 * Fills a new canvas with an empty pixel
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
 * @param   height  Height
 *          width   Width
 * @returns OK if no error,
 *          ERR_CANVAS_TOO_HIGH if too high,
 *          ERR_CANVAS_TOO_WIDE if too wide,
 *          ERR_WITH_VALUE if too small
 */
int validateCanvasSize(int height, int width) {
    int errorNo = OK;

    if (height > MAX_HEIGHT) {
        errorNo = ERR_WITH_VALUE;
    } else if (width > MAX_WIDTH) {
        errorNo = ERR_WITH_VALUE;
    } else if ((width < MIN_WIDTH) || (height < MIN_HEIGHT)) {
        errorNo = ERR_WITH_VALUE;
    }

    return errorNo;
}

/* 
 * Creates a new empty canvas and fills it with empty pixels.
 * The program terminates if the canvas is too big
 *
 * @param   height  Height
 *          width   Width
 */
void newCanvas(int height, int width) {
    int errorNo = validateCanvasSize(height, width);

    if (errorNo == OK) {
        canvas.height = height;
        canvas.width = width;
        canvas.pen = 7;
        canvas.initialized = true;
    } else {
        closeProgram(errorNo, "-n");
    }
}

/*
 * Validates the value of a pixel
 * Must be either a valid pen or the empty pixel
 *
 * @param   pixel   Pixel to validate
 * @returns OK if pixel is valid,
 *          ERR_WRONG_PIXEL if invalid
 */
int validatePixel(char pixel) {
    int errorNo = ERR_WRONG_PIXEL;
    int pixelInt = pixel - '0';

    if ((pixel == EMPTY_PIXEL) || (validatePen(pixelInt) == OK)) {
        errorNo = OK;
    }

    return errorNo;
}

/* 
 * Validates the value of pixels of a string
 *
 * @param   *str    Line to validate
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
 * fgetstr() - mimics behavior of fgets(), but 
 * removes new-line character at end of line if it exists
 *
 * @param   *string String where to store characters
 *          n       Maximum of characters to be read
 *          *STREAM Pointer to FILE where to read stream from (stdin)
 * @returns String read from the stream, until EOF is encountered
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
 * Imports an canvas from stdin input
 * The program terminates if the canvas is too big, not rectangular, or
 * if there is an invalid pixel
 *
 * @param   Reads from stdin
 *
 * @see     validatePixelLine
 */
void importCanvas() {
    int errorNo = OK;
    unsigned int row = 1;
    unsigned int col = 0;
    char buffer[MAX_WIDTH + 2];

    while (fgetstr(buffer, MAX_WIDTH + 2, stdin) != NULL) {
        col = strlen(buffer);

        if (row > MAX_HEIGHT) {
            errorNo = ERR_CANVAS_TOO_HIGH;
            break;
        }
        if (col > MAX_WIDTH) {
            errorNo = ERR_CANVAS_TOO_WIDE;
            break;
        }
        if ((row > 1) && (col != strlen(canvas.pixels[row - 2]))) {
            errorNo = ERR_CANVAS_NON_RECTANGULAR;
            break;
        }
        if ((errorNo = validatePixelLine(buffer)) != OK) {
            break;
        }
        strcpy(canvas.pixels[row - 1], buffer);
        row++;
    }

    if (errorNo == OK) {
        newCanvas(row - 1, col);
    } else {
        closeProgram(errorNo, NULL);
    }
}

/*
 * Gets the color code for a pen
 *
 * @param   pen     Pen to look for
 *          *color  String where to store the color code
 */
void getPenColor(char pen, char *color) {
    for (int i = 0; i < PENS_SIZE; i++) {
        if (pen - '0' == PENS[i].pen) {
            strcpy(color, PENS[i].color);
        }
    }
}

/*
 * Enables printing in color mode
 */
void enableColorPrint() {
    colorPrint = true;
}

/*
 * Prints the canvas on console (stdout) with colors
 *
 */
void colorPrintCanvas() {
    for (unsigned int i = 0; i < canvas.height; i++) {
        for (unsigned int j = 0; j < canvas.width; j++) {
            char pixel = canvas.pixels[i][j];
            char color[10];

	        if (pixel != EMPTY_PIXEL) {
            	getPenColor(pixel, color);
    		} else {
            	strcpy(color, RESET_COLOR);
	            pixel = ' ';
            }
            fprintf(stdout, "%s", color);
            fprintf(stdout, "%c", pixel);
        }
        fprintf(stdout, "%s", RESET_COLOR); 
        fprintf(stdout, "\n");
    }
    printf("%s", RESET_COLOR);
}

/*
 * Prints the canvas on console (stdout) without colors
 */
void normalPrintCanvas() {
    for (unsigned int i = 0; i < canvas.height; i++) {
        for (unsigned int j = 0; j < canvas.width; j++) {
            fprintf(stdout, "%c", canvas.pixels[i][j]);
        }
        fprintf(stdout, "\n");
    }
}

/*
 * Calls the normal printing mode or color printing mode
 */
void printCanvas() {
    if (colorPrint == true) {
        colorPrintCanvas();
    } else {
        normalPrintCanvas();
    }
}

/* Validates the value of the pen
 *
 * @param   pen Number of pen to validate
 * @returns OK if pen is valid,
 *          ERR_WITH_VALUE if the pen is invalid
 */
int validatePen(int pen) {
    int errorNo = ERR_WITH_VALUE;

    for (int i = 0; i < PENS_SIZE; i++) {
        if (pen == PENS[i].pen) {
            errorNo = OK;
            break;
        }
    }

    return errorNo;
}

/*
 * Sets a new pen
 * The program terminates if an invalid pen is set
 *
 * @param   pen The pen to set
 * @see     validatePen
 */
void setPen(int pen) {
    int errorNo = validatePen(pen);

    if (errorNo == OK) {
        canvas.pen = pen;
    } else {
        closeProgram(errorNo, NULL);
    }
}

/*
 * Draws a single pixel on canvas
 *
 * @param   row,col Coordinates starting from top left
 * @returns OK if pixel is inside bounds of canvas,
 *          ERR_WITH_VALUE if pixel is out of bounds
 */
int drawPixel(int row, int col) {
    int errorNo = OK;

    if (isInRange(row, 0, canvas.height) && isInRange(col, 0, canvas.width)) {
        canvas.pixels[row][col] = canvas.pen + '0';
    } else {
        errorNo = ERR_WITH_VALUE;
    }

    return errorNo;
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
 * The program terminates if the row is out of bounds
 *
 * @param   row Row to draw on
 * @see     validateRow
 */
void drawFullHLine(int row) {
    int errorNo = validateRow(row);

    if (errorNo == OK) {
        for (unsigned int i = 0; i < canvas.width; i++) {
            drawPixel(row, i);
        }
    } else {
        closeProgram(errorNo, "-h");
    }
}

/*
 * Draws a horizontal line from a column to another
 *
 * @param   row         Row to draw on
 *          colBegin    Column to begin drawing
 *          colEnd      column where to stop drawing
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
 * @param   col Column to draw on
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
 * The program terminates if the col is out of bounds
 *
 * @param   col Column to draw on
 * @see     validateColumn
 */
void drawFullVLine(int col) {
    int errorNo = validateColumn(col);

    if (errorNo == OK) {
        for (unsigned int i = 0; i < canvas.height; i++) {
            drawPixel(i, col);
        }
    } else {
        closeProgram(errorNo, "-v");
    }
}

/*
 * Draws a vertical line from a row to another
 *
 * @param   col         Column to draw on
 *          rowBegin    Row to begin drawing
 *          rowEnd      Row to end drawing
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
 * The program terminates if a negative height or width is set
 *
 * @param   row, col        Coordinates to start drawing, from top left
 *          height          Height
 *          width           Width
 */
void drawRectangle(int row, int col, int height, int width) {
    if (height > 0 && width > 0) {
        int rowEnd = row + height - 1;
        int colEnd = col + width - 1;
        drawVLine(col, row, rowEnd);
        drawVLine(colEnd, row, rowEnd);
        drawHLine(row, col, colEnd);
        drawHLine(rowEnd, col, colEnd);
    } else {
        closeProgram(ERR_WITH_VALUE, "-r");
    }
}

/*
 * Draws a segment from a point to another
 *
 * @param   row1, col1  Coordinates to start drawing, from top left
 *          row2, col2  Coordinates to stop drawing
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
 * The program terminates if a negative radius is set
 *
 * @param   int, col    Coordinates of the center of the circle
 *          radius      Radius that must be >= 0
 */
void drawCircle (int row, int col, int radius) {
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
        closeProgram(ERR_WITH_VALUE, "-c");
    }
}

/*
 * Parses a string of 1 to 4 numbers separated by commas into an array
 * The program terminates if a different amount of values is stored
 *
 * @params  *str        String to read
 *          noValues    Number of values to expect
 *          *values     Array to store the parsed values
 */
void parseArgument(const char *str, int noValues, int *values) {
    char trailing;
    int noParsed;

    if ((str != NULL) && (values != NULL)) {
        switch (noValues) {
            case 1 :
                noParsed = sscanf(str, "%d%c", &values[0], &trailing);
                break;
            case 2 :
                noParsed = sscanf(str, "%d,%d%c", 
                    &values[0], &values[1], &trailing);
                break;
            case 3 :
                noParsed = sscanf(str, "%d,%d,%d%c,", 
                    &values[0], &values[1], &values[2], &trailing);
                break;
            case 4 :
                noParsed = sscanf(str, "%d,%d,%d,%d%c",
                    &values[0], &values[1], &values[2], &values[3], &trailing);
                break;
            default :
                break;
        }
    }

    if (noParsed != noValues) {
        closeProgram(ERR_WITH_VALUE, NULL);
    }
}

/*
 * Returns the number of characters of an option starting by - (ex. -%c)
 * Stores first character of the passed option
 *
 * @param   *strOpt String of the option (-%c format)
 *          *opt    Character to store
 * @returns The number of characters parsed after (-) character
 *          If the number > 1 then there must be an error
 */
int parseOption(const char *strOpt, char *opt) {
    char trailing;
    int noParsed;

    if (strOpt != NULL) {
        noParsed = sscanf(strOpt, "-%c%c", opt, &trailing);
    }

    return noParsed;
}

/*
 * Validates if a specified option exists
 * Stores character of the passed option
 *
 * @param       *strOpt String of the option (-%c format)
 *              *opt    Character to store
 * @returns     OK if specified option exists,
 *              ERR_UNRECOGNIZED_OPTION if not
 */
int validateOption(const char *strOpt, char *opt) {
    int errorNo = ERR_UNRECOGNIZED_OPTION;

    if ((parseOption(strOpt, opt) == 1)) {
        for (int i = 0; i < OPTIONS_SIZE; i++) {
           if (*opt == OPTIONS[i].option) {
                errorNo = OK;
                break;
           }
       }
    }

    return errorNo;
}

/*
 * Returns the number of arguments required for an option
 * Stores character of the passed option
 *
 * @param   *strOpt String of the option (-%c format)
 *          *opt    Character to store
 * @returns The number of arguments required for an option
 * @see     validateOption
 */
int getNoArgsRequired(const char *strOpt, char *opt) {
    int errorNo = validateOption(strOpt, opt);
    int noArgs;

    for (int i = 0; i < OPTIONS_SIZE; i++) {
        if (*opt == OPTIONS[i].option) {
            noArgs = OPTIONS[i].noArgs;
            errorNo = OK;
        }
    }

    if (errorNo == ERR_UNRECOGNIZED_OPTION) {
        char error[strlen(strOpt)];
        strcpy(error, strOpt);
        closeProgram(errorNo, error);
    }

    return noArgs;
}

/*
 * Calls the functions based on a character specified in the program arguments
 *
 * @param   opt     Option (function) wanted
 *          optp    Position of option in program arguments
 *          *args   Array for the arguments of the options
 */
void callFunction(const char opt, const int optp, int *args) {
    switch (opt) {
        case 'k' :
            enableColorPrint();
            break;
        case 's' :
            closeProgram(OK, NULL);
            break;
        case 'h' :
            drawFullHLine(args[0]);
            break;
        case 'p' :
            setPen(args[0]);
            break;
        case 'v' :
            drawFullVLine(args[0]);
            break;
        case 'n' :
            if (optp == 1) {
                // Must be first option in program arguments to run
                newCanvas(args[0], args[1]);
                fillCanvas();
            }
            break;
        case 'c' :
            drawCircle(args[0], args[1], args[2]);
            break;
        case 'l' :
            drawSegment(args[0], args[1], args[2], args[3]);
            break;
        case 'r' :
            drawRectangle(args[0], args[1], args[2], args[3]);
            break;
        default :
            closeProgram(ERR_UNRECOGNIZED_OPTION, NULL);
            break;
    }
}

/*
 * Calls the functions based on specified arguments when running the program
 *
 * @param   argc    Number of arguments passed on the program
 *          **argv  Array of arguments
 */
void parseProgramArguments(const int argc, char* argv[]) {
   for (int i = 1; i < argc; i++) {
        char opt[1];
        int noArgsRequired = getNoArgsRequired(argv[i], opt);
        int args[NO_ARGUMENT_MAX];
        //int noArgs;

        if ((i == 1) && opt[0] != 'n') {
            importCanvas();
        }

        if (noArgsRequired == 0) {
            callFunction(*opt, i, args);
        } else {
            parseArgument(argv[i + 1], noArgsRequired, args);
            callFunction(*opt, i, args);
            i++;
        }
    }
}

int main(int argc, char *argv[]) {
    canvas.initialized = false;

    if (argc == 1) {
        printUsage();
    } else if (argc > 1) {
        parseProgramArguments(argc, argv);
    }

    closeProgram(OK, NULL);
}
