#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <hs.h>

static int eventHandler(unsigned int id, unsigned long long from, unsigned long long to, unsigned int flags, void *ctx) 
{
    printf("Match for pattern \"%s\" at offset %llu\n", (char *)ctx, to);
    return 0;
}

static char *readInputData(const char *inputFN, unsigned int *length) 
{
    FILE *f = fopen(inputFN, "rb");
    if (!f) {
        fprintf(stderr, "ERROR: unable to open file \"%s\": %s\n", inputFN, strerror(errno));
        return NULL;
    }

    if (fseek(f, 0, SEEK_END) != 0) {
        fprintf(stderr, "ERROR: unable to seek file \"%s\": %s\n", inputFN,
                strerror(errno));
        fclose(f);
        return NULL;
    }

    long dataLen = ftell(f);
    if (dataLen < 0) {
        fprintf(stderr, "ERROR: ftell() failed: %s\n", strerror(errno));
        fclose(f);
        return NULL;
    }

    if (fseek(f, 0, SEEK_SET) != 0) {
        fprintf(stderr, "ERROR: unable to seek file \"%s\": %s\n", inputFN,
                strerror(errno));
        fclose(f);
        return NULL;
    }

    if ((unsigned long)dataLen > UINT_MAX) {
        dataLen = UINT_MAX;
        printf("WARNING: clipping data to %ld bytes\n", dataLen);
    }
	else if (dataLen == 0) {
        fprintf(stderr, "ERROR: input file \"%s\" is empty\n", inputFN);
        fclose(f);
        return NULL;
    }

    char *inputData = malloc(dataLen);
    if (!inputData) {
        fprintf(stderr, "ERROR: unable to malloc %ld bytes\n", dataLen);
        fclose(f);
        return NULL;
    }

    char *p = inputData;
    size_t bytesLeft = dataLen;
    while (bytesLeft) {
        size_t bytesRead = fread(p, 1, bytesLeft, f);
        bytesLeft -= bytesRead;
        p += bytesRead;
        if (ferror(f) != 0) {
            fprintf(stderr, "ERROR: fread() failed\n");
            free(inputData);
            fclose(f);
            return NULL;
        }
    }

    fclose(f);

    *length = (unsigned int)dataLen;
    return inputData;
}

int main(int argc, char *argv[]) 
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <pattern> <input file>\n", argv[0]);
        return -1;
    }

    char *pattern = argv[1];
    char *inputFN = argv[2];

    if (access(inputFN, F_OK) != 0) {
        fprintf(stderr, "ERROR: file doesn't exist.\n");
        return -1;
    }
    if (access(inputFN, R_OK) != 0) {
        fprintf(stderr, "ERROR: can't be read.\n");
        return -1;
    }

    hs_database_t *database;
    hs_compile_error_t *compile_err;
    if (hs_compile(pattern, HS_FLAG_DOTALL, HS_MODE_BLOCK, NULL, &database, &compile_err) != HS_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to compile pattern \"%s\": %s\n", pattern, compile_err->message);
        hs_free_compile_error(compile_err);
        return -1;
    }

    /* Next, we read the input data file into a buffer. */
    unsigned int length;
    char *inputData = readInputData(inputFN, &length);
    if (!inputData) {
        hs_free_database(database);
        return -1;
    }

    hs_scratch_t *scratch = NULL;
    if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to allocate scratch space. Exiting.\n");
        free(inputData);
        hs_free_database(database);
        return -1;
    }

    printf("Scanning %u bytes with Hyperscan\n", length);

    if (hs_scan(database, inputData, length, 0, scratch, eventHandler, pattern) != HS_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to scan input buffer. Exiting.\n");
        hs_free_scratch(scratch);
        free(inputData);
        hs_free_database(database);
        return -1;
    }

    hs_free_scratch(scratch);
    free(inputData);
    hs_free_database(database);
    return 0;
}
