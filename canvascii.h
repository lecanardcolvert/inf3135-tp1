#define MIN_HEIGHT 0
#define MAX_HEIGHT 40
#define MIN_WIDTH 0
#define MAX_WIDTH 80
#define USAGE "\
Usage: ./canvascii [-n HEIGHT,WIDTH] [-s] [-k] [-p CHAR]\n\
          [-h ROW] [-v COL] [-r ROW,COL,HEIGHT,WIDTH]\n\
          [-l ROW1,COL1,ROW2,COL2] [-c ROW,COL,RADIUS]\n\
Draws on an ASCII canvas. The canvas is provided on stdin and\n\
the result is printed on stdout. The dimensions of the canvas\n\
are limited to at most 40 rows and at most 80 columns.\n\
\n\
If no argument is provided, the program prints this help and exit.\n\
\n\
Canvas options:\n\
  -n HEIGHT,WIDTH           Creates a new empty canvas of HEIGHT rows\n\
                            and WIDTH columns. Must be used as first option.\n\
                            Ignores stdin.\n\
  -s                        Shows the canvas and exit.\n\
  -k                        Enables colored output. Replaces characters\n\
                            between 0 and 9 by their corresponding ANSI\n\
                            colors:\n\
                              0: black  1: red      2: green  3: yellow\n\
                              4: blue   5: magenta  6: cyan   7: white\n\
\n\
Drawing options:\n\
  -p CHAR                   Sets the pen to CHAR. Allowed pens are\n\
                            0, 1, 2, 3, 4, 5, 6 or 7. Default pen\n\
                            is 7.\n\
  -h ROW                    Draws an horizontal line on row ROW.\n\
  -v COL                    Draws a vertical line on column COL.\n\
  -r ROW,COL,HEIGHT,WIDTH   Draws a rectangle of dimension HEIGHTxWIDTH\n\
                            with top left corner at (ROW,COL).\n\
  -l ROW1,COL1,ROW2,COL2    Draws a discrete segment from (ROW1,COL1) to\n\
                            (ROW2,COL2) with Bresenham's algorithm.\n\
  -c ROW,COL,RADIUS         Draws a circle centered at (ROW,COL) of\n\
                            radius RADIUS with the midpoint algorithm.\n\
"

struct Canvas {
    char pixels[MAX_HEIGHT][MAX_WIDTH]; // A matrix of pixels
    unsigned int height;                // Its height
    unsigned int width;                 // Its width
    int pen;                            // The character we are drawing with
    bool initialized;                   // To determine if it's empty
};

const char EMPTY_PIXEL = '.';
const int PEN_SIZE = 8;
const int PEN_LIST[8] = {0, 1, 2, 3, 4, 5, 6, 7};

enum error {
    ERR_BAD_INPUT              = -1,// For functions that return a value
    OK                         = 0, // Everything is ok
    ERR_WRONG_PIXEL            = 1, // Wrong pixel value in canvas
    ERR_CANVAS_TOO_HIGH        = 2, // Canvas is too high
    ERR_CANVAS_TOO_WIDE        = 3, // Canvas is too wide
    ERR_CANVAS_NON_RECTANGULAR = 4, // Canvas is non rectangular
    ERR_UNRECOGNIZED_OPTION    = 5, // Unrecognized option
    ERR_MISSING_VALUE          = 6, // Option with missing value
    ERR_WITH_VALUE             = 7  // Problem with value
};

const char *ERROR_MSG[8] = {
    "Everything is OK\0",
    "Error: wrong pixel value #\0",
    "Error: canvas is too high (max height: 40)\0",
    "Error: canvas is too wide (max width: 80)\0",
    "Error: canvas should be rectangular\0",
    "Error: unrecognized option \0",
    "Error: missing value with option \0",
    "Error: incorrect value with option \0"
};

struct Options {
    const char option;
    const int noArgs;
};

const int NO_ARGUMENT_MAX = 4;
const char OPTION_PREFIX = '-';
const int OPTIONS_SIZE = 9;
const struct Options OPTIONS[] = {
    {'k', 0}, {'s', 0}, {'h', 1}, {'p', 1}, {'v', 1}, {'n', 2}, {'c', 3},
    {'l', 4}, {'r', 4}
};

void printUsage();
void closeProgram(int errorNo, char *detail);
int min(int int1, int int2);
int max(int int1, int int2);
bool isInRange(int value, int lowerLimit, int upperLimit);
int validateStrToInt(char str[]);
int strToInt(char str[]);
void fillCanvas();
int validateCanvasSize(int height, int width);
void newCanvas(int height, int width);
int validatePixel(char pixel);
int validatePixelLine(char *str);
char *fgetstr(char *string, int n, FILE *stream);
void importCanvas();
void enableColorPrint();
void printCanvas();
int validatePen(int pen);
void setPen(int pen);
int drawPixel(int row, int col);
int validateRow(int row);
void drawFullHLine(int row);
void drawHLIne(int row, int colBegin, int colEnd);
int validateColumn(int col);
void drawFullVLine(int col);
void drawVLine(int col, int rowBegin, int rowEnd);
void drawRectangle(int row, int col, int height, int width);
void drawSegment(int row1, int col1, int row2, int col2);
void drawCircle(int row, int col, int radius);
// TODO Complete the rest of the functions
