#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {

    time_t rawtime;
    struct tm *info;
    char buffer[100] = {0};

    setenv("TZ", "Asia/Taipei", 1);
    tzset();

    time(&rawtime);
    info = localtime(&rawtime);

    strftime(buffer, 100, "%a %b %d %X %Z %Y", info);
    printf("%s\n", buffer);

}