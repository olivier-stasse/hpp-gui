#ifndef CONFIGURATIONLISTWIDGET_H
#define CONFIGURATIONLISTWIDGET_H

#include <QWidget>

#include "hpp/gui/fwd.h"
#include "hpp/gui/mainwindow.h"

#include "hpp/corbaserver/common.hh"

#include <hppwidgetsplugin.hh>

Q_DECLARE_METATYPE (hpp::floatSeq*)

namespace Ui {
  class ConfigurationListWidget;
}

class ConfigurationListWidget : public QWidget
{
  Q_OBJECT

public:
  static const int ConfigRole;

  ConfigurationListWidget(HppWidgetsPlugin* plugin, QWidget* parent = 0);

  virtual ~ConfigurationListWidget();

public slots:
  void onSaveClicked ();
  void updateCurrentConfig (QListWidgetItem* item);
  void showListContextMenu (const QPoint& pos);

private:
  inline QListWidget* list ();
  inline QLineEdit* name ();

  HppWidgetsPlugin* plugin_;
  Ui::ConfigurationListWidget* ui_;

  MainWindow* main_;
  QString basename_;
  int count_;
};

#endif // CONFIGURATIONLISTWIDGET_H
