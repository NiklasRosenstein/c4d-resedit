/*********************************************************************\
  File name        : IconButton.h
  Description      : A bitmap button user area.
  Created at       : 05.11.2015, @ 22:03
  Created by       : Niklas Rosenstein
\*********************************************************************/

#ifndef RESEDIT_ICONBUTTON_H_
#define RESEDIT_ICONBUTTON_H_

#include <c4d.h>

class IconButton : public GeUserArea
{
  typedef GeUserArea super;
public:
  IconButton() : m_icon(), m_border(NOTOK), m_width(NOTOK), m_height(NOTOK) { }
  IconButton(IconData const& icon, Int32 width=NOTOK, Int32 height=NOTOK)
    : m_icon(icon), m_border(BORDER_THIN_IN), m_width(width), m_height(height) { }

  virtual void DrawMsg(Int32 x, Int32 y, Int32 w, Int32 h, BaseContainer const& msg) override;
  virtual Bool GetMinSize(Int32& w, Int32& h) override;
  virtual Bool InputEvent(BaseContainer const& msg) override;

public:
  IconData m_icon;
  Int32 m_border;
  Int32 m_width;
  Int32 m_height;
};

#endif // RESEDIT_ICONBUTTON_H_
