#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// metadata
#define PROG    "mywc"

// msgs
#define NOFILE  "%s: No such file or directory '%s'"
#define USAGE   "%s: Missing file operand\nUsage: %s <OPERAND>\n"
#define RESULT  "lines: %d\nwords: %d\nchars: %d\n"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, USAGE, PROG, PROG);
        return 1;
    }

    FILE *stream = fopen(argv[1], "r");

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
    
    while (cc != -1)
    {
        c += cc;
        ++l;

        // count words
        int wc = 0;

        words[wc] = strtok(buf, " \t");

        while (words[wc] != NULL)
        {
            words[++wc] = strtok(NULL, " \t");
        }

        w += wc;

        cc = getline(&buf, &s, stream);
    }

    // Output:
    // line_count
    // word_count
    // chsr_count
    fprintf(stdout, RESULT, l, w, c);

    // free stuff
    free(buf), free(words);

    return 0;
}
