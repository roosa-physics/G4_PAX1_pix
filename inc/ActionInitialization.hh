#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "EventAction.hh"
#include "G4VUserActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"

class ActionInitialization : public G4VUserActionInitialization {
 public:
  void BuildForMaster() const override { SetUserAction(new RunAction); }

  void Build() const override {
    SetUserAction(new PrimaryGeneratorAction);
    SetUserAction(new RunAction);
    SetUserAction(new EventAction);
  }
};

#endif
