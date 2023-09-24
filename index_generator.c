#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "common_words.h"

#define BUF_LEN 255 // + 1 FOR '\n'
#define WORD_LEN 21 // + 1 FOR '\0'
#define WORD_LINE_COUNT 20
#define UNIQ_WORDS_COUNT 1200

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
void store_words(char *this_word, int line_number)
{
    bool exist = false;

    // CHECKING IF THE WORD ALREADY EXISTS BEFORE STORING IN THE ARRAY
    for (int i = 0; i < UNIQ_WORDS_COUNT; i++)
    {
        // IF THE WORD EXISTS, INCREASE ITS OCCURENCIES & SET THE FLAG TO TRUE
        if (strcmp(words[i].wrd, this_word) == 0)
        {
            words[i].wrd_occ++;
            exist = true;

            // TRACKING THE DISTINCT LINES A WORD CAN APPEAR ON
            int distinct_lines = 0;
            for (distinct_lines = 0; distinct_lines < WORD_LINE_COUNT; distinct_lines++)
            {
                if (words[i].line_nbrs[distinct_lines] == 0)
                {
                    // CHECKING IF THE LINE NUMBER ALREADY EXISTS BEFORE STORING IN THE ARRAY
                    int lineExists = 0;
                    for (int k = 0; k < distinct_lines; k++)
                    {
                        if (words[i].line_nbrs[k] == line_number)
                        {
                            lineExists = 1;
                            break; // GO TO THE NEXT WORD
                        }
                    }

                    // IF THE LINE NUMBER DOESN'T EXIST, STORE IT
                    if (!lineExists)
                        words[i].line_nbrs[distinct_lines] = line_number;

                    // GO TO THE NEXT WORD
                    break;
                }
            }

            // IF THE WORD APPEARS ON MORE THAN 20 LINES - ERROR
            if (distinct_lines >= WORD_LINE_COUNT)
            {
                fprintf(stderr, "The word '%s' appears lines greater than 20.\n", words[i].wrd);
                exit(1);
            }
        }
    }

    // IT DOESN'T EXIST
    if (exist != true)
    {
        // STORING THE WORD AND INITIALIZE OTHER FIELDS
        strncpy(words[unique_word_count].wrd, this_word, sizeof(words[unique_word_count].wrd) - 1);
        words[unique_word_count].wrd[sizeof(words[unique_word_count].wrd) - 1] = '\0'; // TERMINATING THE WORD
        words[unique_word_count].wrd_occ = 1;
        words[unique_word_count].line_nbrs[0] = line_number;

        // INCREMENTING THE INDEX
        unique_word_count++;
    }
}

/*
 * Check if a word is a common word - compare with words in common_words.h
 *
 * Parameters:
 * word:            One word to be checked
 *
 * Returns:
 * true if the word is a common word, false otherwise
 */
bool isCommonWord(const char *word)
{
    bool result = false;
    for (int i = 0; common_words[i] != NULL; i++)
    {
        if (strcmp(word, common_words[i]) == 0)
            result = true;
    }
    return result;
}

/*
 * Reads all the input line by line and parse out words
 *
 * Parameters:
 * ptr_line:        One line of input.
 * line_number:     Line number a word will be parsed from.
 *
 * Returns:
 * None
 */
void read_words(char *ptr_line, int line_number)
{
    char this_word[WORD_LEN];
    while (*ptr_line != '\0')
    {
        // START AT THE FIRST ADDRESS OF word
        int i = 0;

        // CHECK EVERY LETTER TO MAKE A WORD
        while (isalpha(*ptr_line) || (*ptr_line == '\'' && isalpha(*(ptr_line + 1)) && isalpha(*(ptr_line - 1))) || (*ptr_line == '-' && isalpha(*(ptr_line + 1)) && isalpha(*(ptr_line - 1))))
        {
            if (i >= WORD_LEN - 1)
            {
                // TERMINATE WORD
                this_word[i] = '\0';
                fprintf(stderr, "The word '%s...' is longer than 20 characters!\n", this_word);
                exit(1);
            }

            // STORE EACH CHAR FOR CURRENT WORD
            this_word[i] = tolower(*ptr_line);
            i++;
            ptr_line++;
        }

        // TERMINATE THE RESULTED WORD
        this_word[i] = '\0';

        // IF THE WORD IS NOT EMPTY AND NOT A COMMON WORD - STORE IT
        if (this_word[0] != '\0' && isCommonWord(this_word) == false)
        {
            // IF THE WORD DIDN'T EXCEED 1200 COUNT - STORE IT
            if (unique_word_count < UNIQ_WORDS_COUNT)
            {
                store_words(this_word, line_number);

                // RESET THE WORD TO EMPTY FOR THE NEXT WORD TO BE STORED
                memset(this_word, '\0', WORD_LEN);
            }
            else
            {
                fprintf(stderr, "Input has more than 1200 unique words, after common words are discounted.\n");
                exit(1);
            }
        }

        // MOVE TO THE NEXT CHAR IN THE LINE TO CHECK FOR THE NEXT WORD
        ptr_line++;
    }
}

