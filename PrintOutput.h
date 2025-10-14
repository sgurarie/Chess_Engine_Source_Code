//
// Created by Sam Gurarie on 3/28/24.
//

#include <string>
#include <iostream>
#include "AI.h"

using namespace std;

void printBoard();
void setup();
void loadFile(string const& file_name);
void playerMove();
ll getMemoryUsage();

extern thread_local RootChessMoveTree rootChessMoveTree;