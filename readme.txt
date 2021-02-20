chroma.io

################################################################
##
## Documentation
##
################################################################
-Remember to copy-paste any "Note:", "Suggestion:" or "Warning:" comments from the code into
the known issues at the time they are written, that way they can be tracked easily using
ctrl-find on the entire project.

################################################################
##
## Interesting/Planned Feature Ideas
## 
################################################################
1. Timer Widget
	-Count up/down, calendar-based events (read system time, not internet)
	-Play sounds from a file???
	-Run a script or other macro along intervals or at start/end
		-As simple as saving a file, but could also run anything
		that's possible to do in the script language, like apply
		layer effects, flip the canvas, or run UI effects like
		pop-up windows
		-Update the UI color scheme based on time of day
			-Technically not necessary to have a widget for this, with the bindLoopEvent()
			callback, could update a color variable or image texture (expensive!!) every frame.
		-In-App method of setting work reminders
2. Notepad/Text Widget
	-Keep notes about a project, could include this as meta-data in the custom file format
	-Write scripts in-app and have some color formatting
	and autocomplete features and enhanced debug messages
	-Allow live WYSIWYG editing of the UI?
3. Transform Tool
	-Apply symmetry to warp transforms, even with freeform warp
4. Color Picker
	-Custom sample size and sample shape
	-Option to apply color jitters to sampling
5. Axis Constrained Drawing
	-Add support for custom angle input & larger/smaller axis increments
		-Include some relatively fast way to snap/rotate/select between sets of user
		defined angles. Useful for isometric drawing
		-Include option to snap by viewport, and snap by canvas
	-Ability to define a custom vector shape as the axis, which can be open or closed
	in form, and can be previewed with a toggle in the UI
		-Perhaps create an editor widget for this that keeps the start/end snapped and
		lets users create whatever bezier and/or line shape between them. This line
		could be set to repeat, or set to stretch.
	-Add programmatic effects to how tools adhere to the axis.
		-Could allow for smoothing + variance settings, which would let the
		tool wander a certain distance from the axis, and smoothing that would
		recalculate to smooth out bumps. Good for drawing straight lines that still
		have an organic feel to them.
			-Related, a "gravity" setting that pulls the brush back towards the axis
			depending on various factors like pressure, speed, time
		-Jitter based variance with smoothing
		-Programmatic function callback with specific signature of 
			vec2 constraintEffect(ToolInput input) {}
			-Takes a ToolInput of (X, Y, Pressure, Rotation, Tilt, Index, Time)
			that has already been pre-constrained to the current axis,
			which the user can access to do calculations and then return a vec2
			with the modified location.
6. Palette(Literal) Widget
	-Gives the user a virtual palette to mix colors on with any tool
	-Pagination for multiple active palettes
	-Also support for creating limited color palettes similar to the Gurney
	technique in Color and Light
7. Color History Widget
	-Simple support for adding color squares for each color pick
	-Advanced support for scatter plotting the history according to various
	axis of difference. Could plot by value, hue, saturation on single axis, or
	combine on two axis. Could also plot along a circular path for color wheel.
	-Itermediate support for sorting by single characteristic, and setting the number
	of rows/columns
	-Could also create characteristic constraints which update all the color
	swatches when a new color is picked.
		-User locks in their color history, stopping the cycling of new states
		-Now, when a new color is picked, rather than add it, all existing swatches
		update to constrain to one or more characteristics of that color
			-Could lock-value of swatches, so as to paint with the same values at different
			hues and saturations
			-Could lock-saturation, or lock-hue as well
8. Tool Engine
	-Support for distance based effects along a number of mathematical factors
		-By total/relative/interval distance traveled, total/relative/interval distance from screen or canvas location
			-Screen location would maybe enable a sub-sampler for the tool on the alt-modifier
			that lets the user place with their pen, rather than type in
		-Fully customizable weight graph option, similar to the PS Gradient Editor, where
		users can set relative/total markers and assign a weight % to them
			-Would allow for a head/tail system, or even a head/neck/body/leg/tail system
			-Could modify any number of properties by distance, even changing the brush tip along
			each point
		-Programmatic callback in the form of
			num distanceEffect(num distance)
			-Gives the user the current distance from starting point in pixel distance
			and returns to the program an output weight %
		-Advanced callback in the form of
			ToolInput distanceEffectPlus(num distance, ToolInput input)
	-Mathmatic/Procedural based properties (such as distance, relative angles, direction, etc.) for
	all or most tools.
		-Support for distance properties on the line tool or pen/curve tool
		-Apply angle-based effects to a line or pen/curve/brush tool by calculating the direction
		in relative or absolute relation to the canvas.
	-Programmatic repetition of tool inputs
		-Copy the last input in whole but mathematically alter the starting location & rotation
		-Support for array application, and repeat this several times
		-Might be useful for hatching/cross-hatching
8.1. Tool Settings Palette
	-A 'virtual' canvas for designing brush tips and other shapes to use with tools.
		-Allow full painting/tool use to hand-craft a brush tip
		-2.0 or 3.0 version could also get into procedural effects and some kind of node
		based editor.
		-User can live-test the brush tip by toggling it as a setting on their current tool
9. Assisted Tool Engine
	-Analysis based tools for fixing lines to be straight, creating ellipses, etc.
		-Similar to the procreate smart shape mode for drawing
	-Take a step further and add more tools like intelligent shape closure, or custom
	shape matching
		-Could create automatic image macro replacement.
			-Depending on the shape detected, ie. number of corners, could macro replace the
			tool visual result with a stored image file, and then automatically enter the typical
			transform widget for that shape in order to apply rotation/scale adjustments before applying
	-Apply procedural effects to detected shapes, like tapers, bends, inflation, etc.
		-Flatten the southern-most portion of an ellipse
		-Taper a rectangle along it's longest axis by a %
		-Inflate/deflate squares/rectangles/polygons between each point
		-Smooth the corners of detected shapes by a %
10. Clipboard Palette
	-Maintain a history of visual data that gets copied using CTRL+C, and allow manual placement into
	the widget
	-Let history states be locked to avoid getting purged
	-Can reaccess these clipboard snippets and paste them again or drag and drop into the canvas
11. Images/Externals Palette
	-A in-app palette of images that can be drag/dropped into the canvas
	-Let the user live-edit the images in a pop-up window
	-Insert the image into the canvas as either raster data, or a linked-object that shares the same
	data as the palette image.
12. Tool Settings Widget
	-CSP like ability to choose what tool settings get exposed on the mini widget for quick access
	-Support for various increment types for settings. Slider, 5-step boxes, numeric input
	-In-Widget support for setting all the associated graphs for complex settings like pressure ramps
	or gradient editors
13. Customizable Tool pop-up Menu
	-Rather than just settings for size, rotation, hardness and tip orientation/scale, allow to also
	pop up any number of settings or presets
14. Customizable PIE Menus
	-A circular or rectangular PIE menu that can be bound to almost any key/mouse button
	-Support for multiple custom PIE menus with different keys
	-Support for sub-PIE menus that appear when clicking one of the PIE buttons
	-Allow for binding virtually anything to the PIE buttons, scripts, macros, tool presets, etc.
	-Add support for multiple types of input control on how PIE gets pressed
		-Press and release while hovering selection
		-Press and click selection
		-Press and press hot key for PIE selection. Would take priority and mask over overlapping binds,
		giving users the ability to substantially increase the keybind space with the virtual PIE key mapping
