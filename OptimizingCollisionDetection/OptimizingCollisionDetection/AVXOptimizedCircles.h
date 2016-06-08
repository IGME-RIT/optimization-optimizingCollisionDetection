/*
Title: Optimizing Collision Detection
File Name: AVXOptimizedCircle.h
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
Optimization of SIMDOptimizedCircle.cpp with inline assembly and AVX instructions.
*/
#pragma once
#include "Settings.h"

class AVXOptimizedCircles
{
private:
	float* boolTest;

public:
	float* xPosition;
	float* xVelocity;
	float* yPosition;
	float* yVelocity;
	float* radius;



	float** isCollided;

	AVXOptimizedCircles();
	~AVXOptimizedCircles();

	void Update();
	void CheckForCollisions();
};
