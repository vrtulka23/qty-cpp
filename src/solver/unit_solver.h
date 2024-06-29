#ifndef PUQ_SOLVER_H
#define PUQ_SOLVER_H

#include <set>
#include <format>
#include <cmath>

#include "../settings.h"
#include "../../external/exs-cpp/src/exs.h"
#include "../value/unit_value.h"
#include "../lists/lists.h"

class UnitAtom: public exs::AtomBase<UnitValue> {
 public:
  UnitAtom(UnitAtom &a): AtomBase(a) {};
  UnitAtom(UnitValue v): AtomBase(v) {}; 
  static UnitValue from_string(std::string s);
  std::string to_string();
  void math_power(EXPONENT_TYPE &e);
  void math_multiply(UnitAtom *other);
  void math_divide(UnitAtom *other);
};

class OperatorParentheses: public exs::OperatorGroup<UnitAtom, 1> {
public:
  EXPONENT_TYPE exponent = 1;
  OperatorParentheses(): OperatorGroup<UnitAtom, 1>("par","(",exs::PARENTHESES_OPERATOR) {}
  virtual void parse(exs::Expression &expr);
  void operate_group(exs::TokenListBase<UnitAtom> *tokens);
};

#ifdef MAGNITUDE_ARRAYS

enum CustomOperatorType {
  ARRAY_OPERATOR = exs::NUM_OPERATOR_TYPES
};

class OperatorArray: public exs::OperatorGroup<UnitAtom> {
public:
  OperatorArray():
    OperatorGroup<UnitAtom>("arr", SYMBOL_ARRAY_START, ARRAY_OPERATOR,
			    SYMBOL_ARRAY_START, SYMBOL_ARRAY_SEPARATOR, SYMBOL_ARRAY_END) {}
  virtual void parse(exs::Expression &expr);
  void operate_group(exs::TokenListBase<UnitAtom> *tokens);
};

#endif

class UnitSolver {
public:
  std::unique_ptr<exs::Solver<UnitAtom>> solver;
  UnitSolver();
  UnitAtom solve(std::string expression);
};

#endif // PUQ_SOLVER_H
