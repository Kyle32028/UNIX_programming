#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int fd1 = open("source.txt", O_RDONLY);
    int fd2 = open("destination.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    char buffer[50] = {0};
    int n = 0;
    while((n = read(fd1, buffer, 50) > 0)) {
        write(fd2, buffer, 50);
    }
    close(fd1);
    close(fd2);
}