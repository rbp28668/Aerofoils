# Aerofoils
Program for plotting aerofoils and driving a CNC cutter for foam wings.  This probject started out in 1992 with a prototype wing cutter and ran, with a character based UI on a PC-AT. That had a massive 5MB hard drive.  Luxury! The cutter hardware was eventually sold to a local model shop and the software was converted to plotting rib sections / formers onto paper.  As such, the code is slightly schizophrenic due to this dual role. 

## Kernel
Kernel keeps a list of 3D objects - wings, ellipse-pairs and points.  Each of those may have a number of plotters which plot the objects in some fashion - either as a cutter or to print to paper.  The objects know their shape, the plotters know where to plot them.  

The wing is basically two aerofoil sections plus a transform to scale and rotate (think washout). Coordinates are converted to a parametric spline ( x and y defined by fx(u) and fy(u) 0<=u<=1 ).  That allows root and tip to be kept in sync for hot wire cutting  even when the sections are different, spars are being cut etc.  Because of the need to cut spars etc the cutting happens between "intercepts".  At each intercept something happens e.g. a spar slot is cut or LE/TE are cut.   Plotting on paper is a much simpler proposition!

## UI
Standard windows MFC application.  Not very exciting.

## Arduino
This implements a fairly simple stepper driver that expects to be pulsing a modern microstepping driver chip (e.g. DRV8825 or TB6600) rather than driving the phases directly.  It expects commands via a USB serial port and either executes them immediately or adds them to a command queue.  As commands are executed they can put stepper data into an interrupt driven FIFO which empties at a constant rate.