/*
 * Compare two words by their occurencies and alphabetical order
 *
 * Parameters:
 * a:   Pointer to the first word to be compared
 * b:   Pointer to the second word to be compared
 *
 * Returns:
 * 0 if the words are equal, negative if the first word is less than the second, positive otherwise
 */
int compare_words(const void *a, const void *b)
{
    const word_t *wordA = (const word_t *)a;
    const word_t *wordB = (const word_t *)b;

    // COMPARING WORDS BY OCCURENCE IF NOT EQUAL
    if (wordB->wrd_occ != wordA->wrd_occ)
        return wordB->wrd_occ - wordA->wrd_occ;

    // OTHERWISE, ORDER BY ALPHABETICAL ORDER
    else
        return strcmp(wordA->wrd, wordB->wrd) > 0;
}

/*
 * Print the sorted words and their occurencies
 *
 * Parameters:
 * words:   Pointer to the array of words to be printed
 *
 * Returns:
 * None
 */
void print_sorted(word_t *words)
{
    for (int i = 0; i < UNIQ_WORDS_COUNT; i++)
    {
        // PRINTING THE WORDS AND THEIR OCCURENCIES IF THE WORD IS NOT EMPTY
        if (*words[i].wrd != '\0')
        {
            // STRING TO STORE THE LINE NUMBERS OF THE WORD TO BE PRINTED
            char word_lines[255] = "";

            // INITIALIZING TO EMPTY
            word_lines[0] = '\0';

            for (int k = 0; k < words[i].wrd_occ; k++)
            {
                // STRING TEMP TO STORE THE CURRENT LINE NUMBER
                char temp[255];

                // IF LINE NUMBER IS 0, CONTINUE
                if (words[i].line_nbrs[k] == 0)
                    continue;

                // IF THE CURRENT LINE NUMBER IS THE SAME AS THE PREVIOUS ONE, CONTINUE
                if (k > 0 && (words[i].line_nbrs[k] == words[i].line_nbrs[k - 1]))
                    continue;

                // PRINTING THE LINE NUMBER TO THE STRING TEMP
                snprintf(temp, sizeof(temp), k == 0 ? "%d" : ", %d", words[i].line_nbrs[k]);

                // CONCATENATING THE STRING TEMP TO THE STRING word_lines
                strcat(word_lines, temp);
            }

            // PRINTING THE WORDS AND THEIR OCCURENCIES TO stdout IF THE WORD IS NOT EMPTY
            if (fprintf(stdout, "%-*s [%d]: %s\n", 15, words[i].wrd, words[i].wrd_occ, word_lines) < 0)
            {
                fprintf(stderr, "Error writing to stdout: %s\n", strerror(errno));
                exit(1);
            }
        }
    }
}

int main()
{
    char input_line_buf[BUF_LEN];

    // POINTING TO THE START OF THE OF THE FIRST LINE
    char *ptr_line = input_line_buf;

    // READING LINE BY LINE FROM stdin
    int line_number = 0;
    while (fgets(input_line_buf, sizeof(input_line_buf), stdin) != NULL)
    {
        // INCREMENTING THE LINE NUMBER
        line_number++;

        // CALCULATING THE LENGTH OF THE INPUT LINE
        int line_len;
        for (line_len = 1; input_line_buf[line_len - 1] != '\n'; line_len++)
            ;

        // CHECKING IF THE LINE IS LONGER THAN 254 CHARS
        if (line_len > BUF_LEN)
        {
            fprintf(stderr, "A line longer than 254 encountered: '%s...'\n", input_line_buf);
            exit(1);
        }

        // READING WORDS FROM THE INPUT LINE AND STORING THEM
        read_words(ptr_line, line_number);

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

    return 0;
}