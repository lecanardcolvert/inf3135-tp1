#define MIN_HEIGHT 0
#define MAX_HEIGHT 40
#define MIN_WIDTH 0
#define MAX_WIDTH 80
#define USAGE "\
Usage: %s [-n HEIGHT,WIDTH] [-s] [-k] [-p CHAR]\n\
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
    char pen;                           // The character we are drawing with
};

const char EMPTY_PIXEL = '.';
const char PEN_LIST[8] = "01234567";

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
    "Everything is OK\n\0",
    "Wrong pixel value in canvas\n\0",
    "Canvas is too high\n\0",
    "Canvas is too wide\n\0",
    "Canvas is non rectangular\n\0",
    "Unrecognized option\n\0",
    "Option with missing value\n\0",
    "Problem with value\n\0"
};

const char *OPT_NEW_CANVAS = "-n";
const char *OPT_PRINT_CANVAS = "-s";
const char *OPT_ENABLE_COLORS = "-k";
const char *OPT_SET_PEN = "-p";
const char *OPT_DRAW_HLINE = "-h";
const char *OPT_DRAW_VLINE = "-v";
const char *OPT_DRAW_RECT = "-r";
const char *OPT_DRAW_SEGMENT = "-l";
const char *OPT_DRAW_CIRCLE = "-c";

int validateStrToInt(char str[]);
int strToInt(char str[]);
char *fgetstr(char *string, int n, FILE *stream);

int min(int int1, int int2);
int max(int int1, int int2);
bool isInRange(int value, int lowerLimit, int upperLimit);

void fillCanvas();
int validateCanvasSize(int height, int width);
int newCanvas(int height, int width);
int validatePixel(char pixel);
int importCanvas();
void printCanvas();
int validatePen(char pen);
int setPen(char pen);
void drawPixel(int row, int col);
int validateRow(int row);
int drawFullHLIne(int row);
void drawHLIne(int row, int colBegin, int colEnd);
int validateColumn(int col);
int drawFullVLine(int col);
void drawVLine(int col, int rowBegin, int rowEnd);
int drawRectangle(int row, int col, int height, int width);
void drawSegment(int row1, int col1, int row2, int col2);
int drawCircle(int row, int col, int radius);

int parseArguments(int argc, char *argv[]);
