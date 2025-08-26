#ifndef CLI_FRAMEWORK_H
#define CLI_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_COMMANDS 20
#define MAX_CMD_NAME_LEN 30
#define MAX_INPUT_LEN 100

// IO Function pointers - define your own implementations
typedef void (*IO_PrintFunc)(const char* str);
typedef char (*IO_GetCharFunc)(void);
typedef int (*IO_CharAvailableFunc)(void);

// Command handler function pointer
typedef void (*CommandHandler)(int argc, char *argv[]);

typedef struct {
    char name[MAX_CMD_NAME_LEN];
    char* description;
    CommandHandler handler;
} Command;

typedef struct {
    Command commands[MAX_COMMANDS];
    int command_count;
    char prompt[20];
    IO_PrintFunc print_func;
    IO_GetCharFunc getchar_func;
    IO_CharAvailableFunc char_available_func;
} CLI;

// Core CLI functions
void cli_init(CLI* cli, const char* prompt);
void cli_set_io_functions(CLI* cli, IO_PrintFunc print_func, 
                         IO_GetCharFunc getchar_func,
                         IO_CharAvailableFunc char_available_func);
int cli_add_command(CLI* cli, const char* name, const char* desc, CommandHandler handler);
void cli_start(CLI* cli);

// Utility functions (internal use)
void cli_show_help(CLI* cli);
void cli_process_input(CLI* cli, const char* input);
char* cli_read_line(CLI* cli);
void cli_print(CLI* cli, const char* str);
void cli_print_prompt(CLI* cli);

#endif // CLI_FRAMEWORK_H