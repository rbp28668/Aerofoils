/* Aerofoil
Aerofoil plotting and CNC cutter driver
Kernel / core algorithms
Copyright(C) 1995-2019 R Bruce Porteous

This program is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "PointT.h"
class COutputDevice;

// StructureOutput defines a base interface that both cutters and plotters implement.  
// Allows complex cut algorithms to be plotted to paper (and vice versa).
// Not quite as useful as you'd expect as usually the cutting and plotting algorithms
// are slightly different anyway to allow for kerf size etc in the case of cutting.

class StructureOutput
{
public:
	virtual void move(COutputDevice* pdev, const PointT& root, const PointT& tip) = 0;
	virtual void line(COutputDevice* pdev, const PointT& root, const PointT& tip) = 0;

};

