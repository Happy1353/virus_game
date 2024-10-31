add_executable(sfml_example_game)
target_link_libraries(sfml_example_game PRIVATE sfml-graphics)
target_compile_features(sfml_example_game PRIVATE cxx_std_20)


target_sources(sfml_example_game PRIVATE
    "main.cpp"
    "engine.h"
    "engine.cpp"
    "common.h"
    "display.h"
    "display.cpp"
    "game_logic.h"
    "game_logic.cpp")

if(WIN32)
    add_custom_command(
        TARGET sfml_example_game
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:sfml_example_game>
        VERBATIM)
endif()
