#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// metadata
#define PROG    "mywc"

// msgs
#define USAGE   "Usage: %s <OPTION> <OPERAND>\n"
#define NOFILE  "%s: No such file or directory '%s'"
#define MISSING "%s: Missing file operand\n%s"
#define TOOMANY "%s: Too many arguments\n%s"
#define UNRECOGNISED    "%s: %s Is not a recognised argument\n%s"

// Program modes/options
#define AUT "-a" // Count all words, lines and chars
#define WOR "-w" // Count words only
#define LIN "-l" // Count lines only
#define CHA "-c" // Count characters only

// Outputs
#define ALLCOUNT    "lines: %d\nwords: %d\nchars: %d\n"
#define OPTIONAL    "%d\n"

int main(int argc, char *argv[])
{
    char *mode;

    if (argc < 2)
    {
        fprintf(stderr, USAGE, PROG);
        return 1;
    }
    else if (argc > 3)
    {
        fprintf(stderr, TOOMANY, PROG, USAGE );
        return 1;
    }
    else if (argc == 2)
    {
        // default mode count everything
        // with two command line args we assume:
        // PROG <OPERAND>
        mode = AUT;
    }
    else if (argc == 3)
    {
        if (strcmp(argv[1], AUT) == 0)
        {
            mode = AUT;
            // if -a then make args as if they are two:
            // <PROG> -a <OPERAND> --> <PROG> <OPRAND>
            argv[1] = argv[2];
            argv[2] = NULL;
        }
        else if (strcmp(argv[1], WOR) == 0) mode = WOR;
        else if (strcmp(argv[1], LIN) == 0) mode = LIN;
        else if (strcmp(argv[1], CHA) == 0) mode = CHA;
        else
        {
            fprintf(stderr, UNRECOGNISED, PROG, argv[1], USAGE);
            return 1;
        }
    }

    FILE *stream = (strcmp(mode, AUT) == 0) ? fopen(argv[1], "r") : fopen(argv[2], "r");

    if (stream == NULL)
    {
        fprintf(stderr, NOFILE, PROG, argv[1]);
        return 1;
    }
    
    // char, line and word count
    int c, l, w = 0;
    int cc = 0;
    
    char *buf = NULL;
    size_t s = 0;

    // get initial line
    cc = getline(&buf, &s, stream);

    // arr for por ptrs to ptrs that point to word tokens
    char **words = malloc(s);

    // Avoid unecessary word counting if not in word or all mode
    int include_words = (strcmp(mode, AUT) == 0 || strcmp(mode, WOR) == 0) ? 1 : 0;
    
    while (cc != -1)
    {
        c += cc;
        ++l;

        // count words
        if (include_words)
        {
            int wc = 0;

            words[wc] = strtok(buf, " \t");

            while (words[wc] != NULL)
            {
                words[++wc] = strtok(NULL, " \t");
            }

            w += wc;
        }

        cc = getline(&buf, &s, stream);
    }

    // Output:
    // line_count
    // word_count
    // chsr_count
    if (strcmp(mode, AUT) == 0) fprintf(stdout, ALLCOUNT, l, w, c);
    else
    {
        if (strcmp(mode, LIN) == 0)
            fprintf(stdout, OPTIONAL, l);
        else if (strcmp(mode, WOR) == 0)
            fprintf(stdout, OPTIONAL, w);
        else if (strcmp(mode, CHA) == 0)
            fprintf(stdout, OPTIONAL, c);
    }

    // free stuff
    free(buf), free(words);

    return 0;
}
