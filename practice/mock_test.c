#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define I_FLAG 0b100
#define C_FLAG 0b010
#define U_FLAG 0b001

void handle_line_output(FILE *f2, const char *buf, const char *temp, int count, int flags) {
    if (flags == 0b000) {
        fprintf(f2, "%s", buf);
    }
    else if (flags == 0b100) {
        if (count > 1) fprintf(f2, "%s", temp);
        else fprintf(f2, "%s", buf);
    }
    else if (flags == 0b010) {
        fprintf(f2, "%d %s", count, buf);
    }
    else if (flags == 0b001 || flags == 0b101) {
        if (count == 1) fprintf(f2, "%s", buf);
    }
    else if (flags == 0b011 || flags == 0b111) {
        if (count == 1) fprintf(f2, "%d %s", count, buf);
    }
    else if (flags == 0b110) {
        if (count != 1) fprintf(f2, "%d %s", count, temp);
        else fprintf(f2, "%d %s", count, buf);
    }
}

void process_file(FILE *f1, FILE *f2, int flags) {
    char buf_1[100] = {0};
    char buf_2[100] = {0};
    char temp[100] = {0};
    int count = 1;

    if (fgets(buf_1, 100, f1) == NULL) {
        printf("The file is empty");
        return;
    }

    while (fgets(buf_2, 100, f1) != NULL) {
        int len = strlen(buf_2);
        if (buf_2[len - 1] != '\n') {
            buf_2[len - 1] = '\n';
        }

        int result = (flags & I_FLAG) ? strcasecmp(buf_1, buf_2) : strcmp(buf_1, buf_2);

        if (result != 0) {
            handle_line_output(f2, buf_1, temp, count, flags);
            count = 1;
        } else {
            if (count == 1) strcpy(temp, buf_1);
            count++;
        }
        strcpy(buf_1, buf_2);
    }
    handle_line_output(f2, buf_1, temp, count, flags);
}

int main(int argc, char* argv[]) {
    int opt;
    int flags = 0;
    FILE* f1;
    FILE* f2;

    while ((opt = getopt(argc, argv, "icu")) != -1) {
        switch (opt) {
            case 'i': 
                flags |= I_FLAG; 
                break;
            case 'c': 
                flags |= C_FLAG; 
                break;
            case 'u': 
                flags |= U_FLAG; 
                break;
            default:
                perror("unknown arguments");
        }
    }

    int remain_argc = argc - optind;
    if (remain_argc == 2) {
        f1 = fopen(argv[optind], "r");
        f2 = fopen(argv[optind + 1], "w");
    } else if (remain_argc == 1) {
        f1 = fopen(argv[optind], "r");
        f2 = fdopen(1, "w");
    } else if (remain_argc == 0) {
        f1 = fdopen(0, "r");
        f2 = fdopen(1, "w");
    }

    process_file(f1, f2, flags);
    
    fclose(f1);
    fclose(f2);
    return 0;
}
