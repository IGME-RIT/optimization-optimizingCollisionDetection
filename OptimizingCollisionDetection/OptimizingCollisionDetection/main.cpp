/*
Title: Optimizing Collision Detection
File Name: main.cpp
Copyright © 2016
Original authors: Luna Meier
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Description:
This example is meant to be used as an introduction to the concepts involved in optimizing
code.  In it we explore a number of techniques used for optimization, as well as discuss the
thought process one should use whilst trying to optimize code.

The examples start with very simple common optimizations, then go into a discussion of
how to program while considering how computers work.  After that we discuss SIMD operations
and show how using them correctly can ultimately lead to massive gains in speed.  Finally
we utilize inline assembly to tell the computer exactly how the program should work, leading
to incredibly huge increases in speed.

While reading this guide you should not necessarily expect to understand all of it.  Some
potions, like using SIMD and Assembly, are very complex and foreign to someone used to very
high level programming.  It's suggested that the average user of this tutorial read up to
the section on SIMD programming, and then give SIMD a try at the very least.  Within
this tutorial series we will be writing other tutorials that cover concepts like SIMD
in detail; this example is NOT meant to be a tutorial on writing SIMD instructions in
Visual Studio, nor a guide on how to write inline assembly.

It's worth noting that over the course of this example we took a process that took
27 seconds to run 1000 times and brought it down to 0.19 seconds to run 1000 times.

References:
http://www.cs.virginia.edu/~evans/cs216/guides/x86.html
https://msdn.microsoft.com/en-us/library/t467de55(v=vs.90).aspx
*/

#include <conio.h>
#include "BasicCircle.h"
#include "OptimizedCircle.h"
#include "MoreOptimizedCircle.h"
#include "LoopOptimizedCircles.h"
#include "DataOptimizedCircles.h"
#include "SIMDOptimizedCircles.h"
#include "AssemblyOptimizedCircles.h"
#include "HelperFunctions.h"
#include "Settings.h"


