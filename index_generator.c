#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "common_words.h"

#define BUF_LEN 255
#define WORD_LEN 20
#define WORD_LINE_COUNT 20
#define UNIQ_WORDS_COUNT 1200
#define OUTPUT_FILE "./out/out.out"

// WORD STRUCTURE
typedef struct
{
    char wrd[WORD_LEN];
    int wrd_occ;
    int line_nbrs[WORD_LINE_COUNT];
} word_t;

// ARRAY OF WORD STRUCTS
word_t words[UNIQ_WORDS_COUNT];

// OTHER VARIABLES
int unique_word_count = 0;
FILE *output;

/*
 * Record a word into the array of words structure
 *
 * Parameters:
 * word:            One word to be stored into words struct
 * line_number:     Line number of the word to be stored
 *
 * Returns:
 * None
 */
void store_words(char *word, int line_number)
{
    int exist = 1;

    // CHECKING IF THE WORD ALREADY EXISTS BEFORE STORING IN THE ARRAY
    for (int i = 0; i < UNIQ_WORDS_COUNT; i++)
    {
        if (strcmp(words[i].wrd, word) == 0)
        {
            // CHECK IF WORD DOESN'T APPEAR ON MORE THAN 20 DISTINCT LINES
            int lineCount = 0; // Initialize a counter for line numbers

            for (int l = 0; words[i].line_nbrs[l]; l++)
            {
                lineCount++; // Increment the line count
                if (lineCount > 20)
                {
                    fprintf(stderr, "The word '%s' is appearing on more than 20 lines\n", word);
                    exit(1);
                }
            }

            // INCREASE OCCURENCIES
            words[i].wrd_occ++;

            // ADD NEW LINE OCCURENCE
            for (int j = 0; j < WORD_LINE_COUNT; j++)
            {
                if (words[i].line_nbrs[j] == 0)
                {
                    words[i].line_nbrs[j] = line_number;
                    break;
                }
            }
            exist = 0;
        }
    }

    if (exist != 0)
    {
        // STORING THE WORD AND INITIALIZE OTHER FIELDS
        strncpy(words[unique_word_count].wrd, word, sizeof(words[unique_word_count].wrd) - 1);
        words[unique_word_count].wrd[sizeof(words[unique_word_count].wrd) - 1] = '\0'; // TERMINATING THE WORD
        words[unique_word_count].wrd_occ = 1;
        words[unique_word_count].line_nbrs[0] = line_number;

        // INCREMENTING THE INDEX
        unique_word_count++;
    }
}

int isCommonWord(const char *word)
{
    int result = 1;
    for (int i = 0; common_words[i] != NULL; i++)
    {
        if (strcmp(word, common_words[i]) == 0)
            result = 0;
    }

    return result;
}

/*
 * Reads all the input line by line and parse out words
 *
 * Parameters:
 * ptr_line:        One line of input.
 * word:            One word that has been parsed
 * line_number:     Line number a word is parsed from.
 *
 * Returns:
 * None
 */
void read_words(char *ptr_line, char *word, int line_number)
{
    while (*ptr_line != '\0')
    {
        // START AT THE FIRST ADDRESS OF word
        int i = 0;

        // CHECK EVERY LETTER TO MAKE A WORD
        while (isalpha(*ptr_line) || (*ptr_line == '\'' && isalpha(*(ptr_line + 1)) && isalpha(*(ptr_line - 1))) || (*ptr_line == '-' && isalpha(*(ptr_line + 1)) && isalpha(*(ptr_line - 1))))
        {
            if (i > 19)
            {
                fprintf(stderr, "The word '%s...' is longer than 20 characters!\n", word);
                exit(1);
            }

            // STORE EACH CHAR FOR CURRENT WORD
            word[i] = tolower(*ptr_line);
            i++;
            ptr_line++;
        }

        // TERMINATE RESULTED WORD
        word[i] = '\0';

        // STORING IN STRUCT ARRAY
        // IF NOT EXIST, CREATE NEW ENTRY INTO WORDS STRUCT AND INCREMENT THE unique_word_count
        if (word[0] != '\0' && isCommonWord(word) == 1)
        {
            if (unique_word_count < UNIQ_WORDS_COUNT)
            {
                store_words(word, line_number);
                // EMPTYING THE WORD BUFFER FOR THE NEXT WORD
                memset(word, '\0', WORD_LEN);
            }
            else
            {
                fprintf(stderr, "Input has more than 1200 unique words, after common words are discounted.\n");
                exit(1);
            }
        }

        // ADVANCE THE POINTER TO THE END OF THE LINE
        ptr_line++;
    }
}

