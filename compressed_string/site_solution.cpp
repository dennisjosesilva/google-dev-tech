#include <iostream>

int char_to_int(char c) {
  return c - '0';
}

int decomp(const std::string &text, int start=0, int times=1)
{
  int i;
  for (int count = 0; count < times; count++)
  {
    i = start;
    while (i < text.size() && text[i] != ']') {
      if (islower(text[i])) {
        std::cout << text[i];
        i++;
      }
      else {
        int sub_times = 0;
        while (isdigit(text[i])) {
          sub_times = sub_times * 10 + char_to_int(text[i]);
          i++;
        }
        i++;   // Skip past open-bracket
        i = decomp(text, i, sub_times);        
      }      
    }
    i++;
  }
  return i;
}

int main(int argc, char *argv[])
{
  decomp(std::string(argv[1]));
  std::cout << "\n";
  return 0;
}