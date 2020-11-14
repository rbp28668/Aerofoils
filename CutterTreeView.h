/* Aerofoil
Aerofoil plotting and CNC cutter driver
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

// CutterTreeView is the view class for CutterDoc that displays the structures
// to be cut and the cut instances for those structures.
#pragma once
#include <map>
#include <string>
#include "Kernel/Cutpath.h"

class CutterDoc;
class CStructure;
class CutStructure;

// CutterTreeView view

class CutterTreeView : public CTreeView
{
	// Tracks popup menus - needs to align with IDR_CUTTER_POPUPS menu
	// Used to select correct popup via NodeT below.
	enum Menus {
		// Top level
		STRUCTURE = 0,
		CUTTING,
		// Structure items
		WING,
		ELLIPSE,
		POINT,
		GCODE,
		DXF,
		// Cutting items
		WING_CUT,
		ELLIPSE_CUT,
		POINT_CUT,
		DXF_CUT,
		GCODE_CUT,
		HOME_CUT

	};

	// NodeT is used to store the relationship between items in the tree,
	// the actual object and the popup menu used to edit that item.
	// Also, different constructors implement a degree of run-time type information as most specific constructor should be called.
	typedef struct NodeT {
		HTREEITEM itemHandle;
		void* pItem;
		int popupIndex;
		bool isCutter;
		bool isStructure;
		inline NodeT() : itemHandle(0), pItem(0), popupIndex(0), isCutter(false), isStructure(false) {}
		inline NodeT(HTREEITEM item, CStructure* structure, CutterTreeView::Menus menu) : 
			itemHandle(item), 
			pItem(structure), 
			popupIndex(menu), 
			isStructure(true), 
			isCutter(false) 
		{}
		inline NodeT(HTREEITEM item, CutStructure* object, CutterTreeView::Menus menu) : 
			itemHandle(item), 
			pItem(object), 
			popupIndex(menu),
			isStructure(false),
			isCutter(true)
		{}
		inline NodeT(HTREEITEM item, void* object, CutterTreeView::Menus menu) : 
			itemHandle(item), 
			pItem(object), 
			popupIndex(menu) ,
			isStructure(false),
			isCutter(false)
		{}
		inline NodeT(const NodeT & src) : 
			itemHandle(src.itemHandle), 
			pItem(src.pItem), 
			popupIndex(src.popupIndex),
			isStructure(src.isStructure),
			isCutter(src.isCutter)
		{}
	} Node;

	HTREEITEM hSelected;
	HTREEITEM hStructure;
	HTREEITEM hCutting;
	std::map<HTREEITEM, Node*> treeNodes;

	std::map<std::string, Menus> menuLookup;

	DECLARE_DYNCREATE(CutterTreeView)

protected:
	CutterTreeView();           // protected constructor used by dynamic creation
	virtual ~CutterTreeView();

	CutterDoc* GetDocument();
	void processClick(UINT nFlags, CPoint point);
	void addStructureNode(CStructure* pItem, Menus menu);
	void deleteStructureNode();
	void addCutNode(CutStructure* pItem, Menus menu);
	void deleteCutNode();

	Node* newNode(HTREEITEM item, CStructure* object, Menus menu);
	Node* newNode(HTREEITEM item, CutStructure* object, Menus menu);
	Node* newNode(HTREEITEM item, void* object, Menus menu);
	Node* getNode(HTREEITEM item);
	void freeNode(HTREEITEM item);

	void labelCutterNodes();
	void sortCutterNodes();
	void deleteOrphanCutterNodes();
	void createTreeFromDoc();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:

	void addStructure(CStructure* pItem);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnInitialUpdate();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	
	afx_msg void OnStructureNewellipse();
	afx_msg void OnStructureNewpoint();
	afx_msg void OnStructureNewgcode();
	afx_msg void OnStructureNewdxf();
	afx_msg void OnStructureNewwing();

	afx_msg void OnWingEdit();
	afx_msg void OnWingNewcore();
	afx_msg void OnWingDelete();

	afx_msg void OnEllipseEdit();
	afx_msg void OnEllipseNewcut();
	afx_msg void OnEllipseDelete();

	afx_msg void OnPointEdit();
	afx_msg void OnPointNewcut();
	afx_msg void OnPointNewmove();
	afx_msg void OnPointDelete();

	afx_msg void OnGcodeEdit();
	afx_msg void OnGcodeNewcut();
	afx_msg void OnGcodeDelete();

	afx_msg void OnDxfNewcut();
	afx_msg void OnDxfDelete();

	afx_msg void OnCuttingHome();
	
	afx_msg void OnWingcutDelete();
	
	afx_msg void OnEllipsecutDelete();
	
	afx_msg void OnPointcutDelete();
	
	afx_msg void OnGcodecutDelete();

	afx_msg void OnDxfcutDelete();
	afx_msg void OnCutMoveup();
	afx_msg void OnCutMovedown();
	afx_msg void OnCutMovetotop();
	afx_msg void OnCutMovetobottom();
	afx_msg void OnCutClone();
	afx_msg void OnCutDelete();
	afx_msg void OnCutTransform();
	afx_msg void OnWingPlotflags();
	afx_msg void OnPointcutMakecut();
	afx_msg void OnPointcutMakemove();
	afx_msg void OnWingCutTypeNormal();
	afx_msg void OnWingCutTypeReverse();
	afx_msg void OnWingCutTypeTopLeToTe();
	afx_msg void OnWingCutTypeTopTeToLe();
	afx_msg void OnWingCutTypeBottomLeToTe();
	afx_msg void OnWingCutTypeBottomTeToLe();
	void setWingCutMode(CPathCutter::Mode mode);
	afx_msg void OnWingCutAlignLeadingedge();
	afx_msg void OnWingCutAlignTrailingedge();
	afx_msg void OnWingCutAlignNothing();
	afx_msg void OnHomecutDelete();
	afx_msg void OnUpdateIsCutterNode(CCmdUI* pCmdUI);
	
	afx_msg void OnCutShowbounds();
	afx_msg void OnCutInsertstartline();
	afx_msg void OnCutInsertfinishline();
	afx_msg void OnEllipsePlotflags();
	afx_msg void OnDxfExplode();
};


