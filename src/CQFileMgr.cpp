#include <CQFileMgr.h>
#include <CQDirBar.h>
#include <CQDirPlaces.h>

#include <CQFileMgrIcons.h>
#include <CQFileMgrFilmstripIcons.h>
#include <CQFileMgrFilmstripImage.h>
#include <CQFileMgrDetails.h>
#include <CQFileMgrToolbar.h>
#include <CQFileMgrInfo.h>

#include <CQUtil.h>
#include <CQDirView.h>
#include <CFileMgrLib.h>
#include <CDir.h>
#include <COSFile.h>

#include <QAction>
#include <QMenu>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QLabel>

class CFileMgrImpl : public CFileMgr {
 private:
  CQFileMgr *filemgr_;

 public:
  CFileMgrImpl(CQFileMgr *filemgr) :
   CFileMgr(), filemgr_(filemgr) {
  }

  virtual void redraw() {
    filemgr_->changed();
  }

  virtual void changeDir(const std::string &dirname) {
    filemgr_->changeDir(dirname);
  }

  virtual void setTableColumns(const CFileMgrDetails::ColumnList &columns) {
    filemgr_->setTableColumns(columns);
  }

  virtual void deleteAllTableRows() {
    filemgr_->deleteAllTableRows();
  }

  virtual void addTableRow(CFileMgrFile *file, const std::vector<std::string> &values) {
    filemgr_->addTableRow(file, values);
  }

  virtual void activateFileType(const std::string &filename, CFileType type) {
    if (! filemgr_->activateFileTypeFwd(filename, type))
      CFileMgr::activateFileType(filename, type);
  }
};

CQFileMgr::
CQFileMgr(QWidget *parent) :
 QWidget                (parent),
 filemgr_               (0),
 icons_canvas_          (0),
 filmstrip_image_canvas_(0),
 filmstrip_icons_canvas_(0),
 details_table_         (0)
{
  setObjectName("fileMgr");

  filemgr_ = new CFileMgrImpl(this);

  filemgr_->setForceRGB();

  //------

  QVBoxLayout *layout = new QVBoxLayout(this);

  layout->setMargin(0); layout->setSpacing(0);

  //------

  toolbar_ = new CQFileMgrToolbar(this);

  layout->addWidget(toolbar_);

  //------

  dirbar_ = new CQDirBar;

  layout->addWidget(dirbar_);

  connect(CQDirViewInst, SIGNAL(dirChangedSignal(const QString &)),
          this, SLOT(setDir(const QString &)));

  //------

  QSplitter *splitter = new QSplitter;
  splitter->setObjectName("splitter");

  layout->addWidget(splitter);

  //------

  places_frame_ = new QWidget;
  places_frame_->setObjectName("places");

  QVBoxLayout *places_layout = new QVBoxLayout(places_frame_);

  places_layout->setMargin(2); places_layout->setSpacing(2);

  QLabel *label = new QLabel("Places");

  places_layout->addWidget(label);

  places_ = new CQDirPlaces(this);

  places_layout->addWidget(places_);

  splitter->addWidget(places_frame_);

  //------

  QWidget *browser = new QWidget;
  browser->setObjectName("browser");

  browser->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  QVBoxLayout *vlayout = new QVBoxLayout(browser);

  vlayout->setMargin(0); vlayout->setSpacing(0);

  //------

  // Icons Frame
  icons_frame_ = new QWidget(this);

  icons_frame_->setObjectName("icons_frame");

  QVBoxLayout *vlayout1 = new QVBoxLayout(icons_frame_);

  vlayout1->setMargin(0);
  vlayout1->setSpacing(0);

  icons_canvas_ = new CQFileMgrIcons(icons_frame_, this);

  vlayout1->addWidget(icons_canvas_);

  //------

  // Filmstrip Frame
  filmstrip_frame_ = new QWidget(this);

  filmstrip_frame_->setObjectName("filmstrip_frame");

  QVBoxLayout *vlayout2 = new QVBoxLayout(filmstrip_frame_);

  vlayout2->setMargin(0);
  vlayout2->setSpacing(0);

  filmstrip_image_canvas_ = new CQFileMgrFilmstripImage(filmstrip_frame_, this);
  filmstrip_icons_canvas_ = new CQFileMgrFilmstripIcons(filmstrip_frame_, this);

  filmstrip_icons_canvas_->setMinimumHeight(128);
  filmstrip_icons_canvas_->setMaximumHeight(128);

  vlayout2->addWidget(filmstrip_image_canvas_);
  vlayout2->addWidget(filmstrip_icons_canvas_);

  filmstrip_frame_->hide();

  //------

  // Details Frame

  details_frame_ = new QWidget(this);

  details_frame_->setObjectName("details_frame");

  QVBoxLayout *vlayout3 = new QVBoxLayout(details_frame_);

  vlayout3->setMargin(0);
  vlayout3->setSpacing(0);

  details_table_ = new CQFileMgrDetails(details_frame_, this);

  vlayout3->addWidget(details_table_);

  details_frame_->hide();

  //------

  vlayout->addWidget(icons_frame_);
  vlayout->addWidget(filmstrip_frame_);
  vlayout->addWidget(details_frame_);

  //------

  splitter->addWidget(browser);

  QFontMetrics fm(font());

  int pw = fm.width("/home/user") + 32;

  places_frame_->setFixedWidth(pw);

  //------

  filemgr_->setDirName(CDir::getCurrent());

  changed();
}

