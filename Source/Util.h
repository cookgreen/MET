/*
Copyright (c) 2007 Johan Gustafsson

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#pragma once

using namespace System::Runtime::InteropServices;

__inline std::string GetUnmanagedString(System::String^ str) {
	const char* str2 = (char*)(void*)Marshal::StringToHGlobalAnsi(str);
	std::string str3(str2);
	Marshal::FreeHGlobal((System::IntPtr)(void*)str2);
	return str3;
}

__inline Mogre::Vector3 GetManagedVector3(Ogre::Vector3 v) {
	Mogre::Vector3 v2;
	v2.x = v.x;
	v2.y = v.y;
	v2.z = v.z;
	return v2;	
}

__inline Ogre::Vector3 GetUnmanagedVector3(Mogre::Vector3 v) {
	Ogre::Vector3 v2;
	v2.x = v.x;
	v2.y = v.y;
	v2.z = v.z;
	return v2;	
}

__inline Ogre::ColourValue GetUnmanagedColourValue(Mogre::ColourValue c) {
	Ogre::ColourValue c2;
	c2.r = c.r;
	c2.g = c.g;
	c2.b = c.b;
	c2.a = c.a;
	return c2;	
}

__inline Ogre::Ray GetUnmanagedRay(Mogre::Ray^ ray) {
	Ogre::Ray r;

	r.setDirection(GetUnmanagedVector3(ray->Direction));
	r.setOrigin(GetUnmanagedVector3(ray->Origin));

	return r;
}