#pragma once

typedef void(__thiscall* PaintTraverse_t)(IPanel *_this, unsigned int VGUIPanel, bool forceRepaint, bool allowForce);

extern PaintTraverse_t oPaintTraverse;

void __fastcall PaintTraverse(IPanel* _this, void *edx, unsigned int VGUIPanel, bool forceRepaint, bool allowForce);

