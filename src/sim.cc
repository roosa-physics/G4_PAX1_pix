#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"

#include "FTFP_BERT.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4RunManagerFactory.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"

int main(int argc, char** argv) {
  // Interactive session if no macro is given
  G4UIExecutive* ui = nullptr;
  if (argc == 1) ui = new G4UIExecutive(argc, argv);

  auto* runManager =
    G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  runManager->SetUserInitialization(new DetectorConstruction);

  // FTFP_BERT + EM option4 (better low-energy EM, relevant at 100 keV)
  auto* physicsList = new FTFP_BERT(0);
  physicsList->ReplacePhysics(new G4EmStandardPhysics_option4);
  runManager->SetUserInitialization(physicsList);

  runManager->SetUserInitialization(new ActionInitialization);

  auto* visManager = new G4VisExecutive;
  visManager->Initialize();

  auto* uiManager = G4UImanager::GetUIpointer();
  if (ui) {
    uiManager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  } else {
    uiManager->ApplyCommand(G4String("/control/execute ") + argv[1]);
  }

  delete visManager;
  delete runManager;
  return 0;
}
