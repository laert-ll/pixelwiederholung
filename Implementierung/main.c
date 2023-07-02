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
    // TODO: wenn hinter -h eine Zahl kommt als datei, also zB -h 4123.bmp
    // TODO: tests edge cases
    // TODO: handling von Leerzeichen
    regex_t regex;
    int ret;
    char *pattern1 = "^-(V|B|s|w|h|f|o|help)$";
    ret = regcomp(&regex, pattern1, REG_EXTENDED);
    if (ret) {
        fprintf(stderr, "Fehler beim Kompilieren des Regex-Patterns\n");
        return 1;
    }

    int i = 1;
    while (i < argc) {
        char *arg = argv[i];
        // Check, restlichen Argumente
        ret = regexec(&regex, arg, 0, NULL, 0);
        if (!ret) {
            if (strcmp(arg, "-V") == 0) {
                // -V <Zahl> -B <Zahl>
                if (i + 1 < argc) {
                    version = atoi(argv[i + 1]);
                }
                i += 2;
                continue;
            } else if (strcmp(arg, "-B") == 0) {
                if (i + 1 < argc) {
                    laufzeit = atoi(argv[i + 1]);
                }
                i += 2;
                continue;
            } else if (strcmp(arg, "-s") == 0) {
                // TODO: handling Leerzeichen nach Komma
                // -s <Zahl>,<Zahl>
                if (i + 2 < argc) {
                    char *token = strtok(arg + 3, ",");
                    if (token != NULL) {
                        x = atoi(token);
                    }
                    token = strtok(NULL, ",");
                    if (token != NULL) {
                        y = atoi(token);
                    }
                    i += 2;
                    continue;
                }
            } else if (strcmp(arg, "-w") == 0) {
                // -w <Zahl>
                if (i + 1 < argc) {
                    width = atoi(argv[i + 1]);
                }
                i += 2;
                continue;
            } else if (strcmp(arg, "-help") == 0 || strcmp(arg, "--help") == 0) {
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
            } else if (strncmp(arg, "-h", 2) == 0) {
                char *nextArg = argv[i + 1];
                if (nextArg[0] == '\0') {
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
                } else {
                    if (i + 1 < argc) {
                        height = atoi(argv[i + 1]);
                    }
                    i += 2;
                    continue;
                }
            } else if (strcmp(arg, "-f") == 0) {
                // -f <Zahl>
                if (i + 1 < argc) {
                    scale = atoi(argv[i + 1]);
                }
                i += 2;
                continue;
            } else if (strcmp(arg, "-o") == 0) {
                // -o <Dateiname>
                if (i + 1 < argc) {
                    output = argv[i + 1];
                }
                i += 2;
                continue;
            } else {
                // Ungültiges Argument
                printf("Ungültiges Argument: %s\n", arg);
                return -1;
            }
        } else {
            char *extension = ".bmp";
            size_t argLen = strlen(arg);
            size_t extLen = strlen(extension);

            if (argLen >= extLen && strcmp(arg + argLen - extLen, extension) == 0) {
                input = argv[i];
                i += 1;
                continue;
            } else {
                printf("Fehler: Falsches Dateiformat angegeben");
                return 0;
            }
        }

    }

     input = "/Users/till-olelohse/CLionProjects/pixelwiederholung/Implementierung/4_pixels.bmp";
     output = "/Users/till-olelohse/CLionProjects/pixelwiederholung/Implementierung/4_pixels_output.bmp";

    window_test(input, output, x, y, width, height, scale);

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
