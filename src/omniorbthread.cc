#include "hpp/gui/omniorb/omniorbthread.h"

#include <gepetto/viewer/corba/server.hh>
#include <QDebug>

#include <hpp/gui/mainwindow.h>
#include <hpp/gui/plugin-interface.h>

int WorkItem::idGlobal = 0;

ServerProcess::ServerProcess()
  : initDone_ ()
{
  initDone_.lock();
}

void ServerProcess::waitForInitDone()
{
  initDone_.lock();
  initDone_.unlock();
}

void ServerProcess::init()
{
  initDone_.unlock();
}

ViewerServerProcess::ViewerServerProcess (graphics::corbaServer::Server *server)
  : server_ (server)
{}

ViewerServerProcess::~ViewerServerProcess()
{
  delete server_;
}

void ViewerServerProcess::init()
{
  server_->startCorbaServer ();
  emit done ();
  ServerProcess::init();
}

void ViewerServerProcess::processRequest(bool loop)
{
  server_->processRequest (loop);
  emit done();
}

CorbaServer::CorbaServer (ServerProcess* process) :
  QObject (), control_ (process), worker_ (), timerId_ (-1), interval_ (100)
{
  connect (this, SIGNAL (process(bool)), control_, SLOT (processRequest (bool)));
  connect (control_, SIGNAL (done()), this, SLOT (processed()));
  connect (&worker_, SIGNAL (started()), control_, SLOT (init()));
  control_->moveToThread(&worker_);
}

CorbaServer::~CorbaServer()
{
  if (control_)
    delete control_;
}

void CorbaServer::wait ()
{
  worker_.quit ();
  worker_.wait(200);
  if (worker_.isRunning()) {
      worker_.terminate();
      worker_.wait();
  }
}

void CorbaServer::waitForInitDone()
{
  control_->waitForInitDone();
}

void CorbaServer::start()
{
  worker_.start();
}

void CorbaServer::timerEvent(QTimerEvent* event)
{
  assert (event->timerId () == timerId_);
  emit process (false);
  killTimer(timerId_);
}

void CorbaServer::processed()
{
  timerId_ = startTimer(interval_);
}

BackgroundQueue::BackgroundQueue(QObject *parent) :
  QObject (parent)
{
}

void BackgroundQueue::perform(WorkItem *item)
{
  try {
    item->performWork();
    emit done (item->id ());
  } catch (std::exception& e) {
    emit failed (item->id(), QString (e.what()));
  } catch (const CORBA::Exception& e) {
    /// Enable plugins to get corba errors.
    /// hpp plugins can thus handle CORBA Exceptions (hpp::Error)
    bool handled = false;
    foreach (CorbaErrorInterface* errorHandler, MainWindow::instance()->pluginManager()->get <CorbaErrorInterface>()) {
        if (errorHandler->corbaException (item->id(), e)) {
            handled = true;
            break;
          }
      }
    if (!handled)
      emit failed (item->id(), QString ("CORBA Exception %1").arg(e._name()));
  } catch (...) {
    emit failed (item->id(), QString ("Unkown error type"));
  }
  delete item;
}
