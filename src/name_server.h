#ifndef NAME_SERVER_H_
#define NAME_SERVER_H_

#define PORT 8888

/**
 * Converts a null terminated array of characters to an integer.
 * @param str The array of characters to be read.
 * @param i A pointer specifying where the conversion should be stored.
 * @return True if the conversion was successful, otherwise false.
 *          A conversion is considered successful iff all the str characters
 *          were used in the conversion and the result was within the range an
 *          int can store.
 */
int str2int(char *str, int *i);

/**
 * Prints the usage statement to stdout
 * @param prog The name of the program binary. (argv[0])
 */
void printUsage(char *prog);

#endif /* NAME_SERVER_H_ */
