#ifndef EventAction_h
#define EventAction_h 1

#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4THitsMap.hh"
#include "G4UserEventAction.hh"

// At the end of each event, sums the scorer maps of the three sensitive
// layers and fills one ntuple row.
class EventAction : public G4UserEventAction {
 public:
  void EndOfEventAction(const G4Event* event) override {
    // Collection IDs are looked up lazily (they exist only after SDs are built)
    if (fIDs[0] < 0) {
      auto* sdm = G4SDManager::GetSDMpointer();
      fIDs[0] = sdm->GetCollectionID("SnSD/Edep");
      fIDs[1] = sdm->GetCollectionID("AuSD/Edep");
      fIDs[2] = sdm->GetCollectionID("SiSD/Edep");
    }

    auto* hce = event->GetHCofThisEvent();
    if (!hce) return;

    auto* am = G4AnalysisManager::Instance();
    for (int i = 0; i < 3; ++i) {
      G4double total = 0.;
      auto* map = static_cast<G4THitsMap<G4double>*>(hce->GetHC(fIDs[i]));
      if (map) {
        for (auto& kv : *map->GetMap()) total += *(kv.second);
      }
      am->FillNtupleDColumn(i, total);
    }
    am->AddNtupleRow();
  }

 private:
  G4int fIDs[3] = {-1, -1, -1};
};

#endif
