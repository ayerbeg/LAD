#include "LADGEMHit.hh"

#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<LADGEMHit>* LADGEMHitAllocator = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool LADGEMHit::operator==(const LADGEMHit& right) const
{
 G4cout << "<LADGEMHit>operator" << G4endl; 
  
  return ( this == &right ) ? true : false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LADGEMHit::Print()
{
  G4cout
     << "Edep: "
     << std::setw(7) << G4BestUnit(fEdep,"Energy")
     << " track length: "
     << std::setw(7) << G4BestUnit( fTrackLength,"Length")
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