/*
 * Comparison function for qsort to sort words by wrd_occ in descending order
 */
int compare_words(const void *a, const void *b)
{
    const word_t *wordA = (const word_t *)a;
    const word_t *wordB = (const word_t *)b;

    // COMPARING WORDS BY OCCURENCE IF NOT EQUAL, AND BY ALPHA ORDER OTHERWISE
    if (wordB->wrd_occ != wordA->wrd_occ)
        return wordB->wrd_occ - wordA->wrd_occ;
    else
        return strcmp(wordA->wrd, wordB->wrd) > 0;
}
/*
 *  Sorts and prints the output
 *
 * Parameters:
 * words:   Array of words structs to be sorted and printed.
 *
 * Returns:
 * None
 */
void print_sorted(word_t *words)
{
    for (int i = 0; i < UNIQ_WORDS_COUNT; i++)
    {
        // PRINTING THE OUTPUT
        if (*words[i].wrd != '\0')
        {
            // NUMBER OF LINES A WORD CAN APPEAR ON AS A STRING
            char word_lines[255];

            // INITIALIZING TO EMPTY
            word_lines[0] = '\0';

            for (int k = 0; k < words[i].wrd_occ; k++)
            {
                char temp[255];

                // IF LINE NUMBER EXIST, CONTINUE
                if (k > 0 && (words[i].line_nbrs[k] == words[i].line_nbrs[k - 1]))
                    continue;

                snprintf(temp, sizeof(temp), k == 0 ? "%d" : ", %d", words[i].line_nbrs[k]);

                // CONCATENATE CURRENT NUMBER TO THE STRING
                strcat(word_lines, temp);
            }

            printf("Hello\n");

            fprintf(output, "%-*s [%d]: %s\n", 15, words[i].wrd, words[i].wrd_occ, word_lines);
            // OUTPUTTING TO THE FILE - PRINTING ERROR WHEN RETURN VALUE IS < 0
            if (fprintf(stdout, "%-*s [%d]: %s\n", 15, words[i].wrd, words[i].wrd_occ, word_lines) < 0)
            {
                fprintf(stderr, "Error writing to output: %s\n", strerror(errno));
                exit(1);
            }
        }
    }
}

int main()
{
    char input_line_buf[BUF_LEN];
    char this_word[WORD_LEN];

    // POINTING TO THE START OF THE OF THE FIRST LINE
    char *ptr_line = input_line_buf;

    output = fopen(OUTPUT_FILE, "w");
    if (output == NULL)
    {
        fprintf(stderr, "Error opening output %s: %s\n", OUTPUT_FILE, strerror(errno));
        exit(1);
    }

    // READING LINE BY LINE FROM INPUT FILE
    int line_number = 0;
    while (fgets(input_line_buf, sizeof(input_line_buf), stdin) != NULL)
    {
        // CALCULATING THE LENGTH OF THE INPUT LINE
        int line_len;
        for (line_len = 1; input_line_buf[line_len - 1] != '\0'; line_len++)
            ;

        if (line_len > 254)
        {
            fprintf(stderr, "A line longer than 254 encountered: '%s...'\n", input_line_buf);
            exit(1);
        }

        line_number++;
        read_words(ptr_line, this_word, line_number);

        // READ NEXT INPUT LINE. POINTING TO FRONT OF THE NEXT LINE
        ptr_line = input_line_buf;
    }

    // IF NO WORDS FOUND
    if (unique_word_count < 1)
    {
        fprintf(stderr, "No words found\n");
        exit(1);
    }

    // SORTING THE ARRAY OF WORD STRUCTS
    qsort(words, unique_word_count, sizeof(word_t), compare_words);

    // PRINTING
    print_sorted(words);

    if (ferror(stdin))
    {
        fprintf(stderr, "Error reading from stdin\n");
        exit(1);
    }

    if (ferror(stdout))
    {
        fprintf(stderr, "Error writing to stdout\n");
        exit(1);
    }

    fclose(output);
    return 0;
}