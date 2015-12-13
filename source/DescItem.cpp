// Copyright (C) 2015 Niklas Rosenstein
// Licensed under the Apache License Version 2.0
//   http://www.apache.org/licenses/

#include "DescItem.h"
#include "TreeView.h"
#include "globals.h"  // TREEVIEW_DATA

// ===========================================================================
// ===========================================================================
DescItem::DescItem(Int32 dtype, String name)
: _dtype(dtype), _treeItem(NewObjClear(TreeViewItem))
{
  BaseContainer* data = GetData();
  if (data) {
    GetCustomDataTypeDefault(_dtype).CopyTo(data, COPYFLAGS_0, nullptr);
    if (!name.Content()) {
      name = GetTypeName();
    }
    data->SetString(DESC_NAME, name);
    data->SetString(DESC_SHORT_NAME, name);

    // Link the DescItem with the TreeViewItem.
    data->SetVoid(TREEVIEW_DATA, this);
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
Bool DescItem::GetDDescription(Description* desc, DescID const& parentId)
{
  BaseContainer* data = GetData();
  if (!data) {
    return false;
  }

  if (GetType() == DTYPE_GROUP) {
    if (IsRoot()) {
      BaseContainer* param = desc->GetParameterI(parentId, nullptr);
      CriticalAssert(param != nullptr);
      param->SetString(DESC_NAME, data->GetString(DESC_NAME));
      param->SetString(DESC_SHORT_NAME, data->GetString(DESC_SHORT_NAME));
    }
  }
  return true;
}

// ===========================================================================
// ===========================================================================
String DescItem::GetTypeName() const
{
  CUSTOMDATATYPEPLUGIN* plug = FindCustomDataTypePlugin(_dtype);
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
    return reinterpret_cast<DescItem*>(item->m_Data.GetVoid(TREEVIEW_DATA));
  }
  return nullptr;
}
