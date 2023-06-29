#include "bmp_io.h"
#include "img_proc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <regex.h>

// Vorläufige Version, keine Tests für ungültige Eingaben
int main(int argc, char *argv[]) {

    int version, laufzeit = 0;
    char *input, *output;
    size_t x, y, width, height, scale;

    // TODO: Wenn möglich soll das Programm sinnvolle Standardwerte definieren, sodass
    //  nicht immer alle Optionen gesetzt werden müssen.
    regex_t regex;
    int ret;
    char *pattern1 = "^-(V|B|s|w|h|f|o)$";
    ret = regcomp(&regex, pattern1, REG_EXTENDED);
    if (ret) {
        fprintf(stderr, "Fehler beim Kompilieren des Regex-Patterns\n");
        return 1;
    }

    int i = 1;
    while (i < argc) {
        char *arg = argv[i];
        printf("Argument %d: %s %s\n", i, argv[i], argv[i + 1]);

        // Check, restlichen Argumente
        ret = regexec(&regex, arg, 0, NULL, 0);
        if (!ret) {
            char option = arg[1]; // Das Zeichen nach dem Bindestrich (-)
            switch (option) {
                case 'V':
                    // -V <Zahl> -B <Zahl>
                    if (i + 1 < argc) {
                        version = atoi(argv[i + 1]);
                    }
                    i += 2;
                    break;
                case 'B':
                    if (i + 1 < argc) {
                        laufzeit = atoi(argv[i + 1]);
                    }
                    i += 2;
                    break;
                case 's':
                    // -s <Zahl>,<Zahl>
                    if (i + 2 < argc) {
                        // TODO: abfangen, dass <Zahl,Zahl> und <Zahl,_Zahl> funktioniert
                        char *token;
                        token = strtok(argv[i + 1], ",");
                        x = atoi(token);
                        token = strtok(argv[i + 1], ",");
                        y = atoi(token);
                    }
                    i += 2;
                    break;
                case 'w':
                    // -w <Zahl>
                    if (i + 1 < argc) {
                        width = atoi(argv[i + 1]);
                    }
                    i += 2;
                    break;
                case 'h':
                    // -h<Zahl>
                    if (i + 1 < argc) {
                        height = atoi(argv[i + 1]);
                    }
                    i += 2;
                    break;
                case 'f':
                    // -f <Zahl>
                    if (i + 1 < argc) {
                        scale = atoi(argv[i + 1]);
                    }
                    i += 2;
                    break;
                case 'o':
                    // -o <Dateiname>
                    if (i + 1 < argc) {
                        output = argv[i + 1];
                    }
                    i += 2;
                    break;
                case 'h' | 'help':
                    printf("%s\n",
                           "Das Programm ermöglicht die Verarbeitung von Eingabedateien mit verschiedenen Implementierungen und bietet verschiedene Optionen zur Steuerung der Verarbeitung und Ausgabe.\n"
                           "\n"
                           "Verwendung:\n"
                           "  program [Optionen] <Dateiname>\n"
                           "\n"
                           "Optionen:\n"
                           "  -V<Zahl>       Die Implementierung, die verwendet werden soll.\n"
                           "                 -V 0: Verwendet die Hauptimplementierung (Standard).\n"
                           "                 -V <Nummer>: Verwendet die spezifizierte Implementierung.\n"
                           "  -B<Zahl>       Misst und gibt die Laufzeit der angegebenen Implementierung aus.\n"
                           "                 Das optionale Argument gibt die Anzahl der Wiederholungen an.\n"
                           "  -s<Zahl>,<Zahl> Setzt den Startpunkt des Fensterausschnitts auf die Koordinaten (x, y).\n"
                           "  -w<Zahl>       Legt die Breite des Bildfensters fest.\n"
                           "  -h<Zahl>       Legt die Höhe des Bildfensters fest.\n"
                           "  -f<Zahl>       Legt den Skalierungsfaktor fest.\n"
                           "  -o<Dateiname>  Legt den Dateinamen für die Ausgabedatei fest.\n"
                           "  -h             Gibt eine Beschreibung aller Optionen und Verwendungsbeispiele aus und beendet das Programm.\n"
                           "  --help         Gibt eine Beschreibung aller Optionen und Verwendungsbeispiele aus und beendet das Programm.\n"
                           "\n"
                           "program -V 1 lena.bmp                  Verwendet die Implementierung 1 mit dem BMP-Bildformat \"lena.bmp\" (24-Bit Farbtiefe).\n"
                           "program -B2 lena.bmp                   Misst die Laufzeit der Hauptimplementierung mit 2 Wiederholungen für das BMP-Bild \"lena.bmp\".\n"
                           "program -s100,50 -w800 -h600 -f2 -lena_output.bmp lena.bmp\n"
                           "                                       Verarbeitet das BMP-Bild \"lena.bmp\" mit den angegebenen Optionen und speichert die Ausgabe in der Datei \"lena_output.bmp\"."
                    );
                    return 0;
                default:
                    // Hier fangen wir alle anderen Argumente ab
                    printf("Ungültiges Argument: %s\n", argv[optind]);
                    return -1;

            }
        } else {
            char *extension = ".bmp";
            size_t argLen = strlen(arg);
            size_t extLen = strlen(extension);

            if (argLen >= extLen && strcmp(arg + argLen - extLen, extension) == 0) {
                input = argv[i];
                i += 1;
            } else {
                printf("Fehler: Falsches Dateiformat angegeben");
                return 0;
            }
        }

    }

    // input = "/Users/till-olelohse/CLionProjects/pixelwiederholung/Implementierung/lena.bmp";



    window_test(input, output, x, y, width, height);

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
