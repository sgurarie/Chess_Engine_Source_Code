cmake_minimum_required(VERSION 3.22)
project(Ctemplate)

set(CMAKE_CXX_STANDARD 20)

add_executable(Ctemplate main.cpp
        PrintOutput.cpp
        PrintOutput.h
        ChessPiece.cpp
        ChessPiece.h
        ChessMove.cpp
        ChessMove.h
        AI.cpp
        AI.h
        Bitboard.cpp
        Bitboard.h
)
