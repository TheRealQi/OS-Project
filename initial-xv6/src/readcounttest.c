#include "types.h"
#include "stat.h"
#include "user.h"

int main (void){
  int count = 0;
  count = getreadcount();
  printf("%d", count);
  exit();
}
