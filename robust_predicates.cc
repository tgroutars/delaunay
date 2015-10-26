#include "robust_predicates.h"

float TriangleArea(float a[], float b[], float c[]) {
  return (a[0] * b[1] + a[1] * c[0] + b[0] * c[1]) - (a[1] * b[0] + a[0] * c[1] + b[1] * c[0]);
}

float InCircle(float a[], float b[], float c[], float d[]) {
  return (a[0] * b[1] * (c[0] * c[0] + c[1] * c[1]))
       + (b[0] * c[1] * (d[0] * d[0] + d[1] * d[1]))
       + (c[0] * d[1] * (a[0] * a[0] + a[1] * a[1]))
       + (d[0] * a[1] * (b[0] * b[0] + b[1] * b[1]))
       - (a[1] * b[0] * (d[0] * d[0] + d[1] * d[1]))
       - (b[1] * c[0] * (a[0] * a[0] + a[1] * a[1]))
       - (c[1] * d[0] * (b[0] * b[0] + b[1] * b[1]))
       - (d[1] * a[0] * (c[0] * c[0] + c[1] * c[1]));
}
