// Copyright 2019 Apex.AI, Inc.
// Co-developed by Tier IV, Inc. and Apex.AI, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPTIMIZATION__LINE_SEARCH_HPP_
#define OPTIMIZATION__LINE_SEARCH_HPP_

#include <helper_functions/crtp.hpp>
#include <optimization/visibility_control.hpp>
#include <limits>
#include <cmath>

namespace autoware
{
namespace common
{
namespace optimization
{
/// Base class (CRTP) to mange the step length during optimization.
template<typename Derived>
class OPTIMIZATION_PUBLIC LineSearch : public common::helper_functions::crtp<Derived>
{
public:
  // TODO(zozen): should this be forced to be positive?
  using StepT = float_t;

  /// Constructor.
  /// \param step_max Maximum step length. By default initialized to the minimum value.
  explicit LineSearch(const StepT step_max = std::numeric_limits<StepT>::min())
  {
    m_step_max = step_max;
  }

  /// Computes the optimal step length for the optimization problem
  /// \tparam OptimizationProblemT Optimization problem type. Must be an
  /// implementation of `common::optimization::OptimizationProblem`.
  /// \param optimization_problem optimization objective
  /// \return The resulting step length.
  template<typename OptimizationProblemT>
  StepT compute_step_length(OptimizationProblemT & optimization_problem)
  {
    return this->impl().compute_step_length_(optimization_problem);
  }

  /// Getter for the maximum step length
  /// \return The maximum step length.
  StepT get_step_max() const noexcept
  {
    return m_step_max;
  }

  /// Setter for the maximum step length
  /// \param step_max the new maximal step length
  void set_step_max(const StepT step_max) noexcept
  {
    m_step_max = step_max;
  }

private:
  StepT m_step_max;
};

/// Class to use a fixed step length during optimization.
class OPTIMIZATION_PUBLIC FixedLineSearch : public LineSearch<FixedLineSearch>
{
public:
  /// Constructor.
  /// \param step Fixed step to be used.
  explicit FixedLineSearch(const StepT step = std::numeric_limits<StepT>::min())
  : LineSearch(step) {}
  /// Returns directly the pre-set (maximum) step length
  /// \return The fixed step length.
  template<typename OptimizationProblemT>
  StepT compute_step_length_(OptimizationProblemT &) const noexcept
  {
    return get_step_max();
  }
};

}  // namespace optimization
}  // namespace common
}  // namespace autoware

#endif  // OPTIMIZATION__LINE_SEARCH_HPP_
