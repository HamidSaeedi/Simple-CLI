#include "cli_framework.h"

// Initialize CLI with default values
void cli_init(CLI* cli, const char* prompt) {
    cli->command_count = 0;
    strncpy(cli->prompt, prompt, sizeof(cli->prompt) - 1);
    cli->prompt[sizeof(cli->prompt) - 1] = '\0';
    
    // Set default IO functions to NULL
    cli->print_func = NULL;
    cli->getchar_func = NULL;
    cli->char_available_func = NULL;
}

// Set custom IO functions
void cli_set_io_functions(CLI* cli, IO_PrintFunc print_func, 
                         IO_GetCharFunc getchar_func,
                         IO_CharAvailableFunc char_available_func) {
    cli->print_func = print_func;
    cli->getchar_func = getchar_func;
    cli->char_available_func = char_available_func;
}

// Add a command to the CLI
int cli_add_command(CLI* cli, const char* name, const char* desc, CommandHandler handler) {
    if (cli->command_count >= MAX_COMMANDS) {
        return -1;
    }
    
    Command* cmd = &cli->commands[cli->command_count];
    strncpy(cmd->name, name, MAX_CMD_NAME_LEN - 1);
    cmd->name[MAX_CMD_NAME_LEN - 1] = '\0';
    
    cmd->description = (char*)malloc(strlen(desc) + 1);
    strcpy(cmd->description, desc);
    
    cmd->handler = handler;
    
    cli->command_count++;
    return 0;
}

// Start the CLI main loop
void cli_start(CLI* cli) {
    // Check if IO functions are set
    if (!cli->print_func || !cli->getchar_func) {
        // Use stdio as fallback
        cli_print(cli, "Warning: Using stdio as fallback IO\r\n");
    }
    
    cli_print(cli, "CLI Framework Started. Type 'help' for commands.\r\n");
    cli_print(cli, "Type 'exit' to quit.\r\n\r\n");
    
    char* input;
    while (1) {
        cli_print_prompt(cli);
        input = cli_read_line(cli);
        
        if (input == NULL) continue;
        
        if (strcmp(input, "exit") == 0) {
            free(input);
            break;
        }
        
        cli_process_input(cli, input);
        free(input);
    }
    
    cli_print(cli, "CLI Framework Exited.\r\n");
}

// Internal: Process user input
void cli_process_input(CLI* cli, const char* input) {
    char input_copy[MAX_INPUT_LEN];
    strncpy(input_copy, input, MAX_INPUT_LEN - 1);
    input_copy[MAX_INPUT_LEN - 1] = '\0';
    
    char* argv[10];
    int argc = 0;
    char* token = strtok(input_copy, " ");
    
    while (token != NULL && argc < 10) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }
    
    if (argc == 0) return;
    
    if (strcmp(argv[0], "help") == 0) {
        cli_show_help(cli);
        return;
    }
    
    int found = 0;
    for (int i = 0; i < cli->command_count; i++) {
        if (strcmp(argv[0], cli->commands[i].name) == 0) {
            found = 1;
            cli->commands[i].handler(argc, argv);
            break;
        }
    }
    
    if (!found) {
        char error_msg[50];
        snprintf(error_msg, sizeof(error_msg), "Unknown command: %s\r\n", argv[0]);
        cli_print(cli, error_msg);
    }
}

// Internal: Show help
void cli_show_help(CLI* cli) {
    cli_print(cli, "\r\nAvailable Commands:\r\n");
    cli_print(cli, "------------------\r\n");
    
    for (int i = 0; i < cli->command_count; i++) {
        char line[50];
        snprintf(line, sizeof(line), "%-15s - %s\r\n", 
                cli->commands[i].name, cli->commands[i].description);
        cli_print(cli, line);
    }
    cli_print(cli, "help            - Show this help message\r\n");
    cli_print(cli, "exit            - Exit the CLI\r\n\r\n");
}

// Internal: Read a line using configured IO functions
char* cli_read_line(CLI* cli) {
    static char line[MAX_INPUT_LEN];
    int pos = 0;
    char c;
    
    // Use stdio if no custom getchar function is set
    IO_GetCharFunc getchar_fn = cli->getchar_func ? cli->getchar_func : getchar;
    
    while (1) {
        c = getchar_fn();
        
        // Handle backspace
        if (c == '\b' || c == 127) {
            if (pos > 0) {
                pos--;
                cli_print(cli, "\b \b");
            }
            continue;
        }
        
        // Handle enter/return
        if (c == '\r' || c == '\n') {
            cli_print(cli, "\r\n");
            line[pos] = '\0';
            return strdup(line);
        }
        
        // Regular character
        if (pos < MAX_INPUT_LEN - 1 && isprint(c)) {
            line[pos++] = c;
            // Echo character
            char echo_str[2] = {c, '\0'};
            cli_print(cli, echo_str);
        }
    }
}

// Internal: Print using configured IO function
void cli_print(CLI* cli, const char* str) {
    if (cli->print_func) {
        cli->print_func(str);
    } else {
        // Fallback to printf
        printf("%s", str);
    }
}

// Internal: Print prompt
void cli_print_prompt(CLI* cli) {
    char prompt_str[25];
    snprintf(prompt_str, sizeof(prompt_str), "%s> ", cli->prompt);
    cli_print(cli, prompt_str);
}