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

// CutterTreeView.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include "Aerofoil.h"
#include "CutterTreeView.h"
#include "CutterDoc.h"
#include "resource.h"
#include "EllipsePairDlg.h"
#include "EllipseFlagsDlg.h"
#include "PlotPointDlg.h"
#include "WingDlg.h"
#include "WingCoreFlagsDlg.h"
#include "TransformDialog.h"
#include "GCodeEditDialog.h"
#include "CutBoundsDisplay.h"
#include "Kernel/Bounds.h"
#include "Kernel/Wing.h"
#include "Kernel/EllipsePair.h"
#include "Kernel/PointStructure.h"
#include "Kernel/CutStructure.h"
#include "Kernel/GCodeSnippet.h"
#include "Kernel/DXFObject.h"
#include "Kernel/Cutpath.h"
#include "Kernel/EllipseCutter.h"
#include "Kernel/PointCutter.h"
#include "Kernel/GCodeSnippetCutter.h"
#include "Kernel/DXFObjectCutter.h"
#include "Kernel/HomeCutter.h"



// CutterTreeView

IMPLEMENT_DYNCREATE(CutterTreeView, CTreeView)

CutterTreeView::CutterTreeView()
	: hSelected(0)
	, hCutting(0)
	, hStructure(0)
{
	menuLookup.insert(make_pair(CWing::TYPE, WING));
	menuLookup.insert(make_pair(CEllipsePair::TYPE, ELLIPSE));
	menuLookup.insert(make_pair(CPointStructure::TYPE, POINT));
	menuLookup.insert(make_pair(GCodeSnippet::TYPE, GCODE));
	menuLookup.insert(make_pair(DXFObject::TYPE, DXF));

	menuLookup.insert(make_pair(CPathCutter::TYPE, WING_CUT));
	menuLookup.insert(make_pair(EllipseCutter::TYPE, ELLIPSE_CUT));
	menuLookup.insert(make_pair(PointCutter::TYPE, POINT_CUT));
	menuLookup.insert(make_pair(DXFObjectCutter::TYPE, DXF_CUT));
	menuLookup.insert(make_pair(GCodeSnippetCutter::TYPE, GCODE_CUT));
	menuLookup.insert(make_pair(HomeCutter::TYPE, HOME_CUT));
}

CutterTreeView::~CutterTreeView()
{
	for (std::map<HTREEITEM, NodeT*>::iterator it = treeNodes.begin(); it != treeNodes.end(); ++it) {
		delete it->second;
	}
	treeNodes.clear();
}

CutterDoc * CutterTreeView::GetDocument()
{
	return (CutterDoc*)CView::GetDocument();
}

// Creates a new Node indexed by HTREEITEM for that menu item in treeNodes.  treeNodes
// can then be used to lookup the Node by the HTREEITEM and hence get the structure and information
// about the object that tree item refers to.
CutterTreeView::Node* CutterTreeView::newNode(HTREEITEM item, CStructure* object, Menus menu)
{
	assert(this);
	assert(object);
	assert(item);
	Node* node = new Node(item, object, menu);
	treeNodes.insert(std::pair<HTREEITEM, Node*>(item, node));
	return node;
}
CutterTreeView::Node* CutterTreeView::newNode(HTREEITEM item, CutStructure* object, Menus menu)
{
	assert(this);
	assert(object);
	assert(item);
	Node* node = new Node(item, object, menu);
	treeNodes.insert(std::pair<HTREEITEM, Node*>(item, node));
	return node;
}
CutterTreeView::Node* CutterTreeView::newNode(HTREEITEM item, void* object, Menus menu)
{
	assert(this);
	assert(object);
	assert(item);
	Node* node = new Node(item, object, menu);
	treeNodes.insert(std::pair<HTREEITEM, Node*>(item, node));
	return node;
}

CutterTreeView::Node* CutterTreeView::getNode(HTREEITEM item)
{
	assert(this);
	assert(treeNodes.find(item) != treeNodes.end());
	return treeNodes[item];
}

void CutterTreeView::freeNode(HTREEITEM item) {
	assert(this);
	assert(treeNodes.find(item) != treeNodes.end());
	Node* node = treeNodes[item];
	treeNodes.erase(item);
	delete node;
}

