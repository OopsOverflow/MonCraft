#pragma once

#include <memory>

/**
 * A sequential execution of functions (function composition).
 *
 * A pipeline is a Spec for a sequential execution of transforms.
 * The (n)th link output is connected to the (n+1)th link input.
 * A pipeline run converts an input value to an output value by going througn
 * each link in sequence.
 */

template <typename F1, typename F2>
class Pipeline {
private:
  F1 f1;
  F2 f2;

public:
  Pipeline(F1 f1, F2 f2) : f1(std::move(f1)), f2(std::move(f2)) {}

  /**
   * Runs the pipeline.
   *
   * @param in: The input value
   * @return The input value transformed by all the links in serial
   */
  template <typename In>
  auto operator()(In i) {
    return f2(f1(i));
  }
};

template <typename F1, typename F2>
auto make_pipeline(F1 f, F2 g) {
  return Pipeline<F1, F2>(f, g);
}

/**
 * Creates a pipeline.
 *
 * The pipeline's input type is the input type of the first link.
 * The pipeline's output type is the output type of the last link.
 *
 * @param f, rest... the steps, given in order of execution.
 * @see Pipeline
 */
template <typename F1, typename... Fs>
auto make_pipeline(F1 f, Fs... rest) {
  return make_pipeline(f, make_pipeline(rest...));
}
