# ⚙️  C++ Library Template for GraphScope

Use this template as an example to build your own c++ application and run on GraphScope cluster! For more detail about the [Analytical Engine](https://graphscope.io/docs/analytics_engine.html), please visit the [API Doc](https://graphscope.io/docs/reference/analytical_engine_index.html#).

Note that this template is used for application with **only one result for each vertex**, e.g. the `rank value` of each vertex for [PageRank Algorithm](https://en.wikipedia.org/wiki/PageRank).

## Getting Started
- Click "Use this template" button on github to create your own repository.
- Use the following instructions below to build the project.
- Make sure to change `<username>/<repo-name>` to your repository.

```bash
pip3 install graphscope    # install graphscope first
git clone https://github.com/<username>/<repo-name>.git
cd <repo-name>
mkdir build && cd build    # with example algorithm: vertex_degree
cmake .. && make -j4       # build dynamic library
make package               # package as a graphscope-recognized resource (.gar)
make test                  # run and test app on graphscope
```

### Develop Your Own Application

**Step-1: Modify the file under the `src` directory and implement your own algorithm logic**
- `my_app_context.h`: Used to record the result or the intermediate data of each iteration of the algorithm
- `my_app.h`: Implement your algorithm logic here

**step-2: build and test your application**
```bash
cd build && cmake ..
make package && make test
```
**Step-3: Run the GAR resource on GraphScope**
```python3
import graphscope

from graphscope.framework.app import load_app
from graphscope.dataset import load_p2p_network

sess = graphscope.session(cluster_type="hosts")
simple_graph = load_p2p_network(sess)._project_to_simple()

my_app = load_app('<path_to_your_gar_resource>')
context = my_app(simple_graph, 10)  # pass 10 as param1 defined in 'MyAppContext.h'

print(context.to_numpy('r'))
```

### Code Base Explained
Very simple directory structure, add your source files to `src`. We provide an example of `vertex_data`.
```
.
├── CMakeLists.txt ➝ Set this project as a library and package app as graphscope-recognized resource
├── FindGraphScope.cmake ➝ Fetch and find graphscope module
├── README.md
└── src
    ├── my_app_context.h ➝ context definition used by algorithm
    └── my_apph ➝ algorithm definition
```
