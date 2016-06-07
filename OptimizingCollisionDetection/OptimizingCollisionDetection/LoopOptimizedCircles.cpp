/*
Title: Optimizing Collision Detection
File Name: LoopOptimizedCircle.cpp
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
Shows off an example of how pulling operations out of a loop can make a big
difference in performance.
*/
#include "LoopOptimizedCircles.h"


// This is just setting up that array again.
LoopOptimizedCircles::LoopOptimizedCircles()
{
	for (int i = 0; i < NUM_CIRCLES; ++i){
		isCollided[i] = (bool*)malloc(sizeof(bool) * NUM_CIRCLES);
		memset(isCollided[i], 0, sizeof(bool) * NUM_CIRCLES);
	}
}

// And clearing it at the end of the program.  Always free your memory folks.
LoopOptimizedCircles::~LoopOptimizedCircles()
{
	for (int i = 0; i < NUM_CIRCLES; ++i){
		free(isCollided[i]);
	}
}

// Okay, so showing off update first because you get the idea (and this method needed
// some love).
void LoopOptimizedCircles::Update(){

	// All this is is the same thing, but the function to Update all of the circles is being
	// called once, instead of once for each circle.
	for (int i = 0; i < NUM_CIRCLES; ++i){
		circles[i].xPosition += circles[i].xVelocity;
		circles[i].yPosition += circles[i].yVelocity;
	}
}

void LoopOptimizedCircles::CheckForCollisions(){
	// Same down here, actually.
	for (int i = 0; i < NUM_CIRCLES; ++i){
		for (int j = 0; j < NUM_CIRCLES; ++j){
			isCollided[i][j] = (circles[i].xPosition - circles[j].xPosition) * (circles[i].xPosition - circles[j].xPosition)
				+ (circles[i].yPosition - circles[j].yPosition) * (circles[i].yPosition - circles[j].yPosition)
				< (circles[i].radius + circles[j].radius) + (circles[i].radius + circles[j].radius);
		}
	}
}

// So as you can see pulling operations outside of loops when you don't need them is just
// good practice.  But hold on a second before going back to the tests, because there's a 
// lesson here.  What we're really doing here is designing the program to accomplish our
// intended goal.  If your goal is to check the collisions of every circle, your goal isn't
// to check the collision between two specific circles.  So take that into consideration when
// you write your code.  It's really important.  This is a lesson even high level programmers
// need to learn because it's really only a little more work to get incredible gains in speed.
//
// Don't get confused with what I said in TEST THREE though.  This isn't the same as
// premature optimization.  This is considering the layout of your program from the very
// beginning, so that less problems like the function being called in the loop crop up.
//
// So think about how considering what the program is trying to accomplish helps you design
// better systems, and with that in mind let's head to TEST FIVE.