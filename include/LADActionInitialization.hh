#ifndef LADActionInitialization_h
#define LADActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

#include "PrimaryGeneratorAction.hh"
#include "LADRunAction.hh"
#include "LADEventAction.hh"
#include "LADSteppingAction.hh"
#include "HistoManager.hh"

#include "HodoAnalysis.hh"
class HodoAnalysis;

class PrimaryGeneratorAction;
class LADDetectorConstruction;
class HistoManager;
class LADRunAction;
class LADEventAction;
class LADSteppingAction;
/// Action initialization class.

class LADActionInitialization : public G4VUserActionInitialization
{
  public:
    LADActionInitialization(LADDetectorConstruction*) ;
    ~LADActionInitialization();
  //     ~LADActionInitialization() override = default;

    void BuildForMaster() const override;
    void Build() const override;

private: 
  LADDetectorConstruction* detConstruction;

 };

#endif