CQFileMgr::
~CQFileMgr()
{
  delete filemgr_;
  delete icons_canvas_;
  delete filmstrip_image_canvas_;
  delete filmstrip_icons_canvas_;
}

const std::string &
CQFileMgr::
getDirName() const
{
  return filemgr_->getDirName();
}

void
CQFileMgr::
setDirName(const std::string &dirName)
{
  filemgr_->setDirName(dirName);
}

void
CQFileMgr::
showPlaces(bool show)
{
  places_frame_->setVisible(show);

  toolbar_->showIcon(CQFileMgrToolbar::ADD_PLACE_BUTTON, show);
}

void
CQFileMgr::
resize()
{
}

void
CQFileMgr::
changed()
{
  if      (filemgr_->getViewType() == CFileMgr::VIEW_ICONS) {
    if (icons_canvas_ != 0)
      icons_canvas_->setContentsChanged();
  }
  else if (filemgr_->getViewType() == CFileMgr::VIEW_FILMSTRIP) {
    if (filmstrip_image_canvas_ != 0)
      filmstrip_image_canvas_->setContentsChanged();

    if (filmstrip_icons_canvas_ != 0)
      filmstrip_icons_canvas_->setContentsChanged();
  }

  expose();
}

void
CQFileMgr::
expose()
{
  if      (filemgr_->getViewType() == CFileMgr::VIEW_ICONS) {
    if (icons_canvas_ != 0)
      icons_canvas_->update();
  }
  else if (filemgr_->getViewType() == CFileMgr::VIEW_FILMSTRIP) {
    if (filmstrip_image_canvas_ != 0)
      filmstrip_image_canvas_->update();

    if (filmstrip_icons_canvas_ != 0)
      filmstrip_icons_canvas_->update();
  }
}

void
CQFileMgr::
keyPress(const CKeyEvent &event)
{
  filemgr_->keyPress(event);
}

void
CQFileMgr::
keyRelease(const CKeyEvent &event)
{
  filemgr_->keyRelease(event);
}

void
CQFileMgr::
draw()
{
  filemgr_->draw();
}

void
CQFileMgr::
drawIcons()
{
  filemgr_->drawIcons();
}

void
CQFileMgr::
drawFilmstripImage()
{
  filemgr_->drawFilmstripImage();
}

void
CQFileMgr::
drawFilmstripIcons()
{
  filemgr_->drawFilmstripIcons();
}

int
CQFileMgr::
getCanvasWidth() const
{
  if      (filemgr_->getViewType() == CFileMgr::VIEW_ICONS)
    return icons_canvas_->width();
  else if (filemgr_->getViewType() == CFileMgr::VIEW_FILMSTRIP)
    return filmstrip_icons_canvas_->width();
  else
    return details_table_->width();
}

int
CQFileMgr::
getCanvasHeight() const
{
  if      (filemgr_->getViewType() == CFileMgr::VIEW_ICONS)
    return icons_canvas_->height();
  else if (filemgr_->getViewType() == CFileMgr::VIEW_FILMSTRIP)
    return filmstrip_icons_canvas_->height();
  else
    return details_table_->height();
}

