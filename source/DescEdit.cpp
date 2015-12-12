// Copyright (C) 2015 Niklas Rosenstein
// Licensed under the Apache License Version 2.0
//   http://www.apache.org/licenses/

#include <c4d.h>
#include <customgui_description.h>

enum
{
  PLUGIN_ID = 1036513,
  PLUGIN_ID_NODE = 1036514,
  __FIRST_ITEM__ = 10000,
  IDC_DESC,
};

Bool RegisterDescEdit();

// ===========================================================================
// ===========================================================================
class DescPreviewDialog : public GeDialog
{
  typedef GeDialog super;
public:
  DescPreviewDialog();
  Bool CreateLayout() override;
  Bool InitValues() override;
  DescriptionCustomGui* GetDescriptionGui() const { return _descGui; }
private:
  DescriptionCustomGui* _descGui;
};

// ===========================================================================
// ===========================================================================
class DescEditDialog : public GeDialog
{
  typedef GeDialog super;
public:
  DescEditDialog();
  Bool CreateLayout() override;
  Bool InitValues() override;
  Bool Command(Int32, BaseContainer const&) override;
  void SetPreviewDialog(DescPreviewDialog* dlg) { _dlgPreview = dlg; }
private:
  BaseList2D* _descNode;
  DescPreviewDialog* _dlgPreview;
};

// ===========================================================================
// ===========================================================================
class DescEditCommand : public CommandData
{
  typedef CommandData super;
public:
  Bool Execute(BaseDocument*) override;
  Bool RestoreLayout(void* secret) override;
private:
  DescPreviewDialog _dlgPreview;
  DescEditDialog _dlgEdit;
};

// ===========================================================================
// This NodeData plugin evaluates the description elements which is then
// displayed in the DescPreviewDialog .
// ===========================================================================
class DescNode : public NodeData
{
  typedef NodeData super;
public:
  Bool GetDDescription(GeListNode*, Description*, DESCFLAGS_DESC&) override;
  static NodeData* Alloc() { return NewObjClear(DescNode); }
};

// ===========================================================================
// ===========================================================================
DescPreviewDialog::DescPreviewDialog() : _descGui(nullptr)
{
}

// ===========================================================================
// ===========================================================================
Bool DescPreviewDialog::CreateLayout()
{
  SetTitle("DescEdit - Preview");
  GroupBegin(0, BFH_SCALEFIT | BFV_SCALEFIT, 1, 0, "", 0); {
    BaseContainer bc;
    bc.SetBool(DESCRIPTION_ALLOWFOLDING, true);
    bc.SetBool(DESCRIPTION_SHOWTITLE, true);
    bc.SetBool(DESCRIPTION_NOUNDO, true);
    bc.SetBool(DESCRIPTION_OBJECTSNOTINDOC, true);
    bc.SetBool(DESCRIPTION_MODALMODE, false);
    bc.SetBool(DESCRIPTION_LEFTMATEDITREGION, false);
    bc.SetVoid(DESCRIPTION_STORAGE, nullptr);
    bc.SetVoid(DESCRIPTION_FOLDERSTORE, nullptr);
    bc.SetBool(DESCRIPTION_SCALE_ALL_ELEMENTS, false);
    bc.SetBool(DESCRIPTION_NO_SHOW_SUBCHANNELS, false);
    bc.SetBool(DESCRIPTION_OPEN_ALL_GROUPS, false);
    _descGui = static_cast<DescriptionCustomGui*>(AddCustomGui(
      IDC_DESC, CUSTOMGUI_DESCRIPTION, "", BFH_SCALEFIT | BFV_SCALEFIT,
      300, 130, bc));
  } GroupEnd();
  return true;
}

// ===========================================================================
// ===========================================================================
Bool DescPreviewDialog::InitValues()
{
  if (!_descGui) {
    return false;
  }
  _descGui->SetMaster(this, nullptr);
  _descGui->SetObject(nullptr);
  return true;
}

// ===========================================================================
// ===========================================================================
DescEditDialog::DescEditDialog() : _descNode(nullptr), _dlgPreview(nullptr)
{
  // xxx: Allocate custom node plugin here.
  _descNode = BaseList2D::Alloc(PLUGIN_ID_NODE);
}

// ===========================================================================
// ===========================================================================
Bool DescEditDialog::CreateLayout()
{
  SetTitle("DescEdit");
  return true;
}

// ===========================================================================
// ===========================================================================
Bool DescEditDialog::InitValues()
{
  if (!_descNode || !_dlgPreview) {
    GePrint("[DescEdit]: Error: (!_descNode || !_dlgPreview)");
    return false;
  }
  auto gui = _dlgPreview->GetDescriptionGui();
  gui->SetObject(_descNode);
  return true;
}

// ===========================================================================
// ===========================================================================
Bool DescEditDialog::Command(Int32 param, BaseContainer const& bc)
{
  return true;
}

// ===========================================================================
// ===========================================================================
Bool DescEditCommand::Execute(BaseDocument* doc)
{
  _dlgEdit.SetPreviewDialog(&_dlgPreview);
  Bool res = true;
  res &= _dlgPreview.Open(DLG_TYPE_ASYNC, PLUGIN_ID, 600, 200, 400, 330, 1);
  res &= _dlgEdit.Open(DLG_TYPE_ASYNC, PLUGIN_ID, 200, 200, 300, 120, 2);
  return res;
}

// ===========================================================================
// ===========================================================================
Bool DescEditCommand::RestoreLayout(void* secret)
{
  auto data = reinterpret_cast<RestoreLayoutSecret*>(secret);
  if (!data) {
    return false;
  }
  if (data->subid == 1) {
    _dlgEdit.SetPreviewDialog(&_dlgPreview);
    return _dlgEdit.RestoreLayout(PLUGIN_ID, 1, secret);
  }
  if (data->subid == 2) {
    return _dlgPreview.RestoreLayout(PLUGIN_ID, 2, secret);
  }
  return false;
}

// ===========================================================================
// ===========================================================================
Bool DescNode::GetDDescription(GeListNode*, Description* desc, DESCFLAGS_DESC& flags)
{
  flags |= DESCFLAGS_DESC_LOADED;
  return true;
}

// ===========================================================================
// ===========================================================================
Bool RegisterDescEdit()
{
  {
    Int32 const info = PLUGINFLAG_HIDE;
    Int32 const disklevel = 0;
    BaseBitmap* const icon = nullptr;
    if (!RegisterNodePlugin(PLUGIN_ID_NODE, "DescEdit_Node", info,
        DescNode::Alloc, icon, disklevel, nullptr)) {
      return false;
    }
  }
  {
    Int32 const info = 0;
    BaseBitmap* const icon = nullptr;
    String const help = "Description Resource Editor";
    if (!RegisterCommandPlugin(PLUGIN_ID, "DescEdit", info, icon,
        help, NewObjClear(DescEditCommand))) {
      return false;
    }
  }
  return true;
}
