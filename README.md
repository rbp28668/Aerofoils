# Aerofoils

Programs for plotting aerofoils and driving a CNC cutter for foam wings. Now at version 6.1, this project builds 2 programs _Aerofoil_ and _Cutter_ and as a pair and with appropriate hardware these can:
* Cut wing cores including special treatment of leading and trailing edges, spars and cutouts.
* Allow the cutting of tapered or delta wings.
* Incorporate washout in a wing panel.
* Import components from DXF files.
* Plot ellipse sections for constructing fuselages etc.
* Print wing and ellipse sections on paper for wing ribs etc.
* Animated preview of cut paths.
* Export G-Code files for the _Cutter_ program or for use by Mach3.
* Process G-Code originally destined for Mach3.
* Allow import of G-code sections created externally.
* Run canned G-code snippets with single button presses.

#Project History
This probject started out in 1992 with a prototype wing cutter and ran with a character based user interface on a PC-AT. That had a massive 5MB hard drive.  Luxury! The cutter hardware was eventually sold to a local model shop and the software was converted to have a Windows user interface with the emphasis changed to plotting rib sections / formers onto paper. 
More recently - 2018 onwards the code has been extensively modified and the ability to drive a CNC wing cutter has been split out into a separate _Cutter_ program.  _Aerofoil_ now produces G-Code and _Cutter_ takes in G-Code and drives an attached CNC cutter.  These 2 programs are designed to work together and _Aerofoil_ can send G-Code data over the network to _Cutter_ as well as writing files which are then read by the other.
As of version 6.1  _Aerofoil_ can be configured to produce G-Code that's compatible with Mach3 and _Cutter_ will take G-Code output from Jedicut that was originally destined for Mach3.
Currently the attached CNC cutter is fairly smart - it understands a few high level commands such as moving 4 axes together to produce a straight line and move to home.  There's plenty of scope to make it drive other hardware if needed.

#Software Components

## Aerofoil
Standard windows MFC application.  Allows the development of plot files (.plt) for plotting aerofoils on paper and cut (.cut) files for generating G-Code for a CNC Cutter.

## Cutter
This provides single page control of the cutter hardware in response to manual input or G-Code files.  _Cutter_ contains a full G-Code interpreter and also allows lower level control of the cutter hardware for setup/diagnostics.

## Kernel
Kernel provides the core algorithms and code that's shared between _Aerofoil_ and _Cutter_.
Kernel keeps a list of 3D objects - wings, ellipse-pairs and points.  Each of those may have a number of plotters which plot the objects or cutters that know how to drive a CNC cutter.  The objects know their shape, the plotters know where to plot them, the cutters know where and how to cut them.

The wing is basically two aerofoil sections plus a transform to scale and rotate (think washout). Coordinates are converted to a parametric spline ( x and y defined by fx(u) and fy(u) 0<=u<=1 ).  That allows root and tip to be kept in sync for hot wire cutting  even when the sections are different, spars are being cut etc.  Because of the need to cut spars etc the cutting happens between "intercepts".  At each intercept something happens e.g. a spar slot is cut or LE/TE are cut.   Plotting on paper is a much simpler proposition!


## Arduino
This implements a fairly simple stepper driver that expects to be pulsing a modern microstepping driver chip (e.g. DRV8825 or TB6600) rather than driving the phases directly.  It expects commands via a USB serial port and either executes them immediately or adds them to a command queue.  As commands are executed they can put stepper data into an interrupt driven FIFO which empties at a constant rate. The key command is Line which will drive each of the 4 motors together over a given time period.
