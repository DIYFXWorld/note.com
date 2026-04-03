#pragma once

#include <stdio.h>

struct Median5
{
  int buf[5] = {0};
  int bufIndex = 0;

  int operator()(int x)
  {
    buf[bufIndex++] = x;
    bufIndex %= 5;
    // clang-format off
    #define SWAP(a, b) do { if ((a) > (b)) { int tmp = (a); (a) = (b); (b) = tmp; } } while (0)
    // clang-format on

    int a = buf[0];
    int b = buf[1];
    int c = buf[2];
    int d = buf[3];
    int e = buf[4];

    SWAP(a, b); // a <= b
    SWAP(c, d); // c <= d
    SWAP(a, c); // a <= c

    SWAP(b, e); // b <= e
    SWAP(b, c); // b <= c

    SWAP(d, e); // d <= e

    SWAP(c, d); // c <= d

    return c; // 中央値
  }
};
/*
int main(void)
{
  Median5 mid;
  int m = mid(9);
  m = mid(1);
  m = mid(5);
  m = mid(3);
  m = mid(7);

  printf("median = %d\n", m);
  return 0;
}*/