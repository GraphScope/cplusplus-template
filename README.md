# ⚙️  C++ Library Template for GraphScope

Use this template as an example to build your own c++ application and run on GraphScope cluster! For more information about `GraphScope` please visit the [repo](https://github.com/alibaba/GraphScope) or [website](https://graphscope.io/).

## Getting Started
- Click "Use this template" button on github to create your own repository.
- Use the following instructions below to build the project.
- Make sure to change `<username>/<repo-name>` to your repository.

```bash
git clone https://github.com/<username>/<repo-name>.git
cd <repo-name>
mkdir build && cd build
cmake ..
make -j4          # build dynamic library
make vertex_data  # package the vertex_data app as graphscope-recognized resource
make test         # run and test app on graphscope
```

### Code Base Explained
Very simple directory structure, add your source files to `src`. We provide an example of `vertex_data/hello.h`.
```
.
├── CMakeLists.txt ➝ Set this project as a library and package app as graphscope-recognized resource
├── README.md
├── cmake
│   └──  FindGraphScope.cmake ➝ Fetch and find graphscope module
└── src
    └── vertex_data 
        └──  hello.h ➝ algorithm definition
        └──  hello_context.h ➝ context definition used by algorithm
        └──  conf.yaml ➝ configuration
```
