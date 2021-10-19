chroma.io - 0.0.6c

################################################################
##
## Forward
##
################################################################

The program is still very much in development. Unexpected crashes can happen, save often.
Memory leaks may be present, there's a lot of data moving around. If performance gets sluggish
over time, or resource usage appears high, send a report to adam.taylor@chromatinker.com. The
software relies almost entirely on GPU rendering, so expect to see moderate GPU utilization rates.
Expect to encounter oddities when using the UI. The UI engine is built from scratch using
just openGL and Freetype2, and loads entirely from external files in the plugins & config folders.
While entirely customizable, be careful editing the UI and script files, there is no
way to diagnose compilation errors without running inside debug mode in VS.

################################################################
##
## Basic Instructions
##
################################################################

Supported Devices - Windows 10, OpenGL 4.0+ Support

1. After opening the program, use the menu bar "File -> New" to open the "Create New Document"
popup window. Document size can be set by changing the text values. Avoid using a canvas larger than 8k x 8k pixels in total size.
2. To save the current Tool configurations, use "Toolbox -> Export All". The tools are located in
the /assets/tools folder, and will automatically load on future launches.
3. To use a stylus, ensure that Windows Ink is turned on in your Tablet Driver settings
4. If the stylus is not lined up to the cursor location, close the program, locate the file
named "config.txt" inside the /config folder, and open it. Where the setting named 'WINTABLET_DOWNSCALEFACTOR'
is, change it's value to 0 and save the file. Re-launch the program, and hover the stylus above the window
while moving the pen slowly around the screen. Hover for about 30 seconds. Once stylus callibration is
complete, the "config.txt" file should update and show a new value for 'WINTABLET_DOWNSCALEFACTOR'.
5. Use CTRL+S to render the canvas to a bitmap file in the /renders folder.
6. While it is possible to use "File -> Close" and "File -> New" to close and make new documents during runtime,
this can sometimes crash the program, and may cause memory leaks.
7. In order to save tool presets, use the Menu Bar. Toolbox -> Export All will save all tools, including zoom/rotate/pan,
to a folder in the .exe's directory under "/assets/tools/". Tool files are basic .txt files with a .tf extension, and 
can be edited externally if needed.
8. If for any reason tools being loaded from the "assets/tools/" folder are causing runtime issues, either clear the folder,
or move the suspected trouble brushes to a different folder. 

################################################################
##
## Control/Noise Instructions
##
################################################################

1. To access the stylus curve graphs for a given setting, click the "Control" Icon (Graph Icon) for the setting.
2. To enable pen control, the checkbox for the Interest Mask (Alpha, Color, Scattering, etc.) must be on,
and the checkbox next to the desired Control Icon for the setting must also be on, as well as at least one of the
stylus dynamics inside the Control Pop-Up Panel.
3. Clicking a Graph on the Control panel will add a new Handle. Clicking while holding "CTRL" will delete a Handle (Red indicator)
4. Click and drag handles or the top/bottom (End/Start) handles to adjust the graph.
5. The slider next to the Graph is the Dampening value. 1.0 has no effect. Values lower than 1.0 push the resulting values
towards 1.0. Values higher than 1.0 push the values towards 0.0
6. For a control setting to work, ensure the Range sliders for that node (Left-hand-side of the Control Pop-up) are set as desired.
7. The Pressure, Direction, Tilt, Velocity, Rotation, and Function Icons below the Range sliders for a node toggle an input on/off.

1. To access the noise settings for a given setting, click the "Noise" Icon (Checkered-Gradient Icon) for the setting.
2. For noise to be enabled, the checkbox next to the Interest Mask, the checkbox next to the Noise Icon, and either 1D or 2D Noise
Enabled checkboxes must all be toggled on. When 1D and 2D noise are both enabled, they follow the Noise Blend Mode rules to combine.
3. For best 2D noise results, it is recommended to use "Scale to Canvas" as "On" and scale values between 1.0-10.0 

################################################################
##
## Default Tool Hotkeys (Use Tool Settings -> MetaData to edit)
##
################################################################

Z - Zoom Camera Tool
H - Pan Camera Tool
R - Rotate Camera Tool
	SHIFT - Snap Rotation Mode
		Note: Edit the angle & keys under 'Rotate' Tool Settings
B - Brush Tool
	SHIFT - When starting a new brush stroke, hold SHIFT when clicking down
	to connect the end of the last stroke to the start of the next in a straight line
	SHIFT - While drawing, hold SHIFT to axis-constrain drawing to 90/0/45/-45 angles
		Note: Edit the angles & keys under 'Continuous' Tool Settings
I - Color Picker
	ALT - Sample to Background Color
Q - Drag-Shape Field Brush
	CTRL - Size By Center Mode
	SPACE - Move Shape Mode
	ALT - Change Size Mode
W - Rectangle Brush
	CTRL - Size By Center Mode
	SPACE - Move Shape Mode
	ALT - Change Rotation Mode
	SHIFT - Constrain Proportion (1:1) Mode
E - Vortex Brush Tool
A - Rake Brush Tool
F - Fan Brush Tool
	Note: Toggle Alpha for Airbrush Mode
K - Shape Draw Brush Tool
Y - Spline Draw Brush Tool
	F4 - Record Next Brush Stroke

D - Reset Default Colors (Black/White)
X - Swap FG/BG Colors

CTRL+S - Save Current Canvas Image
	Note: Outputs to the /renders/ folder where the program is unzipped to.