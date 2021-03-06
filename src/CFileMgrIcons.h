#ifndef CFILE_MGR_ICONS_H
#define CFILE_MGR_ICONS_H

#include <CFile.h>
#include <CISize2D.h>
#include <CEvent.h>

class CFileMgr;
class CPixelRenderer;
class CFileMgrFile;

class CFileMgrIcons {
 private:
  CFileMgr       *file_mgr_;
  CPixelRenderer *renderer_;
  CISize2D        size_;

 public:
  CFileMgrIcons(CFileMgr *file_mgr);

 ~CFileMgrIcons();

  CFileMgr *getFileMgr() const { return file_mgr_; }

  void setRenderer(CPixelRenderer *renderer);

  CPixelRenderer *getRenderer() const { return renderer_; }

  int getWidth () const { return size_.width; }
  int getHeight() const { return size_.height; }

  void redraw();
  void draw();

  void keyPress  (const CKeyEvent &event);
  void keyRelease(const CKeyEvent &event);

 private:
  void stepPosition(int *x, int *y);
};

#endif