// label cutter nodes with their index so that they can be sorted.
void CutterTreeView::labelCutterNodes()
{
	// Iterate through the children of the cutting node, finding out where
	// each corresponding CutStructure is in the sequence and labelling
	// the node appropriately.
	if (GetTreeCtrl().ItemHasChildren(hCutting))
	{
		HTREEITEM hChild = GetTreeCtrl().GetChildItem(hCutting);

		while (hChild != NULL)
		{
			Node* node = treeNodes[hChild];
			int idx = GetDocument()->indexOf(static_cast<CutStructure*>(node->pItem));
			assert(idx != -1);
			GetTreeCtrl().SetItemData(hChild, idx);
			hChild = GetTreeCtrl().GetNextItem(hChild, TVGN_NEXT);
		}
	}
}

static int CALLBACK  CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
	if (lParam1 == lParam2) return 0;
	return (lParam1 < lParam2) ? -1 : 1;
}

void CutterTreeView::sortCutterNodes()
{
	// sort children of hCutting by their params
	TVSORTCB tvsortcb;
	tvsortcb.hParent = hCutting;
	tvsortcb.lParam = 0;
	tvsortcb.lpfnCompare = &CompareFunc;
	GetTreeCtrl().SortChildrenCB(&tvsortcb);
}

void CutterTreeView::deleteOrphanCutterNodes()
{
	// Iterate through the children of the cutting node, deleting any that no longer
	// have a valid CutStructure e.g. after deleting the parent structure.
	if (GetTreeCtrl().ItemHasChildren(hCutting))
	{
		HTREEITEM hChild = GetTreeCtrl().GetChildItem(hCutting);

		while (hChild != NULL)
		{
			Node* node = treeNodes[hChild];
			int idx = GetDocument()->indexOf(static_cast<CutStructure*>(node->pItem));
			HTREEITEM hNext = GetTreeCtrl().GetNextItem(hChild, TVGN_NEXT);
			if(idx == -1){
				GetTreeCtrl().DeleteItem(hChild);
			}
			hChild = hNext;
		}
	}
}

void CutterTreeView::createTreeFromDoc()
{
	CutterDoc* pDoc = GetDocument();
	for (Cut::StructureIterator iter = pDoc->beginStructures();
		iter != pDoc->endStructures();
		++iter) {
		std::string typeName = (*iter)->getType();
		Menus menu = menuLookup[typeName];
		addStructureNode(*iter, menu);
	}

	for (Cut::CutIterator iter = pDoc->beginCutStructures();
		iter != pDoc->endCutStructures();
		++iter) {
		std::string typeName = (*iter)->getType();
		Menus menu = menuLookup[typeName];
		addCutNode(*iter, menu);
	}

}

void CutterTreeView::processClick(UINT nFlags, CPoint point)
{
	assert(this);

	UINT uFlags = 0;
	HTREEITEM hItem = GetTreeCtrl().HitTest(point, &uFlags);

	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
	{
		int idx = (int)GetTreeCtrl().GetItemData(hItem);
		Node* node = getNode(hItem);
		CMenu menu;
		menu.LoadMenu(IDR_CUTTER_POPUPS);
		CMenu* pMenu = menu.GetSubMenu(node->popupIndex);
		ClientToScreen(&point);

		GetTreeCtrl().SelectItem(hItem);

		pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x,
			point.y, this, 0);
	}
}


void CutterTreeView::addStructureNode(CStructure * pItem, Menus menu)
{
	assert(this);
	assert(pItem);
	if (pItem) {
		HTREEITEM item = GetTreeCtrl().InsertItem(pItem->getDescriptiveText().c_str(), hStructure);
		newNode(item, pItem, menu);
		GetTreeCtrl().SelectItem(item);
		GetDocument()->RedrawNow();
	}
}

void CutterTreeView::deleteStructureNode()
{
	assert(this);

	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	assert(item != hStructure);
	assert(item != hCutting);

	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		CStructure* pStructure = static_cast<CStructure*>(node->pItem);
		GetDocument()->deleteStructure(pStructure);
		GetTreeCtrl().DeleteItem(item);
		freeNode(item);
		deleteOrphanCutterNodes();
		GetDocument()->RedrawNow();
	}
}

void CutterTreeView::addCutNode(CutStructure * pItem, Menus menu)
{
	assert(this);
	assert(pItem);
	if (pItem) {
		HTREEITEM item = GetTreeCtrl().InsertItem(pItem->getDescriptiveText().c_str(), hCutting);
		newNode(item, pItem, menu);
		GetTreeCtrl().SelectItem(item);
		GetDocument()->RedrawNow();
	}
}

void CutterTreeView::deleteCutNode()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	assert(item != hStructure);
	assert(item != hCutting);

	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		CutStructure* pCut = static_cast<CutStructure*>(node->pItem);
		GetDocument()->deleteCut(pCut);
		GetTreeCtrl().DeleteItem(item);
		freeNode(item);
		GetDocument()->RedrawNow();
	}

}

