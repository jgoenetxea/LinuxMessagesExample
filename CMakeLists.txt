CMAKE_MINIMUM_REQUIRED( VERSION 3.0 )

project(ExampleMessages)

#---------------------------------------------------#
#              Project Files                        #
#---------------------------------------------------#
# List source files in a variable so we can use it later more conveniently
SET(SERVER_SRC "server.cpp" )
SET(CLIENT_SRC "client.cpp" )

set(CMAKE_DEBUG_POSTFIX "_d")

add_executable(${PROJECT_NAME}_server ${SERVER_SRC})
add_executable(${PROJECT_NAME}_client ${CLIENT_SRC})

# set c++14 as the used c++ standard for this library
SET_PROPERTY(TARGET ${PROJECT_NAME}_server PROPERTY CXX_STANDARD 14)
SET_PROPERTY(TARGET ${PROJECT_NAME}_server PROPERTY CXX_STANDARD_REQUIRED ON)

SET_PROPERTY(TARGET ${PROJECT_NAME}_client PROPERTY CXX_STANDARD 14)
SET_PROPERTY(TARGET ${PROJECT_NAME}_client PROPERTY CXX_STANDARD_REQUIRED ON)