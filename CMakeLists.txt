# ======================================================================
#  larrecodnn main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/larrecodnn/ups/setup_for_development <-d|-p>
#  buildtool [-I /install/path] -bt[i] -jN
# ======================================================================

cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
# cmake_policy(VERSION 3.19) ### MIGRATE-ACTION-COMPATIBILITY (migrate-3.16.00) - Uncomment to preserve compatibility with older CMake versions

find_package(cetmodules REQUIRED)
project(larrecodnn VERSION 09.11.10 LANGUAGES CXX)

# cetbuildtools contains our cmake modules
find_package(Threads)

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS CAUTIOUS
  WERROR
  NO_UNDEFINED
  EXTRA_FLAGS -Wno-unused-local-typedefs
)

cet_report_compiler_flags() ### MIGRATE-ACTION-RECOMMENDED (migrate-3.16.00) - add args: REPORT_THRESHOLD VERBOSE

find_package(Range-v3 REQUIRED EXPORT)
find_package (Eigen3 3.3...3.3.9 REQUIRED NO_MODULE)
find_package(canvas_root_io REQUIRED)
find_package(art REQUIRED EXPORT)
find_package(art_root_io REQUIRED EXPORT)
find_package(nusimdata REQUIRED EXPORT)
find_package(nurandom REQUIRED EXPORT)
find_package(nug4 REQUIRED EXPORT)
find_package(nutools REQUIRED EXPORT)
find_package(PostgreSQL REQUIRED EXPORT)
find_package(ROOT COMPONENTS Core Minuit Minuit2 REQUIRED EXPORT)

find_package( larcoreobj REQUIRED )
find_package( larcorealg REQUIRED )
find_package( larcore REQUIRED )
find_package( lardataobj REQUIRED )
find_package( lardataalg REQUIRED )
find_package( lardata REQUIRED )
find_package( larevt REQUIRED )
find_package( larsim REQUIRED )
find_package( larreco REQUIRED )

# macros for dictionary and simple_plugin
include(ArtDictionary)
include(ArtMake)
include(BuildPlugins)

if(( DEFINED ENV{TENSORFLOW_DIR} ) OR ( DEFINED ENV{TRITON_DIR} ))
  find_package(Protobuf REQUIRED)
endif ()
# tensorflow is optional
if( DEFINED ENV{TENSORFLOW_DIR} )
  # find tensorflow library
  find_package( tensorflow REQUIRED )
  cet_find_library(TENSORFLOW NAMES tensorflow PATHS ENV TENSORFLOW_LIB NO_DEFAULT_PATH ) ### MIGRATE-ACTION-REQUIRED (migrate-3.16.00) - avoid: use find_package() with custom Find<pkg>.cmake for Spack compatibility
  cet_find_library(FOR_TENSORFLOW NAMES tensorflow_framework PATHS ENV TENSORFLOW_LIB NO_DEFAULT_PATH ) ### MIGRATE-ACTION-REQUIRED (migrate-3.16.00) - avoid: use find_package() with custom Find<pkg>.cmake for Spack compatibility
endif ()
# trtis_clients is optional
if( DEFINED ENV{TRITON_DIR} )
  find_package(triton REQUIRED)
  find_package(grpc REQUIRED)
endif ()

include_directories( $ENV{EIGEN_INC} ) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.16.00) - avoid directory-scope functions: use target_link_libraries() with target semantics or target_include_directories() whenever possible

# source
add_subdirectory(larrecodnn)

# ups - table and config files

# packaging utility
cet_cmake_config()
