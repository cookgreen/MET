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

#include "ETBrush.h"

namespace MET {

	public ref class Brush : System::IDisposable {
	private:
		ET::Brush* brush;

	public:
		Brush() {
			brush = new ET::Brush();
		}

		Brush(array<float>^ brush, unsigned int width, unsigned int height) {
			pin_ptr<float> p = &brush[0];
			float* b = p;
			this->brush = new ET::Brush(b, width, height);
		}

		~Brush() {
			this->!Brush();
		}

		property unsigned int Width {
			unsigned int get() {
				return brush->getWidth();
			}
		}

		property unsigned int Height {
			unsigned int get() {
				return brush->getHeight();
			}
		}

		property float default[unsigned int, unsigned int] {
			float get(unsigned int x, unsigned int y) {
				return brush->at(x, y);
			}
			void set(unsigned int x, unsigned int y, float value) {
				float& f = brush->at(x, y);
				f = value;
			}
		}

		float At(unsigned int x, unsigned int y) {
			return brush->at(x, y);
		}

		static Brush^ LoadBrushFromImage(Mogre::Image^ image) {
			ET::Brush b = ET::loadBrushFromImage(*(Ogre::Image*)image);

			Brush^ brush = gcnew Brush();
			delete brush->brush;
			brush->brush = new ET::Brush(b);

			return brush;
		}

		static void SaveBrushToImage(Brush^ brush, Mogre::Image^ image) {
			ET::saveBrushToImage(*brush->_getNativePtr(), *(Ogre::Image*)image);
		}

		ET::Brush* _getNativePtr() {
			return brush;
		}

	protected:
		!Brush() {
			delete brush;
		}
	};
}