BEGIN_MESSAGE_MAP(CutterTreeView, CTreeView)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_STRUCTURE_NEWELLIPSE, &CutterTreeView::OnStructureNewellipse)
	ON_COMMAND(ID_STRUCTURE_NEWPOINT, &CutterTreeView::OnStructureNewpoint)
	ON_COMMAND(ID_STRUCTURE_NEWWING, &CutterTreeView::OnStructureNewwing)
	ON_COMMAND(ID_WING_EDIT, &CutterTreeView::OnWingEdit)
	ON_COMMAND(ID_WING_DELETE, &CutterTreeView::OnWingDelete)
	ON_COMMAND(ID_ELLIPSE_EDIT, &CutterTreeView::OnEllipseEdit)
	ON_COMMAND(ID_ELLIPSE_DELETE, &CutterTreeView::OnEllipseDelete)
	ON_COMMAND(ID_POINT_EDIT, &CutterTreeView::OnPointEdit)
	ON_COMMAND(ID_POINT_DELETE, &CutterTreeView::OnPointDelete)
	ON_COMMAND(ID_CUTTING_HOME, &CutterTreeView::OnCuttingHome)
	ON_COMMAND(ID_WING_NEWCORE, &CutterTreeView::OnWingNewcore)
	ON_COMMAND(ID_ELLIPSE_NEWCUT, &CutterTreeView::OnEllipseNewcut)
	ON_COMMAND(ID_POINT_NEWCUT, &CutterTreeView::OnPointNewcut)
	ON_COMMAND(ID_STRUCTURE_NEWGCODE, &CutterTreeView::OnStructureNewgcode)
	ON_COMMAND(ID_STRUCTURE_NEWDXF, &CutterTreeView::OnStructureNewdxf)
	ON_COMMAND(ID_WINGCUT_DELETE, &CutterTreeView::OnWingcutDelete)
	ON_COMMAND(ID_ELLIPSECUT_DELETE, &CutterTreeView::OnEllipsecutDelete)
	ON_COMMAND(ID_POINTCUT_DELETE, &CutterTreeView::OnPointcutDelete)
	ON_COMMAND(ID_GCODECUT_DELETE, &CutterTreeView::OnGcodecutDelete)
	ON_COMMAND(ID_GCODE_DELETE, &CutterTreeView::OnGcodeDelete)
	ON_COMMAND(ID_GCODE_EDIT, &CutterTreeView::OnGcodeEdit)
	ON_COMMAND(ID_GCODE_NEWCUT, &CutterTreeView::OnGcodeNewcut)
	ON_COMMAND(ID_DXF_DELETE, &CutterTreeView::OnDxfDelete)
	ON_COMMAND(ID_DXF_NEWCUT, &CutterTreeView::OnDxfNewcut)
	ON_COMMAND(ID_DXFCUT_DELETE, &CutterTreeView::OnDxfcutDelete)
	ON_COMMAND(ID_CUT_MOVEUP, &CutterTreeView::OnCutMoveup)
	ON_COMMAND(ID_CUT_MOVEDOWN, &CutterTreeView::OnCutMovedown)
	ON_COMMAND(ID_CUT_MOVETOTOP, &CutterTreeView::OnCutMovetotop)
	ON_COMMAND(ID_CUT_MOVETOBOTTOM, &CutterTreeView::OnCutMovetobottom)
	ON_COMMAND(ID_CUT_TRANSFORM, &CutterTreeView::OnCutTransform)
	ON_COMMAND(ID_CUT_CLONE, &CutterTreeView::OnCutClone)
	ON_COMMAND(ID_WING_PLOTFLAGS, &CutterTreeView::OnWingPlotflags)
	ON_COMMAND(ID_POINT_NEWMOVE, &CutterTreeView::OnPointNewmove)
	ON_COMMAND(ID_POINTCUT_MAKECUT, &CutterTreeView::OnPointcutMakecut)
	ON_COMMAND(ID_POINTCUT_MAKEMOVE, &CutterTreeView::OnPointcutMakemove)
	ON_COMMAND(ID_TYPE_NORMAL, &CutterTreeView::OnWingCutTypeNormal)
	ON_COMMAND(ID_TYPE_REVERSE, &CutterTreeView::OnWingCutTypeReverse)
	ON_COMMAND(ID_TYPE_TOPLEADINGTOTRAILINGEDGE, &CutterTreeView::OnWingCutTypeTopLeToTe)
	ON_COMMAND(ID_TYPE_TOPTRAILINGTOLEADINGEDGE, &CutterTreeView::OnWingCutTypeTopTeToLe)
	ON_COMMAND(ID_TYPE_BOTTOMLEADINGTOTRAILINGEDGE, &CutterTreeView::OnWingCutTypeBottomLeToTe)
	ON_COMMAND(ID_TYPE_BOTTOMTRAILINGTOLEADINGEDGE, &CutterTreeView::OnWingCutTypeBottomTeToLe)
	ON_COMMAND(ID_ALIGN_LEADINGEDGE, &CutterTreeView::OnWingCutAlignLeadingedge)
	ON_COMMAND(ID_ALIGN_TRAILINGEDGE, &CutterTreeView::OnWingCutAlignTrailingedge)
	ON_COMMAND(ID_ALIGN_NOTHING, &CutterTreeView::OnWingCutAlignNothing)
	ON_COMMAND(ID_EDIT_MOVEUP, &CutterTreeView::OnCutMoveup)
	ON_COMMAND(ID_EDIT_MOVEDOWN, &CutterTreeView::OnCutMovedown)
	ON_COMMAND(ID_EDIT_MOVETOTOP, &CutterTreeView::OnCutMovetotop)
	ON_COMMAND(ID_EDIT_MOVETOBOTTOM, &CutterTreeView::OnCutMovetobottom)
	ON_COMMAND(ID_EDIT_CLONE, &CutterTreeView::OnCutClone)
	ON_COMMAND(ID_EDIT_DELETE, &CutterTreeView::OnCutDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVEUP, &CutterTreeView::OnUpdateIsCutterNode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVEDOWN, &CutterTreeView::OnUpdateIsCutterNode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVETOTOP, &CutterTreeView::OnUpdateIsCutterNode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVETOBOTTOM, &CutterTreeView::OnUpdateIsCutterNode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLONE, &CutterTreeView::OnUpdateIsCutterNode)
	ON_COMMAND(ID_HOMECUT_DELETE, &CutterTreeView::OnHomecutDelete)
	ON_COMMAND(ID_CUT_SHOWBOUNDS, &CutterTreeView::OnCutShowbounds)
	ON_COMMAND(ID_CUT_INSERTSTARTLINE, &CutterTreeView::OnCutInsertstartline)
	ON_COMMAND(ID_CUT_INSERTFINISHLINE, &CutterTreeView::OnCutInsertfinishline)
	ON_COMMAND(ID_ELLIPSE_PLOTFLAGS, &CutterTreeView::OnEllipsePlotflags)
	ON_COMMAND(ID_DXF_EXPLODE, &CutterTreeView::OnDxfExplode)
