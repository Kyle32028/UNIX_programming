#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {

    int fd = open("sample.txt", O_RDWR);
    char buffer[50] = {0};

    lseek(fd, 14, SEEK_SET);
    read(fd, buffer, 8);
    buffer[8] = '\0';
    write(STDOUT_FILENO, buffer, 8);
    printf("\n");

    lseek(fd, 14, SEEK_SET);
    write(fd, "NTHU student.", 13);

    lseek(fd, 0, SEEK_SET);
    read(fd, buffer, 27);
    buffer[27] = '\0';
    write(STDOUT_FILENO, buffer, 27);
    printf("\n");

    close(fd);
}