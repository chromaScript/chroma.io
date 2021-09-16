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

1. After opening the program, use the menu bar "File -> New" to open the "Create New Document"
popup window. Document size can be set by changing the text values.
2. Make sure to change tool from the pointer to any other tool before clicking the canvas or
Tool Settings panel.
3. To save the current Tool configurations, use "Toolbox -> Export All". The tools are located in
the /assets/tools folder, and will automatically load on future launches.
4. To use a stylus, ensure that Windows Ink is turned on in your Tablet Driver settings
5. If the stylus is not lined up to the cursor location, close the program, locate the file
named "config.txt" inside the /config folder, and open it. Where the setting named 'WINTABLET_DOWNSCALEFACTOR'
is, change it's value to 0 and save the file. Re-launch the program, and hover the stylus above the window
while moving the pen slowly around the screen. Hover for about 30 seconds. Once stylus callibration is
complete, the "config.txt" file should update and show a new value for 'WINTABLET_DOWNSCALEFACTOR'.
6. Use CTRL+S to render the canvas to a bitmap file in the /renders folder.
7. While it is possible to use "File -> Close" and "File -> New" to close and make new documents during runtime,
this can sometimes crash the program, and may cause memory leaks.

################################################################
##
## Default Tool Hotkeys
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

D - Reset Default Colors (Black/White)
X - Swap FG/BG Colors

CTRL+S - Save Current Canvas Image
	Note: Outputs to the /renders/ folder where the program is unzipped to.