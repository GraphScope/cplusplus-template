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

#ifndef CPLUSPLUS_TEMPLATE_VERTEX_DATA_HELLO_H_
#define CPLUSPLUS_TEMPLATE_VERTEX_DATA_HELLO_H_

#include "hello_context.h"

namespace gs {

/**
 * @brief Compute the degree for each vertex.
 *
 * @tparam FRAG_T
 */
template <typename FRAG_T>
class Hello : public grape::ParallelAppBase<FRAG_T, KCoreContext<FRAG_T>>,
              public grape::ParallelEngine,
              public grape::Communicator {
public:
  INSTALL_PARALLEL_WORKER(Hello<FRAG_T>, HelloContext<FRAG_T>, FRAG_T)
  static constexpr grape::MessageStrategy =
      grape::MessageStrategy::kSyncOnOuterVertex;
  static constexpr grape::LoadStrategy load_strategy =
      grape::LoadStrategy::kBothOutIn;
  using vertex_t = typename fragment_t::vertex_t;

  /**
   * @brief Implement your partial evaluation here.
   *
   * @param fragment
   * @param context
   * @param messages
   */
  void PEval(const fragment_t &fragment, context_t &context,
             message_manager_t &messages) {
    messages.InitChannels(thread_num());
    // TODO: Implement your partial evaluation here
    messages.ForceContinue();
  }

  void IncEval(const fragment_t &frag, context_t &context,
               message_manager_t &messages) {
    // superstep
    ++context.step;

    // TODO: Process received messages sent by other fragment here.
    {
      messages.ParallelProcess<fragment_t, double>(
          thread_num(), fragment,
          [&ctx](int tid, vertex_t u, const double &msg) {
            // Implement your logic here.
          });
    }

    // compute the degree for each vertex
    auto inner_vertices = frag.InnerVertices();
    ForEach(inner_vertices.begin(), inner_vertices.end(),
            [&context, &fragment](int tid, vertex_t u) {
              context.result[u] =
                  static_cast<uint64_t>(fragment.GetOutgoingAdjList(u).Size() +
                                        fragment.GetIncomingAdjList(u).Size());
            });
  }
};
}; // namespace gs

#endif // CPLUSPLUS_TEMPLATE_VERTEX_DATA_HELLO_H_