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

#include "ETTerrainManager.h"
#include "OgreSceneManager.h"

#include "TerrainInfo.h"
#include "Brush.h"

namespace MET {

	public ref class TerrainManager : System::IDisposable {
	private:
		ET::TerrainManager* terrainManager;
		MET::TerrainInfo^ terrainInfo;

	public:

		TerrainManager(Mogre::SceneManager^ sceneMgr) {
			Ogre::SceneManager* sm = (Ogre::SceneManager*)sceneMgr;
			terrainManager = new ET::TerrainManager(sm);
		}

		TerrainManager(Mogre::SceneManager^ sceneMgr, System::String^ name) {
			Ogre::SceneManager* sm = (Ogre::SceneManager*)sceneMgr;
			terrainManager = new ET::TerrainManager(sm, GetUnmanagedString(name));
		}

		~TerrainManager() {
			delete terrainInfo;
			this->!TerrainManager();
		}

		void CreateTerrain(TerrainInfo^ info) {
			CreateTerrain(info, 33);
		}

		void CreateTerrain(MET::TerrainInfo^ info, unsigned int tileSize) {
			CreateTerrain(info, tileSize, 255);
		}

		void CreateTerrain(MET::TerrainInfo^ info, unsigned int tileSize, int maxLOD) {
			CreateTerrain(info, tileSize, maxLOD, false);
		}

		void CreateTerrain(MET::TerrainInfo^ info, unsigned int tileSize, int maxLOD, bool vertexNormals) {
			// 1/11/08: Kerion @ Ogre3D: We now take the getTerrainInfo() reference in aggregate instead
			// of keeping our own reference. MET::TerrainInfo is now aware of whether it owns, or does not
			// own, the reference to the ET::TerrainInfo.
			terrainManager->createTerrain(*info->_getNativePtr(), tileSize, maxLOD, vertexNormals);
			terrainInfo = gcnew MET::TerrainInfo((ET::TerrainInfo*)(&terrainManager->getTerrainInfo()));
		}

		void SetUseLodMorphing() {
			SetUseLodMorphing(true);
		}

		void SetUseLodMorphing(bool lodMorph) {
			SetUseLodMorphing(lodMorph, 0.25f);
		}

		void SetUseLodMorphing(bool lodMorph, float startMorphing) {
			SetUseLodMorphing(lodMorph, startMorphing, "morphFactor");
		}

		void SetUseLodMorphing(bool lodMorph, float startMorphing, System::String^ morphParamName) {
			terrainManager->setUseLODMorphing(lodMorph, startMorphing, GetUnmanagedString(morphParamName));
		}

		void SetLodErrorMargin(unsigned int maxPixelError, unsigned int viewPortHeight) {
			terrainManager->setLODErrorMargin(maxPixelError, viewPortHeight);
		}

		void Deform(int x, int z, Brush^ brush) {
			Deform(x, z, brush, 1.0f);
		}

		void Deform(int x, int z, Brush^ brush, float intensity) {
			terrainManager->deform(x, z, *brush->_getNativePtr(), intensity);
		}

		void SetHeights(int x, int z, Brush^ brush) {
			terrainManager->setHeights(x, z, *brush->_getNativePtr());
		}

		void GetHeights(int x, int z, Brush^ brush) {
			terrainManager->getHeights(x, z, *brush->_getNativePtr());
		}

		property MET::TerrainInfo^ TerrainInfo {
			MET::TerrainInfo^ get() {
				return terrainInfo;
			}
		}

		property Mogre::MaterialPtr^ Material {
			Mogre::MaterialPtr^ get() {
				return (Mogre::MaterialPtr^)terrainManager->getMaterial();
			}
			void set(Mogre::MaterialPtr^ value) {
				terrainManager->setMaterial((Ogre::MaterialPtr)value);
			}
		}

	protected:
		!TerrainManager() {
			delete terrainManager;
		}
	};
}