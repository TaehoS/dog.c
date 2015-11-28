/*
* Author: Taeho Sung
*
 * Description: dog.c tries to emulate some of the
 * functionalities of cat.c.
 *
 * Input: various options and file names
 *
 * Output: differs according to the user input
 * commands and option
 * 
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

// checks whether the encryption or the decryption
// map has valid values after initial length
// check when they are being made
int is_map_valid(char *map) {
	for (int i = 0; i < 26; i++) {
		if (!map[i]) 
			return 0;
	}
	return 1;
}

// makes encryption map according to given file name
// does a preliminary check on the validness of the 
// mapfile by checking its lengths
char *encrypt_map(char *filename) {
	size_t len = 0;
	size_t read;
	FILE *fp;

	fp = fopen(optarg, "r");
	
	if(fp == NULL){
		fprintf(stderr, "map input file does not exist");
		exit(0);
	}

	else {
		static char encrypt_map[26];
		char *line = NULL;
		char key, element;
		int i = 0;

		while ((read = getline(&line, &len, fp) != -1)) {
			sscanf(line, "%c %c", &key, &element);
			encrypt_map[(int) key - (int) 'a'] = element;
			i++;
			// exit with error message if file is longer than 26 lines
			if ((i > 26)) {
				fprintf(stderr, "This map in invalid, please check your mapfile");
				exit(EXIT_FAILURE);
			}
			
		}
	
		fclose(fp);

		return encrypt_map;
	}
}

// makes decryption map according to given file name
// does a preliminary check on the validness of the 
// mapfile by checking its length
char *decrypt_map(char *filename) {
        size_t len = 0;
        size_t read;
        FILE *fp;
        
        fp = fopen(optarg, "r");
        
        if(fp == NULL){
                fprintf(stderr, "map input file does not exist");
                exit(0);
        }
        
        else {   
                static char decrypt_map[26];
                char *line = NULL;
                char key, element;
		
                int i = 0;
                while ((read = getline(&line, &len, fp) != -1)) {
                        sscanf(line, "%c %c", &element, &key); 
                        decrypt_map[((int) key - (int) 'a')] = element;
			i++;

			// exit with error message if file is longer than 26 lines			
			if((i > 26)) {
				fprintf(stderr, "This map is invalid, please check your mapfile");
				exit(EXIT_FAILURE);				
			}
                }
                
                fclose(fp);
                
                return decrypt_map;
        }
}

// given the appropriate flag(represented with integers) and map values
// prints out the desired outcome to stdout
void print_output(FILE *file, int l_n_flag, int en_flag, int de_flag, char *en_map, char *de_map) {
	char *line;
	size_t len = 0;
	size_t read;
	
	static int line_count = 1;

	while ((read = getline(&line, &len, file) != -1)) {
        	if (l_n_flag == 1) {
 	               fprintf(stdout, "\t%d ", line_count);
                       line_count++;
                }

                if (en_flag == 1) {

                       for (int j = 0; j < strlen(line); j++) {
   	                    if ( (0 <= (int) line[j] - (int) 'a') && ((int) line[j] - (int) 'a' <= 25)) {
            	                line[j] = en_map[(int) line[j] - (int) 'a'];
                            }
                       }
                }

                if (de_flag == 1) {
   	        	for (int j = 0; j < strlen(line); j++) {
                        	if ( (0 <= (int) line[j] - (int) 'a') && ((int) line[j] - (int) 'a' <= 25)) {
                      	        	line[j] = de_map[(int) line[j] - (int) 'a'];
                                }
                        }
                }

                fprintf(stdout, "%s", line);
		fflush(stdout);
        }
}

int main(int argc, char *argv[]) {
	// initialize variables to be used later	
	FILE *fp;
	
	int line_number_flag = 0;
	int encrypt_flag = 0;
	int decrypt_flag = 0;

	char *encrypted_map;
	char *decrypted_map;
	
	int option_index = 0;
	int c;

	static struct option long_options[] = 
	{
		{"help", no_argument, NULL, 'h'},
		{0, 0, 0, 0}
	};

	// flags for various option cases the user chooses
	while (((c = getopt_long(argc, argv, "nm:M:r", long_options, &option_index)) != -1))
		switch(c) {
			case 'n':

				line_number_flag = 1;
				break;

			case 'm':

				encrypt_flag = 1;
				// creates maps from mapfile and checks whether they are valid
                              	encrypted_map = encrypt_map(optarg);
				decrypted_map = decrypt_map(optarg);
				
				if(!is_map_valid(encrypted_map) || !is_map_valid(decrypted_map)) {
					fprintf(stderr, "This map in invalid, please check your mapfile");
					return 1;
				}

				break;

			case 'M':

				decrypt_flag = 1;
				// creates maps from mapfile and checks whether they are valid
				decrypted_map = decrypt_map(optarg);
				encrypted_map = encrypt_map(optarg);

				if(!is_map_valid(decrypted_map) || !is_map_valid(decrypted_map)) {
                                        fprintf(stderr, "This map in invalid, please check your mapfile");
                                        return 1;
                                }

				break;
			
			case 'h':

				fprintf(stdout, "age: dog [OPTION]... [FILE]… \nConcatenate FILE(s), or standard input, to standard output.\n(attempt to mimic cat)\n\n-n:	number all output lines\n-m 	(map_file): encrypts output according to given map_file\n-M	(map_file): decrypts output according to given map_file\n—help: gives the help page\n\nWith no FILE, or when FILE is -, read standard input\n\nReport dog bugs to taeho.sung.17@dartmouth.edu");
				return 0;

			default:

				fprintf(stdout, "wrong use of command please reference the help page for dog");
				return 1;
		}

	// checks to see whether there are no non-option arguments	
	if ((argv[optind] == NULL)) {
		fp = stdin;
		print_output(fp, line_number_flag, encrypt_flag, decrypt_flag,encrypted_map, decrypted_map);
	}
	//goes through each non-option arguments and prints out the output accordingly
	for ( ; optind < argc; optind++) {
		if (strcmp(argv[optind], "-") == 0) {        
			fp = stdin;
			print_output(fp, line_number_flag, encrypt_flag, decrypt_flag,encrypted_map, decrypted_map);
		}

		else {
			fp = fopen(argv[optind], "r");
			if (fp != NULL) {
				print_output(fp, line_number_flag, encrypt_flag, decrypt_flag,encrypted_map, decrypted_map);
			}

			else {
				fprintf(stderr, "file %s could not be found or read\nPlease check if you have the right file name and permission", argv[optind]);
                                return 1;
			}

			fclose(fp);
		}
	}
	
	return 0;	
}
