cmake_minimum_required(VERSION 3.19)
project(Linkage_Editor C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Linkage_Editor
        externalSymbolTable.c
        headers.h
        main.c t_record_tables.c m_record_tables.c)
