
### BOOST (1.62 min)

set(Boost_USE_STATIC_LIBS ON) # only find static libs
set(Boost_USE_MULTITHREADED ON)

find_package(Boost 1.62 COMPONENTS program_options system thread)

option(ENABLE_LOG "If logging needed" ON)

### LOGGER (log4cplus)
if (ENABLE_LOG)
  if (EXISTS "$ENV{LOG4CPLUS_DIRECTORY}/lib/liblog4cplus.a")
    set(LOGGER_HOME $ENV{LOG4CPLUS_DIRECTORY})
    if (EDISON_BUILD)
      set(LOGGER_HOME "$ENV{LOG4CPLUS_DIRECTORY}i586/")
    endif()
    add_definitions(-DENABLE_LOG)
    set(LOG4CPLUS_INCLUDE_DIR "${LOGGER_HOME}include")
    set(LOG4CPLUS_LIB_DIR "${LOGGER_HOME}lib")
    include_directories ( ${LOG4CPLUS_INCLUDE_DIR} )
  else()
    message(FATAL_ERROR " LOG4CPLUS is not present. Please install log4cplus logger ver. >= 2.0.0!")
  endif()
endif(ENABLE_LOG)


### TESTS (gtest)
if(BUILD_TESTS)
    add_definitions(-DBUILD_TESTS)
    set(GTEST_INCLUDE_DIR ${3RD_PARTY_PATH}/googletest)
    include_directories(
        ${GTEST_INCLUDE_DIR}
    )
    enable_testing()
endif()


### ARM build (arm dependent from another libs)
if (ARM_BUILD)
  set(Boost_LIBRARIES {Boost_EDISON_LIBRARIES})
endif()



### LIBRARY BUIDING
include_directories(
  ${3RD_PARTY_PATH}
  include
  ${GTEST_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR}
)
set(BOOST_INCLUDE ${Boost_INCLUDE_DIR} PARENT_SCOPE)
set(BOOST_LIBS ${Boost_LIBRARIES} PARENT_SCOPE)

file(GLOB SOURCES
  src/*.cc
  src/*/*.cc
  src/*/*/*.cc)

set(LIBRARIES
    ${Boost_LIBRARIES}
)

if(ENABLE_LOG)
  list(APPEND LIBRARIES log4cplus -L${LOG4CPLUS_LIB_DIR})
endif()

add_library("Utils" ${SOURCES})
target_link_libraries("Utils" ${LIBRARIES})