END_MESSAGE_MAP()


// CutterTreeView diagnostics

#ifdef _DEBUG
void CutterTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CutterTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG




// Allow structure to be fed in externally.
void CutterTreeView::addStructure(CStructure* pItem)
{
	std::string typeName = pItem->getType();
	Menus menu = menuLookup[typeName];
	addStructureNode(pItem, menu);

}

//===============================================================
// CutterTreeView message handlers
//===============================================================


int CutterTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


void CutterTreeView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();
	
	hStructure = GetTreeCtrl().InsertItem("Structure");
	newNode(hStructure, this, STRUCTURE);

	hCutting = GetTreeCtrl().InsertItem("Cutting");
	newNode(hCutting,this, CUTTING);

	createTreeFromDoc();
}


void CutterTreeView::OnRButtonDown(UINT nFlags, CPoint point)
{
	processClick(nFlags, point);
}


void CutterTreeView::OnRButtonUp(UINT nFlags, CPoint point)
{
}

void CutterTreeView::OnStructureNewwing()
{
	CFileDialog dlg(TRUE,
		".DAT",
		NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		"Aerofoil Files (.DAT)|*.DAT||"
	);

	OPENFILENAME& ofn = dlg.GetOFN();
	ofn.lpstrTitle = "Select Root Section";

	if (dlg.DoModal() == IDOK)
	{
		CString rootName = dlg.GetPathName();
		ofn.lpstrTitle = "Select Tip Section";

		if (dlg.DoModal() == IDOK)
		{
			CString tipName = dlg.GetPathName();
			CWing* pWing = GetDocument()->newWing(rootName, 1.0f, tipName, 1.0f);
			addStructureNode(pWing, WING);
		}
	}
}

