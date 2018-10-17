# CMake module to search for the libyaml library
# (library for parsing YAML files)
#
# If it's found it sets LIBYAML_FOUND to TRUE
# and following variables are set:
#    LIBYAML_INCLUDE_DIR
#    LIBYAML_LIBRARY


FIND_PATH(LIBYAML_INCLUDE_DIR NAMES yaml.h)
FIND_LIBRARY(LIBYAML_LIBRARIES NAMES yaml-cpp libyaml-cpp)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Yaml DEFAULT_MSG LIBYAML_LIBRARIES LIBYAML_INCLUDE_DIR)
MARK_AS_ADVANCED(LIBYAML_INCLUDE_DIR LIBYAML_LIBRARIES)
