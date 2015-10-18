#include "robust_predicates.h"

float TriangleArea(float a[], float b[], float c[]) {
  return (a[0] * b[1] + a[1] * c[0] + b[0] * c[1]) - (a[1] * b[0] + a[0] * c[1] + b[1] * c[0]);
}
