#include "./LoggingObserver.h"
#include <fstream>
#include <iostream> // To be removed.
#include <memory>   // To be removed.
#include <vector>   // To be removed.

// Created by Maxime Landry (maxime334).

// GameEngine stringToLog() -> Outputs state of the GameEngine.

//

/*
  Holds path to the log file. Can change this directly to change the log file's
  location.
  Weirdly depends on where the executable is located rather than the location of
  the class itself.

  If executable inside build directory: ../ is needed.
*/
const std::string path = "gamelog.txt";

// --- Subject ---

Subject::Subject() : m_list{new std::list<Observer *>()} {
  new LogObserver(*this);
  // Memory leak.
}

Subject::Subject(const Subject &subject) : m_list{subject.m_list} {
  // Ptr is copied. Both are pointing to the same list.
}

Subject::~Subject() {
  // Set up each observer's subject as nullptr.
  // Removes the subject relationship.
  for (auto &i : *m_list) {
    i->clear();
  }

  delete m_list->front();
  //  Subject on initialization always create a LogObserver, stored at the
  //  front. Should be removed before list destruction else memory leak.
  delete m_list;
}

void Subject::Attach(Observer *observer) noexcept {
  // Observer o's ptr is pushed by value inside the list.
  // o and the pushed value still point to the same element.
  m_list->push_back(observer);
}

void Subject::Detach(Observer *observer) noexcept {
  // Removes element satisfying specific value.
  // O(n) operation, as each ptr of the list is checked to see if equal to o.
  m_list->remove(observer);
}

void Subject::Notify(ILoggable *ilog) const noexcept {
  for (Observer *o : *m_list) {
    o->Update(*ilog);
  }
}

// --- Observer ---

Observer::Observer() : m_subject(nullptr) {}

Observer::Observer(Subject &s) {
  m_subject = &s;
  s.Attach(this);
}

Observer::~Observer() {
  if (m_subject != nullptr)
    m_subject->Detach(this);
}

void Observer::clear() noexcept { m_subject = nullptr; }

// --- LogObserver ---

LogObserver::LogObserver() : Observer() {}

LogObserver::LogObserver(Subject &s) : Observer(s) {}

void LogObserver::Update(ILoggable &ilog) const {
  std::ofstream ofs;
  ofs.open(path);
  ofs << ilog.stringToLog() << std::endl;
  ofs.close();
}