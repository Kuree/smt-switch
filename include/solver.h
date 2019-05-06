#ifndef SMT_SOLVER_H
#define SMT_SOLVER_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "exceptions.h"
#include "op.h"
#include "sort.h"
#include "term.h"

namespace smt
{
  /**
     Abstract solver class to be implemented by each supported solver.
   */
  class AbsSolver
  {
  public:
    AbsSolver() {};
    virtual ~AbsSolver() {};
    virtual Sort declare_sort(const std::string name, unsigned int arity) const = 0;
    virtual Term declare_const(const std::string name, Sort sort) const = 0;
    // virtual Function declare_fun(const std::string name,
    //                              const std::vector<Sort>& sorts,
    //                              Sort sort) const = 0;
    virtual Term make_const(unsigned int i, Sort sort) const = 0;
    virtual void assert_formula(Term& t) const = 0;
    virtual bool check_sat() const = 0;
    virtual Term get_value(Term& t) const = 0;
    // virtual bool check_sat_assuming() const = 0;
    virtual Sort construct_sort(Kind k) const = 0;
    virtual Sort construct_sort(Kind k, unsigned int size) const = 0;
    virtual Sort construct_sort(Kind k, Sort idxsort, Sort elemsort) const = 0;
    virtual Sort construct_sort(Kind k, std::vector<Sort> sorts, Sort sort) const = 0;
    virtual Term apply_op(PrimOp op, std::vector<Term> terms) const = 0;
    virtual Term apply_op(Op op, std::vector<Term> terms) const = 0;
  };

  using Solver = std::shared_ptr<AbsSolver>;
}

#endif
