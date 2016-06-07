/*
Title: Optimizing Collision Detection
File Name: HelperFunctions.h
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
A few simple helper functions designed to generate random numbers, as well as
function as a simple timer.
*/
#pragma once
#include <random>
#include <ctime>


namespace Helper{
	static std::default_random_engine generator;
	static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

	/// <summary>
	/// Returns a random float between two values
	/// </summary>
	/// <param name="min">Minimum value</param>
	/// <param name="max">Maximum value</param>
	/// <returns>Random float between the two values</returns>
	static float RandomFloat(float min, float max){
		return (max - min) * distribution(generator) + min;
	}

	static clock_t timer;

	static void StartTimer(){
		timer = clock();
	}

	static float StopTimer(){
		timer = clock() - timer;
		return (float)timer / CLOCKS_PER_SEC;
	}

}