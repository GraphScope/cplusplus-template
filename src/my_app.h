/** Copyright 2020 Alibaba Group Holding Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * 	http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANALYTICAL_ENGINE_APPS_PREGEL_SSSP_PREGEL_H_
#define ANALYTICAL_ENGINE_APPS_PREGEL_SSSP_PREGEL_H_

#include <algorithm>
#include <limits>
#include <string>

#include "vineyard/graph/fragment/arrow_fragment.h"

#include "core/app/pregel/i_vertex_program.h"
#include "core/app/pregel/pregel_compute_context.h"
#include "core/app/pregel/pregel_property_app_base.h"

namespace gs {

template<typename FRAG_T>
class PregelSSSP
    : public IPregelProgram<
          PregelPropertyVertex<FRAG_T, double, double>,
          PregelPropertyComputeContext<FRAG_T, double, double>> {
  using fragment_t = FRAG_T;
      
 public:
  void Init(PregelPropertyVertex<fragment_t, double, double>& v,
            PregelPropertyComputeContext<fragment_t, double, double>& context)
      override {
    v.set_value(std::numeric_limits<double>::max());
  }

  void Compute(grape::IteratorPair<double*> messages,
               PregelPropertyVertex<fragment_t, double, double>& v,
               PregelPropertyComputeContext<fragment_t, double, double>&
                   context) override {
    bool updated = false;
    if (context.superstep() == 0) {
      std::string source_id = context.get_config("src");
      if (v.id() == "1") {
        std::cout << "Source: " << source_id << " v.id() " << v.id() << std::endl;
      }
      if (v.id() == source_id) {
        updated = true;
        v.set_value(0);
      }
    } else {
      double cur_value = v.value();
      double new_value = cur_value;
      for (auto msg : messages) {
        new_value = std::min(new_value, msg);
      }
      if (new_value != cur_value) {
        v.set_value(new_value);
        updated = true;
      }
    }

    if (updated) {
      double dist = v.value();
      for (int label_id = 0; label_id < context.edge_label_num(); label_id++) {
        for (auto& e : v.outgoing_edges(label_id)) {
          double new_dist = dist + static_cast<double>(e.get_int(0));
          v.send(e.vertex(), new_dist);
        }
      }
    }
    v.vote_to_halt();
  }
};

template<typename FRAG_T>
using SSSP = gs::PregelPropertyAppBase<FRAG_T, PregelSSSP<FRAG_T>>;

}  // namespace gs

#endif  // ANALYTICAL_ENGINE_APPS_PREGEL_SSSP_PREGEL_H_

