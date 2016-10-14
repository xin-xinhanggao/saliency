#include <cstdio>
#include <cstdlib>
#include "image.h"
#include "misc.h"
#include "pnmfile.h"
#include "saliency.h"
#include "hiscolor.h"
#include "filter.h"

int main(int argc, char **argv) {
  printf("loading input image.\n");
  image<rgb> *input = loadPPM(argv[1]);

  image<rgb> *modify = changepipe(input,12);

  image<rgb> *output = saliency(modify);

  savePPM(output, argv[2]);
  printf("done! uff...thats hard work.\n");

  return 0;
}
