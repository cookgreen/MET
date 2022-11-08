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

#include "ETSplattingManager.h"

namespace MET {

	public ref class SplattingManager : System::IDisposable {
	private:
		ET::SplattingManager* splattingManager;

	public:

		SplattingManager(System::String^ baseName, System::String^ group, unsigned int width, unsigned int height) {
			splattingManager = new ET::SplattingManager(GetUnmanagedString(baseName), GetUnmanagedString(group), width, height);
		}

		SplattingManager(System::String^ baseName, System::String^ group, unsigned int width, unsigned int height, unsigned int channels) {
			splattingManager = new ET::SplattingManager(GetUnmanagedString(baseName), GetUnmanagedString(group), width, height, channels);
		}

		void LoadMapFromImage(unsigned int mapNum, Mogre::Image^ image) {
			splattingManager->loadMapFromImage(mapNum, *(Ogre::Image*)image);
		}

		void SaveMapToImage(unsigned int mapNum, Mogre::Image^ image) {
			splattingManager->saveMapToImage(mapNum, *(Ogre::Image*)image);
		}

		void Paint(unsigned int texNum, int x, int y, Brush^ brush) {
			Paint(texNum, x, y, brush, 1.0f);
		}

		void Paint(unsigned int texNum, int x, int y, Brush^ brush, float intensity) {
			splattingManager->paint(texNum, x, y, *brush->_getNativePtr(), intensity);
		}

		void CreateColourMap(Mogre::Image^ image, array<Mogre::ColourValue>^ colours) {
			std::vector<Ogre::ColourValue> c;
			c.reserve(colours->Length);

			for(int i = 0; i < colours->Length; i++)
				c[i] = GetUnmanagedColourValue(colours[i]);

			splattingManager->createColourMap(*(Ogre::Image*)image, c);
		}

		void CreateBaseTexture(Mogre::Image^ image, unsigned int width, unsigned int height, array<Mogre::Image^>^ textures, float repeatX, float repeatZ) {
			std::vector<Ogre::Image> t;
			t.reserve(textures->Length);

			for(int i = 0; i < textures->Length; i++)
				t[i] = *(Ogre::Image*)textures[i];

			splattingManager->createBaseTexture(*(Ogre::Image*)image, width, height, t, repeatX, repeatZ);
		}

		static Mogre::Image^ CreateMinimap(Mogre::Image^ colourMap, Mogre::Image^ lightMap) {
			return (Mogre::Image^)ET::createMinimap(*(Ogre::Image*)colourMap, *(Ogre::Image*)lightMap);
		}

		property unsigned int NumTextures {
			unsigned int get() {
				return splattingManager->getNumTextures();
			}
			void set(unsigned int value) {
				splattingManager->setNumTextures(value);
			}
		}

		property unsigned int NumMaps {
			unsigned int get() {
				return splattingManager->getNumMaps();
			}
			void set(unsigned int value) {
				splattingManager->setNumMaps(value);
			}
		}

		property array<System::String^>^ MapTextureNames {
			array<System::String^>^ get() {
				std::vector<std::string> names = splattingManager->getMapTextureNames();

				array<System::String^>^ n = gcnew array<System::String^>(names.size());
				for(unsigned int i = 0; i < names.size(); i++)
					n[i] = gcnew System::String(names[i].c_str());

				return n;
			}
		}

		~SplattingManager() {
			this->!SplattingManager();
		}

	protected:
		!SplattingManager() {
			delete splattingManager;
		}
	};
}