void CutterTreeView::OnStructureNewellipse()
{
	CEllipsePair pair;
	CEllipsePairDlg dlg(&pair);
	if (dlg.DoModal() == IDOK)
	{
		CEllipsePair* pEllipse = GetDocument()->newEllipse(pair);
		addStructureNode(pEllipse, ELLIPSE);
	}
}


void CutterTreeView::OnStructureNewpoint()
{
	CPointStructure point;
	CPlotPointDlg dlg(&point);
	if (dlg.DoModal() == IDOK)
	{
		CPointStructure* pPoint = GetDocument()->newPoint(point);
		addStructureNode(pPoint, POINT);
	}
}


void CutterTreeView::OnStructureNewgcode()
{
	GCodeEditDialog dlg;
	if (dlg.DoModal() == IDOK) {
		GCodeSnippet* pSnippet = GetDocument()->newGcodeSnippet(dlg.programText);
		addStructureNode(pSnippet, GCODE);
	}
}


void CutterTreeView::OnStructureNewdxf()
{
	CFileDialog dlg(TRUE,
		".DXF",
		NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		"DXF CAD File (.DXF)|*.DXF||"
	);

	OPENFILENAME& ofn = dlg.GetOFN();
	ofn.lpstrTitle = "Select DXF File";

	if (dlg.DoModal() == IDOK)
	{
		CWaitCursor waitCursor;
		CString path = dlg.GetPathName();
		DXFObject* pdxf = GetDocument()->newDxfObject(path);
		addStructureNode(pdxf, DXF);
	}
}



void CutterTreeView::OnWingEdit()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		CWing* pWing = static_cast<CWing*>(node->pItem);
		CWingDlg dlg(pWing, GetParent());
		if (dlg.DoModal() == IDOK){
			GetTreeCtrl().SetItemText(item, pWing->getDescriptiveText().c_str());
			GetDocument()->RedrawNow();
		}

	}
}

void CutterTreeView::OnWingPlotflags()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		CWing* pWing = static_cast<CWing*>(node->pItem);
		WingCoreFlagsDlg dlg(GetParent());
		dlg.cutLeadingEdge = pWing->getPlotFlags()->plot_le;
		dlg.cutTrailingEdge = pWing->getPlotFlags()->plot_te;
		dlg.cutSpars = pWing->getPlotFlags()->plot_spars;
		dlg.cutCutouts = pWing->getPlotFlags()->plot_cutouts;
		if (dlg.DoModal() == IDOK) {
			pWing->getPlotFlags()->plot_le = dlg.cutLeadingEdge == TRUE;
			pWing->getPlotFlags()->plot_te = dlg.cutTrailingEdge == TRUE;
			pWing->getPlotFlags()->plot_spars = dlg.cutSpars == TRUE;
			pWing->getPlotFlags()->plot_cutouts = dlg.cutCutouts == TRUE;
		}
	}
}

void CutterTreeView::OnWingNewcore()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		CWing* pWing = static_cast<CWing*>(node->pItem);

		CPathCutter* cut = GetDocument()->newPathCutter(pWing);
		addCutNode(cut, WING_CUT);
	}
}

void CutterTreeView::OnWingDelete()
{
	deleteStructureNode();
}


void CutterTreeView::OnEllipseEdit()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		CEllipsePair* pEllipses = static_cast<CEllipsePair*>(node->pItem);
		CEllipsePairDlg dlg(pEllipses, GetParent());
		if (dlg.DoModal() == IDOK) {
			GetTreeCtrl().SetItemText(item, pEllipses->getDescriptiveText().c_str());
			GetDocument()->RedrawNow();
		}
	}
}


void CutterTreeView::OnEllipsePlotflags()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		CEllipsePair* pEllipses = static_cast<CEllipsePair*>(node->pItem);
		CEllipseFlagsDlg dlg(pEllipses->getPlotFlags(), GetParent());
		if (dlg.DoModal() == IDOK) {
			GetTreeCtrl().SetItemText(item, pEllipses->getDescriptiveText().c_str());
			GetDocument()->RedrawNow();
		}
	}
}

void CutterTreeView::OnEllipseNewcut()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		CEllipsePair* pEllipses = static_cast<CEllipsePair*>(node->pItem);

		EllipseCutter* cut = GetDocument()->newElipseCutter(pEllipses);
		addCutNode(cut, ELLIPSE_CUT);
	}
}



void CutterTreeView::OnEllipseDelete()
{
	deleteStructureNode();
}



