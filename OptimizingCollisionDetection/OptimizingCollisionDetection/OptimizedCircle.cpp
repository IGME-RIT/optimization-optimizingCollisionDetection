/*
Title: Optimizing Collision Detection
File Name: OptimizedCircle.cpp
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
Shows off a slightly more optimized version of BasicCircle.cpp.
*/
#include "OptimizedCircle.h"
#include "HelperFunctions.h"

OptimizedCircle::OptimizedCircle(){
	this->xPosition = Helper::RandomFloat(0, 1000.0f);
	this->yPosition = Helper::RandomFloat(0, 1000.0f);
	this->xVelocity = Helper::RandomFloat(-1.0f, 1.0f);
	this->yVelocity = Helper::RandomFloat(-1.0f, 1.0f);
	this->radius = Helper::RandomFloat(5.0f, 100.0f);
}


OptimizedCircle::~OptimizedCircle()
{
}


// So this is the only real change to the code.  Look at the parameter.
// You'll notice I'm passing by pointer instead.  You could also pass by reference,
// it's the same thing for this really.  I just like using pointers because it's explicit.
bool OptimizedCircle::CheckForCollision(OptimizedCircle* otherCircle){
	float xOff = xPosition - otherCircle->xPosition;
	xOff = xOff * xOff;
	float yOff = yPosition - otherCircle->yPosition;
	yOff = yOff * yOff;

	float magSquared = xOff + yOff;

	float radSquared = radius + otherCircle->radius;
	radSquared = radSquared * radSquared;


	return magSquared < radSquared;

}

// This wasn't changed at all.
void OptimizedCircle::Update(){
	xPosition += xVelocity;
	yPosition += yVelocity;
}

// So passing by reference or pointers is important.  Especially when you're doing it a lot.
// Think about whether it's faster or not when you're writing parameters.

// Go to TEST THREE when you're ready.