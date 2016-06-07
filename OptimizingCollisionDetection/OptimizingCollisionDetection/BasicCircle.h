/*
Title: Optimizing Collision Detection
File Name: BasicCircle.h
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
A simple circle class with no optimizations.
*/
#pragma once

// As you can see basic circle is very... well... basic.
class BasicCircle
{
public:
	// We're not using anything like a vector struct just to ease the
	// simplicty of the code we're writing.
	float xPosition;
	float yPosition;
	float xVelocity;
	float yVelocity;
	float radius;

	BasicCircle();
	~BasicCircle();

	bool CheckForCollision(BasicCircle otherCircle);
	void Update();
};

// When you're finished here (not like there's much here at all) go to
// BasicCircle.cpp

