#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4NistManager.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4VisAttributes.hh"

// Geometry (beam along +z):
//   Zirconium target (non-sensitive)  1 cm x 1 cm x 2 mm
//   Gap                                             2 mm
//   Tin layer                         1 cm x 1 cm x 1 mm   (sensitive)
//   Gold layer                        1 cm x 1 cm x 1 mm   (sensitive)
//   Silicon layer                     1 cm x 1 cm x 1 mm   (sensitive)
// All pieces are stacked directly against each other, Zr first.
class DetectorConstruction : public G4VUserDetectorConstruction {
 public:
  G4VPhysicalVolume* Construct() override {
    auto* nist = G4NistManager::Instance();
    auto* vacuum = nist->FindOrBuildMaterial("G4_Galactic");
    //auto* tar = nist->FindOrBuildMaterial("G4_Zr"); // Zr target
    auto* tar = nist->FindOrBuildMaterial("G4_H");    // H  target
    auto* sn = nist->FindOrBuildMaterial("G4_Sn");
    auto* au = nist->FindOrBuildMaterial("G4_Au");
    auto* si = nist->FindOrBuildMaterial("G4_Si");

    //const G4double xy = 1. * cm;

    const G4double tarXY    = 700. * um;
    const G4double tarThick = 100. * um;
    G4double z_gap          = 500. * um;

    //Dimensions are from an email exchange with JN (NIST) ~June 2025
    const G4double snXY    = 1.30 * mm;
    const G4double snThick = 500. * um;

    const G4double auXY    = 500. * um; // The pad itself is smaller than the absorber
    const G4double auThick = 500. * nm; 

    //Could consider making the cut outs for the legs... 
    const G4double siXY    = 2.00 * mm;
    const G4double siThick = 380. * um;
    
    
  
    // World
    auto* worldS = new G4Box("World", 1. * cm, 1. * cm, 2. * cm);
    auto* worldLV = new G4LogicalVolume(worldS, vacuum, "WorldLV");
    auto* worldPV =
      new G4PVPlacement(nullptr, {}, worldLV, "World", nullptr, false, 0, true);
    worldLV->SetVisAttributes(G4VisAttributes::GetInvisible());


    // Funcitonal block maker
    auto makeBlock = [&](const char* name, G4Material* mat, G4double xy, G4double thick,
                         G4double zCentre, const G4Colour& col) {
      auto* s = new G4Box(name, xy / 2, xy / 2, thick / 2);
      auto* lv = new G4LogicalVolume(s, mat, std::string(name) + "LV");
      new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zCentre), lv, name,
                        worldLV, false, 0, true);
      lv->SetVisAttributes(new G4VisAttributes(col));
    };

    // Tar occupies z in [-2.5, +2.5] mm
    G4double z = 0.;
    makeBlock("Tar", tar, tarXY, tarThick, z, G4Colour(0.6, 0.6, 0.7));
    z += tarThick / 2 + z_gap + snThick / 2;    
    makeBlock("Sn", sn, snXY, snThick, z, G4Colour(0.8, 0.8, 0.8));
    z += snThick / 2 + auThick / 2;
    makeBlock("Au", au, auXY, auThick, z, G4Colour(1.0, 0.84, 0.0));
    z += auThick / 2 + siThick / 2;
    makeBlock("Si", si, siXY, siThick, z, G4Colour(0.2, 0.4, 0.9));

    return worldPV;
  }

  void ConstructSDandField() override {
    // One multi-functional detector (with an energy-deposit scorer) per layer,
    // as in example B4d. Tar is deliberately left without an SD.
    for (const char* layer : {"Sn", "Au", "Si"}) {
      auto* mfd = new G4MultiFunctionalDetector(std::string(layer) + "SD");
      mfd->RegisterPrimitive(new G4PSEnergyDeposit("Edep"));
      G4SDManager::GetSDMpointer()->AddNewDetector(mfd);
      SetSensitiveDetector(std::string(layer) + "LV", mfd);
    }
  }
};

#endif