15. Super-Key Support
	-Similar to PS tool hotkeys, but for everything.
		-Allow multiple actions, tools, scripts, etc. to hold the same keybind in the same context
		-Pressing the key cycles through each item bound to it
			-Can enforce a modifier key combination for that key in order to soft-lock it (Like shift in PS)
16. Enhanced Tool Symmetry
	-Add symmetry support for as many tools as possible
	-Each tool has very similar inputs, so this should not be a difficult task.
	-Would allow symmetry with selection tools, cut-paste, mix/smudge, gradients, etc.
17. Live Tool Input Tiling
	-Custom tile sizes and live-preview/edit/painting of tiling textures
18. Stamp (Sampler) Tool Output Method
	-Similar to the stamp/clone tool in PS but allows for wider range of possibilities by enabling
	a lot of the other tool engine effects.
	-Apply tints, scattering, rotations, size, etc. to the output
19. Animation Panel
	-Allow users to define a preset onion-layer template for when adding new frames, such as
	a group of 'name' + 'key#', containing any collection of layers and groups, with empty masks
	effect layers, or clipping layers generated as well. Could add pre-defined image templates as well
	so that a custom frame or perspective grid is inserted into any of the images
	-Automatically adjust the previous onion-layers opacity when adding a frame
	-Create a timeline widget where user can drag each layer template that corresponds to canvas layers
	on the timeline, and when adding frames to the timeline, automatically insert those in the right index
		-On the timeline, users can set different markers for frames.
			-Could have a 'keyframe' or 'moment' marker that causes every frame between it and the
			next marker of that type to be automatically evenly spaced.
				-When dragging one of these on the timeline, it should automatically re-position every inbetween
	-Easy set up for paging through frames, can bind various keys to different pagination methods
		-Individaul frame, every-n frames, by marker, etc.
	-Try to support as many layers as possible and also provide cleaner or more readable organization of
	frames. Could implement search tools, a scrubber (that also moves the layer-slider), or grouping
	every number of frames into a larger folder, or grouping by keyframe and every frame until the next, etc.
20. Parallax/Camera Shakes
	-Some way of creating a parallax effect for layers and adding camera rumbles/shakes
	-Could potentially accomplish by creating a perspective camera and then pushing the parallax layer into
	space and removing the canvas shader, or making the background the canvas shader
21. Branching Undo States and/or branching Image States
	-A way of storing internally undo states from different lineages
	-And/Or a method of in-app keeping version control branches on an image, and storing that
	into the save file for the file.
22. Multiple or Customizable UI Visibility Toggle States
	-Like in Photoshop using ',' or TAB, but can pick/choose what gets hidden, save multiple
	of these sets, and also hot-swap widgets to compact versions
	-Could also create a subset of states that shrink or remove excess UI/widget styles, like getting rid
	of the tab-bar on panels, or shrinking a color hue panel.
		-Could condense or remove excess info on layer panel, etc.

################################################################
##
## TO-DO LIST (Numbered living list of things to do, marked by intended version #)
## Should work from bottom up unless it's a quick add that makes sense to do right now
##
################################################################
////////////////
// 0.1.0 - The Custom File Format Update
////////////////
0. Create a custom file format for saving documents in the program, should preserve
all of the data, compacted/composite data, etc. as well as a portion of the history state
1. Research file formats and writing to disk. Look into multi-threading so that a save/load
can be done in the background.
2. Design a name, logo, and file extension for the chroma.io filetype

////////////////
// 0.0.12 - Documentation Update && Bug Squishing (May cut this short if it gets boring)
////////////////
0-1. Check every "Note:" "Warning:" and "Suggestion:" comment
0. Go over the entire code-base and clean up code/comments/formatting
1. Thoroughly document functions in the code-base, and unify comment style
2. Create some visual diagrams of important functions & visual diagrams for
overall program structure and design (such as the I/O method, class inheritance, plugins, etc.)
3. Look into a more organized way of documenting the program, could just be more text
files, but also look into web-based documentation
4. Clean up any warning/note/suggestion items in the code base
6-1. Make sure that no smart_ptr controlled object class has defined deleters, and the composite
components are trivially deletable.
6. For every source page, make sure that smart pointers are not made inside of parameter lists
in function calls. Per MS documentation "Always create smart pointers on a separate line of code, 
never in a parameter list, so that a subtle resource leak won't occur due to certain parameter 
list allocation rules."
7-1. Check that in every scenario where shared_ptr's objects are doubly-linked, that is,
the shared pointer has member variable of shared_ptr, which in turn has member variable of shared_ptr
to the owning object, that the link to the parent is actually weak_ptr
7. Comb over the code for cases where unique_ptr would be best, especially when an object
really should only have one copy floating around.
8-1. An additional note on this is that much of the program should probably be reworked to rely
on mostly unique_ptr and non-ptr objects. For example, the (internal)Widget "style" property 
really should just be a plain WidgetStyle object, not a shared_ptr because every widget's style should
be unique. Apply the same logic for the rest of the program. While pointers are convenient,
it is suspected now that there may be a lot of unreleased data floating around from shared_ptr overuse
8. Look through the script engine environment tables and see where redundant data and pointers can
be removed. One obvious example is that there does not need to be a matching scope for functions,
classes, and instances. They have their own environments and declaration statements saved and
the duplicate scope should be unnecessary
9. Look for areas of code where a weak_ptr is being used and it's .lock() is getting called frequently during
program execution. The .lock().get() method is apparently a lot more expensive than .get() alone in a shared_ptr
wealk_ptr is good for double-linked lists, and useful for infrequently trafficked areas, but not for higher
performance code.

////////////////
// 0.0.11 - The Interface & Script pt 2 Update
////////////////
0. Add grab/resize functionality to window
1. Write the .plugin parser in the same way as the others
2-1. Add panel undocking & split-tab to own-panel support
2. Add panel docking & multi-tab panel support via drag/drop interface
3. Add on-load and priority ordering for widgets/plugins, should fire -after- building the widget
tree but before buildAllWidgetTrees is called. This will allow plugins to bind callbacks with widget pointers
at load once and forget about it.
4. Upgrade 'bindCallbackEvent' to also accept a widgetObj to save the call target for faster
call execution.
5. Do clean up on the script engine & components (typeWiz, resolver). Lots of hack-in
solutions right now, especially in typeWiz/resolver, that were solved better in the interpreter.
6. Add drag/move functionality to widgets
7. Add grab/resize functionality to widgets
8-1. Look back over the native function/class library and update functions where it would make sense
to swap from a number array to a vector, and other QOL improvements.
8. Add Member access & swizzling to the Vector, Array and String objects.
	-glm style (.x, .y, .z, .w, .r, .g, .b, .a)
	-vector math
	-string char array access string[x]
	-string native functions, .insert, .erase, .find, .length, etc.
	-array native functions, .insert, .erase, .find, .size, etc.