int main(){	

#pragma region SETUP
	// The way this example is setup I created a separate class to show off each test.
	// Each test changes more or less a single thing from the previous example.
	// To read this guide just follow the instructions in each section, starting with TEST ONE
	BasicCircle basicCircles[NUM_CIRCLES];
	OptimizedCircle optimizedCircles[NUM_CIRCLES];
	MoreOptimizedCircle moreOptimizedCircles[NUM_CIRCLES];
	LoopOptimizedCircles loopOptimizedCircles;
	DataOptimizedCircles dataOptimizedCircles;
	SIMDOptimizedCircles simdOptimizedCircles;
	AssemblyOptimizedCircles assemblyOptimizedCircles;

	// This is just setting up a jagged array to store the results in.  In an actual
	// simulation this would let you resolve collisions after the fact, so I figured it
	// was worth throwing in.
	bool* isCollided[NUM_CIRCLES];
	for (int i = 0; i < NUM_CIRCLES; ++i){
		// I use malloc later in the program so I figured I'd just use it throughout the whole
		// program.  There's no optimization reason I'm using malloc over new.
		isCollided[i] = (bool*)malloc(sizeof(bool) * NUM_CIRCLES);
		memset(isCollided[i], 0, sizeof(bool) * NUM_CIRCLES);
	}

#pragma endregion Setup for the rest of the section.

//#pragma region TEST_ONE
//
//	// You'll notice throughout this code there are references to the Helper namespace.
//	// I wrote just a couple of very very simple static helper functions so I could
//	// properly randomly generate data, as well as easily start and stop the timer I was
//	// using.  If you want to see what's in helper go to HelperFunctions.h
//	// If you'd like to use Visual Studio Profiler for your tests instead feel free to.
//	// I also suggest running in release when you're not using breakpoints or debugging
//	// as sometimes debug mode tends to throw some stuff in that messes with your results.
//	// To run in release click on the Visual Studio's menu at the top of the screen
//	// Debug->Start Without Debugging, or press ctrl-F5
//	Helper::StartTimer();
//	
//	// In order to get statistically significant results we'll need to run this code
//	// a large number of times to exacerbate any differences.  I set the number of iterations
//	// to 1000 tests, and then the number of circles to do collisions of to 1000.  If you'd
//	// like to change these values to see what happens go to Settings.h
//	for (int test = 0; test < ITERATIONS; ++test){
//		// This code is fairly basic.  We just update each circle, and check if each
//		// circle is colliding with every other circle.  We also don't need to test if each
//		// circle is colliding with itself, and we don't need to test ones we've already
//		// tested, so this array gets filled out in only the top right diagonal.
//		// Technically we should be using smaller arrays for each increase in i, but
//		// since that's not the emphasis of what we're optimizing we'll use the same
//		// size of array for every test in this series.
//
//
//		// Let's go into BasicCircle.h and take a look around.
//		for (int i = 0; i < NUM_CIRCLES; ++i){
//			basicCircles[i].Update();
//			for (int j = i + 1; j < NUM_CIRCLES; ++j){
//				isCollided[i][j] = basicCircles[i].CheckForCollision(basicCircles[j]);
//				
//			}
//		}
//	}
//
//	float timeOne = Helper::StopTimer();
//	std::printf("Test One Complete. \n");
//
//#pragma endregion Basic test of non-optimized code.  Base for everything else.
//
//#pragma region TEST_TWO
//
//	// The second test doesn't optimize much.  In fact there's only really one change.
//	// Go into OptimizedCircle.cpp to see it.
//	Helper::StartTimer();
//	for (int test = 0; test < ITERATIONS; ++test){ 
//		for (int i = 0; i < NUM_CIRCLES; ++i){
//			optimizedCircles[i].Update();
//			for (int j = i + 1; j < NUM_CIRCLES; ++j){
//				isCollided[i][j] = optimizedCircles[i].CheckForCollision(optimizedCircles + j);
//			}
//		}
//	}
//
//	float timeTwo = Helper::StopTimer();
//	std::printf("Test Two Complete. \n");
//
//#pragma endregion Test showing off the difference passing by pointer or reference can make.
//
//#pragma region TEST_THREE
//	Helper::StartTimer();
//
//	// Test three makes a slightly more noticeable change.  Go into MoreOptimizedCircle.cpp.
//	for (int test = 0; test < ITERATIONS; ++test){ 
//		for (int i = 0; i < NUM_CIRCLES; ++i){
//			moreOptimizedCircles[i].Update();
//			for (int j = i + 1; j < NUM_CIRCLES; ++j){
//				isCollided[i][j] = moreOptimizedCircles[i].CheckForCollision(moreOptimizedCircles + j);
//			}
//		}
//	}
//
//	float timeThree = Helper::StopTimer();
//	std::printf("Test Three Complete. \n");
//
//#pragma endregion Test showing off the advantage of removing store calls and simplifying code.
//
//#pragma region TEST_FOUR
//
//	// Right away it's obvious that this test makes a big change.
//	// Instead of having our loop outside calling the functions, we're looping through the data
//	// inside of the functions.  Go to LoopOptimizedCircles.h to see how we handled the data.
//	Helper::StartTimer();
//	for (int test = 0; test < ITERATIONS; ++test){
//		loopOptimizedCircles.Update();
//		loopOptimizedCircles.CheckForCollisions();
//	}
//
//	float timeFour = Helper::StopTimer();
//	std::printf("Test Four Complete. \n");
//
//#pragma endregion Test showing off the advantage of pulling operations out of loops.
//
//#pragma region TEST_FIVE
//	Helper::StartTimer();
//
//	// At first glance everything seems the same, but this is one of our biggest speedups.
//	// This optimization is one that any programmer, high level or low level, should be aware of
//	// and consider using.  Go to DataOptimizedCircle.h to see what's happening.
//	for (int test = 0; test < ITERATIONS; ++test){ 
//		dataOptimizedCircles.Update();
//		dataOptimizedCircles.CheckForCollisions();
//	}
//
//	float timeFive = Helper::StopTimer();
//	std::printf("Test Five Complete. \n");
//
//#pragma endregion Test showing off the difference between SOA and AOS data layout.
//
//#pragma region TEST_SIX
//
//	Helper::StartTimer();
//
//	//Everything's inside the code really, 
//	for (int test = 0; test < ITERATIONS; ++test){ 
//
//		simdOptimizedCircles.Update();
//		simdOptimizedCircles.CheckForCollisions();
//	}
//
//	float timeSix = Helper::StopTimer();
//	std::printf("Test Six Complete. \n");
//
//#pragma endregion Test using SIMD operations.

#pragma region TEST_SEVEN
	Helper::StartTimer();
	// Just head into AssemblyOptimizedCircles.cpp.
	for (int test = 0; test < ITERATIONS; ++test){ 
		assemblyOptimizedCircles.Update();
		assemblyOptimizedCircles.CheckForCollisions();
	}

	float timeSeven = Helper::StopTimer();
	std::printf("Test Seven Complete. \n");
#pragma endregion Test using inline assembly for optimization.

	//std::printf("\nBasic Circle Code: %f seconds.\n", timeOne); // Supports ~750 circles at 60FPS
	//// 1.00x
	//std::printf("Passing By Pointer: %f seconds.\n", timeTwo); // Supports ~1000 circles at 60FPS
	//// 1.62x
	//std::printf("Minimized Code: %f seconds.\n", timeThree); // supports ~1125 circles at 60FPS
	//// 2.12x
	//std::printf("Removed Function Calls from Loop: %f seconds.\n", timeFour); // supports ~1500 circles at 60FPS
	//// 3.81x
	//std::printf("SOA instead of AOS: %f seconds.\n", timeFive); // supports ~2300 circles at 60FPS
	//// 8.06x (EIGHT TIMES SPEEDUP FOR BASIC OPTIMIZATION PATTERNS)
	//std::printf("SIMD ops: %f seconds.\n", timeSix); // supports ~3400 circles at 60FPS
	// 12.0x (You said something about SIMD being too complicated to bother with?)
	std::printf("Assembly optimized: %f seconds.\n", timeSeven); // supports ~8300 circles at 60FPS
	// 138.83x (Okay even I was surprised at this one.  That's just nuts.)

	std::printf("\nPress Enter to Continue.");
	_getch();

	for (int i = 0; i < NUM_CIRCLES; ++i){
		free(isCollided[i]);
	}

	return 1;
}