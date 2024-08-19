#include <sstream>
#include <fstream>
using namespace std;

#include "LADVariables.hh"
#include "G4SystemOfUnits.hh"


#define READBOOL(myx) \
				if (buf == #myx) \
				{ \
					ss >> word; \
					myx = ReadBoolean(word); \
				}

#define READDOUBLE(x,unit) \
				if (buf == #x) \
				{ \
					ss >> doubleword; \
					x = doubleword * unit; \
				}

#define READINT(x) \
				if (buf == #x) \
				{ \
					ss >> intword; \
					x = intword; \
				}

#define READSTRING(x) \
				if (buf == #x) \
				{ \
					ss >> x; \
				}

#define READVECTOR(v) \
				if (buf == #v) \
				{ \
					G4double x,y,z; \
					if (ss >> x >> y >> z) \
					v = G4ThreeVector(x * m, y * m, z * m); \
				}



LADVariables::LADVariables()
{
	G4cout << "<LADVariables::LADVariables>: Start" << G4endl;

	//LAD DEFAULT VALUES
	
	centralWallAngle = 127 *deg;
	leftWallAngle    =  23 *deg; // wrt to the centralWallAngle
	rightWallAngle   =  23 *deg; // wrt to the centralWallAngle

	centralWallDistance = 523 *cm; // wrt to the center of the hall
	leftWallDistance    =  92 *cm;  // wrt to the centralWall
	rightWallDistance   =  92 *cm; // wrt to the centralWallAngle


	// G4int i;
	// for (i=0;i<MAXANZAHLTEILCHEN;i++)
	// {
	// 	Reaktionsteilchen[i]="";
	// }

	
	mamiEnergie = 1500 * MeV;
	Strahlwinkel = 0;

	Elektrondetektor = "SpecB";
	SpecBWinkel = 40 * deg;
	SpecBImpuls = 300 * MeV;

	Target = "C12";

	UseKolli = false;
	UseJoch = true;
	UseDipol = false;
	UseFibers = true;
	UseFieldMap = true;
	
	Wedeln = false;
	LongTarget = false;
	MaxStepLength = 10*m;
//	LimitStep = false;
	doDecay = false;
	doAllBackround = false;
	doSecondarys = true;
	doEMPhysics = true;
	checkTargethit = true;
	coincOnly = false;
	FastMode = false;
	doDrawTracks = false;
		
	FEEPosition = G4ThreeVector(-1*m, 0, 3*m);
	FEELaenge = 1.4 * m;
	FEEWinkel = 0;

	Detektorabstand = 1.4 * m;
	Detektorwinkel = 0 *  deg;
//	JochVerschiebung;
	Detektorhub = 0;
	Feldstaerke = 1 * tesla;
	DetektorTarget = Detektorabstand;
	
	SciFiWinkel=60*deg;
	FEE_OOP=0*mm;
	
	ESchritte = 20;
	xwSchritte = 5;
	ywSchritte= 5;

	xwWeite=10*deg;
	ywWeite=0.5*deg;
		
	nK = 0;
	doDraw = false;
	isInKField = false;
	GunMode = GM_OTHER;

	char buffer[100];
	time_t Jetzt = time(0);	
	strftime((char*)&buffer,100,"%Y-%m-%d-%H-%M-%S",localtime(&Jetzt));
	FileNameSuffix = buffer;
	TargetX=0;
	TargetY=0;
}

LADVariables::~LADVariables()
{
	G4cout << "<LADVariables::~LADVariables>: Finished" << G4endl;
}

G4int 
LADVariables::LoadFromFile(G4String FileName)
{
  G4cout << "<LADVariables::LADVariables>: reading file" << G4endl;
  
	ifstream infile;
	G4String line;
	infile.open(FileName);
	if (infile)
	{
		while (!infile.eof())
		{
			if (getline(infile,line))
			{
				G4String buf;
				G4String word;
				G4double doubleword;
				G4int intword;
				stringstream ss(line);
//				G4cout << ">" << line << G4endl;
				ss >> buf;
//				G4cout << ">" << ss.str() << "<" << G4endl;

				READDOUBLE(centralWallAngle, deg);
				READDOUBLE(leftWallAngle, deg); // wrt to the centralWallAngle
				READDOUBLE(rightWallAngle, deg); // wrt to the centralWallAngle

				READDOUBLE(centralWallDistance, cm); // wrt to the center of the hall
				READDOUBLE(leftWallDistance, cm);  // wrt to the centralWall
				READDOUBLE(rightWallDistance, cm); // wrt to the centralWallAngle

				READBOOL(G4GUI);//GUI on/off

				READINT(MultiTh);//multithread


				
				
				
				READDOUBLE(mamiEnergie,MeV);
			
				READDOUBLE(C_ang,deg);
				READDOUBLE(KAOS_ang,deg);

				READDOUBLE(Strahlwinkel,deg);
				
				READSTRING(Elektrondetektor);
				READDOUBLE(SpecBWinkel,deg);
				READDOUBLE(SpecBImpuls,MeV);

				READBOOL(ScanAngles);
				READDOUBLE(minAng,deg);
				READDOUBLE(maxAng,deg);
				READINT   (angStep);

				READBOOL(Elastic);
				READBOOL(Simple);

				READBOOL(ScanEnergy);
				READDOUBLE(minEne,MeV);
				READDOUBLE(maxEne,MeV);
				READINT   (eneStep);

				READDOUBLE(DCV,mm);
				READVECTOR(TARGETPosition);


				READBOOL(UseJoch);
				READBOOL(UseDipol);
				READBOOL(UseFibers);
				READBOOL(UseFieldMap);
				READBOOL(Wedeln);
				READBOOL(LongTarget);
				READDOUBLE(MaxStepLength,mm);
//				READBOOL(LimitStep);
				READBOOL(doDecay);
				READBOOL(doAllBackround);
				READBOOL(doSecondarys);
				READBOOL(doEMPhysics);
				READBOOL(checkTargethit);
				READBOOL(coincOnly);
				READBOOL(FastMode);
				READBOOL(doDrawTracks);
				
				READSTRING(Target);				
				READDOUBLE(TargetX,mm);				
				READVECTOR(FEEPosition);
				READDOUBLE(FEEWinkel,1);
				READDOUBLE(FEELaenge,m);
				READDOUBLE(Detektorabstand,m);
				READDOUBLE(Detektorwinkel,deg);
				READDOUBLE(Detektorhub,mm);
				READDOUBLE(Feldstaerke,tesla);

				READDOUBLE(SciFiWinkel,deg);
				READDOUBLE(FEE_OOP,mm);
				READINT(ESchritte);
				READINT(xwSchritte);
				READINT(ywSchritte);
				READDOUBLE(xwWeite,deg);
				READDOUBLE(ywWeite,deg);
			} // if getline
		} // if not end of file
		FEERotation.rotateY(FEEWinkel);
		G4cout << FEEWinkel << " -----> " << G4endl;
		DetektorTarget=Detektorabstand;
		Detektorabstand=DetektorTarget*cos(Strahlwinkel);
		if (Strahlwinkel!=0) TargetX=-DetektorTarget*sin(Strahlwinkel);
		return 0;
	} // if infile
	else
	{
		G4cerr << "Konnte Datei nicht zum lesen öffnen!" << G4endl;
		return -1;
	} // if infile else
}


G4int 
LADVariables::SaveToFile(G4String FileName)
{
	ofstream outfile;
	outfile.open(FileName);
	if (outfile)
	{
		outfile << *this;
		G4cout << "Folgendes wurde ins File geschrieben:" << G4endl << *this;
		outfile.close();
		return 0;
	}
	else
	{
		return -1;
	}
}

ostream & operator << (ostream & os, const LADVariables v) {
	return os 
		<< "mamiEnergie " << v.mamiEnergie / MeV << endl
		<< "Strahlwinkel " << v.Strahlwinkel / deg << endl
		<< "Elektrondetektor " << v.Elektrondetektor << endl
		<< "SpecBWinkel " << v.SpecBWinkel / deg << endl
		<< "SpecBImpuls " << v.SpecBImpuls / MeV << endl
		<< "Target " << v.Target << endl
		<< "TargetX " << v.TargetX/mm << endl
		<< "TargetY " << v.TargetY/mm << endl
		<< "UseJoch " << v.UseJoch << endl
		<< "UseDipol " << v.UseDipol << endl
		<< "UseFibers " << v.UseFibers << endl
		<< "UseFieldMap " << v.UseFieldMap << endl
		<< "Wedeln " << v.Wedeln << endl
		<< "LongTarget " << v.LongTarget << endl
		<< "MaxStepLength " << v.MaxStepLength << endl
//		<< "LimitStep " << v.LimitStep << endl
		<< "doDecay " << v.doDecay << endl
		<< "doAllBackround " << v.doAllBackround << endl
		<< "doSecondarys " << v.doSecondarys << endl
		<< "doEMPhysics " << v.doEMPhysics << endl
		<< "checkTargethit " << v.checkTargethit << endl
		<< "coincOnly " << v.coincOnly << endl
		<< "FastMode " << v.FastMode << endl
		<< "doDrawTracks " << v.doDrawTracks << endl
		<< "FEEWinkel " << v.FEEWinkel << endl
		<< "FEEPosition " << v.FEEPosition.x() / m << " "
		<< v.FEEPosition.y() / m <<  " " << v.FEEPosition.z() / m << endl
		<< "FEELaenge " << v.FEELaenge / m << endl
		<< "Detektorabstand " << v.Detektorabstand / m << endl
		<< "Detektorwinkel " << v.Detektorwinkel / deg << endl
		<< "Detektorhub " << v.Detektorhub / mm << endl
		<< "Feldstaerke " << v.Feldstaerke / tesla << endl
		<< "SciFiWinkel " << v.SciFiWinkel / deg << endl
		<< "FEE_OOP " << v.FEE_OOP / mm << endl
		<< "ESchritte " << v.ESchritte << endl
		<< "xwSchritte " << v.xwSchritte << endl
		<< "ywSchritte " << v.ywSchritte << endl
		<< "xwWeite " << v.xwWeite / deg << endl
		<< "ywWeite " << v.xwWeite / deg << endl
;
}

ostream & operator << (ostream & os, const LADVariables *v) {
	return os 
		<< "mamiEnergie " << v->mamiEnergie / MeV << endl
		<< "Strahlwinkel " << v->Strahlwinkel / deg << endl
		<< "Elektrondetektor " << v->Elektrondetektor << endl
		<< "SpecBWinkel " << v->SpecBWinkel / deg << endl
		<< "SpecBImpuls " << v->SpecBImpuls / MeV << endl
		<< "Target " << v->Target << endl
		<< "TargetX " << v->TargetX/mm << endl
		<< "TargetY " << v->TargetY/mm << endl
		<< "UseKolli " << v->UseKolli << endl
		<< "UseJoch " << v->UseJoch << endl
		<< "UseDipol " << v->UseDipol << endl
		<< "UseFibers " << v->UseFibers << endl
		<< "UseFieldMap " << v->UseFieldMap << endl
		<< "Wedeln " << v->Wedeln << endl
		<< "LongTarget " << v->LongTarget << endl
		<< "MaxStepLength " << v->MaxStepLength << endl
//		<< "LimitStep " << v->LimitStep << endl
		<< "doDecay " << v->doDecay << endl
		<< "doAllBackround " << v->doAllBackround << endl
		<< "doSecondarys " << v->doSecondarys << endl
		<< "doEMPhysics " << v->doEMPhysics << endl
		<< "checkTargethit " << v->checkTargethit << endl
		<< "coincOnly " << v->coincOnly << endl
		<< "FastMode " << v->FastMode << endl
		<< "doDrawTracks " << v->doDrawTracks << endl
		<< "FEEWinkel " << v->FEEWinkel << endl
		<< "FEEPosition " << v->FEEPosition.x() / m << " "
		<< v->FEEPosition.y() / m <<  " " << v->FEEPosition.z() / m << endl
		<< "FEELaenge " << v->FEELaenge / m << endl
		<< "Detektorabstand " << v->Detektorabstand / m << endl
		<< "Detektorwinkel " << v->Detektorwinkel / deg << endl
		<< "Detektorhub " << v->Detektorhub / mm << endl
		<< "Feldstaerke " << v->Feldstaerke / tesla << endl
		<< "FEE_OOP " << v->FEE_OOP / mm << endl
		<< "ESchritte " << v->ESchritte << endl
		<< "xwSchritte " << v->xwSchritte << endl
		<< "ywSchritte " << v->ywSchritte << endl
		<< "xwWeite " << v->xwWeite / deg << endl
		<< "ywWeite " << v->xwWeite / deg << endl
;
}


G4bool
LADVariables::ReadBoolean(G4String Value)
{
	if ((Value=="true")
		|| (Value=="True")
		|| (Value=="1"))
	{return true;}
	else
	{return false;}
}


LADVariables *Variables=NULL;
