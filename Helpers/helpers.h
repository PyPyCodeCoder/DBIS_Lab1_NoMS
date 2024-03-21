#ifndef LAB1_FUNCTIONS_H
#define LAB1_FUNCTIONS_H
#pragma once
#include <iostream>
#include <vector>
#include <fstream>

extern std::fstream STUDIO_FILE;
extern std::fstream IND_FILE;
extern std::fstream FILM_FILE;

std::fstream getFile(const std::string& filepath);
std::vector<std::string> parseInput(std::string input);
#endif //LAB1_FUNCTIONS_H
