// This is a program that converts tabs to spaces, and vice-versa.
// the -t option will be for spaces to tabs.
// the -s option will be for tabs to spaces.
// one of those two options must be passed, or else the program will fail.
// -n will dictate how much spacing a single tab should be.
// The program will get a list of positional arguments to go through, each representing a filename.
// it will open up each one individually and print out the result, just like cat.

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <peaceful_death.h>

#define MAX_ARGS_ALLOWED (unsigned long) 69420

typedef enum {
    EXIT_OK,
    EXIT_CANT_OPEN_FILE,
    EXIT_TOO_MANY_ARGS,
    EXIT_PARSE_ERROR,
    EXIT_NO_MODE_SPECIFIED,
    EXIT_OUT_OF_MEM,
} ExitCode;

typedef enum {
    NO_MODE_SET,
    TO_TABS,
    TO_SPACES,
} ConversionMode;

typedef struct {
    ConversionMode mode;
    size_t spaces_per_tab;
} Parameters;

void printFileTabsToSpaces(FILE *file, Parameters params) {
    if(!file) return;

    // tabs to spaces
    size_t spaces_left = params.spaces_per_tab;

    // ugly-ass code, but I'm too tired to care.
    for(int character; (character = fgetc(file)) != EOF;) {
        if(character == '\t') {
            for(size_t i = 0; i < spaces_left; i++) {
                putchar(' ');
            }

            spaces_left = params.spaces_per_tab;
            continue;
        }

        putchar(character);
        if(character == '\n') {
            spaces_left = params.spaces_per_tab;
            continue;
        }

        spaces_left--;
        if(spaces_left == 0) spaces_left = params.spaces_per_tab;
    }
}

// WHY IS MY LSP NOT WORKING RIGHT NOW??? WTF?????????????
void printFileSpacesToTabs(FILE *file, Parameters params) {
    if(!file) return;

    size_t x_dist = 0, spaces = 0;

    for(int character; (character = fgetc(file)) != EOF; x_dist = (x_dist + 1) % params.spaces_per_tab) {
        if(character == ' ') {
            spaces++;

            if(x_dist == params.spaces_per_tab - 1) {
                putchar('\t');
                spaces = 0;
            }

            continue;
        }

        if(spaces) {
            for(; spaces > 0; spaces--) {
                putchar(' ');
            }
        }

        putchar(character);
        if(character == '\t' || character == '\n') x_dist = -1;
    }
}

int printFiles(char **filenames, Parameters params, void (*converterFunction)(FILE*,Parameters)) {
    if(!filenames || !*filenames) return EXIT_SUCCESS;

    int returned = EXIT_SUCCESS;

    for(char **filename = filenames; *filename; filename++) {
        if(!strcmp(*filename,"-")) {
            converterFunction(stdin,params);
            continue;
        }

        FILE *file = fopen(*filename,"r");
        if(!file) {
            fprintf(stderr,"%s Can't open \"%s\": %s!\n",NORMAL_ERROR_TEXT,*filename,strerror(errno));
            returned = EXIT_CANT_OPEN_FILE;
            continue;
        }

        converterFunction(file,params);
        fclose(file);
    }

    return returned;
}

typedef struct {
    bool is_option, convert_next_arg;
} ParseOptionsReturnType;

ParseOptionsReturnType parseOptions(char *arg, Parameters *parameters, bool *do_options) {
    if(*arg != '-') return (ParseOptionsReturnType) {false,false};

    Parameters tmp_parameters = *parameters;
    bool tmp_do_options = true, convert_next_arg = false;

    for(char *char_ptr = arg + 1; *char_ptr; char_ptr++) {
        switch(*char_ptr) {
            case 't':
                tmp_parameters.mode = TO_TABS;
                break;
            case 's':
                tmp_parameters.mode = TO_SPACES;
                break;
            case 'n':
                convert_next_arg = true;
                break;
            case '-':
                tmp_do_options = false;

            default: return (ParseOptionsReturnType) {false,false};
        }
    }

    *parameters = tmp_parameters;
    *do_options = tmp_do_options;
    return (ParseOptionsReturnType) {true,convert_next_arg};
}

char *parseArgs(int argc, char **argv, char **filenames, Parameters *parameters) {
    if(!argv || !filenames || !parameters) return "one or more parameters passed in are NULL";
    if(argc < 1) return "invalid argc value (value is less than 1)";

    parameters->mode = NO_MODE_SET;
    parameters->spaces_per_tab = 8;

    char **filename = filenames;
    bool do_options = true, convert_next_arg = false;

    for(int i = 1; i < argc; i++) {
        if(convert_next_arg) {
            parameters->spaces_per_tab = atoi(argv[i]); // TODO: use something other than atoi.

            convert_next_arg = false;
            continue;
        }

        if(do_options) {
            ParseOptionsReturnType stuff = parseOptions(argv[i],parameters,&do_options);
            convert_next_arg = stuff.convert_next_arg;
            if(stuff.is_option) continue;
        }

        *(filename++) = argv[i];
    }

    *filename = NULL;
    return NULL;
}

char *printlnAndReturn(char *string, FILE *file) {
    if(string) fprintf(file,"%s\n",string);
    return string;
}

int main(int argc, char *argv[]) {
    if(argc - 1 > MAX_ARGS_ALLOWED) {
        die(EXIT_TOO_MANY_ARGS,"Too many args passed! (max allowed is %lu)",MAX_ARGS_ALLOWED);
    }

    char *filenames[argc];
    Parameters parsed_args;
    if(printlnAndReturn(parseArgs(argc,argv,filenames,&parsed_args),stderr)) return EXIT_PARSE_ERROR;

    if(!parsed_args.mode) die(EXIT_NO_MODE_SPECIFIED,"No option for specifying tabs or spaces specified!");
    void (*converterFunction)(FILE*,Parameters) = parsed_args.mode == TO_TABS ?
        &printFileSpacesToTabs : &printFileTabsToSpaces;

    if(!*filenames) {
        converterFunction(stdin,parsed_args);
        return EXIT_SUCCESS;
    }

    return printFiles(filenames,parsed_args,converterFunction);
}

