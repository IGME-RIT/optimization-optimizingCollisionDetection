/*
Title: Optimizing Collision Detection
File Name: AVXOptimizedCircle.cpp
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
Optimization of SIMDOptimizedCircle.cpp with inline assembly and AVX operations.

References:
http://www.cs.virginia.edu/~evans/cs216/guides/x86.html
*/
#include "AVXOptimizedCircles.h"
#include "HelperFunctions.h"

AVXOptimizedCircles::AVXOptimizedCircles()
{

	// This stuff is all the same as the SIMD stuff because we'll be using the 
	// SSE x86 instruction set of assembly instructions.
	boolTest = (float*)_aligned_malloc(4 * sizeof(float), 32);
	xPosition = (float*)_aligned_malloc(NUM_CIRCLES * sizeof(float), 32);
	xVelocity = (float*)_aligned_malloc(NUM_CIRCLES * sizeof(float), 32);
	yPosition = (float*)_aligned_malloc(NUM_CIRCLES * sizeof(float), 32);
	yVelocity = (float*)_aligned_malloc(NUM_CIRCLES * sizeof(float), 32);
	radius = (float*)_aligned_malloc(NUM_CIRCLES * sizeof(float), 32);

	isCollided = (float**)_aligned_malloc(sizeof(float) * NUM_CIRCLES, 32);

	for (int i = 0; i < NUM_CIRCLES; ++i){
		xPosition[i] = Helper::RandomFloat(0, 100.0f);
		yPosition[i] = Helper::RandomFloat(0, 100.0f);
		xVelocity[i] = Helper::RandomFloat(-5.0f, 5.0f);
		yVelocity[i] = Helper::RandomFloat(-5.0f, 5.0f);
		radius[i] = Helper::RandomFloat(5.0f, 100.0f);

		isCollided[i] = (float*)_aligned_malloc(sizeof(float) * NUM_CIRCLES, 32);
		memset(isCollided[i], 0, sizeof(float) * NUM_CIRCLES);
	}

	int b = 0;
}

AVXOptimizedCircles::~AVXOptimizedCircles()
{
	_aligned_free(boolTest);
	_aligned_free(xPosition);
	_aligned_free(yPosition);
	_aligned_free(xVelocity);
	_aligned_free(yVelocity);
	_aligned_free(radius);

	for (int i = 0; i < NUM_CIRCLES; ++i){
		_aligned_free(isCollided[i]);
	}
	_aligned_free(isCollided);
}

void AVXOptimizedCircles::Update(){

	/*
	__asm{
		mov edi, dword ptr[this]; //edi will contain this.
		xor esi, esi; //esi will be used as our counter.  Set it to 0.

		mov eax, [edi].xPosition;//eax is the xPosition Register
		mov ebx, [edi].xVelocity;//ebx is the xVelocity Register
		mov ecx, [edi].yPosition;//ecx is the yPosition Register
		mov edx, [edi].yVelocity;//edx is the yVelocity Register

	MovementLoop: //Beginning of a do{...esi+=32}while(esi < NUM_CIRCLES*4) loop.

		vmovaps ymm0, ymmword ptr[eax + esi];//Move the xPosition at index esi into the ymm0 register.
		vmovaps ymm1, ymmword ptr[ecx + esi];//Move the yPosition at index esi into ymm1
		vaddps ymm0, ymm0, ymmword ptr[ebx + esi];//Add to the ymm0 register the floats stored at xVelocity + esi.
		vaddps ymm1, ymm1, ymmword ptr[edx + esi];//Adds to ymm1 the float stored at yVelocity + esi
		vmovaps ymmword ptr[eax + esi], ymm0;//Store the ymm0 register into xPosition + esi
		vmovaps ymmword ptr[ecx + esi], ymm1;//Stores ymm1 to yPosition + esi

		add esi, 32;//esi += 32 (it's += 8, but * 4 since a float is four bytes.

		cmp esi, NUM_CIRCLES * 4; //Compare esi to NUM_CIRCLES * 4 (times four to both)
		jl MovementLoop;// If it's less then jump back up to MovementLoop.
	}*/

}

void AVXOptimizedCircles::CheckForCollisions(){
	
	int i = 0;

	/*
	__asm{
		mov edi, dword ptr[this];//Move this to edi, edi will store this
		mov ebx, [edi].xPosition;//store xPosition in ebx
		mov ecx, [edi].yPosition;//store yPosition in ecx
		mov edx, [edi].radius;//store radius in edx
		mov edi, [edi].isCollided;//store isCollided in edi, since we don't need this anymore actually.
		xor esi, esi;//0 out esi which will be our "i" counter.

	OuterLoop:

		mov eax, esi;//Make a "j" counter that starts at i.  J is in eax
		and al, 0xE0;//make sure it's a multiple of eight, round down.


		// This code sets all parts of the register to the same value.
		vmovss xmm0, dword ptr[ebx + esi];
		vmovss xmm1, dword ptr[ecx + esi];
		vmovss xmm2, dword ptr[edx + esi];
		vshufps xmm0, xmm0, xmm0, 0;
		vshufps xmm1, xmm1, xmm1, 0; 
		vshufps xmm2, xmm2, xmm2, 0;
		vinsertf128 ymm0, ymm0, xmm0, 1;
		vinsertf128 ymm1, ymm1, xmm1, 1;
		vinsertf128 ymm2, ymm2, xmm2, 1;



		// We don't actually need esi in the program... so this seems okay.
		mov i, esi;
		mov esi, dword ptr[edi + esi]; //So that means this code should still function.
		// So from now on esi is the pointer pointing to the current i's isCollided array in the buffer.

	CollisionStart:
		vmovaps ymm3, ymmword ptr[ebx + eax];//So load the current 4 xPositions into xmm3.
		vmovaps ymm4, ymmword ptr[ecx + eax];//xmm4 is the yPositions
		vmovaps ymm5, ymmword ptr[edx + eax];//xmm5 is the radiuses.
		// Note that since we stored the xPosition, yPosition, and zPosition into buffers without
		// worrying about temp storage we could really really quickly load the right ones here.

		vsubps ymm3, ymm3, ymm0;//(x2-x1)
		vsubps ymm4, ymm4, ymm1;//(y2-y1)
		vaddps ymm5, ymm5, ymm2;//(r1+r2)
		vmulps ymm3, ymm3, ymm3;//(x2-x1)^2
		vmulps ymm4, ymm4, ymm4;//(y2-y1)^2
		vmulps ymm5, ymm5, ymm5;//(r1 + r2)^2
		vaddps ymm3, ymm3, ymm4;//(x2-x1)^2 + (y2-y1)^2

		vcmpltps ymm3, ymm3, ymm5;//(x2-x1)^2 + (y2-y1)^2 < (r1+r2)^2


		vmovaps ymmword ptr[esi + eax], ymm3;//Store the result into isCollided[i] + j (esi + j)

		add eax, 32;// Push j up by 8, multiplied by 4 because it's a floating point and that's 4 bytes.
		cmp eax, NUM_CIRCLES * 4;//Multiply j by 4, multiply this by 4.
		jl CollisionStart;//Jump if less than, yadah yadah.

		mov esi, i;

		add esi, 4;//increase i by 1 * 4.
		cmp esi, NUM_CIRCLES * 4;//*4 for both.
		jl OuterLoop;
	}
	*/

}

// I wish someone told me AVX existed before.  This is so good.