#ifndef LADGEMHit_h
#define LADGEMHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"


class LADGEMHit : public G4VHit
{
  public:
    LADGEMHit() = default;
    LADGEMHit(const LADGEMHit&) = default;
    ~LADGEMHit() override = default;

    // operators
    LADGEMHit& operator=(const LADGEMHit&) = default;
    G4bool operator==(const LADGEMHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    void Draw()  override{}
    void Print() override;

    // methods to handle data
    void Add(G4double de, G4double dl);

    // get methods
    G4double GetEdep() const;
    G4double GetTrackLength() const;

  private:
    G4double fEdep = 0.;        ///< Energy deposit in the sensitive volume
    G4double fTrackLength = 0.; ///< Track length in the  sensitive volume
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using GEMHitsCollection = G4THitsCollection<LADGEMHit>;

extern G4ThreadLocal G4Allocator<LADGEMHit>* LADGEMHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* LADGEMHit::operator new(size_t)
{
  if (!LADGEMHitAllocator) {
    LADGEMHitAllocator = new G4Allocator<LADGEMHit>;
  }
  void *hit;
  hit = (void *) LADGEMHitAllocator->MallocSingle();
  return hit;
}

inline void LADGEMHit::operator delete(void *hit)
{
  if (!LADGEMHitAllocator) {
    LADGEMHitAllocator = new G4Allocator<LADGEMHit>;
  }
  LADGEMHitAllocator->FreeSingle((LADGEMHit*) hit);
}

inline void LADGEMHit::Add(G4double de, G4double dl) {
  fEdep += de;
  fTrackLength += dl;
}

inline G4double LADGEMHit::GetEdep() const {
  return fEdep;
}

inline G4double LADGEMHit::GetTrackLength() const {
  return fTrackLength;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