9. Test multi-file scripts and scope resolution between plugin scopes
10. Create a script-namespace called "cb" with num variables corresponding to the callback
names. Sort of like a native enum list
11. Add dynamic population of widget containers during runtime using some kind of stored
prototype method when plugins are loaded. Need this for things like layers, tools, menus, etc.
12. Make the color history panel
13. Add the Color Square Panel
14. Add the Color Popup Panel (Not picker, but a hue-square with color hist.)
15. Make all widget vertex data use Position, Color/Alpha, Tex-Coord as their vertex data.

////////////////
// 0.0.10 - The History, Console & Macro/Action/Script Update
////////////////
0. Add a history state to the program.
1-1. Add persistent state to the program so that it restores it's previous state
after closing
1. Create event/action logging and unify the way in which events are logged
2-1. Develop a keybinding system thats similar to blender's
2. Setup an action/macro panel, and add in in-app-created user macros
3. Add a app function that will save/load/run 

////////////////
// 0.0.9 - Data Visualization Update
////////////////
0. Anchor manipulation, bezier curves & selecting/moving strokes
1. Add the select object tool
3-3. Selection tools and data manipulation (Transform)

////////////////
// 0.0.8 - Layers & Groups
////////////////
3-2. Make the Layers/Groups/Masks panel
3-1. Layers, blending modes, groups, masks, etc.
3. Eraser Tool (AKA Eraser blend Mode against composite data)
4. Implement composite data structure for the canvas
5. Look into how it would be possible to split the layer/canvas into tiles because
the openGL max-Texture area (8192 x 8192, for my graphics card at least), will limit
the canvas size otherwise. (Can implement this now or later depending on complexity)

////////////////
// 0.0.7 - Tool Engine V1 Update
////////////////
0. Add additional input/output tool methods that can be done without needing to
fix the layer/canvas tiling bottleneck. 
2. Flesh out the engine for applying various brush/tool settings
3. Add tool preset saving/loading, serialization of preset configs into external files
6. Add anchor culling during the output_draw method
7. Make the tool settings panel & the subtool panel (CSP-like toolbar)
8-2. Add Opacity property for style so that images use that instead of background-color-alpha
8-1. Images & Textures manager class
8. Support to change background-texture


################################################################
##
## Current Bug-Tracker (Delete from this list as issues are resolved)
## Work From the Bottom Up
##
################################################################
0.0.6b

0.0.6a
-Pan Camera no longer handles rotated canvas correctly. Small problem, simple fix, did not have time when swapping
over the input/output functions to adjust it appropriately.
-The onclick callback gets triggered accidentally for some reason on brushSettingsHUD when using the sliders
	-This is likely inside the widget class functions for handling the clicks.
	-Reproduce this by swapping the callback from ondragstart to onclick and then clicking sliders after toggling
	the checkbox once.
-The Stylus input cannot differentiate between clicks sent by a side button and clicks from the pen touching
the tablet surface. A slight rework of the WinStylusHandler is needed to add the callback for the stylusButtons

0.0.5n
-Performance of the color picker HUD widget is noticably slow when dragging rapidly over the screen
-The rendering of texture data for Layers is wrong. When the canvas is 1024 x 512, the pixels appear twice
as tall as they should be. Potentially for the same reason the brush pixels were wrong

0.0.5m
-Still need a lot of polish and tweaking on the UI display engine. Lots of weird rules interactions
and edge cases that should be resolved. Many areas where functionality is missing or an interaction
doesn't do what is predictable based on other rules.
0.0.5k
0.0.5j

0.0.i
0.0.5h
-Must make the resolver more robust. Need to actually work through the CExpr visit functions.
	-Big issue is that a variable refrencing itself during initialization goes unchecked
	-"num c = a + c;" should throw an error but it does not currently.
	-Should also throw warnings for every uninitialized variable.
	-Will need try-catch blocks around get/set on variables for release because otherwise
	nullptr or nullvariant access will happen to uncautious users.
-Should enforce l-value / r-value in the CExpr_Assign in typeWizard or optimizerRed. Should not allow
statements that have no result, such as 5 = 5;
-Lots of untested edge cases & outcomes of badly written scripts is untested

0.0.5g
-Note: Add in rules support for enforcing the maxChild count on widgets, or else behavior of widget
hierarchies can potentially get weird.
-Note: In HorizontalBox, UI_FLOATING_CENTER is not thoroughly tested or verified for accuracy. Be
sure to test this later and verify the math is correct.
-The right: and bottom: offset properties do not currently have any function, not needed for now,
and their purpose/use isn't obvious, but leave this on the to-do list.
-position:fixed has no function currently, may remove this property later because widgets shouldn't
get rooted to the window
-Prior to this update, margin and pxPosition were coupled together in calculation. The swap to
using left/right/top/bottom now means these are decoupled. When a left/right/top/bottom property
or margin/border property is behaving unexpectedly, investigate this. To clear this issue, do a thorough
search against getMargin() and getOffset() and where those pop up.
0.0.5f
0.0.5e
0.0.5d
0.0.5c
0.0.5b
-Currently implementing several different setVertData functions in children of the Visual Entity class,
consider tidying this up in the future.
0.0.5a
0.0.4
0.0.3
-Warning: RealTimeStylus bug - If the stylus is captured by certain 
windows (console/terminal) then the program locks up
0.0.2
0.0.1

################################################################
##
## Version & Update History
##
################################################################
////////////////
// 0.0.6b
////////////////
Notes:
-Began work on fixing the UI engine in preparation for the Tool Settings panel


Updates:
-Fixed camera panning so that it's now world-space based.
-Polished the stylusDownscale calibrator so it'll gradually settle in on a target factor
-Fixed the lag that was happening on UI input when using the stylus to drag widgets
	-This was being caused by the UI always fetching the glfw mouse position, which is now known to
	lag noticeably behind stylus input.
	-The script engine/UI now fetches the position from the mousePosBuffer when the stylus is in range
	-A boolean is added now to the arguments so that only the script/UI code will use mousePosBuffer
-Changed the way that position:relative and position:absolute function. They now more closely resemble
the way their CSS counterparts work. Absolute now removes a widget from flow entirely, and positions according to the
offset values only. Absolute now anchors to the next static-positioned widget, rather than relative. Relative now
behaves similarly to static, but allows the offset values to produce an effect. Static now will ignore offset values
entirely.
-Fixed some of the height/width and sizing functions to now correctly handle inheritance and min/max values.
-Fixed some of the calculations for height/width inheritance combined with margin/border effects. Something that
inherits it's dimensions should now have it's margin/border subtracted from the maximum possible dimensions.

Known Issues:
-While apparently correct for current widget suite, this update is introducing many updates to the UI engine's sizing
and location calculations and the changes may have broken things that were once working with past
test widgets.
	-This issue can be removed around the time 0.0.8 is finished as long as any newly found UI issues are resolved
-Something about the blur/focus callback system causes the main-menu dropdowns to still show themselves
after clicking the canvas or another widget. The main-menu should have lost focus so this should not happen.
-The 'style' property in the Layout system does not work properly. It causes errors and erasure of style data when used.
-'float:right' does not appear to work when the right-ward widget is an IMAGE and the floated widget is TEXT or H_BOX


////////////////
// 0.0.6a
////////////////
Notes:
-First update towards the Tool Engine update
-Added a bunch of new tool inputs plus the necessary UI tools to make painting possible

