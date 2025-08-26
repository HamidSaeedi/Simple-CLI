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

// Custom IO function 2: Get char from stdin
char custom_getchar(void) {
    return getchar();
}

// Custom IO function 3: Check if char available (for non-blocking)
int custom_char_available(void) {
    return 1; // Always available for stdin
}

// Another set of IO functions (example for different hardware)
void uart_print(const char* str) {
    // This would send to UART in real implementation
    printf("[UART] %s", str);
}

char uart_getchar(void) {
    // This would read from UART in real implementation
    return getchar();
}

int uart_char_available(void) {
    // Check UART status
    return 1;
}

int main() {
    CLI cli;
    
    // Initialize CLI
    cli_init(&cli, "myapp");
    
    // Set custom IO functions - YOU CAN CHANGE THESE ANYTIME!
    cli_set_io_functions(&cli, custom_print, custom_getchar, custom_char_available);
    
    // Add commands
    cli_add_command(&cli, "time", "Get current time", cmd_time);
    cli_add_command(&cli, "echo", "Echo arguments", cmd_echo);
    
    // Start CLI with current IO functions
    printf("Using custom IO functions:\n");
    cli_start(&cli);
    
    // Now let's change the IO functions!
    printf("\nChanging to UART IO functions:\n");
    cli_set_io_functions(&cli, uart_print, uart_getchar, uart_char_available);
    
    // Restart CLI with new IO functions
    cli_start(&cli);
    
    // Cleanup
    for (int i = 0; i < cli.command_count; i++) {
        free(cli.commands[i].description);
    }
    
    return 0;
}