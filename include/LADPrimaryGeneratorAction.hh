#ifndef LADPrimaryGeneratorAction_h
#define LADPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"

#include "LADVariables.hh"
#include "LADConstants.hh"

class G4ParticleGun;
class G4Event;

/// The primary generator action class with particle gum.
///
/// It defines a single particle which hits the calorimeter
/// perpendicular to the input face. The type of the particle
/// can be changed via the G4 build-in commands of G4ParticleGun class
/// (see the macros provided with this example).

class LADPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  LADPrimaryGeneratorAction();
  ~LADPrimaryGeneratorAction() override;

  void GeneratePrimaries(G4Event* event) override;
  G4ThreeVector orthovector(G4ThreeVector dir_vec);
private:
  G4ParticleGun* fParticleGun = nullptr; // G4 particle gun

  G4ParticleDefinition *particle;
  
  G4ThreeVector MomDirPer;
  G4double StepEventTheta;
  G4double theta;
  G4double ThetaMax;
  G4double ThetaStep;

  G4double WallDist;
  G4double WallCentAng;
  G4ThreeVector RealMomentumDir;


 G4int RadStepCounter;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
