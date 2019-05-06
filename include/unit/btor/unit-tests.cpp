#include "assert.h"
#include <iostream>
#include <memory>
#include <vector>

#include "boolector/boolector.h"

#include "boolector_solver.h"
#include "boolector_sort.h"
#include "boolector_term.h"
#include "op.h"

using namespace smt;
using namespace std;

bool solver()
{
  bool res = true;
  // Solver s (new BoolectorSolver());
  Solver s = make_shared<BoolectorSolver>();
  s->set_opt("produce-models", true);
  Sort bvsort8 = s->construct_sort(BV, 8);
  Term x = s->declare_const("x", bvsort8);
  Term y = s->declare_const("y", bvsort8);
  Term z = s->declare_const("z", bvsort8);
  res &= (x != y);
  Term x_copy = x;
  res &= (x == x_copy);

  // check sorts
  Sort xsort = x->get_sort();
  Sort ysort = y->get_sort();
  res &= (xsort == ysort);

  Sort arr_sort = s->construct_sort(ARRAY,
                                    s->construct_sort(BV, 4),
                                    bvsort8);
  res &= (xsort != arr_sort);
  res &= (xsort != arr_sort->get_indexsort());
  res &= (xsort == arr_sort->get_elemsort());

  Term xpy = s->apply_op(BVADD, x, y);
  Term z_eq_xpy = s->apply_op(EQUAL, z, xpy);

  Op op = z_eq_xpy->get_op();
  res &= holds_alternative<PrimOp>(op);
  res &= (get<PrimOp>(op) == EQUAL);

  s->assert_formula(z_eq_xpy);
  s->assert_formula(s->apply_op(BVULT,
                                x,
                                s->make_const(4, bvsort8)));
  s->assert_formula(s->apply_op(BVULT,
                                y,
                                s->make_const(4, bvsort8)));
  s->assert_formula(s->apply_op(BVUGT,
                                z,
                                s->make_const(5, bvsort8)));
  res &= (s->check_sat());

  Term xc = s->get_value(x);
  Term yc = s->get_value(y);
  Term zc = s->get_value(z);

  cout << "Got the following values:" << endl;
  cout << "\t" << xc->as_bitstr() << endl;
  cout << "\t" << yc->as_bitstr() << endl;
  cout << "\t" << zc->as_bitstr() << endl;
  return res;
}

int main() {
  assert(solver());
}