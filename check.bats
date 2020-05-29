examples_folder=examples
prog=canvascii
help_first_line="Usage: ./canvascii [-n HEIGHT,WIDTH] [-s] [-k] [-p CHAR]"

# Normal usage

@test "With no argument, shows help" {
    skip
    run ./$prog
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "$help_first_line" ]
}

@test "Creating an empty 3x2 canvas" {
    skip
    run ./$prog -n 3,2
    [ "${lines[0]}" = ".." ]
    [ "${lines[1]}" = ".." ]
    [ "${lines[2]}" = ".." ]
}

@test "Maximum allowed size is 40x80" {
    skip
    run ./$prog -s < $examples_folder/empty40x80.canvas
    [ "$status" -eq 0 ]
}

@test "Loading and prints an empty 5x8 canvas" {
    skip
    run ./$prog -s < $examples_folder/empty5x8.canvas
    [ "$status" -eq 0 ]
}

@test "Using all pens between 0 and 7" {
    skip
    run ./$prog -s < $examples_folder/all-pens5x8.canvas
    [ "$status" -eq 0 ]
}

@test "Drawing horizontal line on 5x8 canvas with option -h" {
    skip
    run ./$prog -h 2 < examples/empty5x8.canvas
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "........" ]
    [ "${lines[1]}" = "........" ]
    [ "${lines[2]}" = "77777777" ]
    [ "${lines[3]}" = "........" ]
    [ "${lines[4]}" = "........" ]
}

@test "Drawing vertical line on 5x8 canvas with option -v" {
    skip
    run ./$prog -v 3 < examples/empty5x8.canvas
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "...7...." ]
    [ "${lines[1]}" = "...7...." ]
    [ "${lines[2]}" = "...7...." ]
    [ "${lines[3]}" = "...7...." ]
    [ "${lines[4]}" = "...7...." ]
}

@test "Drawing rectangle on 5x8 canvas with option -r" {
    skip
    run ./$prog -r 1,2,3,4 < examples/empty5x8.canvas
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "........" ]
    [ "${lines[1]}" = "..7777.." ]
    [ "${lines[2]}" = "..7..7.." ]
    [ "${lines[3]}" = "..7777.." ]
    [ "${lines[4]}" = "........" ]
}

@test "Drawing line on 5x5 canvas with option -l" {
    skip
    run ./$prog -n 5,5 -l 0,0,4,4
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "7...." ]
    [ "${lines[1]}" = ".7..." ]
    [ "${lines[2]}" = "..7.." ]
    [ "${lines[3]}" = "...7." ]
    [ "${lines[4]}" = "....7" ]
}

@test "Drawing circle on 5x8 canvas with option -c" {
    skip
    run ./$prog -c 2,3,2 < examples/empty5x8.canvas
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "..777..." ]
    [ "${lines[1]}" = ".7...7.." ]
    [ "${lines[2]}" = ".7...7.." ]
    [ "${lines[3]}" = ".7...7.." ]
    [ "${lines[4]}" = "..777..." ]
}

@test "Combining multiple options" {
    skip
    run ./$prog -n 5,5 -p 1 -l 0,0,4,4 -p 7 -l 0,4,4,0 -p 2 -r 0,0,5,5
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "22222" ]
    [ "${lines[1]}" = "21.72" ]
    [ "${lines[2]}" = "2.7.2" ]
    [ "${lines[3]}" = "27.12" ]
    [ "${lines[4]}" = "22222" ]
}

@test "Drawing non diagonal segment" {
    skip
    run ./$prog -n 5,10 -l 1,1,3,8
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = ".........." ]
    [ "${lines[1]}" = ".77......." ]
    [ "${lines[2]}" = "...7777..." ]
    [ "${lines[3]}" = ".......77." ]
    [ "${lines[4]}" = ".........." ]
}

@test "Drawing large circle" {
    skip
    run ./$prog -n 11,11 -c 5,5,4
    [ "$status" -eq 0 ]
    [ "${lines[0]}"  = "..........." ]
    [ "${lines[1]}"  = "....777...." ]
    [ "${lines[2]}"  = "..77...77.." ]
    [ "${lines[3]}"  = "..7.....7.." ]
    [ "${lines[4]}"  = ".7.......7." ]
    [ "${lines[5]}"  = ".7.......7." ]
    [ "${lines[6]}"  = ".7.......7." ]
    [ "${lines[7]}"  = "..7.....7.." ]
    [ "${lines[8]}"  = "..77...77.." ]
    [ "${lines[9]}"  = "....777...." ]
    [ "${lines[10]}" = "..........." ]

}

