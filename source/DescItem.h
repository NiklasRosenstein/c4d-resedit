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
  DESCITEM_CONTAINER = 1,
  DESCITEM_GROUP,
};

// ===========================================================================
// ===========================================================================
class DescItem
{
public:
  DescItem(Int32 dtype, String name="");
  virtual ~DescItem();

  DescItem* GetNext() const;
  DescItem* GetPred() const;
  DescItem* GetUp() const;
  DescItem* GetDown() const;

  Bool GetDDescription(Description* desc, DescID const& parentId);
  Int32 GetType() const { return _dtype; }
  String GetTypeName() const;
  Bool IsRoot() const;
  BaseContainer* GetData() const;
  TreeViewItem* GetTreeItem() const { return _treeItem; }

  static DescItem* Get(TreeViewItem* item);
private:
  Int32 _dtype;
  TreeViewItem* _treeItem;
};


#endif // DESCEDITITEM_H__