void CutterTreeView::OnPointEdit()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		CPointStructure* pPoint = static_cast<CPointStructure*>(node->pItem);
		CPlotPointDlg dlg(pPoint, GetParent());
		if (dlg.DoModal() == IDOK) {
			GetTreeCtrl().SetItemText(item, pPoint->getDescriptiveText().c_str());
			GetDocument()->RedrawNow();
		}
	}
}


void CutterTreeView::OnPointNewcut()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		CPointStructure* pPoints = static_cast<CPointStructure*>(node->pItem);

		PointCutter* cut = GetDocument()->newPointCutter(pPoints);
		addCutNode(cut, POINT_CUT);
	}
}

void CutterTreeView::OnPointNewmove()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		CPointStructure* pPoints = static_cast<CPointStructure*>(node->pItem);

		PointCutter* cut = GetDocument()->newPointCutter(pPoints);
		cut->setFast(true);
		addCutNode(cut, POINT_CUT);
	}
}

void CutterTreeView::OnPointDelete()
{
	deleteStructureNode();
}

void CutterTreeView::OnGcodeEdit()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		GCodeSnippet* pGCode = static_cast<GCodeSnippet*>(node->pItem);
		GCodeEditDialog dlg(GetParent());
	
		std::string text;
		pGCode->programText(text);
		dlg.programText = text.c_str();
		if (dlg.DoModal() == IDOK) {
			pGCode->updateProgram(dlg.programText);
			// TODO validate gcode
			GetDocument()->RedrawNow();
		}
	}

}

void CutterTreeView::OnGcodeNewcut()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		GCodeSnippet* pgcode = static_cast<GCodeSnippet*>(node->pItem);

		GCodeSnippetCutter* cut = GetDocument()->newGCodeSnippetCutter(pgcode);
		addCutNode(cut, GCODE_CUT);
	}
}

void CutterTreeView::OnGcodeDelete()
{
	deleteStructureNode();
}


void CutterTreeView::OnDxfNewcut()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		DXFObject* pdxf = static_cast<DXFObject*>(node->pItem);

		DXFObjectCutter* cut = GetDocument()->newDXFObjectCutter(pdxf);
		addCutNode(cut, DXF_CUT);
	}
}

void CutterTreeView::OnDxfDelete()
{
	deleteStructureNode();
}


void CutterTreeView::OnCuttingHome()
{
	HomeCutter* cut = GetDocument()->newHomePosition();
	addCutNode(cut, HOME_CUT);
}


void CutterTreeView::OnHomecutDelete()
{
	deleteCutNode();
}


void CutterTreeView::OnCutTransform()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		CutStructure* pcut = static_cast<CutStructure*>(node->pItem);
		TransformDialog dialog;
		dialog.rootXOffset = pcut->getRootOffsets().fx;
		dialog.rootYOffset = pcut->getRootOffsets().fy;
		dialog.tipXOffset = pcut->getTipOffsets().fx;
		dialog.tipYOffset = pcut->getTipOffsets().fy;
		dialog.invert = pcut->isInverted();
		dialog.reflect = pcut->isReflected();
		dialog.rootSide = pcut->isRootLeft() ? 0 : 1;

		if (dialog.DoModal() == IDOK) {
			PointT root((float)dialog.rootXOffset, (float)dialog.rootYOffset);
			PointT tip((float)dialog.tipXOffset, (float)dialog.tipYOffset);
			pcut->setOffsets(root, tip);
			pcut->setInvert(dialog.invert == TRUE );
			pcut->setReflect(dialog.reflect == TRUE);
			pcut->setRootSide(dialog.rootSide == 0);
			GetTreeCtrl().SetItemText(item, pcut->getDescriptiveText().c_str());
			GetDocument()->RedrawNow();
		}
	}
}


void CutterTreeView::OnWingcutDelete()
{
	deleteCutNode();
}


void CutterTreeView::OnEllipsecutDelete()
{
	deleteCutNode();
}

void CutterTreeView::OnPointcutMakecut()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		PointCutter* pcut = static_cast<PointCutter*>(node->pItem);
		pcut->setFast(false);
		GetTreeCtrl().SetItemText(item, pcut->getDescriptiveText().c_str());
		GetDocument()->RedrawNow();
	}
}


void CutterTreeView::OnPointcutMakemove()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		PointCutter* pcut = static_cast<PointCutter*>(node->pItem);
		pcut->setFast(true);
		GetTreeCtrl().SetItemText(item, pcut->getDescriptiveText().c_str());
		GetDocument()->RedrawNow();
	}
}


void CutterTreeView::OnPointcutDelete()
{
	deleteCutNode();
}


