#ifndef DIALOGLOADROBOT_H
#define DIALOGLOADROBOT_H

#include <QDialog>
#include <QComboBox>

namespace Ui {
  class DialogLoadRobot;
}

class DialogLoadRobot : public QDialog
{
  Q_OBJECT

public:
  struct RobotDefinition {
    QString name_, robotName_, urdfSuf_, srdfSuf_, mesh_, package_, packagePath_, modelName_, rootJointType_;
    RobotDefinition () : rootJointType_ ("Freeflyer") {}
    RobotDefinition (QString name,
                     QString robotName,
                     QString rootJointType,
                     QString modelName,
                     QString package,
                     QString packagePath,
                     QString urdfSuffix,
                     QString srdfSuffix,
                     QString meshDirectory) :
      name_(name), robotName_ (robotName), urdfSuf_(urdfSuffix), srdfSuf_(srdfSuffix), mesh_(meshDirectory),
      package_ (package), packagePath_(packagePath), modelName_ (modelName),
      rootJointType_ (rootJointType){}
  };

  explicit DialogLoadRobot(QWidget *parent = 0);
  ~DialogLoadRobot();

  static void addRobotDefinition (QString name,
                                  QString robotName,
                                  QString rootJointType,
                                  QString modelName,
                                  QString package,
                                  QString packagePath,
                                  QString urdfSuffix,
                                  QString srdfSuffix,
                                  QString meshDirectory);
  static QList <RobotDefinition> getRobotDefinitions ();

  RobotDefinition getSelectedRobotDescription () {
    return selected_;
  }

private slots:
  void accept();
  void meshSelect();
  void packagePathSelect();
  void robotSelect(int index);
  
private:
  Ui::DialogLoadRobot *ui_;
  QComboBox* defs_;
  RobotDefinition selected_;

  static QList <RobotDefinition> definitions;
  static QStringList rootJointTypes;
};

Q_DECLARE_METATYPE (DialogLoadRobot::RobotDefinition)

#endif // DIALOGLOADROBOT_H
