/*
Title: Optimizing Collision Detection
File Name: BasicCircle.cpp
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
Functions for a basic circle class.

*/

#include "BasicCircle.h"
#include "HelperFunctions.h"

// The constructor for this is just randomly setting up positions and velocities,
// as well as sizes for all of the circles.  Nothing significant.
BasicCircle::BasicCircle()
{
	this->xPosition = Helper::RandomFloat(0, 1000.0f);
	this->yPosition = Helper::RandomFloat(0, 1000.0f);
	this->xVelocity = Helper::RandomFloat(-1.0f, 1.0f);
	this->yVelocity = Helper::RandomFloat(-1.0f, 1.0f);
	this->radius = Helper::RandomFloat(5.0f, 100.0f);
}

BasicCircle::~BasicCircle(){

}

// So this is your basic circle collision detection.
bool BasicCircle::CheckForCollision(BasicCircle otherCircle){
	// Note how we pass a different circle in as a parameter.

	// This is just (x1 - x2)^2 + (y1-y2)^2, which is the squared magnitude of the vector
	// between the two circles.
	float xOff = xPosition - otherCircle.xPosition;
	xOff = xOff * xOff;
	float yOff = yPosition - otherCircle.yPosition;
	yOff = yOff * yOff;

	float magSquared = xOff + yOff;

	// This is simply the radius's of the circles added together, squared so we can compare it
	// without using any division.
	float radSquared = radius + otherCircle.radius;
	radSquared = radSquared * radSquared;

	// And we directly return the bool.
	return magSquared < radSquared;

}

// The update method is exactly what you'd expect, super simple.
void BasicCircle::Update(){
	xPosition += xVelocity;
	yPosition += yVelocity;
}

// So now you know what we're working with.  All of our code has to accomplish what this class
// is doing.  This class also seems really simple, but you'd be surprised how much some basic
// consideration can lead to speedups in the code.

// Go to TEST TWO when you're done here.