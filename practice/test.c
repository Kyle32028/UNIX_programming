#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define I_FLAG 0b100
#define C_FLAG 0b010
#define U_FLAG 0b001

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
        }
    }

    int remain_argc = argc - optind;
    if (remain_argc == 2) {
        f1 = fopen(argv[optind], "r");
        f2 = fopen(argv[optind + 1], "w");
    }
    else if (remain_argc == 1) {
        f1 = fopen(argv[optind], "r");
        f2 = fdopen(1, "w");
    }
    else if (remain_argc == 0) {
        f1 = fdopen(0, "r");
        f2 = fdopen(1, "w");
    }

    //implement each case according to flags
    if (flags == 0b000) {
        //no flag
        char buf_1[100] = {0};
        char buf_2[100] = {0};

        if (fgets(buf_1, 100, f1) == NULL) {
            printf("The file is empty");
        }

        while (fgets(buf_2, 100, f1) != NULL) {
            int len = strlen(buf_2);
            if(buf_2[len - 1] != '\n') {
                buf_2[len - 1] = '\n';
            }

            int result = strcmp(buf_1, buf_2);
            if (result != 0) {
                fprintf(f2, "%s", buf_1);
            }
            strcpy(buf_1, buf_2);
        }
        fprintf(f2, "%s\n", buf_1);
    }
    else if (flags == 0b100) {
        //case insensitive comparision with lines, output the first line of each group
        char buf_1[100] = {0};
        char buf_2[100] = {0};
        char temp[100] = {0};
        int count = 1;

        if (fgets(buf_1, 100, f1) == NULL) {
            printf("The file is empty");
        }

        while (fgets(buf_2, 100, f1) != NULL) {
            int len = strlen(buf_2);
            if(buf_2[len - 1] != '\n') {
                buf_2[len - 1] = '\n';
            }
            
            int result = strcasecmp(buf_1, buf_2);
            if (result != 0) {
                if (count != 1) {
                    fprintf(f2, "%s", temp);
                }
                else {
                    fprintf(f2, "%s", buf_1);
                }
                count = 1;
            }
            else {
                if (count == 1) {
                    strcpy(temp, buf_1);
                }
                count++;
            }
            strcpy(buf_1, buf_2);
        }

        if (count != 1) {
            fprintf(f2, "%s", temp);
        }
        else {
            fprintf(f2, "%s", buf_1);
        }
    }
    else if (flags == 0b010) {
        //count the number of occurences of each line
        char buf_1[100] = {0};
        char buf_2[100] = {0};
        int count = 1;

        if (fgets(buf_1, 100, f1) == NULL) {
            printf("The file is empty");
        }

        while (fgets(buf_2, 100, f1) != NULL) {
            int len = strlen(buf_2);
            if(buf_2[len - 1] != '\n') {
                buf_2[len - 1] = '\n';
            }

            int result = strcmp(buf_1, buf_2);
            if (result != 0) {
                fprintf(f2, "%d %s", count, buf_1);
                count = 1;
            }
            else {
                count++;
            }
            strcpy(buf_1, buf_2);
        }
        fprintf(f2, "%d %s\n", count, buf_1);
    }
    else if (flags == 0b001) {
        //Only output lines that are not repeated in the input
        char buf_1[100] = {0};
        char buf_2[100] = {0};
        int count = 1;

        if (fgets(buf_1, 100, f1) == NULL) {
            printf("The file is empty");
        }

        while (fgets(buf_2, 100, f1) != NULL) {
            int len = strlen(buf_2);
            if(buf_2[len - 1] != '\n') {
                buf_2[len - 1] = '\n';
            }
            
            int result = strcmp(buf_1, buf_2);
            if (result != 0) {
                if (count == 1) {
                    fprintf(f2, "%s", buf_1);
                }
                count = 1;
            }
            else {
                count++;
            }
            strcpy(buf_1, buf_2);
        }

        if (count == 1) {
            fprintf(f2, "%s", buf_1);
        }
    }
    else if (flags == 0b011) {
        // -c & -u
        char buf_1[100] = {0};
        char buf_2[100] = {0};
        int count = 1;

        if (fgets(buf_1, 100, f1) == NULL) {
            printf("The file is empty");
        }

        while (fgets(buf_2, 100, f1) != NULL) {
            int len = strlen(buf_2);
            if(buf_2[len - 1] != '\n') {
                buf_2[len - 1] = '\n';
            }
            
            int result = strcmp(buf_1, buf_2);
            if (result != 0) {
                if (count == 1) {
                    fprintf(f2, "%d %s", count, buf_1);
                }
                count = 1;
            }
            else {
                count++;
            }
            strcpy(buf_1, buf_2);
        }

        if (count == 1) {
            fprintf(f2, "%d %s", count, buf_1);
        }
    }
    else if (flags == 0b101) {
        // -i & -u
        char buf_1[100] = {0};
        char buf_2[100] = {0};
        char temp[100] = {0};
        int count = 1;

        if (fgets(buf_1, 100, f1) == NULL) {
            printf("The file is empty");
        }

        while (fgets(buf_2, 100, f1) != NULL) {
            int len = strlen(buf_2);
            if(buf_2[len - 1] != '\n') {
                buf_2[len - 1] = '\n';
            }
            
            int result = strcasecmp(buf_1, buf_2);
            if (result != 0) {
                if (count == 1) {
                    fprintf(f2, "%s", buf_1);
                }
                count = 1;
            }
            else {
                count++;
            }
            strcpy(buf_1, buf_2);
        }

        if (count == 1) {
            fprintf(f2, "%s", buf_1);
        }
    }
    else if (flags == 0b110) {
        // -i & -c
        char buf_1[100] = {0};
        char buf_2[100] = {0};
        int count = 1;

        if (fgets(buf_1, 100, f1) == NULL) {
            printf("The file is empty");
        }

        while (fgets(buf_2, 100, f1) != NULL) {
            int len = strlen(buf_2);
            if(buf_2[len - 1] != '\n') {
                buf_2[len - 1] = '\n';
            }
            
            int result = strcasecmp(buf_1, buf_2);
            if (result != 0) {
                fprintf(f2, "%d %s", count, buf_1);
                count = 1;
            }
            else {
                count++;
            }
            strcpy(buf_1, buf_2);
        }
        fprintf(f2, "%d %s\n", count, buf_1);
    }
    else if (flags == 0b111) {
        // -i & -c & -u
        char buf_1[100] = {0};
        char buf_2[100] = {0};
        int count = 1;

        if (fgets(buf_1, 100, f1) == NULL) {
            printf("The file is empty");
        }

        while (fgets(buf_2, 100, f1) != NULL) {
            int len = strlen(buf_2);
            if(buf_2[len - 1] != '\n') {
                buf_2[len - 1] = '\n';
            }
            
            int result = strcasecmp(buf_1, buf_2);
            if (result != 0) {
                if (count == 1) {
                    fprintf(f2, "%d %s", count, buf_1);
                }
                count = 1;
            }
            else {
                count++;
            }
            strcpy(buf_1, buf_2);
        }

        if (count == 1) {
            fprintf(f2, "%d %s", count, buf_1);
        }
    }
    
    fclose(f1);
    fclose(f2);
}