Updates:
-Reworked the input methods to now produce vertex data (VertexData) as their output
-Reworked the output methods to now intake vertex data (VertexData) instead of InputData (mouse events)
-Output methods are now positioned to handle multiple types of input methods more readily. The
'constant size' flag allows an input method to flag how the output method should evaluate it's vertex data
contents. This will influence how the output method manages telling it's visualizer to render.
-Plugged in the tool settings for Pan/Zoom/Rotate camera. They are not being used but now the output methods
can read their settings for the camera controls
-Added small QOL change to the UI so that when a widget gets hovered the cursor changes to a pointer. Later
can update this so the widget can pick what cursor it wants, but for now it is always a cursor.
-Cleaned up some of the pan/zoom/rotate functions, but be aware that the camera pan function needs to be updated
to unrotate the camera first before calculating the position input. It no longer handles rotated canvas correctly.
-Added the "In_Polygon" input method.
-Added the "In_ShapeField" input method, which produces a randomized distrubtion of points within a defined shape mask
-Added the "In_Vortex" input method, which draws radially distributed vertices along the input spline.
-See documentation notes in the .odt file for further notes about these inputs
-Adjusted the input controls such that they now reside in different "Control Schemes". Written about more extensively
in the documentation notes.
-Added the GRADIENT_BOX widget type, which is able to make use of the vertex-#-color property in .style
	-Utilizes it's own shader which will later be merged with the main widget shader
	-Sorts widgets internally as if it's a H_BOX
	-May later remove the GRADIENT_BOX and instead add an attribute to H_BOX & V_BOX to enable/disable
	the use of the vertex color data.
-Combined the test-hue box into the HSL sliders panel, and then added the graph slider inside that and spent
way too long patching up the math between the X/Y representation and slider representation, but they are mostly synced
now when moving one or the other. There exists a slight math error when moving the hue & saturation sliders, that
looks like a portion of the colorOffset value getting added/subtracted more than once, but less than twice.
-Added the getMousePos_relative() method to the native script library. This fetches the X/Y mouse position
relative to the activeWidget's screen location. This enables the use of generic click-events to begin drag inputs
of the mouse.
	-The HSL sliders panel now allows clicking anywhere along the slider-line or inside the gradient-box to
	begin the drag, and the sliders all snap to the click position when starting a new drag input.
	-This now gives the HSL sliders a really snappy and responsive control and will make color picking using the
	panel a lot faster.
-Fixed the stylus issue
-Fixed the opacity and flow again, which should now be stable until attempting to add blend modes or other features to the
stroke output

Known Issues:
-The Stylus input mode has a start-up lag that causes a skip in the input stream. It's not clear exactly
what's causing it, but mouse input whether it is the physical mouse, Wacom tablet in "Mouse" mode, or
Wacom tablet with "Use Windows Ink" disabled (All of which bypass the stylus pipeline entirely, and disable pressure),
does not have the issue. Using the mouse seems to not have the problem.
	-The recognizable signature of the problem is a blunted side when drawing ellipses rapidly, as if they're
	hammer smashed. The first anchor gets placed, and the input signal then cuts out for just a frame or two
	and then the anchors connect, with the obvious straight line between them.
	-Disabling UI rendering and other more intensive processes doesn't have an input on this stylus skipping
	-Hypothesis : It's possible that the competing input streams, where both GLFW and WinStylusHandler trigger
	callbacks is causing this disconnect. Not entirely sure, but it's clearly a problem on chroma.io's end, or
	an issue with the WinStylusAPI.
-Pan Camera no longer handles rotated canvas correctly. Small problem, simple fix, did not have time when swapping
over the input/output functions to adjust it appropriately.
-In_Polygon periodically produces two sets of anchors for an unknown reason. Cannot reliably reproduce the bug.
-The onclick callback gets triggered accidentally for some reason on brushSettingsHUD when using the sliders
	-This is likely inside the widget class functions for handling the clicks.
	-Reproduce this by swapping the callback from ondragstart to onclick and then clicking sliders after toggling
	the checkbox once.
-The Stylus input cannot differentiate between clicks sent by a side button and clicks from the pen touching
the tablet surface. A slight rework of the WinStylusHandler is needed to add the callback for the stylusButtons
	-Also, looking into this issue just briefly seems to suggest what is suspected about the stylus input
	and the GLFW input. It seems like the two callbacks are competing for first-place timing, and this might
	be the source of the input lag when first touching the stylus down

////////////////
// 0.0.5n
////////////////
Notes:
-So far so good, everything works and is stable. Yet to encounter any exceptions being thrown
-Not sure about performance or memory leak issues.
-Brush rendering is now a magnitude slower, but a working flow/opacity system is now enabled

Updates:
-Refactored a couple of minor class design decisions regarding use of raw pointers. Now almost
all of these are shared pointers. 
-Set up tool settings for later expansion. Using an interest mask system instead of giving every tool
every single property. Tools now will use the interest mask of their I/O methods to determine what
settings are available.
	-ToolSettings is parent to every interest type, allowing for a polymorphic map storage
	-Map is of type <TSetType, std::shared_ptr<ToolSettings>>
-Refactored much of the IOMethod class organization so that now Application does not own them,
and each Tool gets it's own unique_ptr of their IOMethods. This will allow for better input/output
control later on because the Tool can now store IO data and remember the last input, etc.
-Added the Color Picker tool & Out_Sampler method
	-Only utilizes glReadPixels() for now, against single point sampling
-Added support for moving widget locations & app.ui.getWidget_byID()
-Added callback binding support for scripts so that widgets can be notified by a script when
the callback triggers.
-Added the color picker HUD widget and set up it's scripts
-Laid the groundwork for the ToolSettings update. Now using a polymorphic map of settings and
each IO method potentially has it's own interest mask that tells the tool which settings to include
in it's settings map.
-Finished laying the groundwork for the Tool Engine update by enabling strokes to utilize both
'flow' and 'opacity' for rendering/drawing. It appears thus far to look just like it does in PS
-Updated the rendering engine for Strokes to now make use of composite data. Rendering each brush tip
now only takes place once, and the renders are collected into a single texture buffer.
-Added the "HSB SLIDERS" panel widget, as well as a temporary right-click panel for brush size, opacity, flow
	-Spun up the required functions and interaction for the sliders -
		color conversion, widget dragstart/drag/dragend
	-Color conversion looks good.
-Set up the sliders panel to communicate with the program and receive updates from the colorpicker callback.
-Did a few test paintings, at long last!


Known Issues:
-Performance of the color picker HUD widget is noticably slow when dragging rapidly over the screen
and continues to worsen as number of brush strokes increases
	-A lot of this can be improved via optimization though
-The color picker HUD has a weird start-up lag. It appears immediately but doesn't move right away.
	-Probably need to adjust the culling of inputs somehow.
-The rendering of texture data for Layers is wrong. When the canvas is 1024 x 512, the pixels appear twice
as tall as they should be. Potentially for the same reason the brush pixels were wrong

////////////////
// 0.0.5m
////////////////
Notes:
-Adding widget mouse interaction framework
-Beginnings of developing the DOM app.ui / app.ui.getWidget().widgetMethod()
framework for the script engine.

