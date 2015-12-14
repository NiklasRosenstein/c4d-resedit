// Copyright (C) 2015 Niklas Rosenstein
// Licensed under the Apache License Version 2.0
//   http://www.apache.org/licenses/

#ifndef DESCEDITITEM_H__
#define DESCEDITITEM_H__

#include <c4d.h>

class TreeViewItem;

// ===========================================================================
// ===========================================================================
enum
{
  // void* entry in parameter description container to the DescItem
  // Same as DescNode Plugin ID.
  DESCITEM_LINK = 1036514,
};

// ===========================================================================
// ===========================================================================
class DescItem
{
public:
  DescItem(DescID const& id, String name = "", TreeViewItem* item = nullptr);
  virtual ~DescItem();

  DescItem* GetNext() const;
  DescItem* GetPred() const;
  DescItem* GetUp() const;
  DescItem* GetDown() const;
  Bool AddChild(DescItem* node);

  DescID const& GetDescID() const { return _id; }
  Int32 GetID() const { return _id[_id.GetDepth() - 1].id; }
  Int32 GetDType() const { return _id[_id.GetDepth() - 1].dtype; }
  Bool GetDDescription(Description* desc, DescID const& parentId);

  String GetTypeName() const;
  Bool IsRoot() const;
  BaseContainer* GetData() const;
  TreeViewItem* GetTreeItem() const { return _treeItem; }

  static DescItem* Get(TreeViewItem* item);
private:
  DescID _id;
  TreeViewItem* _treeItem;
};


#endif // DESCEDITITEM_H__
