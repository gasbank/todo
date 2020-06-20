
file(GLOB LUA_SOURCES src/*.c)

add_library(lua STATIC ${LUA_SOURCES})

