#include "defn.g"

/* It returns TRUE if the string 'word' contains only alphanumerics
 */

int check_word(char *word) {
    int i = 0;

    for (i = 0; word[i] != EOS; i++)
        if (!isalpha(word[i])) {
            printf("illegal word:|%s|\n", word);
            return (FALSE);
        }
    return (TRUE);
}
