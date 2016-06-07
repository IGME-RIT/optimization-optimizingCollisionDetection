/*
Title: Optimizing Collision Detection
File Name: DataOptimizedCircle.cpp
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

#include "DataOptimizedCircles.h"
#include "HelperFunctions.h"


DataOptimizedCircles::DataOptimizedCircles()
{

	// Setup is still kind of object oriented.  That's fine.
	for (int i = 0; i < NUM_CIRCLES; ++i){
		xPosition[i] = Helper::RandomFloat(0, 1000.0f);
		yPosition[i] = Helper::RandomFloat(0, 1000.0f);
		xVelocity[i] = Helper::RandomFloat(-1.0f, 1.0f);
		yVelocity[i] = Helper::RandomFloat(-1.0f, 1.0f);
		radius[i] = Helper::RandomFloat(5.0f, 100.0f);
	}
	for (int i = 0; i < NUM_CIRCLES; ++i){
		isCollided[i] = (bool*)malloc(sizeof(bool) * NUM_CIRCLES);
		memset(isCollided[i], 0, sizeof(bool) * NUM_CIRCLES);
	}
}


DataOptimizedCircles::~DataOptimizedCircles()
{
	for (int i = 0; i < NUM_CIRCLES; ++i){
		free(isCollided[i]);
	}
}

void DataOptimizedCircles::CheckForCollisions(){
	
	// See, now it's getting kind of hard to read what's going on.  And this is still just
	// a simple algorithm.
	for (int i = 0; i < NUM_CIRCLES; ++i){
		for (int j = i + 1; j < NUM_CIRCLES; ++j){
			isCollided[i][j] = (xPosition[i] - xPosition[j]) * (xPosition[i] - xPosition[j])
				+ (yPosition[i] - yPosition[j]) * (yPosition[i] - yPosition[j])
				< (radius[i] + radius[j]) + (radius[i] + radius[j]);
		}
	}
}

void DataOptimizedCircles::Update(){
	// Hypothetically doing ALL of the x's and then ALL of the y's would be faster,
	// but looping twice through things slows stuff down because of the conditionals.
	// There's a lot of potential for optimization with this format, I suggest moving things
	// around and profiling the results.  The number one rule of optimization is testing
	// testing testing.
	for (int i = 0; i < NUM_CIRCLES; ++i){
		xPosition[i] += xVelocity[i];
		yPosition[i] += yVelocity[i];
	}
}

// So the advantage of using SOA format over AOS?  It goes to how your computer
// accesses memory.  When your computer accesses a bit of memory, it stores it in a closer
// location known as a cache.  That means it can actually access it quickly soon.
// By laying out the memory in a way that is easy to repetitively perform the same
// tasks over, you make it easy for the computer to load the data.
//
// In AOS format, often the memory is in a way that makes it bulkier, and increases the chance
// of a "cache miss", which is when your computer looks for data in the cache, but because it
// hasn't been used for awhile has to go back out to RAM.  Now while that sounds like a
// terrifying mistake of some sort, a cache miss isn't really a big deal.  I mean at some point
// the memory isn't useful anymore and you need to put it away.  The big deal comes from
// having a lot of them.  This really slows down your program.

// Now it's really really important to understand that this isn't simply "Use SOA, it's better
// than AOS".  That's not the lesson to get from this optimization.  The lesson is to
// consider your data's layout, and how your hardware parses the data.  At the end of a day
// all your program is doing is transforming data from one form to another.

// And to people who are wondering what Data Oriented Programming is and how to use it, that's it.
// Yup.  It's just a way of thinking.  It's not a magical set of rules, it's a philosophy.
// It's saying "Hey, instead of creating a variable int b = 0 every iteration of a loop, let's
// just make it outside of the loop and set it to 0 at the start of the loop, because that way
// the computer doesn't have to create and destroy a new variable every time this loop runs.
// Data Oriented Programming is basically every consideration I've pointed out here.

// These aren't complicated ideas.  You might not know about all the odds and ends of your
// hardware, and I'm not saying you have to (looking at you high level programmers).  But
// I hope you realize that even without knowing your hardware you probably have at least
// somewhat of a clue of how computers work.  A lot of these optimizations are really intuitive,
// such as the idea that calling a function takes time, so put loops inside of functions instead
// of functions inside of loops where you can.

// So in the future keep these ideas in mind.  There are some basic small nitpicky things
// I could probably do to make this code faster without doing any SIMD code or inline assembly,
// but honestly after profiling a few of them they don't seem to make any significant difference.
// So if you're not too interested in optimizing you can stop here.  But I suggest you go on
// to at least look at the SIMD section in TEST SIX.  It's actually not as hard to write as it
// looks to be, and if you are at all interested in optimization it's the kind of thing you're
// definitely going to work with.