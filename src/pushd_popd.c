#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_STACK_SIZE 100
#define MAX_PATH_LEN 1024

char *stack[MAX_STACK_SIZE];
int top = -1;

void pushd(const char *dir) {
    if (top == MAX_STACK_SIZE - 1) {
        fprintf(stderr, "Error: Directory stack overflow\n");
        return;
    }

    char cwd[MAX_PATH_LEN];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Error getting current working directory");
        return;
    }

    stack[++top] = strdup(cwd);
    if (chdir(dir) != 0) {
        perror("Error changing directory");
        free(stack[top--]);
    } else {
        printf("Directory changed to %s\n", dir);
    }
}

void popd() {
    if (top == -1) {
        fprintf(stderr, "Error: Directory stack underflow\n");
        return;
    }

    char *dir = stack[top--];
    if (chdir(dir) != 0) {
        perror("Error changing directory");
        free(dir);
    } else {
        printf("Directory changed back to %s\n", dir);
        free(dir);
    }
}

void dirs() {
    if (top == -1) {
        printf("Directory stack is empty\n");
        return;
    }

    printf("Current directory stack:\n");
    for (int i = top; i >= 0; i--) {
        printf("%d: %s\n", i, stack[i]);
    }
}

void clear_stack() {
    while (top != -1) {
        free(stack[top--]);
    }
    printf("Directory stack cleared\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [<dir>]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "pushd") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Usage: %s pushd <dir>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        pushd(argv[2]);
    } else if (strcmp(argv[1], "popd") == 0) {
        popd();
    } else if (strcmp(argv[1], "dirs") == 0) {
        dirs();
    } else if (strcmp(argv[1], "clear") == 0) {
        clear_stack();
    } else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    return 0;
}
