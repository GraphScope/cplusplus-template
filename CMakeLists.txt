project(cplusplus_application_example C CXX)
cmake_minimum_required(VERSION 3.5)

include(GNUInstallDirs)
include(CheckCXXCompilerFlag)

# cmake bulld type----------------------------------------------------------
set(DEFAULT_BUILD_TYPE "RelWithDebInfo")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()

# cmake cxx standard---------------------------------------------------------
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O0 -g")

# cmake cxx compiler flags---------------------------------------------------
check_cxx_compiler_flag(-Wno-deprecated-declarations W_NO_DEPRECATED_DECLARATIONS)
check_cxx_compiler_flag(-Wno-attributes W_NO_ATTRIBUTES)
check_cxx_compiler_flag(-Wno-sign-compare W_NO_SIGN_COMPARE)
check_cxx_compiler_flag(-Wno-class-memaccess W_NO_CLASS_MEMACCESS)
check_cxx_compiler_flag(-Wno-undef-prefix W_NO_UNDEF_PREFIX)
check_cxx_compiler_flag(-Wno-unused-result W_NO_UNUSED_RESULT)
check_cxx_compiler_flag(-Wno-unused-lambda-capture W_NO_UNUSED_LAMBDA_CAPTURE)
check_cxx_compiler_flag(-Wno-unused-but-set-variable W_NO_UNUSED_BUT_SET_VARIABLE)
check_cxx_compiler_flag(-Wno-unknown-pragmas W_NO_UNKNOWN_PRAGMAS)
check_cxx_compiler_flag(-Wno-stringop-overflow W_NO_STRINGOP_OVERFLOW)
check_cxx_compiler_flag(-Wno-unused-variable W_NO_UNUSED_VARIABLE)
check_cxx_compiler_flag(-Wno-redundant-move W_NO_REDUNDANT_MOVE)

if(W_NO_CLASS_MEMACCESS)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-class-memaccess")
endif()
if (W_NO_DEPRECATED_DECLARATIONS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
endif()
if (W_NO_ATTRIBUTES)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes")
endif()
if (W_NO_SIGN_COMPARE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare")
endif()
if (W_NO_UNDEF_PREFIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-undef-prefix")
endif()
if (W_NO_UNUSED_RESULT)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-result")
endif()
if (W_NO_UNUSED_LAMBDA_CAPTURE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-lambda-capture")
endif()
if (W_NO_UNUSED_BUT_SET_VARIABLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-but-set-variable")
endif()
if (W_NO_UNKNOWN_PRAGMAS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
endif()
if (W_NO_STRINGOP_OVERFLOW)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-stringop-overflow")
endif()
if (W_NO_UNUSED_VARIABLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")
endif()
if (W_NO_REDUNDANT_MOVE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-redundant-move")
endif()

# eliminate a lot of warnings for newer version of boost library.
add_compile_options(-DBOOST_BIND_GLOBAL_PLACEHOLDERS)

# rpath----------------------------------------------------------------------
if (APPLE)
  set(CMAKE_MACOSX_RPATH ON)
else ()
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,$ORIGIN")
endif ()

# TODO find analytical engine home---------------------------------------------
set(ANALYTICAL_ENGINE_HOME "/usr/local/lib/python3.9/site-packages/graphscope.runtime/")
message (STATUS "ANALYTICAL_ENGINE_HOME is ${ANALYTICAL_ENGINE_HOME}")
include("${ANALYTICAL_ENGINE_HOME}/${CMAKE_INSTALL_LIBDIR}/cmake/graphscope-analytical/graphscope-analytical-config.cmake" OPTIONAL)
if("${GRAPHSCOPE_GCC_ABI_BACKWARDS_COMPATIBLE}" STREQUAL "0")
  add_definitions(-D_GLIBCXX_USE_CXX11_ABI=1)
elseif("${GRAPHSCOPE_GCC_ABI_BACKWARDS_COMPATIBLE}" STREQUAL "1")
  add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
endif()

# Include directories----------------------------------------------------------
include_directories("${ANALYTICAL_ENGINE_HOME}/openmpi/include"
                    "${ANALYTICAL_ENGINE_HOME}/open-mpi/include"
                    "${ANALYTICAL_ENGINE_HOME}/include"
                    "${ANALYTICAL_ENGINE_HOME}/include/vineyard"
                    "${ANALYTICAL_ENGINE_HOME}/include/graphscope"
                    "${ANALYTICAL_ENGINE_HOME}/include/graphscope/apps"
                    "${ANALYTICAL_ENGINE_HOME}/include/graphscope/proto"
                    "${ANALYTICAL_ENGINE_HOME}/include/grape/analytical_apps")

# Find Threads------------------------------------------------------------------
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_THREAD_PREFER_PTHREAD ON)
find_package(Threads REQUIRED)

add_compile_options(-DENABLE_SELECTOR)

# Still need openmp, as we use it in ParallelMessageManager from libgrape-lite. But we just need
# the preprocessor options
if (APPLE)
    if (POLICY CMP0056)
        cmake_policy(SET CMP0056 NEW)
    endif ()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/usr/local/lib")
endif ()
find_package(OpenMP QUIET)

# set frame dir --------------------------------------------------------------------
set(ANALYTICAL_ENGINE_FRAME_DIR "${ANALYTICAL_ENGINE_HOME}/include/graphscope/frame")

if (APPLE AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -undefined dynamic_lookup")
endif()

# TODO Add library
add_library(${PROJECT_NAME} SHARED ${ANALYTICAL_ENGINE_FRAME_DIR}/app_frame.cc)
target_compile_definitions(${PROJECT_NAME} PRIVATE _GRAPH_TYPE=gs::ArrowProjectedFragment<int64_t,uint64_t,grape::EmptyType,grape::EmptyType>
																									 _GRAPH_HEADER=core/fragment/arrow_projected_fragment.h
                                                   _APP_TYPE=gs::Hello<_GRAPH_TYPE> _APP_HEADER=${CMAKE_CURRENT_SOURCE_DIR}/src/vertex_data/hello.h)
# target_include_directories(${FRAME_NAME} PRIVATE utils apps)
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-fPIC")

if (OpenMP_FOUND)
    target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)
endif ()
