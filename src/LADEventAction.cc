#include "LADEventAction.hh"
// #include "LADHodoSD.hh"
// #include "LADHodoHit.hh"


#include "G4RunManager.hh"
#include "G4Event.hh"
// #include "G4SDManager.hh"
// #include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LADEventAction::LADEventAction(LADRunAction* RunAction, HistoManager* histo, HodoAnalysis* HodoHandle)
  : fRunAct(RunAction)
{
  G4cout<<"LADEventAction::LADEventAction()"<<G4endl;

  // I have to pay more attention to the initializers. 
  // Not initializing the HistoManager was giving me problems
  // This is a way to initialize!
  if (histo!=NULL) fHistoManager=histo;

  if (HodoHandle!=NULL) AnalysisHodo=HodoHandle;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LADEventAction::~LADEventAction()
{
  G4cout<<"LADEventAction::~LADEventAction()"<<G4endl;




}

// MY GOAL is having the Sensitive Detector machinery working, but 
// for some reason I've been unsuscessful.
// I keep these pieces of code for future improvements

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*
LADHodoHitsCollection* 
LADEventAction::GetHitsCollection(G4int hcID,
                                  const G4Event* event) const
{
  G4cout<<"LADEventAction::GetHitsCollection:hcID "<<hcID<<G4endl;
   G4cout<<"LADEventAction::GetHitsCollection "<< event -> GetEventID() <<G4endl;

  auto hitsCollection 
    = static_cast<LADHodoHitsCollection*>(
        event->GetHCofThisEvent()->GetHC(hcID));
  
  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID; 
    G4Exception("LADEventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }         

  return hitsCollection;
}    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LADEventAction::PrintEventStatistics(G4double absoEdep) const
			      // 		  , G4double absoTrackLength,
                              // G4double gapEdep, G4double gapTrackLength) const
{
  // print event statistics
  G4cout
     << "   Absorber: total energy: " 
     << std::setw(7) << G4BestUnit(absoEdep, "Energy")
     // << "       total track length: " 
     // << std::setw(7) << G4BestUnit(absoTrackLength, "Length")
     // << G4endl
     // << "        Gap: total energy: " 
     // << std::setw(7) << G4BestUnit(gapEdep, "Energy")
     // << "       total track length: " 
     // << std::setw(7) << G4BestUnit(gapTrackLength, "Length")
     << G4endl;
}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LADEventAction::BeginOfEventAction(const G4Event* event)
{
  // G4cout<<"LADEventAction::BeginOfEventAction ---- START"<<G4endl;

  // G4cout<<"LADEventAction::BoE "<< event -> GetEventID() <<G4endl;

 EventID = event -> GetEventID();  
 // G4cout<<"LADEventAction::BoE "<< EventID <<G4endl;

 vEnergyDep.clear();
 vPadNumber.clear();


 // G4cout<<"LADEventAction::BoE GOING TO ANALYSIS "<< G4endl;
 AnalysisHodo->BeginOfEventAction(event);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LADEventAction::EndOfEventAction(const G4Event* event)
{
  G4cout<<"LADEventAction::EndOfEventAction ****"<<G4endl;
  auto eventID = event->GetEventID();
// I accumulated all the hits on each paddle sent to this class
// and then it is stored in the HistoManager


// This storage method is temporary until I fix the HodoAnalysis



  
/*

 // Get hits collections IDs (only once)
  if ( fAbsHCID == -1 ) {
    fAbsHCID 
      = G4SDManager::GetSDMpointer()->GetCollectionID("barSD/LADbarsHitsCollection");
    // fGapHCID 
    //   = G4SDManager::GetSDMpointer()->GetCollectionID("GapHitsCollection");
  }

  G4cout<<"LADEventAction::EndOfEventAction fAbsHCID: "<<fAbsHCID<<G4endl;

  // Get hits collections
  auto absoHC = GetHitsCollection(fAbsHCID, event);
  //  auto gapHC = GetHitsCollection(fGapHCID, event);

  // Get hit with total values
  auto absoHit = (*absoHC)[absoHC->entries()-1];
  //  auto gapHit = (*gapHC)[gapHC->entries()-1];
 */
  // Print per event (modulo n)
  //
 // auto eventID = event->GetEventID();
  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl;     
    
    
    //    PrintEventStatistics(
			 // absoHit->GetEdep(), absoHit->GetTrackLength(),
			 //     gapHit->GetEdep(), gapHit->GetTrackLength());
  }  
  
  // G4cout<<"LADEventAction::EoE GOING TO ANALYSIS "<< G4endl;
 AnalysisHodo->EndOfEventAction(event);

 // Delete them, copying a vector twice... bad memory handle
 // vEnergyDep = AnalysisHodo-> GetEneDep();
 // vPadNumber = AnalysisHodo -> GetPad();



 // I could have these lines in the Analysis code and save steps
 // for the time being let's keep them here
 
 fHistoManager->SetPadID(AnalysisHodo  -> GetPad() );
 fHistoManager->SetEDepTot(AnalysisHodo-> GetEneDep() );

 fHistoManager->SetPDG(AnalysisHodo    -> GetPDG() );
 fHistoManager->SetLevel(AnalysisHodo  -> GetLevel() );



 fHistoManager->FillTest(eventID);
 
 
 for (G4int kk = 0; kk < vPadNumber.size() ; kk++)
   {
     
     //     G4cout << "storing in Event: " <<vPadNumber[kk] <<G4endl;	  
   }
 
 
}  


void LADEventAction::FillEvent(G4int copy, G4double ene)
{
// Just filling the vectors with information from SteppingAction

  vEnergyDep.push_back(ene);
  vPadNumber.push_back(copy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
