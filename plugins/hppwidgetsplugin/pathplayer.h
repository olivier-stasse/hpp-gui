#ifndef PATHPLAYER_H
#define PATHPLAYER_H

#include <QWidget>
#include <QSpinBox>
#include <QSlider>
#include <QPushButton>

#include <hpp/gui/fwd.h>

#include <hppwidgetsplugin.hh>

namespace Ui {
  class PathPlayerWidget;
}

class PathPlayer : public QWidget
{
  Q_OBJECT

public:
  PathPlayer (HppWidgetsPlugin* plugin, QWidget* parent = 0);
  ~PathPlayer();

private slots:
  void update ();

  void pathIndexChanged (int i);
  void pathSliderChanged (int value);
//  void timeChanged (double d);
  void playPauseToggled (bool toggled);
  void stopClicked ();
  void recordToggled (bool toggled);
  void pathPulse ();
  void timerEvent(QTimerEvent *event);

private:
  void updateConfiguration ();
  double sliderToLength (int v) const;
  int lengthToSlider (double l) const;
  double timeToLength (double time) const;
  int timeBetweenRefresh() const;
  double lengthBetweenRefresh () const;

  Ui::PathPlayerWidget* ui_;

  QDoubleSpinBox* timeSpinBox () const;
  QSpinBox* pathIndex() const;
  QSlider* pathSlider () const;
  QPushButton* playPause () const;
  QPushButton* stop () const;
  QPushButton* record () const;

  const int frameRate_;
  double pathLength_;
  double currentParam_;
  int timerId_;

  HppWidgetsPlugin* plugin_;
};

#endif // PATHPLAYER_H