Updates:
-Added mouse click, right click, middle click, and respective double clicks
-Added mouse onmouseenter, onmouseover, onmouseleave
-Added activeWidget tracking to the UI class, this allows more brevity in callback functions
by setting an activeWidget before entering the callback. Any activeWidget function that gets
called will be able to dereference that pointer and call methods on it.
-Successfully tested setProperty using 'background-color' in conjunction with
onmouseenter, onmouseleave
-Added setChildProperty_byID(string id, string name, any value)
-Enhanced sweep detection so that widgets with elements outside their bounds can receive
hover and click detection. The check is done during buildAllWidgetTrees in UI
-Added the support for callback bindings. Created a bindTimerEvent function
that allows the script to bind a function to a count-down timer. Also added cancelTimerEvent to
cancel a timer by name.
	-bindTimerEvent(string timerID, num duration, num repeat, function callback)
	-Calling bindTimerEvent on an already existing ID resets the duration
	remaining and overwrites the repeat property. A repeat of -1 is infinite
	-cancelTimerEvent(string timerID)
	-This will let scripts manage timed events for UI things like hover duration, tooltips, etc.
	-All of the timer erasure occurs in the same update function, so calling a cancel from
	within the callback itself should never cause errors.
-Added the first hitTestWidget function hitTestWidget_byID, which does a simple pointCollision
check against the selected widget and returns true/false. This is useful for stopping a onmouseleave
event from triggering if the mouse enters into a specific widget
-Added onfocus() and onblur() triggers, as well as the onrelease() trigger
-Polished up a few bugs regarding UI display
-Finished up the mainMenu plugin visuals. Now looks really nice and has very good mouse control

Known Issues:
-Stylus interaction with widgets is still buggy. Will clean up once the bulk of
the UI mouse events, drag events, drop, etc. is mostly finished.
-Still need a lot of polish and tweaking on the UI display engine. Lots of weird rules interactions
and edge cases that should be resolved. Many areas where functionality is missing or an interaction
doesn't do what is predictable based on other rules.

////////////////
// 0.0.5k
////////////////
Notes:
-Accidentally skipped over this, notes for 0.0.5k are in 'j' of this version

Updates:

Known Issues:

////////////////
// 0.0.5j
////////////////
Notes:
-Big Success!

Updates:
-Began rework of the .style parser, creating a new one that uses the same technique as scripts
-Working in support for scripts inside of .style files.
	-Can include as many script inputs in 'script' headers
		script $
			num x = 4;
		$
	-Can use a script as a property value
		foo {
			width: $x$;
		}
-Adding in '#' '.' '*' selectors
-Adding support for multi-style that would copy the styles of several others and then enter
the new style body
	bar : foo {
		height: 100;
	}
-Path values now must be written as string values "path/file.ext"
-Will not be adding support for non-px measurements. All numerical units are pixels
-Added native 'Style' class and supporting native functions 'getStyleProperty_byName' and 'set'""
	-Intakes the same names,values as the .style syntax
	-'Style' wraps over the WidgetStyle internal class, which now hosts all of the string to value
	parsing functions for the interpreter. The same getSetProperty() function that gets called by
	the script interface is the same which will be used for setting properties via the interpreter
-Added the resolver for .style
	-Very simple, just looks at the style identifier/names and checks that they exist and get
	defined. Currently doesn't permit for empty-property definitions, throws unresolved symbol
	error to the console.
-Added the interpreter for .style
	-Runs through the properties and script values and uses the WidgetStyle getSetProperty() function
	to update property values. Reusing the getSetProperty in this way localizes all property
	setting in one function in the code.
	-Debugged more issues with regards to loading plugins from plugin folder. Now cleanly reaches
	the .layout parser, which will be added next.
-Added the lexer/parser for .layout
	-Treats every attribute as a string. Creates a nested structure of widget/element statements with
	attribute expressions
-Added the resolver/interpreter for .layout
	-Resolver loads font files
	-The interpreter builds the widget tree
-Made several updates to the Widget classes
	-Migrating the childWidgets/style system over to CObject containers for script accessibility
	-Seems to be stable, but now must comb over the Widget classes and update them to use the new
	objects, and try to maintain readability (accessing these new objects requires several std::get chains)
	-Successfully reaching the end of plugin loading now.
-Reverted changes to widget class regarding use of CObject
	-Emulating the pseudo-calls to the script engine creates a whole mess of pointers that
	are too difficult to ensure get properly decremented on reference count.
	-Rather, will revert to old method and when a user calls app.ui.getWidgetById("foo"), this
	will return a temporary Widget Instance that only contains the "@widgetObj" values and the
	member functions. The "style" of Style class member variable will be removed as well for direct
	access, creating a "hidden" field. It's accessibly by using getStyle(), which would also return a temporary
	Style Instance. While having direct .style access is nice, it complicates the Widget class by an
	additional level of complexity, and the same pointer issues come up.
	-The Style class objects created by the Style Interpreter will be left inside the
	environment variables for now, although it may even make sense to move those into a container
	inside the UI class, and store them as unique_ptr
-Renamed the 'CStyle' class to 'WidgetStyle'
	
Known-Issues:



////////////////
// 0.0.5i
////////////////
Notes:


Updates:
-Removed the "constructor" keyword, constructors are now declared as in C++ using the class name
followed by parameters
-Removed the 'function' keyword as used for declaration statements. 'function' is now a
variable type. Will allow users to store a pointer/reference/copy? of a function in order
to pass as arguments to more complex functions (not yet implemented).
-Added native class support
	-Instantiate native classes the same way as functions by manually defining the object
	and then injecting the environment and object into the 'global' namespace
-Added native class "App"
	-Wrapper class for Application.h
	-Added the member function exit(), which calls Application::closeApplication()
	-It just works

Known-Issues:



////////////////
// 0.0.5h
////////////////
Notes:


Updates:
-Moved the .style and .layout lexer/parser function definitions to their own files:
	-UserInterface_style & UserInterface_layout
-Beginning of chromaScript development, a domain-specific scripting language that will feature
compilation and interpretation, based on an object-oriented design that uses a similar syntax
to javaScript, while maintaining the type-safety of C++. chromaScript is a wrapper (transpiled) language around
C++ and exists only as an embedded language inside the chroma.io domain.
	-chromaScript will fully unlock the potential of user-defined interfaces and scripts, as well as
	provide a strong utility for defining program features, enabling user-macros, and history states
	-The primary challenge of developing the language will be the translation of one type-safe language
	to another. A custom API will be needed alongside the language that allows for the application to bind
	predetermined functions to the script engine, which will call for a very interesting solution to the
	type-safe enforcement of function signatures when handling callback binding in C++. One potential
	solution is to use the Object callback-func(Object context, Args args) signature developed 
	by the Lizzie example project.
-Added the chromaScript Lexer successfully
	-Currently developing chromaScript without type safety, but will add later once it's up and running in full
