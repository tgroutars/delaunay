#ifndef MESH_ROBUST_PREDICATES_H_
#define MESH_ROBUST_PREDICATES_H_

namespace robust_predicates{
  double exactinit();
  double incircle(double *pa, double *pb, double *pc, double *pd);
  double insphere(double *pa, double *pb, double *pc, double *pd, double *pe);
  double orient2d(double *pa, double *pb, double *pc);
  double orient3d(double *pa, double *pb, double *pc, double *pd);
  double isincircle(double *pa, double *pb, double *pc, double *pd) {
    return incircle(pa, pb, pc, pd) * orient2d(pa, pb, pc);
  }
}

#endif
