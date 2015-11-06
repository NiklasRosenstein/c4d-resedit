/*********************************************************************\
  File name        : IconButton.cpp
  Description      : Implementation of IconButton.h
  Created at       : 05.11.2015, @ 22:06
  Created by       : Niklas Rosenstein
\*********************************************************************/

#include "IconButton.h"

void IconButton::DrawMsg(Int32 x, Int32 y, Int32 w, Int32 h, BaseContainer const& msg)
{
  DrawSetPen(COLOR_BG);
  DrawRectangle(x, y, x + w, y + h);
  if (!m_icon.bmp)
    return;

  Int32 iw, ih;
  GetMinSize(iw, ih);
  DrawBitmap(m_icon.bmp, 0, 0, iw, ih, m_icon.x, m_icon.y, m_icon.w, m_icon.h, BMP_ALLOWALPHA);

  if (m_border != NOTOK) {
    DrawBorder(m_border, x, y, w, h);
  }
}

Bool IconButton::GetMinSize(Int32& w, Int32& h)
{
  if (m_width != NOTOK) {
    w = m_width;
    if (m_height != NOTOK) {
      h = m_height;
    }
    else {
      h = Float(m_icon.w) / Float(m_icon.h) * m_width;
    }
  }
  else if (m_height != NOTOK) {
    h = m_height;
    if (m_width != NOTOK) {
      w = m_width;
    }
    else {
      w = Float(m_icon.h) / Float(m_icon.w) * m_height;
    }
  }
  else {
    w = m_icon.w;
    h = m_icon.h;
  }
  return true;
}

Bool IconButton::InputEvent(BaseContainer const& msg)
{
  Int32 const device = msg.GetInt32(BFM_INPUT_DEVICE);
  Int32 const channel = msg.GetInt32(BFM_INPUT_CHANNEL);
  if (device == BFM_INPUT_MOUSE && channel == BFM_INPUT_MOUSELEFT) {
    // XXX: Poll events until mouse is released and check if the button is still pressed.
    BaseContainer bc;
    bc.SetId(BFM_ACTION);
    bc.SetInt32(BFM_ACTION_ID, GetId());
    SendParentMessage(bc);
    return true;
  }
  return super::InputEvent(msg);
}

Bool IconButton::Message(BaseContainer const& msg, BaseContainer& res)
{
  if (msg.GetId() == BFM_GETCURSORINFO && m_tooltip.Content()) {
    res.SetId(msg.GetId());
    res.SetString(RESULT_BUBBLEHELP, m_tooltip);
    return true;
  }
  return super::Message(msg, res);
}