-Added the chromaScript Parser and Interpreter (binary/unary expression with numbers only)
-By allowing nested functions in ChromaScript, closure/enclosure of variables/functions behaves
in weird ways. (Note: Not necessarily an issue, this is actually logically consistent, and the author
of Crafting Interpreters might be wrong because he compares apples to oranges, ie. comparing C or C++
which disallows variable overwriting/shadowing, and Javascript which does (although only within functions,
in JS, a bracket enclosure alone does not declare a new scope))

	var a = "global";
	{
		function showA() {
		print a;
		}

		showA();
		var a = "block";
		showA();
	}
	print a;
	
	-In some languages it may be expected that showA() prints "global" three times because at declaration
	the showA() function captures the definition of a. (This snippet prints "global", "block", "global")
	-However, there appears to be an impassable logic barrier in the code that would allow
	both for nested functions, defining conflicting variables inside a nested scope, 
	and the strict closure in Crafting Interpreters
	-It's worth noting that this is only an issue because the language allows for multiple declarations
	of the same variable with shadowing.
	-Also, at the time of writing, changing the second "var a..." to "a = "block"" causes a read error
	indicating that the enclosure, although buggy (it should've found "global" to print), is logically
	consistent with what you'd expect with shadowing.
	-In fact, it's not unreasonable to suggest that this behavior is preferable because it prevents
	accidental overwriting of global variables by scoped function variables.
		-As proof, if "var a = "yellow"" is added before "print a" in showA(), showA() always prints
		"yellow" irrespective of the "block" and "global" definitions.
		
	var a = "global";
	function test() {
		function showA() {
			console.log(a);
		}
		
		showA();
		var a = "block";
		showA();
	}
	test();
	console.log(a);
	
	-The corresponding JS code behaves almost as ChromaScript does, printing "undefined", "block", "global"
	-The difference between JS and CS is that CS permits unlimited depth testing to find variables up
	an enclosure tree, while JS only permits 1 level of depth, hence why "undefined" occurs on first call.
-Expanded the script language features much further beyond what is covered in Crafting Interpreters
-Rewrite the resolver to now properly instantiate new environments, register variable/function/class
objects inside of their respective scopes, while accounting for forward declarations and definitions
of variable/function/class from other files/scopes. Allows for shadowing of global variables
	-Note that the 'global' scope files (in the default config folder) are sandboxed from the plugins folder
	when they are loaded. If a function/class is forward declared in default config scripts, it must also
	be defined inside the default config scripts. Additionally, this means that plugin scripts cannot
	determine the definition of global functions/classes. The syntax will be valid, but the attempted definition
	will throw an error.
-Wired up the resolver, and altered how plugins are loaded. Scripts are now batched together so that forward
declarations can occur in one file, and the definition can occur in a different file. A map is used to collect
all of the statements from each script file for each plugin namespace. These are then iterated over.
-Changed the order of operations for script compilation. 'Build' now covers lexing, parsing, and resolving.
	-The next step 'Compile' covers type checking and optimization.
	-This seperation of duties allows forward declarations to work without needing strict ordering of files
	into the console. As long as the definition is contained somewhere in the plugins folder, the symbol
	will be defined properly before attempting to type-check and run it.
-Began reintegration of the CObject and CCallable classes into the CScript Program
	-CObject now uses std::variant with success in at least non-array objects (Need to verify arrays!)
	-CCallable is now setup for CClass and CFunction.
	-Thus far std::variant seems to successfully achieve what's needed. As long as the arrays of functions,
	instances, variables, works, then it will be the correct way to go.
-Added the typeWizard to chromaScript
	-TypeChecks the list of statements from the parser
	-Throws back errors when types do not match
	-Strictly works with already declared values, Resolver must have added everything correctly.
-Added the interpreter
-Removed the 'auto' and 'var' keywords, and the Auto variable type. Too much trouble will come
from having auto/var in this language. No good reason to use when most use cases are laziness
and not any particularly inventive programming solution.
-Made many updates to the interpreter.
	-Now 95% where it needs to be in order to move forward
	-Made sure that objects properly copy their values when using get/set
	-No longer treat objects as references, everything gets copied
	-Updated error handling (across the board) to print the step name + code number
		-ex. [interpreter:1402]
	-Smoothed a lot of rough edges out
-Hammered out a lot of bugs that weren't caught in the parser/resolver/typeWizard

Known-Issues:
-Must make the resolver more robust. Need to actually work through the CExpr visit functions.
	-Big issue is that a variable refrencing itself during initialization goes unchecked
	-"num c = a + c;" should throw an error but it does not currently.
	-Should also throw warnings for every uninitialized variable.
	-Will need try-catch blocks around get/set on variables for release because otherwise
	nullptr or nullvariant access will happen to uncautious users.
-Should enforce l-value / r-value in the CExpr_Assign in typeWizard or optimizerRed. Should not allow
statements that have no result, such as 5 = 5;
-Lots of untested edge cases & outcomes of badly written scripts is untested

////////////////
// 0.0.5g
////////////////
Notes:
-Moderately sized update, added a lot of things, revised a lot of things

Updates:
-Added centering behavior for float:center in HorizontalBox
-Added findSizeTextLine() to Fonts.cpp to get the size of a text widget so it
can be correctly centered
-Added a public "localStyle" property to widgets to store a copy of the style on initialization. This will
be used instead of the regular style property, so that resizing, visibility, etc. (dynamic properties)
can be edited safely at runtime when manipulating widgets.
-Changed the way the UI widgets are initialized
	-Move the setBounds, setVertData, generateBuffers, bindBuffers, bindTextures, reportSize, reportLocation,
	out of the constructors and into a new function - sizeWidgetByChildren()
	sizeWidgetByParent & updateWidgetLocation() (Which builds openGL / vertex data and then places the widget)
	-First, iterate through the tree, bottom-up to size widgets by their children (overflow=auto, etc.)
	-Next, iterate through the tree, top-down, to size widgets by parent (inheritance, relative values)
	and then set their location
	-This results in a buildWidgetHierarchy(<Widget> rootDir) function that only needs to be called once
	on each root widget in order to make everything look correct
	-This will allow some optimization in the draw-calls so performing the inheritance and placement math
	every frame can be avoided, it'll also lay the groundwork for dirty/clean optimization and composite
	data & blitting optimization
-Reorganized a few of the .style properties
	-minX, maxX, minY, maxY are now min-width, max-width, min-height, max-height
	-size is now dimensions
	-the delimiter for style properties is now : instead of = (CSS-like)
	-bgColor and bgTexture are now background-color and background-texture
	-added a "-" between upper-left, etc. in anchoring property to be more readable
-Added a NULL value for numeric/float properties that is set to INT_MAX
	-Use MAX instead of MIN to allow for stretch/fill styles
	-Lets the UI engine properly decode an unset dimension value
	-Lets 0 be 0 again
-Fixed anchoring property
	-Now actually works correctly
	-Changed the property names in program to LOWER instead of BOTTOM
	-Added a new parameter to renderTextLine - "offset" which is the x1, y1 of the transform.boundsBox
	for a text widget, which allows the anchoring property to apply to text now as well
-Added the Vertical Box widget container
	-Very similar to the HorizontalBox but swaps the childLocation logic to work with top/bottom on Y size
-Updated maxX/Y inheritance logic slightly. It now subtracts the margin from the size so that the
total size does not override that property.
-Fixed a counting issue in the recursive iterators stepThroughWidgetTree & createPluginFromFile
which was resulting in sibling widgets at the bottom of a branch not getting created
	-lastIndexState.back() > indexSizeState.back() is now thisIndex > indexSizeState.back()
-Added a left, right, top, bottom property for positioning and removed pxPosition.
	-Replaced pxPosition with position: STATIC/RELATIVE/ABSOLUTE/FIXED
	-Fixed has no function currently, and might be removed later
	-The absolute positioning property functions similarly to CSS, and will disrupt normal
	location/position flow until the next parent widget with relative positioning is found.
		-This is useful for parenting widgets like dropdowns or similar purpose widgets
		inside of a widget that needs to position other children based off static location.
	-In practice: the drop-down menu from the mainMenu bar, is parented to the associated
	button container, but doesn't affect the overflow:auto property of the button-wrapper,
	and the drop-down now exists outside that flow-scope
-Prototyped a drop-down menu, used a horizontal box to create the lines between categories
	-Still need to set up the rightCarrot & sub-drop-down menus
-New computer, project ported into VS2019 successfully. Squashed resulting warnings/errors from the
new compiler
-Added a rudimentary drop-shadow to the mainMenu prototype.
	-Updated the findLocation logic for vertical/horizontal box to treat any child widget with
	absolute positioning as if it's the first child in the branch hierarchy, effectively treating
	the origin within the container for the child widget as 0,0, placing it out of normal flow
	-Not sure if this is the best way to handle drop shadow & the positioning property,
	but it seems consistent with absolute positioning logic.
	-This is also technically the fastest means of rendering drop shadows, but won't handle rounded
	corners as well, if rounded corners are ever used.
-Redesigned the drop-shadow feature
	-Now exists as the style properties box-shadow-color, box-shadow-size
	-Rather than create a widget that is a shadow, the drawSelf function now draws the widget
	with the shadow-color & size (offset) first if applicable, then draws the widget on-top
	-The new box-shadow method is extensible for any widget type, including text.
-Added in the overflow:auto logic for when a widget has more than 1 child to size for
	-If all the children are floated left, then the sum of all sizes + margin/border is used
	-If any child is floated right, then the overflow size is the first parent widget with a
	non-0 size value, minus the containing widget's margin left/right.
-Added the sub-drop-menu prototype to the mainMenu widget.

Known Issues:
-Note: Add in rules support for enforcing the maxChild count on widgets, or else behavior of widget
hierarchies can potentially get weird.
-Note: In HorizontalBox, UI_FLOATING_CENTER is not thoroughly tested or verified for accuracy. Be
sure to test this later and verify the math is correct.
-The right: and bottom: offset properties do not currently have any function, not needed for now,
and their purpose/use isn't obvious, but leave this on the to-do list.
-position:fixed has no function currently, may remove this property later because widgets shouldn't
get rooted to the window
-Prior to this update, margin and pxPosition were coupled together in calculation. The swap to
using left/right/top/bottom now means these are decoupled. When a left/right/top/bottom property
or margin/border property is behaving unexpectedly, investigate this. To clear this issue, do a thorough
search against getMargin() and getOffset() and where those pop up.

////////////////
// 0.0.5f
////////////////
Notes:
-Stable release, continuing to work on plugin & UI systems
-Moderately exciting minor update, added text rendering
Updates:
-Added the Fonts class, which is responsible for creating and maintaining font faces and bitmap maps
for fonts in the user interfaces
	-Uses FreeType2
	-Is owned by the UserInterface class, and can be accessed by any class with a pointer to the UI
	-Currently, characters to render are statically loaded, and dynamically rendered, which is to
	say that the a character bitmap is stored statically for each font at each pxSize, and each
	character is rendered every frame with per-frame vertexData. This is obviously
	highly unoptimized, but for now as long as it works this is ok.
	-Needs more testing for different strings and characters, but works for now.
-Added the Text widget class, which is a basic widget that only holds a text-string to render
	-Plays nicely with location inheritance so far, need to fine-tune and adjust behavior so that
	text lines appear in a logical location based on type characteristics
	-Not responsible for drawing itself entirely. It relies on the Fonts class to handle rendering the
	string and setting per-frame vertexData, but Text widget does store it's own VAO/VBO/EBO
-Updated the UI .style format to have font-path and font-size properties.
-Everything seems to be working well enough, all that remains for now is to continue adding
widgets, debugging existing code as things pop up, and expanding the CSS style inheritance/behavior
to be as dynamic and reliable as possible.
-Additional note: So far everything is rendering pixel-perfect in the UI, it has yet to be seen
where an input value in the .style sheet is rendered incorrectly (off by a small but noticeable pixel amount)

Known Issues:
-In conjoinNewStyle Because there is no "unused" value for several WStyle properties, if a style should erase
overwrite a value with 0 or a default boolean value, then it will not do so. This is notably only a problem
for the margin, border, position, and size properties.

////////////////
// 0.0.5e
////////////////
Notes:
-Stable release, continuing to work on the plugin & UI systems
-Minor update, capturing working version
-Next steps: Fleshing out more widget components, creating more container types

Updates:
-Added the WProtoWidget ripper to the pluginLoader
	-Same methodology as the zipper that works on the file
	-Unpacks the WProtoWidget hierarchy into a Widget tree with correctly parented/childed
	nesting structure based on the plugin file.
-Added the default_position layout header attribute to the root widget
-Updated HorizontalBox to now report child locations
-Added the necessary location getting/setting/reporting functions to get a widget's location
in screen-space and inside it's parent container.
-Implemented basic float-left arrangement inside HorizontalBox

Known Issues:
-Must investigate rendering of UI images because now depending on the resolution of the texture
vs the image size on screen vs the monitor resolution, the texture is not true to image in a noticeable
way. Likely culprit is BILINEAR filtering on the UI textures &/or using images that are too high
in resolution relative to the widget they're applied to.

////////////////
// 0.0.5d
////////////////
Notes:
-Stable release, implementing new class features
-Minor update, capturing working version
-Created .layout parser

Updates:
-Success!
-Implemented .layout file parsing using recursive iteration
	-The function works and is well documented, developed without need to use an external tool or
	function library
	-Captures an initial stringState for the ROOT widget interior data
	-Uses increment/decrement counting of element tags to locate the correct terminator tag
	-Saves the start/end location of the full element (including <, >, </, >) and a copy of the
	directory into the stringState arrays
	-Steps into each directory that's not empty until it bottoms out, then traces back up to the
	earliest non-empty directory, deleting from the stringState as it goes.

Known Issues:

////////////////
// 0.0.5c
////////////////
Notes:
-Stable release, but not thoroughly tested
-Micro update, capturing working version
-Next step is to begin writing the parser for .layout files, so the build process
is fully automated.

Updates:
-Added the Image UI Element class, which is just a basic element that takes no children and
displays a texture or background-color if the texPath is empty
-Shuffled some of the Element constructor into the main Widget constructor
-Patched up some of the logic in makeStylesFromTokens with regards to min/max values
-Fixed an issue where multiple widgets were being made for some reason in a switch statement,
was remedied by adding a break; after each case
-Implemented the first rudimentary size reporting function in Widget, which allows basic
size reporting based off inheritance. Have yet to test it fully, but it functions as
expected when the widget has no parent & must reference the window for relative values

Known Issues:

////////////////
// 0.0.5b
////////////////
Notes:
-Stable release, although new features are at best "sketches"
-Second segment of the UI update, enables the render-loop for widgets
-Smaller update than 0.0.5a

Updates:
-Added more property values to the WStyle object
	-Visibility, relativeValue, and pxPosition
	-relativeValue corresponds to the minX/maxX/minY/maxY structure, and is used as a
	switch condition to alter how widgets report their size. The min/max values,
	now accept % and 0.0-1.0 values.
	-Visibility is a toggle value for determining if a widget should render. This
	allows hiding children of widgets without hiding the full widget hierarchy
	-pxPosition gives the offset position of the widget
	-In conjunction with pxPosition, transform.pos supply the position data for the widget
	while allowing the offset pxPosition to be kept.
	-To render/resize/initialize widget hierarchies, rebuildWidgets will be used
	to rebuild the reported dimensions of widgets and their vertexData
-Enabled the rendering loop for widgets
-Developed the math to convert a widget's size/position information into screen-space coords
	-Widgets should properly occupy in visible space what their virtual data reports,
	allowing for hit-tests to the User Interface to correctly report back results.

Known Issues:
-Currently implementing several different setVertData functions in children of the Visual Entity class,
consider tidying this up in the future.
-Lots of unfinished and temporarily placed functions.
	-To-do - The 0.0.6 update should focus on clarifying and organizing the code base,
	as well as thoroughly commenting the code, and patching up the current brush-issues.

////////////////
// 0.0.5a
////////////////
Notes:
-Stable release, nothing has broken thus far.
-First segment of the UI update. Adds .plugin and .style parser/builder functions.
-The .plugin and .style vectors are data dumped to the log file.

Updates:
-Beginning of the UI update
	-Created scaffolding for building the UI from config files (.plugin, .style, .layout)
	-Created scaffolding for adding user defined panel interfaces for plug-ins, including the ability
	to register new title-menu drop-down buttons and add to existing title-menu drop-down lists
-Expanded the UserInterface class substantially.
	-Created the initializeInterface() function, which will load through the config & plugins
	folder to build the UserInterface
	-Added parser for the .plugin file
	-Added parser for the .style file
		-Note that this parser is not very robust and does not tolerate anything similar to CSS
		currently. Later the parser can be expanded to do math, use variables, and pull from
		enum classes for data presets (Color, etc.)

Known Issues:
-Be careful with the style parser and .style files. The current standard is very strict.
-Brush math issues from 0.0.4 still present

////////////////
// 0.0.4
////////////////
Notes:
-Seemingly stable release
-Have not yet encountered any access violations or exceptions
-Performance seems to be enhanced now that the stylus is no longer using SyncPlugin,


Updates:
-Fixed critical issue in 0.0.3. :
	-Discovered that the buffers for the Stroke object were mysteriously failing to bind when
	using the stylus to draw, causing pointer mismanagement and eventually crashing after
	a read access violation stemming from openGL being fed garbage buffer ID's.
	-Investigating the GLFWwindow* value for glfwGetCurrentContext() revealed that the pointer
	was null when the stylus callbacks were triggered, indicating that openGL lost the current
	context and could no longer call functions.
	-This was caused by incorrectly using the IStylusSyncPlugin version of RealTimeStylus,
	rather than the ASynPlugin. Sync makes use of multi-threading to intercept and modify the
	tablet packet stream in real time, whereas ASync is single-threaded and only can perform
	read operations on packets.
	-Sync plugin causes openGL to lose the context and there's no way to manage the pointers in
	the winAPI COM thread and restore context when exiting.
	-There is no good reason to use Sync plugin unless packet editing is needed (Its not for
	most applications).
	-Swapping the inherited class of WinStylusHandler from IStylusSyncPlugin to IStylusAsyncPlugin
	cured virtually all of the previously known issues.
-Fixed issue in 0.0.3 :
	-The In_Draw would have an offset to it's picking location in world space
	-Investigation revealed that resizing the window, resulting in a call to framebuffer_size_callback
	would fix the problem. The true window-size of openGL is not it's created size, but the size of
	the window minus the title-bar (height). This resulted in In_Draw receiving bad information about
	the ratio of the window.
	-Solution found by creating an overload function of setWindowProperties (Application.cpp), that
	takes no arguments and sets winWidth/Height by glfwGetFramebufferSize, and then calling the
	framebuffer_size_callback manually using the new winWidth/Height during initialization.

Known Issues:
-While the critical threading issue was fixed, the pause/move functionality still needs to be investigated.
-The spacing of Shards in Out_Stroke->processNewAnchor (Stroke.cpp), is incorrect. It appears the length values
being used to find the next shard position are short of their intended placement. The direction *seems* correct,
but placement and calculation of position are off.
-Related issue to above - the brush "skips" when rapidly entering a stroke, could be resolved by solving the
spacing calculation, but may be something else related to input-curing from In_Draw
-Brush Opacity calculation feels aggressive. The raw-input range looks and feels correct (double check this), 
but using linear interpretation to modulate the out bound shard opacity tends towards binary-looking results.
Potentially is fixed by using non-linear equation to solve opacity from pressure.
-Brush direction is not accounting for camera rotation, should be an easy fix in the draw-call when iterating
over the shards

////////////////
// 0.0.3
////////////////
Notes:
-Mostly stable release (Did not extensively test)

Updates:

Known Issues:
-Tablet input is causing invalid read access to pointers. Likely caused by issues with the
input pause/move culling functionality in In_Draw and Out_Stroke. Due to differences in how the tablet
and mouse poll inputs and do callbacks, it never encounters pauses like the stylus/tablet does. 
-Related to pointer issue is the brush size being incorrect
-When the window is not 1280 x 720, the screen-world picking function appears to be wrong. Not sure why.
- Out_Stroke::finalize - When the mouse clicks and releases instantly, there is no activeStroke and the ptr
throws an exception, may be related to the broader ptr issues in Out_Stroke

Critical Error Description:
-For whatever reason, when a new stroke is created when using the stylus, the VAO, VBO, EBO & TEX0
all fail to generate correct buffers. Value ranges for the buffers are usually 340,000 or so, or -800,000
-However, this issue fails to materialize when using the mouse for input
-The issue does not seem to stem from deleting anything or a bad pointer, it happens on the very first creation
of a new stroke with the stylus, and before any draw-calls are made.
-Speculatively, this could also stem from the pause/move functions, but it doesn't seem to explain
why openGL is failing to bind ID's for the buffers when the "createNewStroke" is exactly the same
for the mouse and stylus.

////////////////
// 0.0.2
////////////////
Notes:
-Unstable release.

Updates:

Known Issues:
-Tablet input is causing invalid read access to pointers. Likely caused by issues with the
input pause/move culling functionality in In_Draw and Out_Stroke. Due to differences in how the tablet
and mouse poll inputs and do callbacks, it never encounters pauses like the stylus/tablet does. 
-Related to pointer issue is the brush size being incorrect
-When the window is not 1280 x 720, the screen-world picking function appears to be wrong. Not sure why.
- Out_Stroke::finalize - When the mouse clicks and releases instantly, there is no activeStroke and the ptr
throws an exception, may be related to the broader ptr issues in Out_Stroke

////////////////
// 0.0.1
////////////////
Notes:
-Unstable release. 

Updates:

Known Issues:
-Tablet input is causing invalid read access to pointers. Likely caused by issues with the
input pause/move culling functionality in In_Draw and Out_Stroke. Due to differences in how the tablet
and mouse poll inputs and do callbacks, it never encounters pauses like the stylus/tablet does. 
-Related to pointer issue is the brush size being incorrect
-When the window is not 1280 x 720, the screen-world picking function appears to be wrong. Not sure why.