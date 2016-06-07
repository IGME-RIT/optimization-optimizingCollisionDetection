/*
Title: Optimizing Collision Detection
File Name: SIMDOptimizedCircle.cpp
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
Optimizing circle collisions with SIMD code.

References:
https://msdn.microsoft.com/en-us/library/t467de55(v=vs.90).aspx
*/

#include "SIMDOptimizedCircles.h"
#include <intrin.h> //SIMD ops are within <intrin.h>
#include "HelperFunctions.h"


SIMDOptimizedCircles::SIMDOptimizedCircles()
{
	// So first thing's first.  All of the operations we're going to be using require
	// 16 bit alignment from our data.

	// Okay yes we COULD use the unaligned calls, but if you're doing that what's
	// the point of using SIMD in the first place.
	boolTest = (float*)_aligned_malloc(4 * sizeof(float), 16);
	xPosition = (float*)_aligned_malloc(NUM_CIRCLES * sizeof(float), 16);
	xVelocity = (float*)_aligned_malloc(NUM_CIRCLES * sizeof(float), 16);
	yPosition = (float*)_aligned_malloc(NUM_CIRCLES * sizeof(float), 16);
	yVelocity = (float*)_aligned_malloc(NUM_CIRCLES * sizeof(float), 16);
	radius = (float*)_aligned_malloc(NUM_CIRCLES * sizeof(float), 16);

	for (int i = 0; i < NUM_CIRCLES; ++i){
		xPosition[i] = Helper::RandomFloat(0, 1000.0f);
		yPosition[i] = Helper::RandomFloat(0, 1000.0f);
		xVelocity[i] = Helper::RandomFloat(-1.0f, 1.0f);
		yVelocity[i] = Helper::RandomFloat(-1.0f, 1.0f);
		radius[i] = Helper::RandomFloat(5.0f, 100.0f);

		isCollided[i] = (float*)_aligned_malloc(sizeof(float) * NUM_CIRCLES, 32);
		memset(isCollided[i], 0, sizeof(float) * NUM_CIRCLES);
	}

	int b = 0;
}

SIMDOptimizedCircles::~SIMDOptimizedCircles()
{
	// Aligned malloc requires aligned free.
	_aligned_free(boolTest);
	_aligned_free(xPosition);
	_aligned_free(yPosition);
	_aligned_free(xVelocity);
	_aligned_free(yVelocity);
	_aligned_free(radius);

	for (int i = 0; i < NUM_CIRCLES; ++i){
		_aligned_free(isCollided[i]);
	}
}

void SIMDOptimizedCircles::Update(){

	// Alright, our first look at SIMD code.  It looks pretty bad but let's
	// talk about what it's actually doing.

	for (int i = 0; i < NUM_CIRCLES; i += 4){ //Notice that we're doing 4 at a time.

		// The following code is xPosition += xVelocity

		_mm_store_ps(xPosition + i,
			_mm_add_ps(
				_mm_load_ps(xPosition + i)
				,
				_mm_load_ps(xVelocity + i)));

		// Let's break it down into what it's doing.

		// Store_ps, the highest level function here, stores a SIMD value to memory somewhere.
		// A SIMD register is 128 bits, so the data type is __m128, which is what you use
		// for SIMD ops in general.  The ps stands for single point precision (so floats).

		// Add_ps does what you'd think it does, it adds two __m128's together.
		// If __m128 a is x1, x2, x3, x4, and __m128 b is y1, y2, y3, y4, then
		// a + b = x1 + y1, x2 + y2, x3 + y3, x4 + y4.

		// The deepest functions are loads.  They simply load 4 floats into one __m128.
		// That's how we're grabbing the data.


		// This is the same thing with y's.
		_mm_store_ps(yPosition + i,
			_mm_add_ps(
				_mm_load_ps(yPosition + i)
				,
				_mm_load_ps(yVelocity + i)));
	}

}

void SIMDOptimizedCircles::CheckForCollisions(){
	// Now for the fun one.
	for (int i = 0; i < NUM_CIRCLES; ++i){

		// We're going to test the collisions of four circles against one circle at a time.

		// Since SIMD doesn't exactly let you compare 4 things against 1 (It does 4 against 4)
		// we're going to use load1.  Adding 1 to the end of the function name generally implies 
		// that you're loading all 4 parts of the __m128 with the same value.
		__m128 xPos = _mm_load1_ps(xPosition + i); //So this is the value at xPosition + i 4 times.
		__m128 yPos = _mm_load1_ps(yPosition + i);
		__m128 rad = _mm_load1_ps(radius + i);

		// Remember how we were starting the inner loop from one above?  Since we're going four at
		// a time we'll have to overlap a little.  That doesn't mean we have to do all of them now
		// though.
		int j = i & 0xFC;
		// I'm cutting off the last two bits so that the do while loop can still go up
		// against  4 i's at the same time, but every 4 i's j actually goes up by 4.
		// so 0 -> 0, 1-> 0, 2-> 0, 3-> 0, 4 -> 4, 5-> 4, and so on.

		do {
			// This is x1 -x2... FOUR TIMES AT ONCE!  SIMD is pretty cool like that.
			__m128 xDif = _mm_sub_ps(
				xPos,
				_mm_load_ps(xPosition + j));

			__m128 yDif = _mm_sub_ps(
				yPos,
				_mm_load_ps(yPosition + j));

			// Adding the two radius's together.
			__m128 radiusAdd = _mm_add_ps(rad, _mm_load_ps(radius + j));

			// Normally you wouldn't write code spaced out this much, but I wanted to be 
			// explicit since this is a tutorial.
			_mm_store_ps(isCollided[i] + j,//And then we store into isCollided.
				_mm_cmplt_ps(//And this is compare less than.  If a1 < b1, 
							 //then return 0xFFFFFFFF.  Else return 0.
					_mm_add_ps(// we add them together.
						_mm_mul_ps(xDif, xDif) //mul is multiply.
						,
						_mm_mul_ps(yDif, yDif))//so this squares the buffer.
					,
					_mm_mul_ps(radiusAdd,radiusAdd)
				));


			j += 4;// Then grab the next four to compare with.
		} while (j < NUM_CIRCLES);
	}
}

// See.
// 
// That wasn't so bad.
// This is hardly a tutorial on how to use all of the features, but hopefully you're
// a little less scared of SIMD after seeing it.
//
// So go to TEST SEVEN if you want to see inline Assembly.