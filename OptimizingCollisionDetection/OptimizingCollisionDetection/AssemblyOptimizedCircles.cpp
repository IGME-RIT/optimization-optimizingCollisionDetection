/*
Title: Optimizing Collision Detection
File Name: AssemblyOptimizedCircle.h
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
Optimization of SIMDOptimizedCircle.cpp with inline assembly.

References:
http://www.cs.virginia.edu/~evans/cs216/guides/x86.html
*/
#include "AssemblyOptimizedCircles.h"
#include "HelperFunctions.h"
//Look ma, no fancy includes.

AssemblyOptimizedCircles::AssemblyOptimizedCircles()
{

	// This stuff is all the same as the SIMD stuff because we'll be using the 
	// SSE x86 instruction set of assembly instructions.
	boolTest = (float*)_aligned_malloc(4 * sizeof(float), 16);
	xPosition = (float*)_aligned_malloc(NUM_CIRCLES * sizeof(float), 16);
	xVelocity = (float*)_aligned_malloc(NUM_CIRCLES * sizeof(float), 16);
	yPosition = (float*)_aligned_malloc(NUM_CIRCLES * sizeof(float), 16);
	yVelocity = (float*)_aligned_malloc(NUM_CIRCLES * sizeof(float), 16);
	radius = (float*)_aligned_malloc(NUM_CIRCLES * sizeof(float), 16);

	isCollided = (float**)_aligned_malloc(sizeof(float) * NUM_CIRCLES, 16);

	for (int i = 0; i < NUM_CIRCLES; ++i){
		xPosition[i] = Helper::RandomFloat(0, 100.0f);
		yPosition[i] = Helper::RandomFloat(0, 100.0f);
		xVelocity[i] = Helper::RandomFloat(-5.0f, 5.0f);
		yVelocity[i] = Helper::RandomFloat(-5.0f, 5.0f);
		radius[i] = Helper::RandomFloat(5.0f, 100.0f);

		isCollided[i] = (float*)_aligned_malloc(sizeof(float) * NUM_CIRCLES, 16);
		memset(isCollided[i], 0, sizeof(float) * NUM_CIRCLES);
	}

	int b = 0;
}

AssemblyOptimizedCircles::~AssemblyOptimizedCircles()
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