@test "Clipping line from (1,1) to (5,8)" {
    skip
    run ./$prog -n 5,5 -l 1,1,5,8
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "....." ]
    [ "${lines[1]}" = ".7..." ]
    [ "${lines[2]}" = "..77." ]
    [ "${lines[3]}" = "....7" ]
    [ "${lines[4]}" = "....." ]
}

@test "Clipping circle of radius 3 centered at (3,3)" {
    skip
    run ./$prog -n 5,5 -c 3,3,3
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "..777" ]
    [ "${lines[1]}" = ".7..." ]
    [ "${lines[2]}" = "7...." ]
    [ "${lines[3]}" = "7...." ]
    [ "${lines[4]}" = "7...." ]
}

@test "Option -k is recognized" {
    skip
    run ./$prog -n 19,19 -p 1 -c 9,9,9 -l 3,3,15,15 -l 3,15,15,3 -k
    [ "$status" -eq 0 ]
}

# Handling errors

@test "Forbidding character # in canvas" {
    skip
    run ./$prog -s < $examples_folder/wrong-char.canvas
    [ "$status" -eq 1 ]
    [ "${lines[0]}" = "Error: wrong pixel value #" ]
    [ "${lines[1]}" = "$help_first_line" ]
}

@test "Canvas of 41 lines is too high" {
    skip
    run ./$prog -s < $examples_folder/too-high.canvas
    [ "$status" -eq 2 ]
    [ "${lines[0]}" = "Error: canvas is too high (max height: 40)" ]
    [ "${lines[1]}" = "$help_first_line" ]
}

@test "Canvas of 81 columns is too wide" {
    skip
    run ./$prog -s < $examples_folder/too-wide.canvas
    [ "$status" -eq 3 ]
    [ "${lines[0]}" = "Error: canvas is too wide (max width: 80)" ]
    [ "${lines[1]}" = "$help_first_line" ]
}

@test "Width must be uniform for all lines" {
    skip
    run ./$prog -s < $examples_folder/non-uniform-width.canvas
    [ "$status" -eq 4 ]
    [ "${lines[0]}" = "Error: canvas should be rectangular" ]
    [ "${lines[1]}" = "$help_first_line" ]
}

@test "Unrecognized option -a" {
    skip
    run ./$prog -n 5,5 -a
    [ "$status" -eq 5 ]
    [ "${lines[0]}" = "Error: unrecognized option -a" ]
    [ "${lines[1]}" = "$help_first_line" ]
}

@test "Option -n must be provided with values" {
    skip
    run ./$prog -n
    [ "$status" -eq 6 ]
    [ "${lines[0]}" = "Error: missing value with option -n" ]
    [ "${lines[1]}" = "$help_first_line" ]
}

@test "Wrong value with option -p" {
    skip
    run ./$prog -n 5,5 -p a
    [ "$status" -eq 7 ]
    [ "${lines[0]}" = "Error: incorrect value with option -p" ]
    [ "${lines[1]}" = "$help_first_line" ]
}

@test "Wrong value with option -h" {
    skip
    run ./$prog -n 3,5 -h 6
    [ "$status" -eq 7 ]
    [ "${lines[0]}" = "Error: incorrect value with option -h" ]
    [ "${lines[1]}" = "$help_first_line" ]
}

@test "Wrong value with option -v" {
    skip
    run ./$prog -n 3,5 -v 5
    [ "$status" -eq 7 ]
    [ "${lines[0]}" = "Error: incorrect value with option -v" ]
    [ "${lines[1]}" = "$help_first_line" ]
}

@test "Wrong syntax with option -n" {
    skip
    run ./$prog -n 5x5
    [ "$status" -eq 7 ]
    [ "${lines[0]}" = "Error: incorrect value with option -n" ]
    [ "${lines[1]}" = "$help_first_line" ]
}

@test "Wrong dimensions with option -n" {
    skip
    run ./$prog -n 80,40
    [ "$status" -eq 7 ]
    [ "${lines[0]}" = "Error: incorrect value with option -n" ]
    [ "${lines[1]}" = "$help_first_line" ]
}
