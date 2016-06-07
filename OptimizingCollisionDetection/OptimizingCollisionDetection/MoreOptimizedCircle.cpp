/*
Title: Optimizing Collision Detection
File Name: MoreOptimizedCircle.cpp
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
Shows off a slightly more optimized version of OptimizedCircle.cpp.
*/

#include "MoreOptimizedCircle.h"
#include "HelperFunctions.h"

MoreOptimizedCircle::MoreOptimizedCircle()
{
	this->xPosition = Helper::RandomFloat(0, 1000.0f);
	this->yPosition = Helper::RandomFloat(0, 1000.0f);
	this->xVelocity = Helper::RandomFloat(-1.0f, 1.0f);
	this->yVelocity = Helper::RandomFloat(-1.0f, 1.0f);
	this->radius = Helper::RandomFloat(5.0f, 100.0f);
}


MoreOptimizedCircle::~MoreOptimizedCircle()
{
}

// You'll note that this time we really put everyting together.
// This is actually a really simple mathematical operation, so it's more or less okay to
// do this.  With a more complex algorithm you'd still want to break it into parts for
// readability, because the compiler should be setting things up like this for you.
bool MoreOptimizedCircle::CheckForCollision(MoreOptimizedCircle* otherCircle){
	// The one main advantage doing it like this is it allows your CPU to know that it
	// can perform operations in parallel.  Most CPU's would actually be able to do the four
	// subtractions simultaenously because they're setup this way, the addition and the 
	// multiplications at the same time, and then compare.  Plus we're not storing
	// variables more than temporarily here, which helps.
	return (xPosition - otherCircle->xPosition) * (xPosition - otherCircle->xPosition)
		+ (yPosition - otherCircle->yPosition) * (yPosition - otherCircle->yPosition)
		< (radius + otherCircle->radius) + (radius + otherCircle->radius);
	
}

// Don't worry, this one will get some love soon.  Yes there are ways to make
// adding two numbers like this more effective.
void MoreOptimizedCircle::Update(){
	xPosition += xVelocity;
	yPosition += yVelocity;
}


// Now the important thing to learn from this lesson is that the compiler doesn't always
// know what you're trying to accomplish.  Sometimes you have to help it out and lay out
// your code in a manner that the compiler can work with better.

// That said, there's a serious warning to be taken with this.  If that collision detection
// was not so simple to read as an algorithm, then that code would have gotten really messy
// really fast.  That might be great for the compiler, very explicit and all, but if other people
// can't read your code you'll have problems.  You do have to balance readability to some extent.

// Consider using inline functions and breaking code into parts if you need to write more complex
// code and it HAS to be efficient.  Remember that speed isn't LITERALLY everything, sometimes
// you're spending hours on something that'll make almost no difference in your codebase.
// Premature optimization is bad.

// Be aware that you can also profile code to see which functions benefit the most from being
// made faster.

// When you're ready go to TEST FOUR