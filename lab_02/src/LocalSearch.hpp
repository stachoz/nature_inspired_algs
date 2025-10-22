//
// Created by dcend on 22.10.2025.
//

#ifndef ABSTRACTLOCALSEARCHALGORITM_H
#define ABSTRACTLOCALSEARCHALGORITM_H
#include <vector>


template<typename T>
class LocalSearch {
public:
  virtual T find_solution(T starting_solution) = 0;

  virtual ~LocalSearch() = default;

protected:
  /**
   *
   * @param solution
   * @return evaluation of given solution
   */
  virtual double evaluation_function(T solution) = 0;

  /**
   *
   * @param solution
   * @return all neighbors of given solution
   */
  virtual std::vector<T> neighborhood_function(T solution) = 0;

  /**
   *
   * @param solution
   * @return true if solution met stopping condition, otherwise false
   */
  virtual bool is_stopping_condition_met(T solution) = 0;
};


#endif //ABSTRACTLOCALSEARCHALGORITM_H
