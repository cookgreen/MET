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

#include "ETTerrainInfo.h"

namespace MET {

	public ref class TerrainInfo : System::IDisposable {
	private:
		ET::TerrainInfo* terrainInfo;
		bool owner;

	internal:
		TerrainInfo(ET::TerrainInfo* info) {
			terrainInfo = info;
			owner = false;
		}

	public:
		TerrainInfo() {
			terrainInfo = new ET::TerrainInfo();
			owner = true;
		}

		TerrainInfo(unsigned int width, unsigned int height, array<float>^ heightmap) {
			pin_ptr<float> p = &heightmap[0];
			float* hm = p;
			terrainInfo = new ET::TerrainInfo(width, height, hm);
			owner = true;
		}

		~TerrainInfo() {
			this->!TerrainInfo();
		}

		void SetHeightmap(unsigned int width, unsigned int height, array<float>^ heightmap) {
			pin_ptr<float> p = &heightmap[0];
			float* hm = p;
			terrainInfo->setHeightmap(width, height, hm);
		}

		property float default[unsigned int, unsigned int] {
			float get(unsigned int x, unsigned int z) {
				return terrainInfo->at(x, z);
			}
			void set(unsigned int x, unsigned int z, float value) {
				float& f = terrainInfo->at(x, z);
				f = value;
			}
		}

		float At(unsigned int x, unsigned int z) {
			return terrainInfo->at(x, z);
		}

		int PosToVertexX(float x) {
			return terrainInfo->posToVertexX(x);
		}

		int PosToVertexZ(float z) {
			return terrainInfo->posToVertexZ(z);
		}

		float VertexToPosX(int x) {
			return terrainInfo->vertexToPosX(x);
		}

		float VertexToPosZ(int z) {
			return terrainInfo->vertexToPosZ(z);
		}

		float GetHeightAt(float x, float z) {
			return terrainInfo->getHeightAt(x, z);
		}

		Mogre::Vector3 GetNormalAt(float x, float z) {
			return GetManagedVector3(terrainInfo->getNormalAt(x, z));	
		}

		Mogre::Vector3 RayIntersects(Mogre::Ray^ ray) {
			Ogre::Ray r = GetUnmanagedRay(ray);
			std::pair<bool, Ogre::Vector3> p = terrainInfo->rayIntersects(r);
			return GetManagedVector3(p.second);
		}

		property array<float>^ HeightmapData {
			array<float>^ get() {
				std::vector<float> hm = terrainInfo->getHeightmapData();

				array<float>^ heightmap = gcnew array<float>(hm.size());
				for(unsigned int i = 0; i < hm.size(); i++) {
					heightmap[i] = hm[i];
				}

				return heightmap;
			}
		}

		property unsigned int Width {
			unsigned int get() {
				return terrainInfo->getWidth();
			}
		}

		property unsigned int Height {
			unsigned int get() {
				return terrainInfo->getHeight();
			}
		}

		property Mogre::AxisAlignedBox^ Extents {
			Mogre::AxisAlignedBox^ get() {
				return (Mogre::AxisAlignedBox^)terrainInfo->getExtents();
			}
			void set (Mogre::AxisAlignedBox^ extents) {
				terrainInfo->setExtents((Ogre::AxisAlignedBox)extents);
			}
		}

		property Mogre::Vector3 Scaling {
			Mogre::Vector3 get() {
				return GetManagedVector3(terrainInfo->getScaling());
			}
		}

		property Mogre::Vector3 Offset {
			Mogre::Vector3 get() {
				return GetManagedVector3(terrainInfo->getOffset());
			}
		}

		static void LoadHeightmapFromImage(TerrainInfo^ info, Mogre::Image^ image) {
			ET::loadHeightmapFromImage(*info->_getNativePtr(), *(Ogre::Image*)image);
		}

		static void SaveHeightmapToImage(TerrainInfo^ info, Mogre::Image^ image) {
			SaveHeightmapToImage(info, image, 2);
		}

		static void SaveHeightmapToImage(TerrainInfo^ info, Mogre::Image^ image, unsigned int bpp) {
			ET::saveHeightmapToImage(*info->_getNativePtr(), *(Ogre::Image*)image, bpp);
		}

		static void LoadHeightmapFromRawData(TerrainInfo^ info, Mogre::DataStream^ stream, unsigned int width, unsigned int height) {
			ET::loadHeightmapFromRawData(*info->_getNativePtr(), *(Ogre::DataStream*)stream, width, height);
		}

		static void SaveHeightmapToRawData(TerrainInfo^ info, System::IO::BinaryWriter^ stream) {
			SaveHeightmapToRawData(info, stream, 2);
		}

		static void SaveHeightmapToRawData(TerrainInfo^ info, System::IO::BinaryWriter^ writer, int bpp) {
			if(bpp < 1 || bpp > 4) 
				throw gcnew System::Exception("Bpp must be between 1 and 4");

			unsigned int maxVal = (1 << (bpp * 8)) - 1;
			for(unsigned int j = 0; j < info->Height; j++) {
				for(unsigned int i = 0; i < info->Width; i++) {
					unsigned int val = (unsigned int)(maxVal * info->At(i, j));
					writer->Write(val);
				}
			}
		}

		static void CreateTerraingLightmap(TerrainInfo^ info, Mogre::Image^ image, unsigned int width, unsigned int height, Mogre::Vector3 lightDir, Mogre::ColourValue lightCol, Mogre::ColourValue ambient) {
			CreateTerraingLightmap(info, image, width, height, lightDir, lightCol, ambient, true);
		}

		static void CreateTerraingLightmap(TerrainInfo^ info, Mogre::Image^ image, unsigned int width, unsigned int height, Mogre::Vector3 lightDir, Mogre::ColourValue lightCol, Mogre::ColourValue ambient, bool shadowed) {
			ET::createTerrainLightmap(*info->_getNativePtr(), *(Ogre::Image*)image, width, height, GetUnmanagedVector3(lightDir), GetUnmanagedColourValue(lightCol), GetUnmanagedColourValue(ambient), shadowed);
		}

		ET::TerrainInfo* _getNativePtr() {
			return terrainInfo;
		}

	protected:
		!TerrainInfo() {
			if(owner) {
				delete terrainInfo;
			}
		}
	};
}