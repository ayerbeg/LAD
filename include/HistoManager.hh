//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"
#include <vector>

#include <array>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using namespace std;

class TFile;
class TTree;
class TH1D;

const G4int kMaxHisto = 4;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HistoManager
{
public:
  HistoManager();// = default;
  ~HistoManager();

  void Book();
  void Save();

  void FillHisto(G4int id, G4double bin, G4double weight = 1.0);
  void Normalize(G4int id, G4double fac);

  void FillNtuple1(G4double energyAbs, G4double energyGap,
		  G4double trackLAbs, G4double trackLGap);

  void FillTest(G4int ID);

  void PrintStatistic();


  TFile *CreateSaveFile(G4String Name); // Creating a ROOT file for output with the filename "Name"

  void SetPadID(vector<G4int> v){vPadCopy = v;}
  void SetEDepTot(vector<G4double> v){vEneDep = v;}
  void SetPDG(vector<G4int> v){vPDG = v;}
  void SetLevel(vector<G4int> v){vLevel = v;}

  //Need to modify how to store the data
  void SetPadPosition(vector<G4int> v){vPadPosition = v;}
  void SetXbar(vector<G4double> v){vXbar = v;}
  void SetYbar(vector<G4double> v){vYbar = v;}
  void SetZbar(vector<G4double> v){vZbar = v;}

  
private:

  TFile *rootfile; // the ROOT file for output
  TTree *eventtree;
  G4String  FileNameSuffix;	
  
  G4int fEventID = 0;
  G4double fEDepTot = 0 ;
  G4int fpaddle = 0;

  
  TFile* fRootFile = nullptr;
  std::array<TH1D*, kMaxHisto> fHisto = {nullptr, nullptr, nullptr, nullptr};
  TTree* fNtuple1 = nullptr;
  TTree* fNtuple2 = nullptr;

  G4double fEabs = 0.;
  G4double fEgap = 0.;
  G4double fLabs = 0.;
  G4double fLgap = 0.;


  // As I have the code now, I have three vectors in memory, with the same information
  vector<G4double> vEneDep;
  vector<G4int>    vPadCopy;
  vector<G4int>    vPDG;
  vector<G4int>    vLevel;


  // These variables only for position at the bars
  vector<G4int>    vPadPosition;
  vector<G4double> vXbar;
  vector<G4double> vYbar;
  vector<G4double> vZbar;
  
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

