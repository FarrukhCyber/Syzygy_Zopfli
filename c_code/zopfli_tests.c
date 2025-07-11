#include "zopfli.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void single_test(const unsigned char *in, int btype, int blocksplitting, int blocksplittingmax)
{
    ZopfliOptions options;

    // Configure options
    options.verbose = 1;
    options.verbose_more = 0; // Reduce internal verbose output
    options.numiterations = 15;
    options.blocksplitting = blocksplitting;
    options.blocksplittinglast = 0;
    options.blocksplittingmax = blocksplittingmax;

    unsigned char *out = 0;
    size_t outsize = 0;
    unsigned char bp = 0;
    size_t insize = strlen((const char *)in);

    // Perform compression
    ZopfliDeflate(&options, btype, 1, in, insize, &bp, &out, &outsize);
    free(out);
}

void run_all_tests(const unsigned char *in)
{
    single_test(in, 2, 1, 15); // Dynamic Huffman
    single_test(in, 1, 1, 15); // Fixed Huffman
    single_test(in, 0, 1, 15); // Uncompressed

    // Test with different block splitting settings
    single_test(in, 2, 0, 15); // No block splitting
    single_test(in, 2, 1, 5);  // Limited splits
    single_test(in, 2, 1, 0);  // No splits
    single_test(in, 2, 1, 1);  // No splits
    single_test(in, 2, 1, 50); // No splits
    single_test(in, 2, 1, 30); // More splits allowed
}


static void run_repeated_a_suite()
{
    const int lengths[6] = {1, 5, 10, 50, 100, 500};

    for (size_t i = 0; i < 6; ++i)
    {
        int len = lengths[i];
        char *buf = (char *)malloc(len + 1);
        if (!buf)
        {
            fprintf(stderr, "Memory allocation failed\n");
            return;
        }
        memset(buf, 'a', len);
        buf[len] = '\0';
        run_all_tests((const unsigned char *)buf);
        free(buf);
    }
}

static char *generate_random_string(size_t len)
{
    char *buf = (char *)malloc(len + 1);
    if (!buf)
        return NULL;

    for (size_t i = 0; i < len; ++i)
        buf[i] = 'a' + rand() % 26;

    buf[len] = '\0';
    return buf;
}

static void run_random_suite()
{
    const int lengths[6] = {3, 7, 13, 31, 64, 128};

    for (size_t i = 0; i < 6; ++i)
    {
        int len = lengths[i];
        char *buf = generate_random_string(len);
        if (!buf)
        {
            fprintf(stderr, "Memory allocation failed\n");
            return;
        }
        run_all_tests((const unsigned char *)buf);
        free(buf);
    }
}

int main()
{
    srand(0);

    run_repeated_a_suite();
    run_random_suite();

    return 0;
}
