#include <iostream>
#include <array>
#include <bitset>
#include <chrono>

// N = number of rows
// M = number of columns
template<std::size_t N, std::size_t M>
using BitGrid = std::array<std::bitset<M>, N>;

template<std::size_t N, std::size_t M>
static std::size_t numPaths1(std::size_t i, std::size_t j, BitGrid<N, M>& grid)
{
	if(i >= M || j >= N || grid[j][i])
		return 0;
	grid[j][i] = true;
	if((i + 1) == M && (j + 1) == N)
	{
		grid[j][i] = false;
		return 1;
	}
	auto paths = numPaths1<N, M>(i + 1, j, grid) + numPaths1<N, M>(i, j + 1, grid);
	grid[j][i] = false;
	return paths;
}

template<std::size_t N, std::size_t M>
static std::size_t numPaths1()
{
	BitGrid<N, M> grid;
	return numPaths1<N, M>(0, 0, grid);
}

template<std::size_t N, std::size_t M>
struct Solution1
{
	std::size_t operator()()
	{
		return numPaths1<N, M>();
	}
};

template<template<std::size_t, std::size_t> typename Sol, std::size_t N, std::size_t M>
static void runNumPaths()
{
	std::cout << "Input: { Rows = " << N << ", Columns = " << M << " }\n";
	auto start = std::chrono::steady_clock::now();
	std::size_t paths = Sol<N, M> { }();
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(end - start).count();
	std::cout  << "Output: " << paths << " paths\n";
	std::cout << "Time taken: " << elapsed << " ms\n";
}

template<std::size_t N, std::size_t M>
static void run()
{
	static std::size_t runCount = 0;
	++runCount;
	std::cout << "------ RUN: " << runCount << " ---------\n";
	runNumPaths<Solution1, N, M>();
}

int main()
{
	run<1, 1>();
	run<0, 0>();
	run<0, 1>();
	run<2, 2>();
	run<1, 2>();
	run<2, 1>();
	run<3, 3>();
	run<4, 5>();
	run<6, 6>();
	run<7, 8>();
	run<10, 10>();
	run<12, 12>();
	return 0;
}
