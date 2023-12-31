#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 20
#define MEMORYSIZE 20

typedef struct Memory {
  char *arr;
  int size;
  int pos;
} Memory;

int readfn(void *ptr, char *buf, int words) {
  Memory *mem = (Memory *)ptr;
  int words_to_read = 0;
  int remain_space = mem->size - mem->pos;

  if (remain_space >= words)
    words_to_read = words;
  else
    words_to_read = remain_space;

  memcpy(buf, mem->arr + mem->pos, words_to_read);
  mem->pos += words_to_read;

  return words_to_read;
}

int writefn(void *ptr, const char *buf, int words) {
  Memory *mem = (Memory *)ptr;
  int words_to_write = 0;
  int remain_space = mem->size - mem->pos;

  if (remain_space >= words)
    words_to_write = words;
  else
    words_to_write = remain_space;

  memcpy(mem->arr + mem->pos, buf, words_to_write);
  mem->pos += words_to_write;

  return words_to_write;
}

fpos_t seekfn(void *ptr, fpos_t offset, int whence) {
  Memory *mem = (Memory *)ptr;
  switch (whence) {
  case SEEK_SET:
    mem->pos = offset;
    break;
  }

  return mem->pos;
}

int closefn(void *ptr) {
  free(ptr);
  return 0;
}

FILE *my_fmemopen(int size) {
  Memory *mem = (Memory *)calloc(1, sizeof(Memory));
  mem->arr = calloc(size, sizeof(char));
  mem->size = size;
  mem->pos = 0;
  FILE *stream = funopen(mem, readfn, writefn, seekfn, closefn);
  return stream;
}

int main() {

  char read_buf[BUFFERSIZE] = {0};
  char write_buf[BUFFERSIZE] = "hello, world";
  int size = MEMORYSIZE;
  FILE *stream = my_fmemopen(size);
  setvbuf(stream, NULL, _IONBF, 0);

  fwrite(write_buf, 1, 12, stream);

  fseek(stream, 7, SEEK_SET);
  fread(read_buf, 1, 5, stream);
  read_buf[5] = '\0';
  printf("%s\n", read_buf);

  fseek(stream, 0, SEEK_SET);
  fread(read_buf, 1, 12, stream);
  read_buf[12] = '\0';
  printf("%s\n", read_buf);

  fclose(stream);

  return 0;
}