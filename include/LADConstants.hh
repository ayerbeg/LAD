#ifndef LADConstants_H
#define LADConstants_H 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include <vector>
#include "G4SystemOfUnits.hh"

using namespace std;

class LADConstants
{
public:
  LADConstants();
  ~LADConstants();

  G4int NoOfBars; // number of scintillator paddles per wall (Before it was declared static const)
  G4int NoOfWalls;
  G4int NoOfSubWalls;
  G4int NoOfPanels;


  
  G4double BarThick;
  G4double BarWidth;
  G4double WallSeparation;
  
  vector<G4double> Barlength;
  /*
	G4double SFT1tiefe;
	G4double SFT2tiefe;
	G4double SFPbDicke;
	
	G4double ExperimentierhalleX;
	G4double ExperimentierhalleY;
	G4double ExperimentierhalleZ;
	
	G4double EintrittsHoehe;
	G4double EintrittsBreite;
	G4double EintrittsVerschiebung;
	G4double DipolPosX; // Im Koordinatensystem Target = (0,0,0) 
  	G4double Polschuhabstand;
	G4ThreeVector AufhaengungMWPC;
	G4double MWPCAbstand;
	G4double MWPCBreite;
	G4double MWPCDicke;
	G4double MWPCOrtOffset;
	G4ThreeVector CherenkovAbstand;

  	G4double KolliDicke;
  	G4double KolliBreite;
  	G4double KolliHoehe;
  	G4double KolliLochBreite;
  	G4double KolliLochHoehe;
  	G4double Kolliabstand;
  	G4double Kolliversatz;

	G4double DCIRadius;
	G4double DCIBreite;
	G4double DCIBreiteBisSpule;
	G4double DCIGap;
	G4double DCIWinkelbreite;
	G4double TargetLaenge;
	G4double SciFiDurchmesser;
	G4int SciFiKanalzahl;
	G4int SciFiKanalzahl_th;
	G4int SciFiKanalzahl_y;
	G4double CladdingDicke;
	G4double SciFiHoehe;
	G4double SciFiYKantenlaenge;

	G4double StSigma;
	G4double EinTeslaImpuls;
	
	G4double FDetDrehung;

	G4double TraegerBreite1; // Vor der Ringschine
	G4double TraegerBreite2; // LH2 Halterung breit
	G4double TraegerBreite3; // LH2 Halterung schmal
	G4double TraegerBreite4; // letzte Abstuetztung Beamlinetraeger
	G4double TraegerHoehe;
	G4double TraegerTiefe1;
	G4double TraegerTiefe2;
	G4double TraegerTiefe4;
	G4double Traeger1Abstand;
	G4double Traeger2Abstand;
	G4double Traeger23Abstand;
	G4double Traeger34Abstand;

//	G4double SpekBWinkelakzeptanz;
	G4double SpekBImpulsbreite;

	G4int Variationsschritte;
	G4double Winzigkeit;
  */
	
};

extern LADConstants *Constants;

#endif
