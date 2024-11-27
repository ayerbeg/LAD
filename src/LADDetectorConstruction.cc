#include "LADDetectorConstruction.hh"
#include "LADMaterials.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4SDManager.hh"
#include "G4SDChargedFilter.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSTrackLength.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"



LADDetectorConstruction::LADDetectorConstruction()
 {
  Materials = new LADMaterials();
  HodoCreator = new LADDetectorConstructionHodoCreator();
  
 }

 
LADDetectorConstruction::~LADDetectorConstruction()
 {} 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal
G4GlobalMagFieldMessenger* LADDetectorConstruction::fMagFieldMessenger = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Note that this method is called by the Geant4 engine. It is mandatory!
G4VPhysicalVolume* LADDetectorConstruction::Construct()
{
   return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* LADDetectorConstruction::DefineVolumes()
{
  auto  worldSizeXY = 40*m;
  auto  worldSizeZ  = 40*m;
  
  //
  // World
  //
  auto worldS
    = new G4Box("World",           // its name
                 worldSizeXY/2, worldSizeXY/2, worldSizeZ/2); // its size

  auto worldLV
    = new G4LogicalVolume(
                 worldS,           // its solid
                 Materials->defaultMaterial,  // vacuum
                 "World");         // its name

  auto worldPV = new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(),                         // at (0,0,0)
    worldLV,                                 // its logical volume
    "World",                                 // its name
    nullptr,                                 // its mother  volume
    false,                                   // no boolean operation
    0,                                       // copy number
    fCheckOverlaps);                         // checking overlaps


  G4cout<<"Building Hodoscope"<<G4endl;
  //HodoCreator = new LADDetectorConstructionHodoCreator(worldLV, Materials);

  HodoCreator -> BuildHodo(worldLV, Materials);

  GEMCreator = new LADDetectorConstructionGEMCreator();
  GEMCreator ->BuildGEM(worldLV, Materials);

  
  worldLV->SetVisAttributes(G4VisAttributes::GetInvisible());
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LADDetectorConstruction::ConstructSDandField()
{
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);


  GEMCreator -> ConstructSDandField();
  //
  // Scorers
  //
  //  G4cout<<"LADDetectorConstruction::ConstructSDandField() Calling HODO"<<G4endl;
  // HodoCreator -> ConstructSDandField();
  // // declare Absorber as a MultiFunctionalDetector scorer
  // //
  // auto absDetector = new G4MultiFunctionalDetector("Absorber");
  // G4SDManager::GetSDMpointer()->AddNewDetector(absDetector);

  // G4VPrimitiveScorer* primitive;
  // primitive = new G4PSEnergyDeposit("Edep");
  // absDetector->RegisterPrimitive(primitive);

  // primitive = new G4PSTrackLength("TrackLength");
  // auto charged = new G4SDChargedFilter("chargedFilter");
  // primitive ->SetFilter(charged);
  // absDetector->RegisterPrimitive(primitive);

  // // SetSensitiveDetector("AbsoLV",absDetector);

  // // declare Gap as a MultiFunctionalDetector scorer
  // //
  // auto gapDetector = new G4MultiFunctionalDetector("Gap");
  // G4SDManager::GetSDMpointer()->AddNewDetector(gapDetector);

  // primitive = new G4PSEnergyDeposit("Edep");
  // gapDetector->RegisterPrimitive(primitive);

  // primitive = new G4PSTrackLength("TrackLength");
  // primitive ->SetFilter(charged);
  // gapDetector->RegisterPrimitive(primitive);

  // //  SetSensitiveDetector("GapLV",gapDetector);

  // //
  // // Magnetic field
  // //
  // // Create global magnetic field messenger.
  // // Uniform magnetic field is then created automatically if
  // // the field value is not zero.
  // G4ThreeVector fieldValue;
  // fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  // fMagFieldMessenger->SetVerboseLevel(1);

  // // Register the field messenger for deleting
  // G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

