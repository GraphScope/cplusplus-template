# ⚙️  C++ Application Template for GraphScope

[![C++ Library Template CI](https://github.com/GraphScope/cpp-template/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/GraphScope/cpp-template/actions/workflows/ci.yml)

Use this template repo as an example to build your own C++ application and run on GraphScope! 

Please note that this template is designed for applications with **a scalar result for each vertex**, e.g. the `rank value` on each vertex for [PageRank algorithm](https://en.wikipedia.org/wiki/PageRank).

## Getting Started

### Step 1. Install `GraphScope`

```bash
pip3 install graphscope 
```

### Step 2. Use this template to develop your own application

Click "Use this template" button up-right.t to create your own repository, clone it to local.

```bash
# Make sure to replace the <username> and <repo-name> to the right values.

git clone https://github.com/<username>/<repo-name>.git 
cd <repo-name>
```

If you want to build and run the example app, skip this step below and go to Step 3.
Otherwise, develop your own application by revising files in `src`.

- `my_app.h`: Implement your algorithm logic here, e.g., `PEval` and `IncEval` in PIE model.
- `my_app_context.h`: Auxiliary data structures for the intermediate data across iterations. 


### Step 3. Build and test your application

```bash
make build & cd build
cmake .. && make            # build the application.
make package                # package as a graphscope-recognized resource (.gar)
make test                   # run a simple test. 
```
### Step 4. How to run `.gar` file on GraphScope

Here is an example to run the packaged gar file in GraphScope Python interface.

```python3
import graphscope

from graphscope.framework.app import load_app
g = graphscope.g(directed=False, generate_eid=False, vertex_map="local")
g = g.add_edges(
    f"/Users/siyuan/CLionProjects/gstest/property/p2p-31_property_e_0",
    label="knows",
    src_label="person",
    dst_label="person",
)

my_app = load_app('/Users/siyuan/CLionProjects/cpp-template/build/my_app.gar')
result = my_app(g, src=1)

df = result.to_dataframe(selector={'id': 'v:person.id', 'r': 'r:person'}).sort_values(by='id')
print(df)
```

## Codebase Explained
In the `src` folder, we provide a simple example of `my_app`, which computing the degree for each vertex.
```
.
├── CMakeLists.txt
├── FindGraphScope.cmake ➝  find graphscope module
├── README.md
└── src
    ├── my_app.h ➝ algorithm logics
    └── my_app_context.h ➝ context with auxiliary data for the algorithm

```

## Useful Links

- [GraphScope analytical engine](https://graphscope.io/docs/analytics_engine.html)
- [APIs for writing application](https://graphscope.io/docs/reference/analytical_engine_index.html#)
