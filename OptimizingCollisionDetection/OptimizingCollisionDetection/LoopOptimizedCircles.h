/*
Title: Optimizing Collision Detection
File Name: LoopOptimizedCircle.h
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
Shows off a version of BasicCircle.cpp that pulls the function calls outside of
the loop.
*/

#pragma once
#include "Settings.h"
#include "HelperFunctions.h"
class LoopOptimizedCircles
{
	// So notice that we have this Struct, circle.
	struct Circle{
		float xPosition;
		float yPosition;
		float xVelocity;
		float yVelocity;
		float radius;

		Circle(){
			this->xPosition = Helper::RandomFloat(0, 1000.0f);
			this->yPosition = Helper::RandomFloat(0, 1000.0f);
			this->xVelocity = Helper::RandomFloat(-1.0f, 1.0f);
			this->yVelocity = Helper::RandomFloat(-1.0f, 1.0f);
			this->radius = Helper::RandomFloat(5.0f, 100.0f);
		}
	};

public:

	// And we have an array of this struct.
	Circle circles[NUM_CIRCLES];

	// This is actually a data pattern, known as AOS (or Array of Structs).
	// It's when you lay out your data in this fashion.  Having an array of classes is the same
	// thing.  If you are confused why this distinction is important, don't worry it'll be clear
	// in the next test.  For now let's go to LoopOptimizedCircles.cpp to see what's going on.

	bool* isCollided[NUM_CIRCLES];

	LoopOptimizedCircles();
	~LoopOptimizedCircles();

	void Update();
	void CheckForCollisions();
};

