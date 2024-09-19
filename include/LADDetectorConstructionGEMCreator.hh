#ifndef __LADDetectorConstructionGEMCreator_hh
#define __LADDetectorConstructionGEMCreator_hh


//#include "G4SBSComponent.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include <vector>
#include "LADMaterials.hh"
#include "G4SDManager.hh"
#include "LADGEMSD.hh"
#include "G4VUserDetectorConstruction.hh"

using namespace std;

class LADDetectorConstructionGEMCreator
{
public:
  LADDetectorConstructionGEMCreator();
  ~LADDetectorConstructionGEMCreator();


  void BuildGEM(G4LogicalVolume *, LADMaterials *);
  void ConstructSDandField();
private:

  G4String SDname = "LADGEM";
  G4String TrackerPrefix;
  G4LogicalVolume *gplog_driftgas;
  //  LADGEMSD* GEMSD;
  vector<G4String> log_sd_name;
  vector<G4LogicalVolume*> log_sd;
  
};

#endif//__LADDetectorConstructionGEMCreator_hh
