#ifndef RunAction_h
#define RunAction_h 1

#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4UserRunAction.hh"

// Creates the output ROOT file with one TTree ("Edep") holding the total
// energy deposited per event in each layer. Values are stored in MeV
// (Geant4's internal energy unit).
class RunAction : public G4UserRunAction {
 public:
  RunAction() {
    auto* am = G4AnalysisManager::Instance();
    am->SetVerboseLevel(1);
    am->SetDefaultFileType("root");
    am->SetNtupleMerging(true);  // merge worker-thread ntuples into one file

    am->CreateNtuple("Edep", "Energy deposited per layer [MeV]");
    am->CreateNtupleDColumn("E_Sn");  // column 0
    am->CreateNtupleDColumn("E_Au");  // column 1
    am->CreateNtupleDColumn("E_Si");  // column 2

    am->CreateNtupleIColumn("primary_pdg");   // column 3: PDG code (anti_proton = -2212)
    am->CreateNtupleSColumn("primary_name");  // column 4: Geant4 particle name
    am->CreateNtupleDColumn("primary_E");     // column 5: initial kinetic energy [MeV]

    am->FinishNtuple();
  }

  void BeginOfRunAction(const G4Run*) override {
    G4AnalysisManager::Instance()->OpenFile("calo.root");
  }

  void EndOfRunAction(const G4Run*) override {
    auto* am = G4AnalysisManager::Instance();
    am->Write();
    am->CloseFile();
  }
};

#endif