int
CQFileMgr::
getContentsWidth() const
{
  if (filemgr_->getViewType() == CFileMgr::VIEW_FILMSTRIP) {
    CFileMgrFilmstrip *filmstrip = filemgr_->getFilmstrip();

    if (filmstrip != 0)
      return filmstrip->getWidth();
  }

  return getCanvasWidth();
}

int
CQFileMgr::
getContentsHeight() const
{
  if (filemgr_->getViewType() == CFileMgr::VIEW_ICONS) {
    CFileMgrIcons *icons = filemgr_->getIcons();

    if (icons != 0)
      return icons->getHeight();
  }

  return getCanvasHeight();
}

void
CQFileMgr::
setIconsView()
{
  icons_frame_    ->show();
  filmstrip_frame_->hide();
  details_frame_  ->hide();

  filemgr_->setIconsView();

  expose();
}

void
CQFileMgr::
setFilmstripView()
{
  icons_frame_    ->hide();
  filmstrip_frame_->show();
  details_frame_  ->hide();

  filemgr_->setFilmstripView();

  expose();
}

void
CQFileMgr::
setDetailsView()
{
  icons_frame_    ->hide();
  filmstrip_frame_->hide();
  details_frame_  ->show();

  filemgr_->setDetailsView();

  expose();
}

bool
CQFileMgr::
getShowImages()
{
  return filemgr_->getShowImages();
}

bool
CQFileMgr::
getShowHidden()
{
  return filemgr_->getShowHidden();
}

void
CQFileMgr::
setShowImages(bool show_images)
{
  filemgr_->setShowImages(show_images);

  changed();
}

void
CQFileMgr::
setSmallIcons(bool small_icons)
{
  filemgr_->setSmallIcons(small_icons);

  changed();
}

void
CQFileMgr::
setShowHidden(bool show_hidden)
{
  filemgr_->setShowHidden(show_hidden);

  changed();
}

void
CQFileMgr::
deselectAll()
{
  filemgr_->deselectAll();
}

void
CQFileMgr::
selectInside(const CIBBox2D &bbox)
{
  if (filemgr_->getViewType() == CFileMgr::VIEW_ICONS ||
      filemgr_->getViewType() == CFileMgr::VIEW_FILMSTRIP) {
    CFileMgrDir::file_iterator p1 = filemgr_->fileBegin();
    CFileMgrDir::file_iterator p2 = filemgr_->fileEnd  ();

    deselectAll();

    for ( ; p1 != p2; ++p1) {
      if ((*p1)->inside(bbox))
        (*p1)->setSelected(true);
    }
  }

  changed();
}

void
CQFileMgr::
activate(const CIPoint2D &position)
{
  if (filemgr_->getViewType() == CFileMgr::VIEW_ICONS ||
      filemgr_->getViewType() == CFileMgr::VIEW_FILMSTRIP) {
    deselectAll();

    CFileMgrDir::file_iterator p1 = filemgr_->fileBegin();
    CFileMgrDir::file_iterator p2 = filemgr_->fileEnd  ();

    for ( ; p1 != p2; ++p1) {
      if (! (*p1)->inside(position))
        continue;

      activate(*p1);

      break;
    }
  }
}

void
CQFileMgr::
activate(CFileMgrFile *file)
{
  if (file->isDirectory())
    activateDir(file->getPathName());
  else
    activateFile(file->getFileName());

  changed();
}

void
CQFileMgr::
activateFile(const std::string &filename)
{
  filemgr_->activateFile(filename);

  emit fileActivated(filename.c_str());
}

void
CQFileMgr::
activateDir(const std::string &dirname)
{
  filemgr_->activateDir(dirname);

  emit dirActivated(dirname.c_str());
}

bool
CQFileMgr::
activateFileTypeFwd(const std::string &filename, CFileType type)
{
  activated_ = true;

  activateFileType(filename, type);

  return activated_;
}

void
CQFileMgr::
activateFileType(const std::string &, CFileType)
{
  activated_ = false;
}

void
CQFileMgr::
changeDir(const std::string &dirname)
{
  CDirViewInst->setDirName(dirname.c_str());

  emit dirChanged(dirname.c_str());
}

void
CQFileMgr::
refresh()
{
  activateDir(COSFile::getCurrentDir());
}

//----------

void
CQFileMgr::
setTableColumns(const CFileMgrDetails::ColumnList &columns)
{
  CQFileMgrDetails *table = getDetailsTable();

  table->setColumns(columns);
}

