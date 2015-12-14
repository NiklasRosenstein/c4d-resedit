// Copyright (C) 2015 Niklas Rosenstein
// Licensed under the Apache License Version 2.0
//   http://www.apache.org/licenses/

#include "DescItem.h"
#include "TreeView.h"

// ===========================================================================
// ===========================================================================
DescItem::DescItem(DescID const& id, String name, TreeViewItem* item)
: _id(id), _treeItem(item ? item : NewObjClear(TreeViewItem))
{
  BaseContainer* data = GetData();
  if (data) {
    GetCustomDataTypeDefault(GetDType()).CopyTo(data, COPYFLAGS_0, nullptr);
    if (!name.Content()) {
      name = GetTypeName();
      name = name.SubStr(0, 1) + name.SubStr(1, name.GetLength() - 1).ToLower();
    }
    data->SetString(DESC_NAME, name);
    data->SetString(DESC_SHORT_NAME, name);

    // Link the DescItem with the TreeViewItem.
    data->SetVoid(DESCITEM_LINK, this);
  }
}

// ===========================================================================
// ===========================================================================
DescItem::~DescItem()
{
}

// ===========================================================================
// ===========================================================================
DescItem* DescItem::GetNext() const
{
  if (_treeItem) {
    return Get(_treeItem->GetNext());
  }
  return nullptr;
}

// ===========================================================================
// ===========================================================================
DescItem* DescItem::GetPred() const
{
  if (_treeItem) {
    return Get(_treeItem->GetPred());
  }
  return nullptr;
}

// ===========================================================================
// ===========================================================================
DescItem* DescItem::GetUp() const
{
  if (_treeItem) {
    return Get(_treeItem->GetUp());
  }
  return nullptr;
}

// ===========================================================================
// ===========================================================================
DescItem* DescItem::GetDown() const
{
  if (_treeItem) {
    return Get(_treeItem->GetDown());
  }
  return nullptr;
}

// ===========================================================================
// ===========================================================================
Bool DescItem::AddChild(DescItem* item)
{
  if (_treeItem && item && item->_treeItem) {
    _treeItem->AddItem(item->_treeItem);
  }
  return false;
}

// ===========================================================================
// ===========================================================================
Bool DescItem::GetDDescription(Description* desc, DescID const& parentId)
{
  BaseContainer* data = GetData();
  if (!data) {
    return false;
  }

  BaseContainer* param = nullptr;
  if (GetDType() == 0) {
    CriticalAssert(_id == DESCID_ROOT && IsRoot());
    param = desc->GetParameterI(_id, nullptr);
    CriticalAssert(param != nullptr);
    data->CopyTo(param, COPYFLAGS_0, nullptr);
  }
  else {
    GeData iddat;
    iddat.SetCustomDataType(CUSTOMDATATYPE_DESCID, parentId);
    data->SetData(DESC_PARENTGROUP, iddat);
    CriticalAssert(!IsRoot() && _id == GetUp()->_id);
    CriticalAssert(desc->SetParameter(_id, *data, parentId));
  }

  DescItem* child;
  for (child = GetDown(); child; child = child->GetNext()) {
    if (!child->GetDDescription(desc, _id)) {
      return false;
    }
  }
  return true;
}

// ===========================================================================
// ===========================================================================
String DescItem::GetTypeName() const
{
  CUSTOMDATATYPEPLUGIN* plug = FindCustomDataTypePlugin(GetDType());
  if (plug && plug->adr) {
    auto cls = reinterpret_cast<CustomDataTypeClass*>(plug->adr);
    return (cls->*plug->GetResourceSym)();
  }
  return "???";
}

// ===========================================================================
// ===========================================================================
Bool DescItem::IsRoot() const
{
  if (_treeItem)
    return _treeItem->GetUp() == nullptr;
  return true;
}

// ===========================================================================
// ===========================================================================
BaseContainer* DescItem::GetData() const
{
  if (_treeItem)
    return &_treeItem->m_Data;
  return nullptr;
}

// ===========================================================================
// ===========================================================================
/*static*/ DescItem* DescItem::Get(TreeViewItem* item)
{
  if (item) {
    return reinterpret_cast<DescItem*>(item->m_Data.GetVoid(DESCITEM_LINK));
  }
  return nullptr;
}
