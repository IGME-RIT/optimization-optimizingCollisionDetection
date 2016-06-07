/*
Title: Optimizing Collision Detection
File Name: DataOptimizedCircle.h
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
Shows off a version of BasicCircle.cpp that is data oriented in nature.
*/

#pragma once
#include "Settings.h"

class DataOptimizedCircles
{
public:
	// Okay, so remember that AOS thing I mentioned in TEST FOUR?

	// This is the alternative, Struct of Arrays.
	float xPosition[NUM_CIRCLES];
	float xVelocity[NUM_CIRCLES];
	float yPosition[NUM_CIRCLES];
	float yVelocity[NUM_CIRCLES];
	float radius[NUM_CIRCLES];
	// You'll notice that all of these arrays are within one structure, the
	// dataOptimizedCircles class.

	// If you really really wanted to be neat you could always put those arrays
	// into their own struct, but I felt like that was unneccesary for this example.

	// So you're aware, you can also do a hybrid model, where you have an array of
	// structs that have their own arrays inside.  This can be useful if you want
	// the advantages of the SOA format, but need to do it in smaller "chunks"
	// because you don't know how much memory you're loading at a given moment in runtime.
	
	// What are the advantages of SOA?  I'll write them in DataOptimizedCircles.cpp, let's
	// go there.

	bool* isCollided[NUM_CIRCLES];

	DataOptimizedCircles();
	~DataOptimizedCircles();

	void Update();
	void CheckForCollisions();
};