void
CQFileMgr::
deleteAllTableRows()
{
  CQFileMgrDetails *table = getDetailsTable();

  table->clear();
}

void
CQFileMgr::
addTableRow(CFileMgrFile *file, const std::vector<std::string> &values)
{
  CQFileMgrDetails *table = getDetailsTable();

  table->addRow(file, values);
}

//----------

void
CQFileMgr::
popupMenu(QWidget *parent, int x, int y)
{
  static QAction *iconsAction;
  static QAction *filmstripAction;
  static QAction *detailsAction;
  static QAction *smallIconsAction;
  static QAction *showImagesAction;
  static QAction *showHiddenAction;
  static QAction *infoAction;

  if (! showImagesAction) {
    iconsAction      = new QAction("&Icons View"    , this);
    filmstripAction  = new QAction("&Filmstrip View", this);
    detailsAction    = new QAction("&Details View"  , this);
    smallIconsAction = new QAction("Small I&cons"   , this);
    showImagesAction = new QAction("Show &Images"   , this);
    showHiddenAction = new QAction("Show &Hidden"   , this);
    infoAction       = new QAction("Info"           , this);

    smallIconsAction->setCheckable(true);
    showImagesAction->setCheckable(true);
    showHiddenAction->setCheckable(true);

    connect(iconsAction     , SIGNAL(triggered())  , this, SLOT(iconsView()));
    connect(filmstripAction , SIGNAL(triggered())  , this, SLOT(filmstripView()));
    connect(detailsAction   , SIGNAL(triggered())  , this, SLOT(detailsView()));
    connect(showImagesAction, SIGNAL(toggled(bool)), this, SLOT(showImages(bool)));
    connect(smallIconsAction, SIGNAL(toggled(bool)), this, SLOT(smallIcons(bool)));
    connect(showHiddenAction, SIGNAL(toggled(bool)), this, SLOT(showHidden(bool)));
    connect(infoAction      , SIGNAL(triggered())  , this, SLOT(showInfo()));
  }

  QMenu menu(parent);

  if      (filemgr_->getViewType() == CFileMgr::VIEW_ICONS) {
    menu.addAction(filmstripAction);
    menu.addAction(detailsAction);

    menu.addAction(smallIconsAction);
    menu.addAction(showImagesAction);
    menu.addAction(showHiddenAction);
  }
  else if (filemgr_->getViewType() == CFileMgr::VIEW_FILMSTRIP) {
    menu.addAction(iconsAction);
    menu.addAction(detailsAction);

    menu.addAction(smallIconsAction);
    menu.addAction(showImagesAction);
    menu.addAction(showHiddenAction);
  }

  menu.addAction(infoAction);

  QPoint point(x, y);

  menu.exec(point);
}

void
CQFileMgr::
undoDir()
{
  filemgr_->undoDir();
}

void
CQFileMgr::
redoDir()
{
  filemgr_->redoDir();
}

void
CQFileMgr::
parentDir()
{
  CDir dir(getDirName());

  setDir(dir.getParentName().c_str());
}

void
CQFileMgr::
setDir(const QString &dirName)
{
  activateDir(dirName.toStdString());

  changed();
}

void
CQFileMgr::
iconsView()
{
  setIconsView();
}

void
CQFileMgr::
filmstripView()
{
  setFilmstripView();
}

void
CQFileMgr::
detailsView()
{
  setDetailsView();
}

void
CQFileMgr::
toggleImages()
{
  setShowImages(! getShowImages());
}

void
CQFileMgr::
toggleHidden()
{
  setShowHidden(! getShowHidden());
}

void
CQFileMgr::
addToPlaces()
{
  places_->addPlace(CDir::getCurrent(), CDir::getCurrent());
}

void
CQFileMgr::
showImages(bool flag)
{
  setShowImages(flag);
}

void
CQFileMgr::
smallIcons(bool flag)
{
  setSmallIcons(flag);
}

void
CQFileMgr::
showHidden(bool flag)
{
  setShowHidden(flag);
}

void
CQFileMgr::
showInfo()
{
  if (! info_)
    info_ = new CQFileMgrInfo(this);

  CFileMgrDir::FileList selected = filemgr_->getSelected();

  CFileMgrFile *file = 0;

  if (! selected.empty())
    file = *selected.begin();

  if (file)
    info_->init(file->getPathName());
  else
    info_->init(CDir::getCurrent());

  info_->show();
}
