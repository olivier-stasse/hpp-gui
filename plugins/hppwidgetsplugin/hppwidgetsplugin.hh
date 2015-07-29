#ifndef HPPWIDGETSPLUGIN_HH
#define HPPWIDGETSPLUGIN_HH

#include <hpp/gui/plugin-interface.h>
#include <hpp/corbaserver/client.hh>

class SolverWidget;
class PathPlayer;
class JointTreeWidget;
class ConfigurationListWidget;
class JointTreeItem;

class HppWidgetsPlugin : public QObject, public PluginInterface,
    public ModelInterface
{
  Q_OBJECT
  Q_INTERFACES (PluginInterface ModelInterface)

public:
  struct JointElement {
    std::string name;
    std::string bodyName;
    JointTreeItem* item;
    bool updateViewer;

    JointElement ()
      : name (), bodyName (), item (NULL), updateViewer (false) {}
    JointElement (std::string n, std::string bn, JointTreeItem* i, bool updateV = true)
      : name (n), bodyName (bn), item (i), updateViewer (updateV) {}
  };
  typedef QMap <std::string, JointElement> JointMap;
  typedef hpp::corbaServer::Client HppClient;

  explicit HppWidgetsPlugin ();

  virtual ~HppWidgetsPlugin ();

  // PluginInterface interface
public:
  void init();
  QString name() const;

  // ModelInterface interface
public:
  void loadRobotModel (DialogLoadRobot::RobotDefinition rd);
  void loadEnvironmentModel (DialogLoadEnvironment::EnvironmentDefinition ed);
  std::string getBodyFromJoint (const std::string& jointName) const;

signals:
  void configurationValidationStatus (bool valid);

public slots:
  void applyCurrentConfiguration ();
  void configurationValidation ();
  void selectJointFromBodyName (const std::string& bodyName);

  void corbaException (const CORBA::Exception& e);

public:
  HppClient* client () const;
  JointMap& jointMap ();

  void updateRobotJoints (const QString robotName);

private:
  void computeObjectPosition();

  PathPlayer* pathPlayer_;
  SolverWidget* solverWidget_;
  JointTreeWidget* jointTreeWidget_;
  ConfigurationListWidget* configListWidget_;
  QList <QDockWidget*> dockWidgets_;

  HppClient* hpp_;

  JointMap jointMap_;
};

#endif // HPPWIDGETSPLUGIN_HH