// ------------------------------------------------------------
//      HODO Analysis class for LAD
//      
//      Carlos Ayerbe Gayoso (based in KAOS Simulation)
//      2024
// ------------------------------------------------------------

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.h.......oooOO0OOooo....
// #include <string>
// #include <iostream>
// #include <sstream>
// #include <time.h>


// #include <chrono>
// #include <iomanip> // put_time
// #include <fstream>
// #include <sstream> // stringstream



// #include "LADEventAction.hh"

// //#include "SystemOfUnits.h"
// #include "G4ios.hh"

// #include "G4UnitsTable.hh"

// #include "G4VPhysicalVolume.hh"
// #include "G4Event.hh"
// #include "G4Run.hh"
// #include "G4Track.hh"
// #include "G4ClassificationOfNewTrack.hh"
// #include "G4TrackStatus.hh"
// #include "G4Step.hh"
// #include "G4Types.hh"
// #include "G4UImanager.hh"
// #include "G4MaterialCutsCouple.hh"
// #include "G4Material.hh"

// #include "G4SystemOfUnits.hh"

#include "HodoAnalysis.hh"

HodoAnalysis::HodoAnalysis()
{
  G4cout << "<HodoAnalysis::HodoAnalysis> Running ***************************************" <<G4endl;
  // rootfile = NULL;
  // rootfile = CreateSaveFile("HodoFile");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

HodoAnalysis::~HodoAnalysis()
{

  //  if (rootfile) 

    G4cout << "<HodoAnalysis::~HodoAnalysis>: Close: "  << G4endl;
    delete rootfile;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void HodoAnalysis::BeginOfRunAction(const G4Run *aRun)
{
  // although a pointer is brought here, it is not working correctly.

  G4cout<<"<HodoAnalysis::BeginOfRunAction>: begin"<<G4endl;

  // WHY THIS DOESN'T WORK??
  //  time(&start);//time flag for the beggining of run used in counter()

  if(aRun)
    {
      NbOfEvents = aRun->GetNumberOfEvent();   
    }
 
  time_t START = time(0);
  char bufferST[100];
  strftime((char*)&bufferST,100,"%Y-%m-%d - %H-%M-%S",localtime(&START));
  G4cout<<"Time Stamp <START>: "<<bufferST<<G4endl;


  
  // These variables are initialized here because is the summary of the run
  // sumEAbs = sum2EAbs =sumEGap = sum2EGap = 0.;
  // sumLAbs = sum2LAbs =sumLGap = sum2LGap = 0.; 

  // G4cout<<"to createsave"<<G4endl;
  
  // // It was at the beggining of the class, perhaps here we can create a root file every run
  // rootfile = CreateSaveFile("HodoFile");


  
  //Here we define some variable will be in the root file, should be adapted according our needs
  // hittree = new TTree("hittree","module hits");
  // hittree->Branch("hit",&hit.EventID, "EventID/I:Module/I:EDepMod/F:Layer/I:EDepLay/F:EDep/F");

  // eventtree = new TTree("eventtree","Calorimeter Summary");
  // eventtree->Branch("event",&event.EventID, "EventID/I:EDepTot/F:PosCal[3]/F");

  // settree = new TTree("settree","Calorimeter Settings");
  // settree->Branch("set",&set.runID, "runID/I:noCol/I:noRow/I");

  //  G4cout<<"<HodoAnalysis::BeginOfRunAction>: leaving...."<<G4endl;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HodoAnalysis::EndOfRunAction(const G4Run  *aRun)
{
  G4cout<<"<HodoAnalysis::EndOfRunAction>: begin"<<G4endl;

 if(aRun)
    {
      NbOfEvents = aRun->GetNumberOfEvent();   
      G4cout << "EoR: " << NbOfEvents << G4endl;
    }


  /*
  //compute statistics: mean and rms
  //
  sumEAbs /= NbOfEvents; sum2EAbs /= NbOfEvents;
  G4double rmsEAbs = sum2EAbs - sumEAbs*sumEAbs;
  if (rmsEAbs >0.) rmsEAbs = std::sqrt(rmsEAbs); else rmsEAbs = 0.;
    
  sumEGap /= NbOfEvents; sum2EGap /= NbOfEvents;
  G4double rmsEGap = sum2EGap - sumEGap*sumEGap;
  if (rmsEGap >0.) rmsEGap = std::sqrt(rmsEGap); else rmsEGap = 0.;
    
  sumLAbs /= NbOfEvents; sum2LAbs /= NbOfEvents;
  G4double rmsLAbs = sum2LAbs - sumLAbs*sumLAbs;
  if (rmsLAbs >0.) rmsLAbs = std::sqrt(rmsLAbs); else rmsLAbs = 0.;
    
  sumLGap /= NbOfEvents; sum2LGap /= NbOfEvents;
  G4double rmsLGap = sum2LGap - sumLGap*sumLGap;
  if (rmsLGap >0.) rmsLGap = std::sqrt(rmsLGap); else rmsLGap = 0.;
    
  //print
  //
  G4cout
  << "\n--------------------End of Run------------------------------\n"
  << "\n mean Energy in Absorber : " << G4BestUnit(sumEAbs,"Energy")
  << " +- "                          << G4BestUnit(rmsEAbs,"Energy")  
  << "\n mean Energy in Gap      : " << G4BestUnit(sumEGap,"Energy")
  << " +- "                          << G4BestUnit(rmsEGap,"Energy")
  << G4endl;
    
  G4cout
  << "\n mean trackLength in Absorber : " << G4BestUnit(sumLAbs,"Length")
  << " +- "                               << G4BestUnit(rmsLAbs,"Length")  
  << "\n mean trackLength in Gap      : " << G4BestUnit(sumLGap,"Length")
  << " +- "                               << G4BestUnit(rmsLGap,"Length")
  << "\n------------------------------------------------------------\n"
  << G4endl;
  
  //we send the variables for this setup, so the root analysis is independent of the set-up
  // set.runID = Run->GetRunID();//Number of run
  // set.noCol = Variables->NbOfCaloColumns;
  // set.noRow = Variables->NbOfCaloRows;
  // settree->Fill();
	
  // This method is called at the end of every run.
  //	dosave();
  //	if (rootfile) rootfile->Close();
	
  // Those two lines were at the desctructor of class, I comment the first since looks it is repeated
  //    rootfile->Close();
  //    delete rootfile;
  */
  time_t END = time(0);
  char bufferEN[100];
  strftime((char*)&bufferEN,100,"%Y-%m-%d - %H-%M-%S",localtime(&END));
  G4cout<<"Time Stamp <END>: "<<bufferEN<<G4endl;

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HodoAnalysis::BeginOfEventAction(const G4Event *anEvent)
{
  // This method is invoqued at the begin of every event. 
 
  if(anEvent)
  {
    //    G4cout<<"<HodoAnalysis::BeginOfEventAction>: Begin"<<G4endl;
  }

  
  // initialisation per event

  EnergyAbs = EnergyGap = 0.;

  // I would like to avoid the loops, but memset was not setting the initial values as I wanted
 for (G4int ii = 0; ii< 3; ii++)
    {
      for (G4int jj = 0; jj< 2; jj++)
	{
	  for (G4int kk = 0; kk< 11; kk++)
	    {
	      EnergyAbsBar[ii][jj][kk] = 0.;
	      CopyNumberBar[ii][jj][kk] = -1;
	    }
	}
    }

 
 // Clean the vectors before the event runs
  vEneDep.clear();
  vPadNum.clear();
  vPDG.clear();
  vLevel.clear();

  
  //  G4cout<<"<HodoAnalysis::BeginOfEventAction>: Leaving..."<<G4endl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HodoAnalysis::EndOfEventAction(const G4Event* anEvent)
{

  if(anEvent)
  {
    //    G4cout<<"<HodoAnalysis::EndOfEventAction>: Begin"<<G4endl;
  }

  // In time I used this approach to the analysis, in this method
  // was set the variables to be stored.

  

  // this is the code to store by bar
  // for (G4int ii = 0; ii< 3; ii++)
  //   {
  //     for (G4int jj = 0; jj< 2; jj++)
  // 	{
  // 	  for (G4int kk = 0; kk< 11; kk++)
  // 	    {
  // 	      if(CopyNumberBar[ii][jj][kk] != -1)
  // 		{
  // 		  vEneDep.push_back(EnergyAbsBar[ii][jj][kk]);
  // 		  vPadNum.push_back(CopyNumberBar[ii][jj][kk]);
		  
  // 		  G4cout << "storing: " << CopyNumberBar[ii][jj][kk] <<G4endl;	  
  // 		}
  // 	    }
  // 	}
  //   }
  //  G4cout << "size storing: " << vEneDep.size() <<G4endl;	  
      
    // Counter();//call the couter for screen output (better at the end of event)
    
    // //Once an event is finished, HERE is where the data is manipuled to be stored
    
    // //accumulates statistic
    // //

    // //In the original, was called through a handle, here is only an internal method
    
    // //    G4cout<<"HodoAnalysis::EndOfEventAction:EnergyAbs: "<<EnergyAbs<<G4endl;
    // //    G4cout<<"HodoAnalysis::EndOfEventAction:EnergyGap: "<<EnergyGap<<G4endl;


    // fillPerEvent(EnergyAbs, EnergyGap, TrackLAbs, TrackLGap);
    
    
    // //print per event (modulo n)
    // //
    // G4int evtNb = anEvent->GetEventID();

    // //  if (evtNb%printModulo == 0)//this sentence doesn't work, at least with this modulo
    
    // // std::setw(7) is a format command for the following sentence
    // if(Variables->Verbose == 1)
    // 	/home/ayerbe/Downloads/WhatsApp Chat with Marie Boer.zip
    // {
    // G4cout<<"***********************"<<G4endl;
    // G4cout<<"HodoAnalysis"<<G4endl;
    // G4cout << "---> End of event: " << evtNb << G4endl;	
    // G4cout<<"***********************"<<G4endl;
	
    // G4cout
    // << "   Absorber: total energy: " << std::setw(7)
    // << G4BestUnit(EnergyAbs,"Energy")
    // << "       total track length: " << std::setw(7)
    // << G4BestUnit(TrackLAbs,"Length")
    // << G4endl
    // << "        Gap: total energy: " << std::setw(7)
    // << G4BestUnit(EnergyGap,"Energy")
    // << "       total track length: " << std::setw(7)
    // << G4BestUnit(TrackLGap,"Length")
    // << G4endl;
	
    // }    
    
    // hit.EventID = Run->GetNumberOfEvent();//Number of event. It gives the index for the storing data   
    // hit.EventID = evtNb;//Number of event. It gives the index for the storing data   
    
    // for (G4int i=0; i<NoMaxModules;i++)
    // {
    // if (EnergyGapMod[i]>0.)
    // {
    // hit.Module = i;
    // //	    G4cout<<"i:"<<i<<G4endl;
    // hit.EDepMod = EnergyGapMod[i]/MeV;//the energy deposited in the scintillator of the module

    // EnergyTemp = EnergyTemp + EnergyGapMod[i]/MeV;//summing all energies in all scint (it is derived to event.EDepTot

    // hit.EDep = EnergyGapMod[i]/MeV + EnergyAbsMod[i]/MeV;
 
    // if(Variables->Verbose == 1)
    // {
    // G4cout<<"EnergyGapMod["<<i<<"]: "<< EnergyGapMod[i] *MeV<<" MeV"<<G4endl;
    // }	
    // }

	
    // for (G4int j=0; j<20;j++)
    // {
    // if (EnergyGapModLay[i][j]>0.)
    // {
    // hit.Layer = j;
    // hit.EDepLay = EnergyGapModLay[i][j]/MeV;
    // //		G4cout<<"EnergyGapModLay["<<i<<","<<j<<"]: "<< EnergyGapModLay[i][j] *MeV<<" MeV"<<G4endl;
    // }
    // } 
	 
    // //there are events where particles leave 0 energy
    // //with our conditions, such event is not stored, but the index is increased
    // //perhaps this condition could be relaxed in other way

    // if (EnergyGapMod[i]>0. && (hit.Module>-1))
    // {
    // //	    G4cout<<"fill i:"<<i<<G4endl;
    // hittree->Fill();	
    // }
    // }

    // // event.EventID = evtNb;//Number of event. It gives the index for the storing data   
    // // event.EDepTot = EnergyTemp;

    // // event.PosCal[0]=Position.x();
    // // event.PosCal[1]=Position.y();
    // // event.PosCal[2]=Position.z();
   
    // // eventtree->Fill();

    

  return; //is necesary??
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....






//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


void 
HodoAnalysis::StepAnalysis(const G4Step *aStep)
{
  // This method just check the aceptance of the particle in every detector of KAOS
  // I'll keep the method for later, but perhaps, since I'm doing analysis separetly for every detector
  // the consideration of acceptance at every detector could do it in SteppingAction class
  // Must important, here is where the information from the step of the particles is gotten 
  // and stored in the variables to be read for the root variables.    
    
  // This method is invoked in LADSteppingAction:UserSteppingAction

  if(aStep)
  {
    //    G4cout << "<HodoAnalysis::StepAnalysis>: begin" << G4endl;
  }

  
  //***************************************************************************************
  // Get the actual track from this step
  // track should be used to extract information of the particle (ID, position)
  // But be careful about quantities as energy. I understand, track gets info of post-step
  // and that could be OUT of the volume of interest
  actualTrack = aStep->GetTrack();
  
  // Get the name of the volume from this track 
  ActualVolume = actualTrack->GetVolume()->GetName();
  
  // this is more "special" (I have to check its use)
  //from THIS track, we look the NEXT volume and if is true, make a handle and get the name
  if (actualTrack->GetNextVolume())
    {
      NextVolume = actualTrack->GetNextVolume()->GetName();
    }
  //***************************************************************************************
  
 
  //To check only primaries
  //note: it still shows the inmediatly daughters
  /*
    if(aStep -> GetTrack() -> GetTrackID()!=1) 
    {
    aStep -> GetTrack()->SetTrackStatus(fStopAndKill);
    }
  */

  
  // collect energy and track length step by step
  G4double edep = aStep->GetTotalEnergyDeposit();

  
  // Control of localization
  // G4cout<<"\nStart of a STEP I am at: "<<actualTrack->GetVolume()->GetName()<<G4endl;

  // First define the handle point
  G4StepPoint*      pre_point = aStep->GetPreStepPoint();

  
  // From it, define the handle of the touchable (I don't think we need this)
  G4TouchableHandle pre_touch = pre_point->GetTouchableHandle();
  G4int             pre_copyNumber = pre_touch->GetCopyNumber();

  G4StepPoint*      post_point = aStep->GetPostStepPoint();
  G4TouchableHandle post_touch = post_point->GetTouchableHandle();
  G4int             post_copyNumber = post_touch->GetCopyNumber();

  // G4cout<<"\nI am at (copy number): "<<pre_copyNumber<<G4endl;
 
  // G4cout<<"and the next copy number is: "<<post_copyNumber<<G4endl;

  
  //***********************************************
  // To disentangle the number of the paddle
  G4int wall, layer, bar;
  
  tie(wall, layer, bar) =  paddle(pre_copyNumber);
  //***********************************************

  // getting the PDG ID of the particle
  G4int PDGID = actualTrack->GetDefinition()-> GetPDGEncoding();

  G4int ParticleLevel = actualTrack-> GetTrackID();

  //  G4cout<<"PDG: "<<PDGID<<" mother/daughter: "<< ParticleLevel<<G4endl;
  //  G4cout << "pre copy number(tuple function): " <<pre_copyNumber<<" "<< wall<<" "<<layer<<" "<<bar<<G4endl;



  
  if(ActualVolume == "Bar_phy")
    {
      //      G4cout << "adep "<<edep<<G4endl;
      EnergyAbs+= edep;
      
      EnergyAbsBar[wall][layer][bar] += edep;
      CopyNumberBar[wall][layer][bar] = pre_copyNumber;
      
      // G4cout << "acummulated edep: "<< EnergyAbsBar[wall][layer][bar] <<G4endl;
      // G4cout << "pre copy number: " << CopyNumberBar[wall][layer][bar] <<G4endl;

      vEneDep.push_back(edep);
      vPadNum.push_back(post_copyNumber);
      vPDG.push_back(PDGID);
      vLevel.push_back(ParticleLevel);
      
      // // WOW!! It seems that this condition will work
      // if (pre_copyNumber != post_copyNumber)
      // 	{ 
      // 	  // get the energy, copy number and store in a variable (maybe a vector)
      // 	  // that will be stored in the rootfile

      // 	  //	  fEventAction->FillEvent(pre_copyNumber, EnergyAbs);
      // 	  EnergyAbs = 0;
	  
      // 	  G4cout<<"HEY!!! I AM OUT!!\n"<<G4endl;
      // 	}
      
    }

//  G4double stepl = 0.;

//   if (aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.)
//     stepl = aStep->GetStepLength();

//   //*********************************************************************************
//   //***** This part of the code gets the number of the object (module, and layer)****
//   //*********************************************************************************

//   // First define the handle point1
//   G4StepPoint* point1 = aStep->GetPreStepPoint();
  
//   // From it, define the handle of the touchable
//   G4TouchableHandle touch1 = point1->GetTouchableHandle();
  
//   // We can get the volume
//   G4VPhysicalVolume* volume = touch1->GetVolume();
  
//   //from this volume, the name
//   G4String name = volume->GetName();
//   G4cout << "touch name: "<<name<<G4endl;
//   // the copy number of the touchable (no from the volume)
//   //    G4int copyNumber = touch1->GetCopyNumber();
  
//   //from the touchable, the mother volume
//   //    G4VPhysicalVolume* mother = touch1->GetVolume(1);

//   //and the Copy Number 1:layer 2:module

//   // G4int copyNumberLayer = touch1->GetCopyNumber(1);
//   // G4cout << "00"<<G4endl;
//   // G4int copyNumberModule = touch1->GetCopyNumber(2);


//   G4int copyNumberBar = touch1->GetCopyNumber(0);
//   //    G4cout<<"Module: "<<copyNumberModule<<G4endl;
//   //    G4cout<<"Layer: "<<copyNumberLayer<<G4endl;

//   //********************************************
//   //*** End of code*****************************
//   //********************************************

//   // Get the real position of the particle when reach the calorimeter
//   // if((actualTrack->GetVolume()->GetName()=="Calorimeter") &&
//   //    (actualTrack->GetNextVolume()->GetName()=="CaloModul"||
//   // 	actualTrack->GetNextVolume()->GetName()=="Layer"||
//   // 	actualTrack->GetNextVolume()->GetName()=="Gap"||
//   // 	actualTrack->GetNextVolume()->GetName()=="Absorber")
//   // 	)
  
//  G4cout << "10"<<G4endl;

//  // G4double  EnergyAbs;
//  //  G4double  EnergyAbsMod[NoMaxModules];

 
//   //this obtains the position of the particle when reaches the calorimeter
//  if(Entry(actualTrack, "SciWall")&& Exit(actualTrack, "World"))
//     {
//       G4cout << "11"<<G4endl;
//       G4ThreeVector Position = actualTrack->GetPosition();
//       //	G4cout<<"Position at Hodo: "<<Position/cm<<G4endl;
//     }
    
//   if(actualTrack->GetVolume()->GetName()=="Bar_phy")
//     {
//       G4cout << "12 "<<edep<<G4endl;
//       EnergyAbs+= edep;
//       G4cout << "121 "<< EnergyAbs <<G4endl;
//       //      EnergyAbsMod[copyNumberBar] += edep;
//       G4cout << "122 "<<copyNumberBar<<G4endl;
//       //    TrackLAbs += stepl;
//     }

//   G4cout << "13"<<G4endl;
//   // Gap=Scintillator, the only energy we can read
//   // if(actualTrack->GetVolume()->GetName()=="Gap")
//   //   {
//   //     EnergyGap+= edep;
//   //     EnergyGapMod[copyNumberModule] += edep;
//   //     EnergyGapModLay[copyNumberModule][copyNumberLayer] += edep;
//   //     TrackLGap += stepl;
//   //   }

//   //*****************************************************************
//   // if we are not interested in background and leaving the hall,
//   // we stop tracking
    
//   if (actualTrack->GetVolume()->GetName()=="SciWall" &&
//       actualTrack->GetNextVolume()->GetName()=="World")
//     {
//       G4cout << "15"<<G4endl;
//       //	if(Variables->Verbose == 1)
//       {
// 	G4cout << "<HodoAnalysis::StepAceptance> : particle leaves hall...: "
// 	       << NextVolume
// 	       << G4endl;
//       }
//       actualTrack->SetTrackStatus(fStopAndKill);
//     } // Particle is killed so can be proceed with other event
    
//   //    G4cout<<"StepAceptance:EnergyAbs: "<<EnergyAbs *MeV<<G4endl;
//   //    G4cout<<"StepAceptance:EnergyGap: "<<EnergyGap *MeV<<G4endl;
    

//   //    if(Variables->Verbose == 1)
//   {
//     G4cout <<"<HodoAnalysis::StepAceptance>: done" << G4endl;
//   }

//   // SHould I return??? Perhaps without returning, the object is not deleted
//   //   return;
//
}







// //Save the data
// void 
// HodoAnalysis::dosave()
// {
//     G4cout << "<HodoAnalysis::dosave> " <<G4endl;  
    
//     if (hittree)   hittree->Write();
//     if (eventtree) eventtree->Write();
//     if (settree)   settree->Write();
// }

/*

TFile *HodoAnalysis::CreateSaveFile(G4String Name)
{

  G4cout<<"IN createsave"<<G4endl;

  ostringstream command;

  //  G4cout<<"1"<<G4endl;
  // Perhaps later I implement an enviroment variable for this
  //  G4String pfad(getenv("KAOSDATADIR"));
    
  //    G4String pfad("./rootfiles/");
        
  G4String dataname;
  //  G4cout<<"2"<<G4endl;
    
  //  G4UImanager * UI = G4UImanager::GetUIpointer();
    
  //  Watch the enviroment variable in the command
  //  dataname = pfad + Name +"-" + Variablen->FileNameSuffix + ".root";
    
  //  This line creates the name of the file with the time stamp
  //  G4cout<<"3"<<G4endl;
  // char buffer[100];
  // G4cout<<"3a"<<G4endl;
  // time_t Now = time(0);	
  // G4cout<<"3b "<< Now<<G4endl;
  // strftime((char*)&buffer,100,"%Y-%m-%d-%H-%M-%S",localtime(&Now));
  // G4cout<<"3c "<< buffer<<G4endl;
  // FileNameSuffix = buffer;

  // auto now = std::time(nullptr);
  // std::ostringstream os;
  // os << std::put_time(std::gmtime(&now),"%F  %T");

  // os.str();

 

  
  auto now = std::chrono::system_clock::now();  
  auto in_time_t = std::chrono::system_clock::to_time_t(now);
  std::stringstream datetime;
  datetime << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
  
  // DateTime
  G4cout<<Name+ "_" + datetime.str()+".root" <<G4endl;

 G4String FileNameSuffix = Name+ "_" + datetime.str()+".root";


  
  // dataname = Name +"-" + FileNameSuffix + ".root";
  //  G4cout<<"4 "<<FileNameSuffix<<G4endl;
  //  dataname = pfad + Name +"-" + Variables->FileNameSuffix + ".root";

  // useless (??)
  // command.str("");
  // command << "/control/shell cp " 
  // 	    << dataname
  // 	    << " " << dataname << ".bak";
  // if (UI) UI->ApplyCommand(command.str());

  TFile *rootoutfile = new TFile(FileNameSuffix, "recreate");
 //  TFile *rootoutfile = new TFile(Name, "recreate");
  //  G4cout<<"5"<<G4endl;
  //    command.str("");
  //     command << "/control/shell ln -fs " 
  // 	    << dataname
  // //	     << " " << pfad << Name << ".root";
  // 	     << " " << Name << ".root";
  //     //if (verbose >0) G4cout << "Output as: " << command.str() << G4endl;
  //     if (UI) UI->ApplyCommand(command.str());


  return rootoutfile;
}



void 
HodoAnalysis::StepStart(const G4Step *aStep)
{
  // This method was used in KAOS to make things related with the tracks
  // perhaps is not useful for us
  // For the moment, it makes nothing



  G4cout << "<HodoAnalysis::StepStart>: begin" << G4endl;
  // Get the actual track from this step

  // REALLY!! pointers all the time??
    G4Track *actualTrack = aStep->GetTrack();

    //  actualTrack = aStep->GetTrack();
  G4cout<< "1"<<G4endl;
  // Get the name of the volume from this track 
 G4String ActualVolume = actualTrack->GetVolume()->GetName();
     G4cout<< "2"<<G4endl;
  // this is more "special" (I have to check its use)
  //from THIS track, we look the NEXT volume and if is true, make a handle and get the name
  if (actualTrack->GetNextVolume())
    {
  G4String NextVolume = actualTrack->GetNextVolume()->GetName();
  G4cout<<"The next: "<<NextVolume<<G4endl;
    }
    
  // I think the AutoCAD aoutput was somewhere else, maybe was only the file
  // here is the data
  
    // if (Variablen->doDrawTracks)
    // {
    // // If we want the output, we add the step point to the line.
    // G4ThreeVector Punkt = aStep->GetPostStepPoint()->GetPosition();
    // DrawTrackKommando << "\\\"" << Punkt.x()/mm << "," << Punkt.y()/mm << "," 
    // << Punkt.z()/mm << "\\\"" << endl;
    // Variablen->doDraw=true;
    // }
  

  G4cout<< "3"<<G4endl;

}

void 
HodoAnalysis::Counter()
{
  //the couter is set to give output AFTER an event. 

  time(&end); //this is the flag end time between events
  
  NbOfEventsC = Run->GetNumberOfEvent()+1;//number of events start at 0 

  //Because I don't know how to read a function which needs and object as argument
  //when the object is defined somewhere and I don't know how to access it
  TotalNnbEvent =  NumberOfEvents(Run);

  time_elapsed = difftime(end, start);

  Even25 = TotalNnbEvent/4;

  RestEventFactor = ((G4double)TotalNnbEvent-NbOfEventsC)/NbOfEventsC;
  resttime = (time_t)(time_elapsed*RestEventFactor);
   
	// G4cout<<"RestEventFactor: "<<RestEventFactor<<G4endl;
	// G4cout<<"time_elapsed: "<<time_elapsed<<G4endl;
	// G4cout<<"resttime: "<<resttime<<G4endl;
  
  G4int rtage    = resttime / (86400);
  G4int rhour    = (resttime % (86400)) / 3600;
  G4int rminutes = (resttime % 3600) / 60;
  G4int rseconds = resttime % 60;

  if(NbOfEventsC == Even25 || 
     NbOfEventsC == 2*Even25 || 
     NbOfEventsC == 3*Even25 || 
     NbOfEventsC == TotalNnbEvent-1)
    {
      G4cout << NbOfEventsC << " Events from " <<TotalNnbEvent  
	     << " processed so far = " 
	     << ((NbOfEventsC*100.0)/TotalNnbEvent)
	     << "%. ";
      G4cout << G4endl;
        
      G4cout<<"ETA: ";
      if (rtage>0)G4cout<<rtage<<" years ";
      if (rhour>0)G4cout<<rhour<<" h: ";
      if (rminutes>0)G4cout<<rminutes<<" m: ";
      G4cout<<rseconds<<" s";
      G4cout<<""<<G4endl;
    }

  //TOTAL TIME EMPLOYED IN THE RUN
  if (NbOfEventsC == TotalNnbEvent)
    {
      resttime = (time_t)(time_elapsed*1.0);
      G4int ry    = resttime / (86400);
      G4int rh    = (resttime % (86400)) / 3600;
      G4int rm = (resttime % 3600) / 60;
      G4int rs = resttime % 60;
	
      G4cout<<"Total time of process: ";
      if (ry>0)G4cout<<ry<<" years ";
      if (rh>0)G4cout<<rh<<" h: ";
      if (rm>0)G4cout<<rm<<" m: ";
      G4cout<<rs<<" s";
      G4cout<<""<<G4endl;
    }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
// These methods just check if particles enter a volume different to the present (Entry)
// or leaves the volume (Exit)

G4bool 
HodoAnalysis::Entry(G4Track* actualTrack,G4String Volumen)
{
  G4cout<<"Entry "<<Volumen<<G4endl;
  G4String ActualVolume = actualTrack->GetVolume()->GetName();
  G4String NextVolume = actualTrack->GetNextVolume()->GetName();
  G4cout<< ActualVolume<<G4endl;
  if ((ActualVolume != Volumen) 
      && (NextVolume == Volumen))
    return true;
  else 
    return false;
}

G4bool 
HodoAnalysis::Exit(G4Track* actualTrack, G4String Volumen)
{
    G4cout<<"Exit "<<Volumen<<G4endl;
    G4String ActualVolume = actualTrack->GetVolume()->GetName();
    G4String NextVolume = actualTrack->GetNextVolume()->GetName();
  if ((ActualVolume == Volumen) 
      && (NextVolume != Volumen))
    return true;
  else 
    return false; 
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....



void 
HodoAnalysis::CheckAkzeptiert()
{
  //Useless!!! In Kaos only works to check if enter the dipole and then accept the use of some detectors
}


void
HodoAnalysis::fillPerEvent(G4double EAbs, G4double EGap,
			   G4double LAbs, G4double LGap)
{

  //    if(Variables->Verbose == 1)
  {
    G4cout<<"<HodoAnalysis::fillPerEvent>:EAbs "<<EAbs *MeV<<G4endl;
  }

  //accumulate statistic
  sumEAbs += EAbs;  
  sum2EAbs += EAbs*EAbs;
  sumEGap += EGap;
  sum2EGap += EGap*EGap;
    
  sumLAbs += LAbs;  sum2LAbs += LAbs*LAbs;
  sumLGap += LGap;  sum2LGap += LGap*LGap;  

  //    if(Variables->Verbose == 1)
  {
    G4cout<<"<HodoAnalysis::fillPerEvent>:sumEAbs: "<<sumEAbs *MeV<<G4endl;
  }
}



  // void HodoAnalysis::InitilizationOfEventAction()
  // {

  // G4cout<<" HodoAnalysis::InitilizationOfEventAction()"<<G4endl;
  // EnergyAbs = EnergyGap = 0.;
  // TrackLAbs = TrackLGap = 0.;

 

  // }


G4int 
HodoAnalysis::NumberOfEvents(const G4Run* aRun)
{
  nbEventInRun = aRun->GetNumberOfEventToBeProcessed();
  return nbEventInRun;
}
*/
/*
  G4int 
  HodoAnalysis::GetNumberOfEvents()
  {
  return nbEventInRun;
  }
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//
//      ----------- HodoAnalysis   ------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
