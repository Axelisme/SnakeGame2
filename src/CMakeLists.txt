cmake_minimum_required(VERSION 3.5.0)
project(Final_CMake VERSION 0.1.0)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

# Debug模式
set(CMAKE_VERBOSE_MAKEFILE OFF)

# CWD設定
SET(WORKING_DIRECTORY ..)
message("Working directory: ${WORKING_DIRECTORY}")

# 新增原始檔列表變數
SET(SRC_DIR ${WORKING_DIRECTORY}/src)
aux_source_directory(${SRC_DIR} SRC_LIST1)
aux_source_directory(${SRC_DIR}/Entity SRC_LIST2)
aux_source_directory(${SRC_DIR}/Object SRC_LIST3)
aux_source_directory(${SRC_DIR}/Interface SRC_LIST4)
aux_source_directory(${SRC_DIR}/Interface/Menu SRC_LIST5)
aux_source_directory(${SRC_DIR}/Interface/Level SRC_LIST6)
aux_source_directory(${SRC_DIR}/Utility SRC_LIST7)
SET(SRC_LIST ${SRC_LIST1} ${SRC_LIST2} ${SRC_LIST3} ${SRC_LIST4} ${SRC_LIST5} ${SRC_LIST6} ${SRC_LIST7})
message("Source directory: ${SRC_LIST}")

# 指定庫檔案搜尋目錄
link_directories(${WORKING_DIRECTORY}/lib)
link_directories(${WORKING_DIRECTORY}/allegro_mac/lib)
set(LINK_LIB -lallegro_image -lallegro_ttf -lallegro_font -lallegro_dialog -lallegro_primitives -lallegro_acodec -lallegro_audio -lallegro -lallegro_main -lm)
link_libraries(${LINK_LIB})

# 指定標頭檔案目錄
INCLUDE_DIRECTORIES(${WORKING_DIRECTORY}/include)
INCLUDE_DIRECTORIES(${WORKING_DIRECTORY}/allegro_mac/include)

# 生成可執行檔案
SET(EXE_NAME SnakeGame)
SET(MAIN_SRC ${SRC_DIR}/Main/Main.c)
set(EXECUTABLE_OUTPUT_PATH ${WORKING_DIRECTORY}/bin/${CMAKE_BUILD_TYPE})
add_executable(${EXE_NAME} ${MAIN_SRC} ${SRC_LIST})
message("Excutable file: ${EXECUTABLE_OUTPUT_PATH}/${EXE_NAME}")

