cmake_minimum_required(VERSION 3.5)
project(lab2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This command finds and makes library targets like 'glad' and 'glfw' available
add_subdirectory(external)

# --- Target: lab2_building ---
add_executable(lab2_building
		lab2/lab2.cpp
		lab2/render/shader.cpp
)
# Explicitly tell this target where to find ALL necessary header files
target_include_directories(lab2_building PRIVATE
		"${PROJECT_SOURCE_DIR}/lab2"  # <-- ADD THIS LINE for render/shader.h
		"${PROJECT_SOURCE_DIR}/external/glfw-3.1.2/include"
		"${PROJECT_SOURCE_DIR}/external/glm-0.9.7.1"
		"${PROJECT_SOURCE_DIR}/external"  # For stb_image.h
)
# Link the compiled libraries to the executable
target_link_libraries(lab2_building PRIVATE
		glad
		glfw
		${OPENGL_LIBRARIES}
)
