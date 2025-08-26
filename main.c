#include "cli_framework.h"
#include <time.h>

// Example command functions
void cmd_time(int argc, char *argv[]) {
    time_t now = time(NULL);
    char* time_str = ctime(&now);
    printf("Current time: %s", time_str);
}

void cmd_echo(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: echo <message>\r\n");
        return;
    }
    for (int i = 1; i < argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\r\n");
}

// Custom IO function 1: Print to stdout
void custom_print(const char* str) {
    printf("%s", str);
}

// Custom IO function 2: Get char from stdin (with proper signature)
char custom_getchar(void) {
    return (char)getchar(); // Cast to match IO_GetCharFunc
}

// Custom IO function 3: Check if char available
int custom_char_available(void) {
    return 1; // Always available for stdin
}

int main() {
    CLI cli;
    
    // Initialize CLI
    cli_init(&cli, "myapp");
    
    // Set custom IO functions
    cli_set_io_functions(&cli, custom_print, custom_getchar, custom_char_available);
    
    // Add commands
    cli_add_command(&cli, "time", "Get current time", cmd_time);
    cli_add_command(&cli, "echo", "Echo arguments", cmd_echo);
    
    // Start CLI
    cli_start(&cli);
    
    // Cleanup
    for (int i = 0; i < cli.command_count; i++) {
        free(cli.commands[i].description);
    }
    
    return 0;
}