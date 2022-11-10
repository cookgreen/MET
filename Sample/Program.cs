using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace Sample {
	static class Program {
		static Mogre.Camera camera;
		static Mogre.SceneManager sceneManager;
		static Mogre.RenderWindow window;
		static bool running = false;

		static MET.TerrainManager terrainManager;
		static MET.TerrainInfo terrainInfo;
		static MET.SplattingManager splattingManager;
		static MET.Brush brush;

		static bool middleMouseDown = false;

		static int oX = 0, oY = 0;
		static bool resetMousePos = false;

		static bool deform = true;

		static Mogre.RaySceneQuery raySceneQuery;
		static Mogre.Vector3 mousePos = new Mogre.Vector3(0, 0, 0);

		static uint chosenTexture = 0;

		static bool leftMouseDown = false;
		static bool rightMouseDown = false;

		static void Main(string[] args) {
			MainForm app = new MainForm();

			app.ClientSize = new System.Drawing.Size(800, 600);
			app.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;

			Mogre.Root root = new Mogre.Root("", "", "MainForm.log");

			root.LoadPlugin("RenderSystem_Direct3D9");
			root.LoadPlugin("Plugin_OctreeSceneManager");
			root.LoadPlugin("Plugin_CgProgramManager");

			Mogre.RenderSystem renderSystem = root.GetRenderSystemByName("Direct3D9 Rendering Subsystem");
			root.RenderSystem = renderSystem;

			root.Initialise(false);

			Mogre.NameValuePairList misc = new Mogre.NameValuePairList();
			misc["externalWindowHandle"] = app.Handle.ToString();
			window = root.CreateRenderWindow("renderWindow", (uint)app.ClientSize.Width, (uint)app.ClientSize.Height, false, misc);

			sceneManager = root.CreateSceneManager("OctreeSceneManager");

			camera = sceneManager.CreateCamera("the_camera");
			camera.SetPosition(200, 500, 200);
			camera.NearClipDistance = 0.1f;
			camera.FarClipDistance = 6000.0f;

			Mogre.Viewport viewport = window.AddViewport(camera);
			viewport.BackgroundColour = new Mogre.ColourValue(0.5f, 0.5f, 0.9f, 1.0f);

			camera.AspectRatio = (float)window.Width / (float)window.Height;

			Mogre.ResourceGroupManager.Singleton.AddResourceLocation("..\\media", "FileSystem", "ET");
			Mogre.ResourceGroupManager.Singleton.AddResourceLocation("..\\media\\ET", "FileSystem", "ET");
			Mogre.ResourceGroupManager.Singleton.InitialiseAllResourceGroups();

			terrainManager = new MET.TerrainManager(sceneManager);
			terrainManager.SetLodErrorMargin(2, (uint)viewport.ActualHeight);
			terrainManager.SetUseLodMorphing(true, 0.2f, "morphFactor");

			float[] hm = new float[513 * 513];
			for (int i = 0; i < 513 * 513; i++)
				hm[i] = 0.5f;
			terrainInfo = new MET.TerrainInfo(513, 513, hm);
			terrainInfo.Extents = new Mogre.AxisAlignedBox(0, 0, 0, 2000, 512, 2000);
			terrainManager.CreateTerrain(terrainInfo);
			terrainInfo.Dispose();
			terrainInfo = terrainManager.TerrainInfo;

			splattingManager = new MET.SplattingManager("ETSplatting", "ET", 512, 512, 3);
			splattingManager.NumTextures = 6;

			Mogre.MaterialPtr material = Mogre.MaterialManager.Singleton.GetByName("ETTerrainMaterial");
			terrainManager.Material = material;

			Mogre.Image image = new Mogre.Image();
			image.Load("brush.png", "ET");
			image.Resize(16, 16);
			brush = MET.Brush.LoadBrushFromImage(image);

			raySceneQuery = sceneManager.CreateRayQuery(new Mogre.Ray());

			app.MouseDown += OnMouseDown;
			app.MouseUp += OnMouseUp;
			app.MouseMove += OnMouseMove;
			app.KeyUp += OnKeyUp;
			app.KeyDown += OnKeyDown;

			running = true;

			app.Show();

			Timer.Start();

			while (running) {
				float delta = Timer.Delta();
				if (rightMouseDown || leftMouseDown) {
					if (deform) {
						float intens = delta * 0.2f * ((leftMouseDown) ? 1.0f : -1.0f);

						int x = terrainInfo.PosToVertexX(mousePos.x);
						int z = terrainInfo.PosToVertexZ(mousePos.z);
						terrainManager.Deform(x, z, brush, intens);
					} else {
						float intens = delta * 3 * ((leftMouseDown) ? 1.0f : -1.0f);

						int x = terrainInfo.PosToVertexX(mousePos.x);
						int z = terrainInfo.PosToVertexZ(mousePos.z);

						splattingManager.Paint(chosenTexture, x, z, brush, intens);
					}
				}

				root.RenderOneFrame();
				System.Windows.Forms.Application.DoEvents();
				running = running && !app.IsDisposed;
			}

			splattingManager.Dispose();
			brush.Dispose();
			terrainInfo.Dispose();
			terrainManager.Dispose();

			material.Dispose();

			root.Dispose();
			root = null;
		}

		static void OnMouseDown(object sender, System.Windows.Forms.MouseEventArgs e) {
			if (e.Button == System.Windows.Forms.MouseButtons.Middle)
				middleMouseDown = true;

			if (e.Button == System.Windows.Forms.MouseButtons.Left)
				leftMouseDown = true;
			if (e.Button == System.Windows.Forms.MouseButtons.Right)
				rightMouseDown = true;
		}

		static void OnMouseUp(object sender, System.Windows.Forms.MouseEventArgs e) {
			if (e.Button == System.Windows.Forms.MouseButtons.Middle) {
				middleMouseDown = false;
				resetMousePos = true;
			}

			if (e.Button == System.Windows.Forms.MouseButtons.Left)
				leftMouseDown = false;
			if (e.Button == System.Windows.Forms.MouseButtons.Right)
				rightMouseDown = false;
		}

		static void OnMouseMove(object sender, System.Windows.Forms.MouseEventArgs e) {
			int dx = (resetMousePos) ? 0 : oX - e.X;
			int dy = (resetMousePos) ? 0 : oY - e.Y;

			oX = e.X;
			oY = e.Y;

			if (middleMouseDown) {
				camera.Yaw(dx * 0.1f);
				camera.Pitch(dy * 0.1f);

				resetMousePos = false;
			}

			uint w, h, cd;
			int l, r;

			window.GetMetrics(out w, out h, out cd, out l, out r);

			Mogre.Ray ray = camera.GetCameraToViewportRay((float)e.X / (float)w, (float)e.Y / (float)h);
			Mogre.Vector3 result = terrainInfo.RayIntersects(ray);
			if (result != null) {
				mousePos.x = result.x;
				mousePos.y = result.y;
				mousePos.z = result.z;
			}
		}

		static void OnKeyDown(object sender, System.Windows.Forms.KeyEventArgs e) {
			if (middleMouseDown) {
				if (e.KeyCode == System.Windows.Forms.Keys.W)
					camera.MoveRelative(new Mogre.Vector3(0, 0, -9));
				if (e.KeyCode == System.Windows.Forms.Keys.S)
					camera.MoveRelative(new Mogre.Vector3(0, 0, 9));
				if (e.KeyCode == System.Windows.Forms.Keys.A)
					camera.MoveRelative(new Mogre.Vector3(-9, 0, 0));
				if (e.KeyCode == System.Windows.Forms.Keys.D)
					camera.MoveRelative(new Mogre.Vector3(9, 0, 0));
			}
		}

		static void OnKeyUp(object sender, System.Windows.Forms.KeyEventArgs e) {
			if (e.KeyCode == System.Windows.Forms.Keys.F5)
				deform = !deform;

			if (e.KeyCode == System.Windows.Forms.Keys.D1)
				chosenTexture = 0;
			if (e.KeyCode == System.Windows.Forms.Keys.D2)
				chosenTexture = 1;
			if (e.KeyCode == System.Windows.Forms.Keys.D3)
				chosenTexture = 2;
			if (e.KeyCode == System.Windows.Forms.Keys.D4)
				chosenTexture = 3;
			if (e.KeyCode == System.Windows.Forms.Keys.D5)
				chosenTexture = 4;
			if (e.KeyCode == System.Windows.Forms.Keys.D6)
				chosenTexture = 5;
			if (e.KeyCode == System.Windows.Forms.Keys.Escape)
				running = false;

			if (e.KeyCode == System.Windows.Forms.Keys.F10) {
				Mogre.Image img = new Mogre.Image();
				MET.TerrainInfo.SaveHeightmapToImage(terrainInfo, img);
				img.Save("heightmap.png");

				splattingManager.SaveMapToImage(0, img);
				img.Save("splat1.png");

				splattingManager.SaveMapToImage(1, img);
				img.Save("splat2.png");
			}
		}
	}
}
