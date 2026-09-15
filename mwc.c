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
#define ALLCOUNT    "lines: %zu\nwords: %zu\nchars: %zu\n"
#define OPTIONAL    "%zu\n"

ssize_t count_lines(char *buf, size_t nread)
{   
    ssize_t nlines = 0;

    if (buf[nread-1] == '\n')
        ++nlines;
    
    return nlines;
}

ssize_t count_chars(char *buf, size_t nread)
{
    ssize_t nchars = 0;

    int i;
    for (i = 0; i < nread; ++i)
    {
        if (buf[i] != '\0')
            ++nchars;
    }

    return nchars;
}

ssize_t count_words(char *buf, size_t nread)
{
    ssize_t nwords = 0;
    int inword = 0;

    for (int i = 0; i <= nread; ++i)
    {
        int curr = buf[i];
        if ((curr >= 65 && curr <= 90) || (curr >= 97 && curr <= 222))
        {
            if (!inword) inword = 1;
        }
        else
        {
            if (inword) ++nwords;
            inword = 0;
        }
    }

    return nwords;
}


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
    ssize_t nlines, nwords, nchars = 0;
    size_t linelen;
    size_t size = 0;
    char *buf = NULL;

    if (!strcmp(AUT, mode))
    {
        // Count all
        while ((linelen = getline(&buf, &size, stream)) != -1)
        {
            nlines += count_lines(buf, linelen);
            nchars += count_chars(buf, linelen);
            nwords += count_words(buf, linelen);
        }

        fprintf(stdout, ALLCOUNT, nlines, nwords, nchars);
    }
    else if (!strcmp(WOR, mode))
    {
        // count only words
         while ((linelen = getline(&buf, &size, stream)) != -1)
        {
            printf("x");
            nwords += count_words(buf, linelen);

        }

        fprintf(stdout, OPTIONAL, nwords);
       
    }
    else if (!strcmp(LIN, mode))
    {
        // Count only lines
        while ((linelen = getline(&buf, &size, stream)) != -1)
        {
            nlines += count_lines(buf, linelen);

        }

        fprintf(stdout, OPTIONAL, nlines);
    }
    else if (!strcmp(CHA, mode))
    {
        // count only characters
        while ((linelen = getline(&buf, &size, stream)) != -1)
        {
            nchars += count_chars(buf, linelen);

        }

        fprintf(stdout, OPTIONAL, nchars);
    }

    // free stuff
    free(buf);

    return 0;
}
