file(REMOVE_RECURSE include/shaders)

file(GLOB SHADER_FILES *.glsl)

foreach(SHADER_FILE ${SHADER_FILES})
    file(
        READ 
        ${SHADER_FILE} 
        SHADER_FILE_CONTENT
    )

    string(
        REPLACE
        "\n"
        "\\n\"\n\""
        SHADER_FILE_CONTENT
        "${SHADER_FILE_CONTENT}"
    )

    cmake_path(GET SHADER_FILE STEM SHADER_FILE_NAME)

    file(
        WRITE 
        include/shaders/${SHADER_FILE_NAME}.generated.hpp
        "#ifndef ${SHADER_FILE_NAME}_HEADER\n"
        "#define ${SHADER_FILE_NAME}_HEADER\n"
        "const char *const ${SHADER_FILE_NAME} =\n\"${SHADER_FILE_CONTENT}\\0\";\n"
        "#endif\n\n"
    )
endforeach()

add_library(shaders INTERFACE)
target_include_directories(shaders INTERFACE include)