void CutterTreeView::OnGcodecutDelete()
{
	deleteCutNode();
}


void CutterTreeView::OnDxfcutDelete()
{
	deleteCutNode();
}


void CutterTreeView::OnCutMoveup()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		if (node->isCutter) {
			CutStructure* pCut = static_cast<CutStructure*>(node->pItem);
			GetDocument()->moveUp(pCut);
			labelCutterNodes();
			sortCutterNodes();
			GetDocument()->RedrawNow();
		}
	}
}


void CutterTreeView::OnCutMovedown()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		if (node->isCutter) {
			CutStructure* pCut = static_cast<CutStructure*>(node->pItem);
			GetDocument()->moveDown(pCut);
			labelCutterNodes();
			sortCutterNodes();
			GetDocument()->RedrawNow();
		}
	}
}

void CutterTreeView::OnCutMovetotop()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		if (node->isCutter) {
			CutStructure* pCut = static_cast<CutStructure*>(node->pItem);
			GetDocument()->moveToTop(pCut);
			labelCutterNodes();
			sortCutterNodes();
			GetDocument()->RedrawNow();
		}
	}

}


void CutterTreeView::OnCutMovetobottom()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		if (node->isCutter) {
			CutStructure* pCut = static_cast<CutStructure*>(node->pItem);
			GetDocument()->moveToBottom(pCut);
			labelCutterNodes();
			sortCutterNodes();
			GetDocument()->RedrawNow();
		}
	}
}


void CutterTreeView::OnCutClone()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		if (node->isCutter) {
			// Create a clone
			CutStructure* pCut = static_cast<CutStructure*>(node->pItem);
			CutStructure* copy = pCut->clone();

			// Put it in the cut structure
			GetDocument()->insertCutterAfter(pCut, copy);

			// Add it to the tree
			std::string typeName = copy->getType();
			Menus menu = menuLookup[typeName];
			addCutNode(copy, menu);

			// Tidy up try ordering etc.
			labelCutterNodes();
			sortCutterNodes();
			GetDocument()->RedrawNow();
		}
	}
}


void CutterTreeView::OnCutDelete()
{
	deleteCutNode();
}


void CutterTreeView::OnWingCutTypeNormal()
{
	setWingCutMode(CPathCutter::Mode::NORMAL);
}


void CutterTreeView::OnWingCutTypeReverse()
{
	setWingCutMode(CPathCutter::Mode::REVERSE);
}


void CutterTreeView::OnWingCutTypeTopLeToTe()
{
	setWingCutMode(CPathCutter::Mode::TOP_FROM_LE);
}


void CutterTreeView::OnWingCutTypeTopTeToLe()
{
	setWingCutMode(CPathCutter::Mode::TOP_FROM_TE);
}


void CutterTreeView::OnWingCutTypeBottomLeToTe()
{
	setWingCutMode(CPathCutter::Mode::BOTTOM_FROM_LE);
}


void CutterTreeView::OnWingCutTypeBottomTeToLe()
{
	setWingCutMode(CPathCutter::Mode::BOTTOM_FROM_TE);
}

void CutterTreeView::setWingCutMode(CPathCutter::Mode mode) {
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		CPathCutter* pcut = static_cast<CPathCutter*>(node->pItem);
		pcut->set_mode(mode);
		GetTreeCtrl().SetItemText(item, pcut->getDescriptiveText().c_str());
		GetDocument()->RedrawNow();
	}
}


void CutterTreeView::OnWingCutAlignLeadingedge()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		CPathCutter* pcut = static_cast<CPathCutter*>(node->pItem);
		
		const CWing* pWing = pcut->wing();

		const CTransform* rt = pWing->getRootTransform();
		const CTransform* tt = pWing->getTipTransform();
		NumericT rx = rt->getSweep();
		NumericT tx = tt->getSweep();

		NumericT z = pWing->getSpan();
		if (z > 0) {
			NumericT dx = tx - rx;
			double angle = atan2(dx, z);
			PointT origin(rx, 0, 0);
			pcut->setRotation(origin, angle);
		}

		GetDocument()->RedrawNow();
	}
}


void CutterTreeView::OnWingCutAlignTrailingedge()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		CPathCutter* pcut = static_cast<CPathCutter*>(node->pItem);

		const CWing* pWing = pcut->wing();

		const CTransform* rt = pWing->getRootTransform();
		const CTransform* tt = pWing->getTipTransform();
		NumericT rx = rt->getSweep() + rt->getChord();
		NumericT tx = tt->getSweep() + tt->getChord();

		NumericT z = pWing->getSpan();
		if (z > 0) {
			NumericT dx = tx - rx;
			double angle = atan2(dx, z);
			PointT origin(rt->getSweep(), 0, 0);
			pcut->setRotation(origin, angle);
		}
		GetDocument()->RedrawNow();
	}
}


