------------------------------------------------
Managed Editable Terrain Manager
------------------------------------------------
A managed wrapper for Editable Terrain Manager

------------------------------------------------
License
------------------------------------------------
See License.txt

------------------------------------------------
Documentation
------------------------------------------------
See Editable Terrain Manager documentation

------------------------------------------------
Installation
------------------------------------------------
Add MET.dll to your projects references. You will also need EditableTerrainManager(_d).dll both files 
are located in Binaries\Debug(Release)

To run the sample you will need to copy the appropriate dll files from your MOgre directory.

------------------------------------------------
Compiling
------------------------------------------------
Download the Editable Terrain Manager sources and place it in Dependencies.
Reconfigure ETM to link against Mogre headers / libraries
Rebuild ETM
Build MET.

------------------------------------------------
History
------------------------------------------------
October 10 2008
 - Cleaned up, recompiled - both debug and release works.

July 26 2008
 - Recompiled against Mogre 1.4.8

Jan 12 2008
 - Antother small patch by Kerion, this one adds Brush[x, y] and 
   TerrainInfo[x, y] indexers

Jan 11 2008
 - Fix by Kerion to get correct referencing to ET::TerrainInfo*
 - GetTerrainInfo renamed to TerrainInfo in the same fix

Jan 09 2008
 - MET was released