#ifndef __LADDetectorConstructionGEMCreator_hh
#define __LADDetectorConstructionGEMCreator_hh


//#include "G4SBSComponent.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include <vector>
#include "LADMaterials.hh"
//#include "sbstypes.hh"

using namespace std;

class LADDetectorConstructionGEMCreator
{
public:
  LADDetectorConstructionGEMCreator();
  ~LADDetectorConstructionGEMCreator();


  void BuildGEM(G4LogicalVolume *, LADMaterials *);

private:
  
  
};

#endif//__LADDetectorConstructionGEMCreator_hh
