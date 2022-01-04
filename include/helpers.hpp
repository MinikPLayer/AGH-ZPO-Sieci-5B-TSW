// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)
#pragma once

#ifndef HELPERS_HPP_
#define HELPERS_HPP_

#include <functional>
#include <random>

#include "types.hpp"

extern std::random_device rd;
extern std::mt19937 rng;

extern double default_probability_generator();

extern std::function<double()> probability_generator;

#endif /* HELPERS_HPP_ */