void CutterTreeView::OnWingCutAlignNothing()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		CPathCutter* pcut = static_cast<CPathCutter*>(node->pItem);
		PointT origin(0, 0, 0);
		pcut->setRotation(origin, 0);
		GetDocument()->RedrawNow();
	}
}

void CutterTreeView::OnUpdateIsCutterNode(CCmdUI* pCmdUI)
{
	bool cutterSelected = false;
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		cutterSelected = node->isCutter;
	}
	pCmdUI->Enable(cutterSelected ? TRUE : FALSE);
}


void CutterTreeView::OnCutShowbounds()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		if (node->isCutter) {
			CutStructure* pCut = static_cast<CutStructure*>(node->pItem);
			Bounds bounds;
			pCut->cut(&bounds, GetDocument()->getContext());
			CutBoundsDisplay display(&bounds);
			display.DoModal();
		}
	}
}




void CutterTreeView::OnCutInsertstartline()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		if (node->isCutter) {
			CutStructure* pCut = static_cast<CutStructure*>(node->pItem);
			
			// Get bounds including first and last lines.
			Bounds bounds;
			pCut->cut(&bounds, GetDocument()->getContext());

			// Set geometry so that a virtual block with the same width as the effective span of the cut
            // defines the nominal edges of the block.
			CutterGeometry geometry;
			geometry.setBlockLeft(0);
			geometry.setBlockRight(bounds.depth());
			geometry.setWidth(bounds.depth());

			// Calculate line corresponding to the start of the cut.
			Position<double> first;
			first.x = bounds.firstRoot().fx;
			first.y = bounds.firstRoot().fy;
			first.u = bounds.firstTip().fx;
			first.v = bounds.firstTip().fy;
			if (bounds.depth() > 0) {
				geometry.blockToAxes(first, bounds.firstRoot().fz, bounds.firstTip().fz);
			}

			CPointStructure point;
			point.setRoot(PointT(first.x, first.y, 0));
			point.setTip(PointT(first.u, first.v, bounds.depth()));
			point.setSpan(bounds.depth());
			CPointStructure* pPoint = GetDocument()->newPoint(point);
			pPoint->setLabel("[wing start]");
			addStructureNode(pPoint, POINT);

			PointCutter* cut = GetDocument()->newPointCutter(pPoint);
			addCutNode(cut, POINT_CUT);
		}
	}
}


void CutterTreeView::OnCutInsertfinishline()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		if (node->isCutter) {
			CutStructure* pCut = static_cast<CutStructure*>(node->pItem);

			// Get bounds including first and last lines.
			Bounds bounds;
			pCut->cut(&bounds, GetDocument()->getContext());

			// Set geometry so that a virtual block with the same width as the effective span of the cut
			// defines the nominal edges of the block.
			CutterGeometry geometry;
			geometry.setBlockLeft(0);
			geometry.setBlockRight(bounds.depth());
			geometry.setWidth(bounds.depth());

			// Calculate line corresponding to the start of the cut.
			Position<double> last;
			last.x = bounds.lastRoot().fx;
			last.y = bounds.lastRoot().fy;
			last.u = bounds.lastTip().fx;
			last.v = bounds.lastTip().fy;
			if (bounds.depth() > 0) {
				geometry.blockToAxes(last, bounds.lastRoot().fz, bounds.lastTip().fz);
			}

			CPointStructure point;
			point.setRoot(PointT(last.x, last.y, 0));
			point.setTip(PointT(last.u, last.v, bounds.depth()));
			point.setSpan(bounds.depth());
			CPointStructure* pPoint = GetDocument()->newPoint(point);
			pPoint->setLabel("[wing finish]");
			addStructureNode(pPoint, POINT);

			PointCutter* cut = GetDocument()->newPointCutter(pPoint);
			addCutNode(cut, POINT_CUT);
		}
	}
}




void CutterTreeView::OnDxfExplode()
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item) {
		Node* node = getNode(item);
		assert(node->itemHandle == item);
		DXFObject* pdxf = static_cast<DXFObject*>(node->pItem);

		while (pdxf->itemCount() > 1) {
			DXFObject* pItem = pdxf->extractFirstItem();
			GetDocument()->addStructure(pItem);
		}

		GetDocument()->RedrawNow();
	}
}

