#ifndef LADVariables_H
#define LADVariables_H 1

#define MAXANZAHLTEILCHEN 4

#include <iostream>
using namespace std;

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

enum Gun_modes {GM_OTHER, 
		GM_NORMAL, 
		GM_ONE};

enum Event_modes {E_ELASTIC};


class LADVariables
{
public:
  LADVariables();
  ~LADVariables();
  
  G4int LoadFromFile(G4String FileName);
  G4int SaveToFile(G4String FileName);


  G4int MultiTh;

  G4double centralWallAngle;
  G4double leftWallAngle;  // wrt to the centralWallAngle
  G4double rightWallAngle; // wrt to the centralWallAngle

  // this is the distance from the center of the hall to the front face of the wall
  G4double centralWallDistance; // wrt to the center of the hall
  G4double leftWallDistance;  // wrt to the centralWall
  G4double rightWallDistance; // wrt to the centralWallAngle
  G4bool G4GUI; // G4 terminal or GUI




  
  G4String Reaktionsteilchen[MAXANZAHLTEILCHEN];
  G4double mamiEnergie;
  G4double Strahlwinkel;
  
  G4String Elektrondetektor;
  G4double SpecBWinkel;
  G4double SpecBImpuls;
  //	G4double SpecBImpulsbreite;
  
  G4String Target;

  G4bool Elastic;
  G4bool Simple;

  G4bool ScanAngles;
  G4double KAOS_ang;
  G4double C_ang;
  G4double minAng;
  G4double maxAng;
  G4double angStep;
  
  G4bool ScanEnergy;
  G4double minEne;
  G4double maxEne;
  G4double eneStep;

  G4double DCV;
  G4ThreeVector TARGETPosition;


  G4bool UseJoch;
  G4bool UseDipol;
  G4bool UseKolli;
  G4bool UseFibers;
  G4bool UseFieldMap;
	
	G4bool Wedeln;
	G4bool LongTarget;
	G4double MaxStepLength;
//	G4bool LimitStep;
	G4bool doDecay;
	G4bool doAllBackround;
	G4bool doSecondarys;
	G4bool doEMPhysics;
	G4bool checkTargethit;
	G4bool coincOnly;
	G4bool FastMode;
	G4bool doDrawTracks;
		

	G4double FEEWinkel;
	G4RotationMatrix FEERotation;
	G4ThreeVector FEEPosition;
	G4double FEELaenge;

	G4double Detektorabstand;
	G4double Detektorwinkel;
	G4double JochVerschiebung;
	G4double Detektorhub;
	G4double Feldstaerke;
	G4double DetektorTarget;
	
	G4double SciFiWinkel;
	G4double FEE_OOP;

	G4int ESchritte;
	G4int xwSchritte;
	G4int ywSchritte;

	G4double xwWeite;
	G4double ywWeite;
			
// Variablen, die nicht eingelesen werden:
	unsigned int nK;
	G4bool isInKField;
	G4bool doDraw;
	Gun_modes GunMode;

	G4String FileNameSuffix;	

	G4double TargetX;
	G4double TargetY;

private:
	G4bool ReadBoolean(G4String Value);
	
};

ostream & operator << (ostream& s, const LADVariables v);

extern LADVariables *Variables;

#endif