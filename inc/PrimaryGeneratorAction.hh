#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

// 100 keV antiproton pencil beam along +z, starting 1 cm upstream of the
// Zr target centre (the world is vacuum, so nothing happens on the way).
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
 public:
  PrimaryGeneratorAction() {
    fGun = new G4ParticleGun(1);
    fGun->SetParticleDefinition(
      G4ParticleTable::GetParticleTable()->FindParticle("anti_proton"));
    fGun->SetParticleEnergy(100. * eV);
    fGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    fGun->SetParticlePosition(G4ThreeVector(0., 0., -1. * cm));
  }
  ~PrimaryGeneratorAction() override { delete fGun; }

  void GeneratePrimaries(G4Event* event) override {
    // Pencil beam on axis; smear x/y here if you want a finite spot size.
    fGun->GeneratePrimaryVertex(event);
  }

 private:
  G4ParticleGun* fGun = nullptr;
};

#endif