void AssemblyOptimizedCircles::Update(){

	// So anything inside the __asm keyword is assembly code.
	// Notice a few fancy tricks we can use because this is inline assembly,
	// and not just assembly.  For one, we can grab objects that have names and use them
	// in the assembly code.  At the beginning I store this in [edi], and then
	// can just go [edi].variableName to access that variable.  Super easy to work with.
	// It also let's us use // for comments.

	// Before we look at the actual code let me explain how I'll be talking about this.

	// I'm going to copy-paste the Disassembly of the code that was in the SIMD example at the top
	// of the page before the asm keyword.  In it I'll discuss the potential problems I see.
	// I'll then show off the assembly code I wrote.  I'm new to assembly, so I may have made
	// some mistakes.  But the code works, and it works fast, so I'm really pleased with the
	// results so far.  The actual code itself will have comments explaining what every command
	// accomplishes, but not any detail of how it's an optimization.  That'll all be in the
	// disassembly.

	// Speaking of which, if you don't know how to get disassembly, create a breakpoint,
	// and while at that breakpoint during debugging press Debug->Windows->Disassembly

	// Any thing written after a // is comments I'm writing about the assembly.

	// Note that this is not a tutorial on coding with assembly, use http://www.cs.virginia.edu/~evans/cs216/guides/x86.html
	// and google for any instructions I don't explain.  Honestly I just looked at the disassembly
	// and guessed at what everything did.  And it worked!

	/*Dissassembly Below
	for (int i = 0; i < NUM_CIRCLES; i += 4){ // So this is standard for loop code.
00A8A3D5  mov         dword ptr [ebp-14h],0  
00A8A3DC  jmp         SIMDOptimizedCircles::Update+47h (0A8A3E7h)  
00A8A3DE  mov         eax,dword ptr [ebp-14h]  
00A8A3E1  add         eax,4  
00A8A3E4  mov         dword ptr [ebp-14h],eax  
00A8A3E7  cmp         dword ptr [ebp-14h],3E8h  
00A8A3EE  jge         SIMDOptimizedCircles::Update+0FBh (0A8A49Bh)    //Nothing interesting here.

// It did make me realize however that there's an extra compare compared with what I needed for
// the actual program.  By making it into a do while loop it would be faster.

		_mm_store_ps(xPosition + i,
			_mm_add_ps(
				_mm_load_ps(xPosition + i)
				,
				_mm_load_ps(xVelocity + i)));

				// Okay, so code is properly loading xPosition and xVelocity.
00A8A3F4  mov         eax,dword ptr [this]  
00A8A3F7  mov         ecx,dword ptr [eax+8]  
00A8A3FA  mov         edx,dword ptr [ebp-14h]  
00A8A3FD  movaps      xmm0,xmmword ptr [ecx+edx*4]  
00A8A401  movaps      xmmword ptr [ebp-170h],xmm0  //Only it's temporarily storing them on the stack
00A8A408  mov         eax,dword ptr [this]  //It could have totally stored it into xmm1 instead of xmm0
00A8A40B  mov         ecx,dword ptr [eax+4]  
00A8A40E  mov         edx,dword ptr [ebp-14h]  
00A8A411  movaps      xmm0,xmmword ptr [ecx+edx*4]  
00A8A415  movaps      xmmword ptr [ebp-190h],xmm0  
00A8A41C  movaps      xmm0,xmmword ptr [ebp-190h]  
00A8A423  addps       xmm0,xmmword ptr [ebp-170h]  //I mean really this is the only instruction
00A8A42A  movaps      xmmword ptr [ebp-150h],xmm0  //that matters.  Do we really need to be loading
00A8A431  movaps      xmm0,xmmword ptr [ebp-150h]  //and storing all this when we could just do an add?
00A8A438  mov         eax,dword ptr [this]  
00A8A43B  mov         ecx,dword ptr [eax+4]  
00A8A43E  mov         edx,dword ptr [ebp-14h]  
00A8A441  movaps      xmmword ptr [ecx+edx*4],xmm0  //This store is fine and expected.

// Okay this following code is the same as the previous code.  So no need to comment twice.

		_mm_store_ps(yPosition + i,
			_mm_add_ps(
				_mm_load_ps(yPosition + i)
				,
				_mm_load_ps(yVelocity + i)));
00A8A445  mov         eax,dword ptr [this]  
00A8A448  mov         ecx,dword ptr [eax+10h]  
00A8A44B  mov         edx,dword ptr [ebp-14h]  
00A8A44E  movaps      xmm0,xmmword ptr [ecx+edx*4]  
00A8A452  movaps      xmmword ptr [ebp-110h],xmm0  
00A8A459  mov         eax,dword ptr [this]  
00A8A45C  mov         ecx,dword ptr [eax+0Ch]  
00A8A45F  mov         edx,dword ptr [ebp-14h]  
00A8A462  movaps      xmm0,xmmword ptr [ecx+edx*4]  
00A8A466  movaps      xmmword ptr [ebp-130h],xmm0  
00A8A46D  movaps      xmm0,xmmword ptr [ebp-130h]  
00A8A474  addps       xmm0,xmmword ptr [ebp-110h]  
00A8A47B  movaps      xmmword ptr [ebp-0F0h],xmm0  
00A8A482  movaps      xmm0,xmmword ptr [ebp-0F0h]  
00A8A489  mov         eax,dword ptr [this]  
00A8A48C  mov         ecx,dword ptr [eax+0Ch]  
00A8A48F  mov         edx,dword ptr [ebp-14h]  
00A8A492  movaps      xmmword ptr [ecx+edx*4],xmm0  
	}*/

	//So what's going on?  that seemed like WAY too many instructions for what
	//was actually happening.

	__asm{
		mov edi, dword ptr[this]; //edi will contain this.
		xor esi, esi; //esi will be used as our counter.  Set it to 0.

		mov eax, [edi].xPosition;//eax is the xPosition Register
		mov ebx, [edi].xVelocity;//ebx is the xVelocity Register
		mov ecx, [edi].yPosition;//ecx is the yPosition Register
		mov edx, [edi].yVelocity;//edx is the yVelocity Register

		MovementLoop: //Beginning of a do{...esi+=16}while(esi < NUM_CIRCLES*4) loop.

		movaps xmm0, xmmword ptr[eax + esi];//Move the xPosition at index esi into the xmm0 register.
		movaps xmm1, xmmword ptr[ecx + esi];//Move the yPosition at index esi into xmm1
		addps xmm0, xmmword ptr[ebx + esi];//Add to the xmm0 register the floats stored at xVelocity + esi.
		addps xmm1, xmmword ptr[edx + esi];//Adds to xmm1 the float stored at yVelocity + esi
		movaps xmmword ptr[eax + esi], xmm0;//Store the xmm0 register into xPosition + esi
		movaps xmmword ptr[ecx + esi], xmm1;//Stores xmm1 to yPosition + esi

		// Now it seemed as if using both registers was faster than just one, the CPU might
		// be parallelizing this to some extent.  I'm unsure.  No negative to leaving it this
		// way though, only a potential positive.

		// So remember all the stuff that happened in that loop?  These six instructions
		// accomplished the exact same thing.Took 0.4 seconds off of SIMD's 2.4 at the time.

		add esi, 16;//esi += 16 (it's += 4, but * 4 since a float is four bytes.

		cmp esi, NUM_CIRCLES * 4; //Compare esi to NUM_CIRCLES * 4 (times four to both)
		jl MovementLoop;// If it's less then jump back up to MovementLoop.
	}

}

