#pragma once
#include <fstream>
#include <list>
#include <memory>
#include <string>

// Created by Maxime Landry (maxime334).

//

class Observer;
class Subject;
class ILoggable;
class LogObserver;
// Forward declarations.

void test_LoggingObserver();

/*
    Must be inherited by all classes that can be subject of logging mechanism.
*/
class ILoggable {
public:
  ILoggable() = default;
  ~ILoggable() = default;

  /*
    Creates and returns a string to be
    output to the log file.
  */
  virtual std::string stringToLog() const = 0;
};

/*
  Interface.
  Providing implementation of the notification mechanism.
*/
class Subject {
public:
  // List of shared_ptrs.
  std::list<Observer *> *m_list;

public:
  Subject();
  /*
    To be used w/ consideration. Copies the ptr to the list, and as such both
    subjects now have the same list.
    Why--> If the list is copied, there is no way to tell the subject when
    another subject has removed an observer, leading to errors.
  */
  Subject(const Subject &);
  ~Subject();

  /*
    Adds the Observer's ptr to the Subject's list.
    Constant time operation.
  */
  virtual void Attach(Observer *) noexcept;

  /*
    Removes Observer's ptr from the Subject's list.
    O(n) operation.
  */
  virtual void Detach(Observer *) noexcept;

  /*
      Triggers writing of an entry in the log file.
      All observers of this subject will stringToLog() inside the log file.
  */
  virtual void Notify(ILoggable *) const noexcept;
};

/*
  Interface.
  Observer can only be attached to one Observable at most at a time.
*/
class Observer {
protected:
  Subject *m_subject;

public:
  Observer();
  /*
    Creates an Observer and attaches it to the Subject.
  */
  explicit Observer(Subject &);
  /*
    Virtual keyword because we're manipulating the derived class
    polymorphically.
  */
  virtual ~Observer();

  virtual void Update(ILoggable &) const = 0;

  /*
    Removes the Observable and sets up m_subject as nullptr.
    The subject's allocated memory is the responsiblity of the user.
  */
  void clear() noexcept;
};

class LogObserver : public Observer {
public:
  explicit LogObserver();
  /*
    Creates a LogObserver and attaches it to the Subject's m_list.
  */
  LogObserver(Subject &s);
  /*
    Observer is removed from Subject's m_list.
  */
  ~LogObserver() = default;
  void Update(ILoggable &) const override;
};
