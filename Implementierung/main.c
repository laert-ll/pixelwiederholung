#include "bmp_io.h"
#include "img_proc.h"
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <sys/time.h>


// Vorläufige Version, keine Tests für ungültige Eingaben
int main(int argc, char *argv[]) {
    // uncomment for unit test of img_proc.c
    // runtests();
    // return 0;
    bool arg_input = false, arg_v = false, arg_b = false, arg_s = false, arg_w = false, arg_h = false, arg_f = false, arg_o = false;
    int version = 0, durchlaeufe = 0;
    char *input = NULL, *output = NULL;
    size_t x = 0, y = 0, width = 0, height = 0, scale = 0;

    regex_t regex;
    int ret = 0;
    char *pattern1 = "^-(V|B|s|w|h|f|o|help).*";
    ret = regcomp(&regex, pattern1, REG_EXTENDED);
    if (ret) {
        fprintf(stderr, "Fehler beim Kompilieren des Regex-Patterns\n");
        return 1;
    }

    if (argc > 1) {
        // Check, if first argument is '--help', '-help', '-h', '--_help', '-_help', '-_h'
        if (strncmp(argv[1], "-help", 5) == 0 || strncmp(argv[1], "--help", 6) == 0 ||
            strncmp(argv[1], "-h", 2) == 0) {
            printf("%s\n",
                   "Das Programm ermöglicht die Verarbeitung von Eingabedateien mit verschiedenen Implementierungen und bietet verschiedene Optionen zur Steuerung der Verarbeitung und Ausgabe.\n"
                   "\n"
                   "Verwendung:\n"
                   "  program [Optionen] <Dateiname>\n"
                   "\n"
                   "Hinweise:"
                   " - Bitte beachten, dass zwischen Option und <Zahl> ein Leerzeichen steht.\"\n"
                   " - Es müssen nicht alle Optionen gesetzt werden. Falls jedoch eine Option gesetzt ist, muss sie auch mit einer Zahl angegeben werden.\"\n"
                   "Optionen:\n"

                   "  -V <Zahl>       Die Implementierung, die verwendet werden soll.\n"
                   "                 -V 0: Verwendet die Hauptimplementierung (Standard).\n"
                   "                 -V <Nummer>: Verwendet die spezifizierte Implementierung.\n"
                   "  -B <Zahl>       Misst und gibt die Laufzeit der angegebenen Implementierung aus.\n"
                   "                 Das optionale Argument gibt die Anzahl der Wiederholungen an.\n"
                   "  -s <Zahl>,<Zahl> Setzt den Startpunkt des Fensterausschnitts auf die Koordinaten (x, y). Falls nicht angegeben - Standartwerte: x=0; y=0.\n"
                   "  -w <Zahl>       Legt die Breite des Bildfensters fest. Falls nicht angegeben - Standartwert: 40\n"
                   "  -h <Zahl>       Legt die Höhe des Bildfensters fest. Falls nicht angegeben - Standartwert: 40\n"
                   "  -f <Zahl>       Legt den Skalierungsfaktor fest. Falls nicht angegeben - Standartwert: 1\n"
                   "  -o <Dateiname>  Legt den Dateinamen für die Ausgabedatei fest. Falls nicht angegeben - Inputdatei wird überschrieben.\n"
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
            int i = 1;
            while (i < argc) {
                char *arg = argv[i];
                // Überprüfen der restlichen Argumente
                ret = regexec(&regex, arg, 0, NULL, 0);
                if (!ret) {
                    if (strncmp(arg, "-V", 2) == 0) {
                        // -V <Zahl>
                        char *argCheck = argv[i + 1];
                        int isNumber = 1;
                        if (argCheck != NULL) {
                            for (int j = 0; argCheck[j] != '\0'; j++) {
                                if (!isdigit(argCheck[j])) {
                                    isNumber = 0;
                                    break;
                                }
                            }
                            if (isNumber) {
                                version = atoi(argv[i + 1]);
                            } else {
                                printf("Fehler: Es wurde als Versionsnummer keine Zahl übergeben.");
                                return -1;
                            }
                        } else {
                            printf("Fehler: Es wurde keine Versionsnummer übergeben.");
                            return -1;
                        }
                        arg_v = true;
                        i = i + 2;
                        continue;
                    } else if (strncmp(arg, "-B", 2) == 0) {
                        // -B <Zahl>
                        char *argCheck = argv[i + 1];
                        int isNumber = 1;
                        if (argCheck != NULL) {
                            for (int j = 0; argCheck[j] != '\0'; j++) {
                                if (!isdigit(argCheck[j])) {
                                    isNumber = 0;
                                    break;
                                }
                            }
                            if (isNumber) {
                                durchlaeufe = atoi(argv[i + 1]);
                            } else {
                                arg_b = 1;
                            }
                        }
                        i = i + 2;
                        arg_b = true;
                        continue;
                    } else if (strncmp(arg, "-s", 2) == 0) {
                        // -s <Zahl>,<Zahl>
                        if (i + 1 < argc) {
                            char *arg = argv[i + 1];
                            // Überprüfen, ob das Argument die erwartete Form "<Zahl>,<Zahl>" hat
                            char *comma = strchr(arg, ',');
                            if (comma != NULL) {
                                // Das Argument enthält ein Komma
                                *comma = '\0'; // Das Komma durch ein Nullzeichen ersetzen
                                char *endptr1, *endptr2;
                                long int value1 = strtol(arg, &endptr1, 10);
                                long int value2 = strtol(comma + 1, &endptr2, 10);

                                // Überprüfen, ob die Konvertierung erfolgreich war und ob das gesamte Argument korrekt ist
                                if (endptr1 != arg && *endptr1 == '\0' && endptr2 != (comma + 1) &&
                                    *endptr2 == '\0') {
                                    x = value1;
                                    y = value2;
                                } else {
                                    if (endptr1 == arg || *endptr1 != '\0') {
                                        printf("Fehler: Fehlerhafte Übergabe der x-Koordinate für den Startpunkt.\n");
                                    }
                                    if (endptr2 == (comma + 1) || *endptr2 != '\0') {
                                        printf("Fehler: Fehlerhafte Übergabe der y-Koordinate für den Startpunkt.\n");
                                    }
                                    return -1;
                                }
                            } else {
                                printf("Argument hat nicht das erwartete Format: %s\n", arg);
                                return -1;
                            }
                        }
                        i = i + 2;
                        arg_s = true;
                        continue;
                    } else if (strncmp(arg, "-w", 2) == 0) {
                        // -w<Zahl>
                        char *argCheck = argv[i + 1];
                        int isNumber = 1;
                        if (argCheck != NULL) {
                            for (int j = 0; argCheck[j] != '\0'; j++) {
                                if (!isdigit(argCheck[j])) {
                                    isNumber = 0;
                                    break;
                                }
                            }
                            if (isNumber) {
                                width = atoi(argv[i + 1]);
                            } else {
                                printf("Fehler: Es wurde als Bildbreite keine Zahl übergeben.");
                                return -1;
                            }
                        } else {
                            printf("Fehler: Es wurde keine Bildbreite übergeben.");
                            return -1;
                        }
                        arg_w = true;
                        i = i + 2;
                        continue;
                    } else if (strncmp(arg, "-f", 2) == 0) {
                        // -f<Zahl>
                        char *argCheck = argv[i + 1];
                        int isNumber = 1;
                        if (argCheck != NULL) {
                            for (int j = 0; argCheck[j] != '\0'; j++) {
                                if (!isdigit(argCheck[j])) {
                                    isNumber = 0;
                                    break;
                                }
                            }
                            if (isNumber) {
                                scale = atoi(argv[i + 1]);
                            } else {
                                printf("Fehler: Es wurde als Skalierungsfaktor keine Zahl übergeben.");
                                return -1;
                            }
                        } else {
                            printf("Fehler: Es wurde kein Skalierungsfaktor übergeben.");
                            return -1;
                        }
                        arg_f = true;
                        i = i + 2;
                        continue;
                    } else if (strncmp(arg, "-h", 2) == 0) {
                        // -h<Zahl>
                        char *argCheck = argv[i + 1];
                        int isNumber = 1;
                        if (argCheck != NULL) {
                            for (int j = 0; argCheck[j] != '\0'; j++) {
                                if (!isdigit(argCheck[j])) {
                                    isNumber = 0;
                                    break;
                                }
                            }
                            if (isNumber) {
                                height = atoi(argv[i + 1]);
                            } else {
                                printf("Fehler: Es wurde als Bildhöhe keine Zahl übergeben.");
                                return -1;
                            }
                        } else {
                            printf("Fehler: Es wurde keine Bildhöhe übergeben.");
                            return -1;
                        }
                        arg_h = true;
                        i = i + 2;
                        continue;
                    } else if (strncmp(arg, "-o", 2) == 0) {
                        // -o<Dateiname>
                        if (argv[i + 1] != (void *) 0) {
                            output = argv[i + 1]; // Den Teil nach "-o" zuweisen
                        } else {
                            printf("Fehler: Es wurde -o Flag gesetzt, aber keine Outputdatei angegeben.");
                        }
                        arg_o = true;
                        i = i + 2;
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
                        input = arg;
                        arg_input = true;
                    } else {
                        printf("Fehler: Falsche Übergabeparameter. Bitte '--help' aufrufen für mehr Informationen.\n");
                        return -1;
                    }
                }

                i++;
            }
        }

    }
    if (!arg_v) {
        version = 0;
    }
    if (!arg_s) {
        x = 0;
        y = 0;
    }
    if (!arg_input) {
        printf("Fehler: Keine Inputdatei angebenen oder ein falsches Dateiformat verwendet (benötigt .bmp).\n");
        return 0;
    }
    if (!arg_o) {
        printf("Fehler: Keine Outputdatei spezifiziert. Das Originalbild wird überschrieben.\n");
        return -1;
    }
    if (!arg_b) {
        durchlaeufe = 1;
    }
    if (!arg_w) {
        width = 40;
    }
    if (!arg_h) {
        height = 40;
    }
    if (!arg_f) {
        printf("Fehler: Keine Outputdatei spezifiziert. Das Originalbild wird überschrieben.\n");
        return -1;
    }
    if (width == 0) {
        printf("Fehler: Die Bildbreite muss > 0 sein.\n");
        return -1;
    }
    if (height == 0) {
        printf("Fehler: Die Bildhöhe muss > 0 sein.\n");
        return -1;
    }
    if (scale == 0) {
        printf("Fehler: Ungültige Eingabe. Der Skalierungsfaktor sollte mindestens 1 sein.\n");
        return -1;
    }

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL); // Start time

    ret = pixelwiederholung(input, output, x, y, width, height, scale, version);

    gettimeofday(&end_time, NULL); // End time

    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1e6;

    if (durchlaeufe != 0) {
        printf("Elapsed time: %.6f seconds\n", elapsed_time);
    }

    return 0;
}
