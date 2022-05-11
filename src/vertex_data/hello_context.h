/** Copyright 2022 Alibaba Group Holding Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CPLUSPLUS_TEMPLATE_VERTEX_DATA_HELLO_CONTEXT_H_
#define CPLUSPLUS_TEMPLATE_VERTEX_DATA_HELLO_CONTEXT_H_

#include "grape/grape.h"

namespace gs {

/**
 * @brief Vertex Data Context for "Hello" Application.
 *
 * @tparam FRAG_T
 */
template <typename FRAG_T>
class HelloContext : public grape::VertexDataContext<FRAG_T, uint64_t> {
  using oid_t = typename FRAG_T::oid_t;
  using vid_t = typename FRAG_T::vid_t;
  using vertex_t = typename FRAG_T::vertex_t;

 public:
  explicit HelloContext(const FRAG_T &fragment)
      : grape::VertexDataContext<FRAG_T, uint64_t>(fragment, true),
        result(this->data()) {}

  void Init(grape::ParallelMessageManager &messages, int degree) {
    // record current superstep
    this->step = 0;
    this->degree = degree;
    // init results
    result.SetValue(0);
  }

  int step = 0;
  int degree = 0;

  typename FRAG_T::template vertex_array_t<uint64_t>& result;
};
} // namespace gs

#endif // CPLUSPLUS_TEMPLATE_VERTEX_DATA_HELLO_CONTEXT_H_
