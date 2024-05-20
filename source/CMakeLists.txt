# TODO 14: Remove mysqrt.cxx from the list of sources

# TODO 1: Add a library called MathFunctions with sources MathFunctions.cxx
# and mysqrt.cxx
# Hint: You will need the add_library command
set(SOURCES
    textureHandler.cpp
    lodepng.cpp
    
)

set(HEADERS
     textureHandler.h
)
add_library(lodepng ${SOURCES} ${HEADERS})


