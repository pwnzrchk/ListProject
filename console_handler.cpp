#include "console_handler.h"
#include "logger.h"
#include "list.h"
#include <stdio.h>
#include <string.h>

// Локальная (статическая) функция для вывода справки
static void PrintHelp() {
    printf("\nAvailable commands:\n");
    printf("  add <index> <value>  - Add a node with <value> after the node at <index>.\n");
    printf("  del <index>          - Delete the node at <index>.\n");
    printf("  dump                 - Generate a graphical dump of the current list state.\n");
    printf("  help                 - Show this help message.\n");
    printf("  exit                 - Exit the program.\n\n");
}

void RunInteractiveSession(tList* list) {
    const char* dot_file_path  = "files/9mice.dot";
    const char* png_file_path  = "files/logpict.png";
    const char* html_file_path = "files/logfile.html";

    // Инициализируем HTML файл при старте
    FILE* html_init = fopen(html_file_path, "w");
    if (html_init) {
        fprintf(html_init, "<!DOCTYPE html><html><head><title>List Log</title></head><body><h1>List Execution Log</h1><hr>\n");
        fclose(html_init);
    }

    char command[kMaxCommandLength];
    PrintHelp();

    while (true) {
        printf("> ");
        if (scanf("%s", command) != 1) {
             while (getchar() != '\n'); // Очистка буфера ввода
             continue;
        }

        if (strcmp(command, "add") == 0) {
            int index = 0, value = 0;
            if (scanf("%d %d", &index, &value) == 2) {
                if (AddNodeAfter(list, index, value) != kNoErrors) ErrorHandler(kUserError);
                else ListDump(list);
            } else {
                printf("Usage: add <index> <value>\n");
                while (getchar() != '\n'); // Очистка буфера
            }
        } else if (strcmp(command, "del") == 0) {
            int index = 0;
            if (scanf("%d", &index) == 1) {
                if (DeleteNodeAt(list, index) != kNoErrors) ErrorHandler(kUserError);
                else ListDump(list);
            } else {
                printf("Usage: del <index>\n");
                while (getchar() != '\n'); // Очистка буфера
            }
        } else if (strcmp(command, "dump") == 0) {
            GraphDump(list, dot_file_path, png_file_path, html_file_path);
        } else if (strcmp(command, "help") == 0) {
            PrintHelp();
        } else if (strcmp(command, "exit") == 0) {
            break;
        } else {
            printf("Unknown command. Type 'help' for a list of commands.\n");
        }
    }
}
