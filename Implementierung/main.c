#include "bmp_io.h"
#include "img_proc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

// Vorläufige Version, keine Tests für ungültige Eingaben
int main(int argc, char *argv[]) {

    int opt;
    int version, laufzeit = 0;
    char *input, *output;
    size_t x, y, width, height, scale;

    // -h, --help
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        printf("%s\n", "help :'(");
        return 0;
    }

    // -V <Zahl> -B <Zahl>
    // <Dateiname> (ist nicht hier zugewiesen)
    // -s <Zahl>,<Zahl> 
    // -w <Zahl> -h <Zahl> 
    // -f <Zahl> 
    // -o <Dateiname>
    while((opt = getopt(argc, argv, "V:B:s:w:h:f:o:")) != -1) {
        switch(opt) {
            case 'V':
                version = atoi(optarg);
                break;
            case 'B':
                laufzeit = atoi(optarg);
                break;
            case 's':
                x = atoi(strtok(optarg, ","));
                y = atoi(strtok(NULL, ","));
                break;
            case 'w':
                width = atoi(optarg);
                break;
            case 'h':
                height = atoi(optarg);
                break;
            case 'f':
                scale = atoi(optarg);
                break;
            case 'o':
                output = optarg;
                break;
            default:
                return -1;
        }
    }

    // input Dateiname
    input = argv[optind];

    read_header(input);

    // testing
    printf("V: %d\n", version);
    printf("B: %d\n", laufzeit);
    printf("i: %s\n", input);
    printf("x: %ld\n", x);
    printf("y: %ld\n", y);
    printf("w: %ld\n", width);
    printf("h: %ld\n", height);
    printf("f: %ld\n", scale);
    printf("o: %s\n", output);

    return 0;
}
