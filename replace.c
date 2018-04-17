#include <stdio.h> /* fopen, fseek, ftell, rewind, fread, fclose, getchar,
                      printf, perror */
#include <stdlib.h> /* malloc, realloc, free, exit */
#include <string.h> /* strstr, strlen, strcpy, strncpy */
#define INITIAL_STRING_SIZE_EXPECTATION 1000

char * read_file_to_string(char * name) {
  FILE * file;
  size_t size;
  char * string;
  file = fopen(name, "rb");
  if (file == NULL) {
    perror(name);
    exit(EXIT_FAILURE);
  }
  fseek(file, 0, SEEK_END);
  size = ftell(file);
  rewind(file);
  string = malloc(size + 1);
  fread(string, size, 1, file);
  fclose(file);
  string[size] = 0;
  return  string;
}

char * read_stdin_to_string() {
  char c;
  size_t position;
  size_t len_string;
  char * string;
  position = 0;
  len_string = INITIAL_STRING_SIZE_EXPECTATION; 
  string = malloc(len_string + 1);
  c = getchar();
  string[position] = c;
  while (c != EOF) {
    c = getchar();
    position++;
    if (position >= len_string) {
      len_string = len_string * 2;
      string = realloc(string, len_string);
    }
    string[position] = c;
  }
  string[position] = 0;
  return string;
}

char * replace(char * to_replace, char * replace_with, char * text) {
  char changing = 1;
  char * p_substring;
  size_t index_in_text;
  size_t len_remainder;
  size_t len_result;
  char * result;
  while (changing) {
    p_substring = strstr(text, to_replace);
    if (p_substring != NULL) {
      index_in_text = p_substring - text;
      len_remainder = strlen(text) - index_in_text - strlen(to_replace);
      len_result = index_in_text + strlen(replace_with) + len_remainder;
      result = malloc(len_result + 1);
      strncpy(result, text, index_in_text); /* since original may be larger */ 
      strcpy(result + index_in_text, replace_with);
      strcpy(result + index_in_text  + strlen(replace_with),
             p_substring + strlen(to_replace));
      free(text);
      text = result;
    } else {
      changing = 0;
    }
  }
  return text;
}

int main(int argc, char *argv[]) {
  char * to_replace;
  char * replace_with;
  char * text;
  if (argc != 3) {
    printf("%s reads string from stdin, replaces all occurences of a substring"
           " with another string, then prints the result to stdout.\n\nUsage: "
           "%s <file that contains substring to replace> <file that contains "
           "string to replace substring with>\n", argv[0], argv[0]);
    exit(EXIT_FAILURE);
  }
  to_replace = read_file_to_string(argv[1]);
  replace_with = read_file_to_string(argv[2]);
  text = read_stdin_to_string();
  text = replace(to_replace, replace_with, text);
  printf("%s", text);
  free(text);
  free(to_replace);
  free(replace_with);
  exit(EXIT_SUCCESS);
}
