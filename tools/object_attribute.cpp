#include "GameState/object_layer_attributes.h"

void usage(char *name) {
  printf("Usgae:\n");
  printf("\t%s group order [state]\n", name);
  printf("OR\n");
  printf("\t%s group item [state]\n", name);
  exit(1);
}

int main(int ac, char *av[]) {
  if (ac < 3) {
    usage(av[0]);
  }
  TInt group = atoi(av[1]);
  TInt order = atoi(av[2]);
  TInt state = 0;
  if (ac > 3) {
    state = atoi(av[3]);
  }
  TUint16 attr = MAKE_OBJECT_ATTRIBUTE(group, order, state);
  printf("CODE %d ($%0x)\n", attr, attr);
  return 0;
}
