#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "canvascii.h"

struct Canvas canvas;
const char DEFAULT_PIXEL = '.';
bool coloredOutput = false;

// Prints the manual on the console
void printUsage() {
    fprintf(stdout, "%s", USAGE);
}

// Prints the error message, the manual, then closes the program
void closeProgram(int errorNo) {
    fprintf(stderr, "%s", errorMsg[errorNo]);
    fprintf(stderr, "%s", USAGE);
    // EXIT ???
}

// Returns OK (0) if the string is a int, otherwise ERR_BAD_INPUT (7)
int validateStrToInt(char str[]) {
    int errorNo = OK;
    int strLength = strlen(str);

    for (unsigned int i = 0; i < strLength; i++) {
        if (!isdigit(str[i])) {
            errorNo = ERR_BAD_INPUT;
        }
    }

    return errorNo;
}

// Returns the minimum value between two int
int min(int int1, int int2) {
    return (int1 < int2) ? int1 : int2;
}

// Returns the maximum value between two int
int max(int int1, int int2) {
    return (int1 > int2) ? int1 : int2;
}

// Returns TRUE when the value is in range of lowerLimit and upperLimit (inclusive)
bool isInRange(int value, int lowerLimit, int upperLimit) {
    return (lowerLimit <= value && value <= upperLimit);
}

// Converts a string to int
// Returns an int if all characters are numeric, or else -1
int strToInt(char str[]) {
    int errorNo = validateStrToInt(str);
    unsigned int value = 0;

    if (errorNo != ERR_BAD_INPUT) {
        value = atoi(str); 
    }

    return value;
}

// Fills the canvas with the default pixel
void fillCanvas() {
    for (unsigned int i = 0; i < canvas.height; i++) {
        for (unsigned int j = 0; j < canvas.width; j++) {
            canvas.pixels[i][j] = DEFAULT_PIXEL;
        }
    }
}

// Validates the size of a new canvas
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

// Creates a new canvas
int createCanvas(unsigned int height, unsigned int width) {
    int errorNo = validateCanvasSize(height, width);

    if (errorNo == OK) {
        canvas.height = height;
        canvas.width = width;
        canvas.pen = 7;
        fillCanvas();
    }

    return errorNo;
}

// Prints the canvas on the console
void printCanvas() {
    for (unsigned int i = 0; i < canvas.height; i++) {
        for (unsigned int j = 0; j < canvas.width; j++) {
            fprintf(stdout, "%c", canvas.pixels[i][j]);
        }
        fprintf(stdout, "\n");
    }
}

// Validates the value of the pen
int validatePen(char pen) {
    int errorNo = OK;

    if (strchr(penList, pen) == NULL) {
        errorNo = ERR_WITH_VALUE;
    } 

    return errorNo;
}

// Sets a new pen
int setPen(char pen) {
    int errorNo = validatePen(pen);

    if (errorNo == OK) {
        canvas.pen = pen;
    }

    return errorNo;
}

// Draws a single pixel on canvas
int drawPixel(int row, int col) {
    int errorNo = OK;

    if (isInRange(row, 0, canvas.height)) {
        if (isInRange(col, 0, canvas.width)) {
            canvas.pixels[row][col] = canvas.pen;
        }
    }

    // TODO Return error or not ?
    return errorNo;
}

// Validates a row number based on canvas size
int validateRow(int row) {
    int errorNo = OK;

    if ((row < MIN_HEIGHT) || (row > MAX_HEIGHT)) {
        errorNo = ERR_WITH_VALUE;
    } else if (row > canvas.height) {
        errorNo = ERR_WITH_VALUE;
    }

    // TODO Remove MIN_HEIGHT because unsigned int ?
    return errorNo;
}

// Draws a horizontal line on the width of the canvas
int drawFullHLine(int row) {
    int errorNo = validateRow(row);

    if (errorNo == OK) {
        for (unsigned int i = 0; i < canvas.width; i++) {
            drawPixel(row, i);
        }
    }

    return errorNo;
}

// Draws a horizontal line from a column to another
int drawHLine(int row, int colBegin, int colEnd) {
    int errorNo = validateRow(row);
    colEnd++;

    if (errorNo == OK) {
        for (unsigned int i = colBegin; i < colEnd; i++) {
            drawPixel(row, i);
        }
    }

    // TODO Return error or not ?
    return errorNo;
}

// Validates a column number based on canvas size
int validateColumn(int col) {
    int errorNo = OK;

    if ((col < MIN_WIDTH) || (col > MAX_WIDTH)) {
        errorNo = ERR_WITH_VALUE;
    } else if (col > canvas.width) { 
        errorNo = ERR_WITH_VALUE;
    }

    // TODO Remove MIN_WIDTH because unsigned int ? 
    return errorNo;
}

// Draws a vertical line on the height on the canvas
int drawFullVLine(int col) {
    int errorNo = validateColumn(col);

    if (errorNo == OK) {
        for (unsigned int i = 0; i < canvas.height; i++) {
            drawPixel(i, col);
        }
    }

    return errorNo;
}

// Draws a vertical line from a row to another
int drawVLine(int col, int rowBegin, int rowEnd) {
    int errorNo = validateColumn(col);
    rowEnd++;

    if (errorNo == OK) {
        for (int i = rowBegin; i < rowEnd; i++) {
            drawPixel(i, col);
        }
    }

    return errorNo;
}

// Draws a rectangle of (height * width) size starting from (row, col)
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
    
    // TODO Return error if col or row is bad
    return errorNo;
}

// Draws a segment from (row1, col1) to (row2, col2)
int drawSegment (int row1, int col1, int row2, int col2) {
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
void parseArguments(int argc, char* argv[]) {

}
*/

/*
 * SI ARGUMENTS[] = 1 :
 *  AFFICHER LE MANUEL D'UTILISATION
 *
 * SI ARGUMENTS[] > 1 :
 *  CONTINUER CI-DESSOUS...
 *
 *      EST-CE QUE ARGUMENT[i] == 
 *
 * GESTION DES ERREURS :
 *  RETURN NULL
 *
 */

int main(int argc, char* argv[]) {
    
    fprintf(stdout, "CREATECANVAS(10,10) = %d\n", createCanvas(10,10));

    fprintf(stdout, "SETPEN(1) = %d\n", setPen('1'));

    //fprintf(stdout, "DRAWFULLHLINE(2) = %d\n", drawFullHLine(2));

    //fprintf(stdout, "DRAWHLINE(4,1,5) = %d\n", drawHLine(4, 1, 5));

    //fprintf(stdout, "SETPEN(7) = %d\n", setPen('7'));

    //fprintf(stdout, "DRAWFULLVLINE(26) = %d\n", drawFullVLine(26));

    //fprintf(stdout, "DRAWVLINE(28,2,8) = %d\n", drawVLine(28, 2, 8));

    //fprintf(stdout, "SETPEN(5) = %d\n", setPen('5'));

    fprintf(stdout, "DRAWRECT(1,1,15,15) = %d\n", drawRectangle(1, 1, 15, 15));

    //fprintf(stdout, "DRAWSEG(1,1,9,15) = %d\n", drawSegment(1,1,9,15));

    printCanvas();

    /*
    if (argc == 1) {
        printUsage();
    } else if (argc > 1) {
        parseArguments(argc, argv);
    }
    */


    /*
    printf("argc = %d\n", argc);
    for (unsigned int i = 0; i < argc; ++i) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }
    */

    return 0;
}