void AssemblyOptimizedCircles::CheckForCollisions(){

	// Same procedure as last time, // means comment I'm writing now.
/* Dissassembly below
for (int i = 0; i < NUM_CIRCLES; ++i){ //standard for loop, this is fine.
00CBA0A5  mov         dword ptr[ebp - 14h], 0
00CBA0AC  jmp         SIMDOptimizedCircles::CheckForCollisions + 47h (0CBA0B7h)
00CBA0AE  mov         eax, dword ptr[ebp - 14h]
00CBA0B1  add         eax, 1
00CBA0B4  mov         dword ptr[ebp - 14h], eax
00CBA0B7  cmp         dword ptr[ebp - 14h], 3E8h
00CBA0BE  jge         SIMDOptimizedCircles::CheckForCollisions + 204h (0CBA274h)

			
__m128 xPos = _mm_load1_ps(xPosition + i); //So this is the value at xPosition + i 4 times.
00CBA0C4  mov         eax, dword ptr[this]
00CBA0C7  mov         ecx, dword ptr[eax + 4]
00CBA0CA  mov         edx, dword ptr[ebp - 14h]
00CBA0CD  movss       xmm0, dword ptr[ecx + edx * 4]//this load is fine.
00CBA0D2  shufps      xmm0, xmm0, 0 //this is how you fill the register with the same value.
00CBA0D6  movaps      xmmword ptr[ebp - 360h], xmm0//Okay what?  Why?  What purpose does storing
00CBA0DD  movaps      xmm0, xmmword ptr[ebp - 360h]//it have at the moment?
00CBA0E4  movaps      xmmword ptr[ebp - 30h], xmm0//Also what is going on here?  We store twice?
__m128 yPos = _mm_load1_ps(yPosition + i);
00CBA0E8  mov         eax, dword ptr[this]
00CBA0EB  mov         ecx, dword ptr[eax + 0Ch]
00CBA0EE  mov         edx, dword ptr[ebp - 14h]
00CBA0F1  movss       xmm0, dword ptr[ecx + edx * 4]//Oh good, we made room so we could store a different
00CBA0F6  shufps      xmm0, xmm0, 0 // variable.  Except you know, we have 7 other registers we can also use.
00CBA0FA  movaps      xmmword ptr[ebp - 340h], xmm0
00CBA101  movaps      xmm0, xmmword ptr[ebp - 340h]
00CBA108  movaps      xmmword ptr[ebp - 50h], xmm0
__m128 rad = _mm_load1_ps(radius + i);
00CBA10C  mov         eax, dword ptr[this]
00CBA10F  mov         ecx, dword ptr[eax + 14h]
00CBA112  mov         edx, dword ptr[ebp - 14h]
00CBA115  movss       xmm0, dword ptr[ecx + edx * 4] //Sure why not.  Let's move the 3 variables we are going
00CBA11A  shufps      xmm0, xmm0, 0 //to be using every single time in the inner loop out of the loop.
00CBA11E  movaps      xmmword ptr[ebp - 320h], xmm0  //Who need xmm1 through xmm7 anyway?
00CBA125  movaps      xmm0, xmmword ptr[ebp - 320h]
00CBA12C  movaps      xmmword ptr[ebp - 70h], xmm0 //xmm0 is where the real fun happens, apparently


int j = i & 0xFC;
00CBA130  mov         eax, dword ptr[ebp - 14h]
00CBA133  and         eax, 0FCh//I did this against AL instead, but honestly that probably makes little difference
00CBA138  mov         dword ptr[ebp - 7Ch], eax 


do {
	__m128 xDif = _mm_sub_ps(
	xPos,
	_mm_load_ps(xPosition + j));
00CBA13B  mov         eax, dword ptr[this]
00CBA13E  mov         ecx, dword ptr[eax + 4]
00CBA141  mov         edx, dword ptr[ebp - 7Ch]
00CBA144  movaps      xmm0, xmmword ptr[ecx + edx * 4]
00CBA148  movaps      xmmword ptr[ebp - 300h], xmm0
00CBA14F  movaps      xmm0, xmmword ptr[ebp - 30h]
00CBA153  subps       xmm0, xmmword ptr[ebp - 300h]
00CBA15A  movaps      xmmword ptr[ebp - 2E0h], xmm0
00CBA161  movaps      xmm0, xmmword ptr[ebp - 2E0h]
00CBA168  movaps      xmmword ptr[ebp - 0A0h], xmm0 
// Since I created a temp variable I understand why you might do it this way.  But really we have so many
// registers open we should take advantage of them.

		__m128 yDif = _mm_sub_ps(
		yPos,
		_mm_load_ps(yPosition + j));
00CBA16F  mov         eax, dword ptr[this]
00CBA172  mov         ecx, dword ptr[eax + 0Ch]
00CBA175  mov         edx, dword ptr[ebp - 7Ch]
00CBA178  movaps      xmm0, xmmword ptr[ecx + edx * 4]
00CBA17C  movaps      xmmword ptr[ebp - 2C0h], xmm0
00CBA183  movaps      xmm0, xmmword ptr[ebp - 50h]
00CBA187  subps       xmm0, xmmword ptr[ebp - 2C0h]
00CBA18E  movaps      xmmword ptr[ebp - 2A0h], xmm0
00CBA195  movaps      xmm0, xmmword ptr[ebp - 2A0h]
00CBA19C  movaps      xmmword ptr[ebp - 0C0h], xmm0

		
		__m128 radiusAdd = _mm_add_ps(rad, _mm_load_ps(radius + j));
00CBA1A3  mov         eax, dword ptr[this]
00CBA1A6  mov         ecx, dword ptr[eax + 14h]
00CBA1A9  mov         edx, dword ptr[ebp - 7Ch]
00CBA1AC  movaps      xmm0, xmmword ptr[ecx + edx * 4]
00CBA1B0  movaps      xmmword ptr[ebp - 280h], xmm0
00CBA1B7  movaps      xmm0, xmmword ptr[ebp - 70h]
00CBA1BB  addps       xmm0, xmmword ptr[ebp - 280h]
00CBA1C2  movaps      xmmword ptr[ebp - 260h], xmm0
00CBA1C9  movaps      xmm0, xmmword ptr[ebp - 260h]
00CBA1D0  movaps      xmmword ptr[ebp - 0E0h], xmm0

		
		_mm_store_ps(isCollided[i] + j,//And then we store into isCollided.
			_mm_cmplt_ps(
				_mm_add_ps(
					_mm_mul_ps(xDif, xDif)
					,
					_mm_mul_ps(yDif, yDif))
				,
				_mm_mul_ps(radiusAdd, radiusAdd)
			));
00CBA1D7  movaps      xmm0, xmmword ptr[ebp - 0E0h]
00CBA1DE  mulps       xmm0, xmmword ptr[ebp - 0E0h]
00CBA1E5  movaps      xmmword ptr[ebp - 1E0h], xmm0
00CBA1EC  movaps      xmm0, xmmword ptr[ebp - 0C0h]
00CBA1F3  mulps       xmm0, xmmword ptr[ebp - 0C0h]
00CBA1FA  movaps      xmmword ptr[ebp - 220h], xmm0
00CBA201  movaps      xmm0, xmmword ptr[ebp - 0A0h]
00CBA208  mulps       xmm0, xmmword ptr[ebp - 0A0h]
00CBA20F  movaps      xmmword ptr[ebp - 240h], xmm0
00CBA216  movaps      xmm0, xmmword ptr[ebp - 240h]
00CBA21D  addps       xmm0, xmmword ptr[ebp - 220h]
00CBA224  movaps      xmmword ptr[ebp - 200h], xmm0
00CBA22B  movaps      xmm0, xmmword ptr[ebp - 200h]
00CBA232  cmpltps     xmm0, xmmword ptr[ebp - 1E0h]
00CBA23A  movaps      xmmword ptr[ebp - 1C0h], xmm0
00CBA241  movaps      xmm0, xmmword ptr[ebp - 1C0h] //See how much they put things in and out of xmm0?
00CBA248  mov         eax, dword ptr[ebp - 14h]  //It seems like a waste.
00CBA24B  mov         ecx, dword ptr[this]
00CBA24E  mov         edx, dword ptr[ecx + eax * 4 + 18h]
00CBA252  mov         eax, dword ptr[ebp - 7Ch]
00CBA255  movaps      xmmword ptr[edx + eax * 4], xmm0


		j += 4;// Then grab the next four to compare with.
	00CBA259  mov         eax, dword ptr[ebp - 7Ch]
		00CBA25C  add         eax, 4
		00CBA25F  mov         dword ptr[ebp - 7Ch], eax //This is all fine and obvious.
} while (j < NUM_CIRCLES);
	00CBA262  cmp         dword ptr[ebp - 7Ch], 3E8h
		00CBA269  jl          SIMDOptimizedCircles::CheckForCollisions + 0CBh (0CBA13Bh)
}*/

// So it's like the compiler has no awareness of XMM1 - XMM7.  However, I do, so let's see
// how I use them.

int i = 0;

	__asm{
		mov edi, dword ptr[this];//Move this to edi, edi will store this
		mov ebx, [edi].xPosition;//store xPosition in ebx
		mov ecx, [edi].yPosition;//store yPosition in ecx
		mov edx, [edi].radius;//store radius in edx
		mov edi, [edi].isCollided;//store isCollided in edi, since we don't need this anymore actually.
		xor esi, esi;//0 out esi which will be our "i" counter.

	OuterLoop:

		mov eax, esi;//Make a "j" counter that starts at i.  J is in eax
		and al, 0xF0;//make sure it's a multiple of four, round down.

		movss xmm0, dword ptr[ebx + esi]//Okay, see, this is a lot faster to shuffle.
		movss xmm1, dword ptr[ecx + esi];//Significantly fewer instructions.
		movss xmm2, dword ptr[edx + esi];//No storing really required.
		shufps xmm0, xmm0, 0;
		shufps xmm1, xmm1, 0;
		shufps xmm2, xmm2, 0;


		// We don't actually need esi in the program... so this seems okay.
		mov i, esi; //When you push a register it doesn't seem to get rid of the value in it
		mov esi, dword ptr[edi + esi]; //So that means this code should still function.
		// So from now on esi is the pointer pointing to the current i's isCollided array in the buffer.

	CollisionStart:
		movaps xmm3, xmmword ptr[ebx + eax];//So load the current 4 xPositions into xmm3.
		movaps xmm4, xmmword ptr[ecx + eax];//xmm4 is the yPositions
		movaps xmm5, xmmword ptr[edx + eax];//xmm5 is the radiuses.
		// Note that since we stored the xPosition, yPosition, and zPosition into buffers without
		// worrying about temp storage we could really really quickly load the right ones here.

		subps xmm3, xmm0;//(x2-x1)
		subps xmm4, xmm1;//(y2-y1)
		addps xmm5, xmm2;//(r1+r2)
		mulps xmm3, xmm3;//(x2-x1)^2
		mulps xmm4, xmm4;//(y2-y1)^2
		mulps xmm5, xmm5;//(r1 + r2)^2
		addps xmm3, xmm4;//(x2-x1)^2 + (y2-y1)^2
		
		cmpltps xmm3, xmm5;//(x2-x1)^2 + (y2-y1)^2 < (r1+r2)^2

		// See?  Not a single load required during the math.  And we still have xmm6 and xmm7 free.
		
		movaps xmmword ptr[esi + eax], xmm3;//Store the result into isCollided[i] + j (esi + j)

		add eax, 16;// Push j up by 4, multiplied by 4 because it's a floating point and that's 4 bytes.
		cmp eax, NUM_CIRCLES * 4;//Multiply j by 4, multiply this by 4.
		jl CollisionStart;//Jump if less than, yadah yadah.

		mov esi, i;//Oh hey look it's i again.  Remember what I said about pulling things out of loops?
		// Before I pulled this push and pop out of the loop this code took 0.26 seconds to run.  After
		// it went down to 0.19.  Yeah, those sorts of things are important.

		add esi, 4;//increase i by 1 * 4.
		cmp esi, NUM_CIRCLES * 4;//*4 for both.
		jl OuterLoop;
	}

}

// So yeah.
//
// That was WAY fewer instructions to accomplish the same goal.  Number of instructions isn't what matters,
// really it's what they do, but even so the shear number of loads made the difference.
// It makes me wonder if visual studios compiler is even capable of using the other SSE registers.
//
// Note that I had never used assembly before writing this tutorial, but learned it as I was writing.
// If you see any mistakes that I made feel free to point that out with the rest of the tutorials
// at bit.ly/programgames.  Honestly I just saw how many instructions were in the dissassembly and was 
// like "Hey, I bet I could write code that goes faster than that."  And I also really wanted to get it
// under 2 seconds. 
//  
// Hopefully reading through the process by which I optimized my code will help inspire you on how to
// do so with the code you write.  It's really quite satisfying to see the numbers get lower and lower as
// you profile.