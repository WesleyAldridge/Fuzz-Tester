/*
// Wesley Aldridge
// Project 2, Fuzz Tester
// CAP 6135
// Spring 2019
*/

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  /* Read 'cross.jpg' into buffer as binary */
  int buffSize = 1000; //cross.jpg is 808 bytes
  char buff[buffSize];
  FILE *cross  = fopen("cross.jpg", "rb");

  /* Get file size */
  fseek(cross, 0, SEEK_END);
  int fsize = ftell(cross);
  fseek(cross, 0, SEEK_SET); //same as rewind(f);

  /* Read image */
  fread(buff, sizeof(char), fsize, cross); // (destination, size of each element, number of elements, FILE)
  fclose(cross);

  /* Run jpg2bmp program */
  int ret = 0;
  int ret8 = 0;
  int n = 0; // counts the number of bugs we find
  for (int i = 0; i < 10000; i++)
  {
      /* Open & read cross.jpg */
      FILE *cross  = fopen("cross.jpg", "rb");
      fread(buff, sizeof(char), fsize, cross);
      fclose(cross);

      /* Change one byte to a random value between 0 and 255 */
      int randPos = rand() % fsize;
      buff[randPos] = rand() % 256;

      /* Open test file */
      FILE *test = fopen("test.jpg",  "wb");

      /* Write to test.jpg */
      fwrite(buff, sizeof(char), fsize, test);
      fclose(test);

      char commandBuf[200];
      sprintf(commandBuf, "./jpg2bmp test.jpg temp.bmp");
      ret = system(commandBuf);
      ret8 = ret >> 8; // shifting by 8 gives the exit code, if it is 0 or 255 it exits normally (no bug)
      if (ret8 != 0 && ret8 != 255)
      {
          // if program crashed with a bug:
          char fileName[30];
          n += 1;

          sprintf(fileName, "crashed-%d.jpg", n);
          fprintf(stderr, "image %d crashed\n", n);
          FILE * fcrash = fopen(fileName, "wb");
          fwrite(buff, sizeof(char), fsize, fcrash);
          fclose(fcrash);
          //break;
      }
  }

  return 0;
}
