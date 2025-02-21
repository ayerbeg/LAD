#include "LADPrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "G4RandomDirection.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LADPrimaryGeneratorAction::LADPrimaryGeneratorAction()
{
  // I Have to check THIS!!
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  // default particle kinematic
  //
 
  particle
    = G4ParticleTable::GetParticleTable()->FindParticle("proton");
  //  = G4ParticleTable::GetParticleTable()->FindParticle("geantino");
  //  = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
 
  fParticleGun->SetParticleDefinition(particle);

 
  // For testing, I am using this position. 
  G4double ang = cos(50*deg);
 
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,cos((90+30)*deg)));
 
  
  // G4ThreeVector PartMon =   fParticleGun->GetParticleMomentumDirection();
  // G4cout<<"*************************************************************"<<G4endl;
  // G4cout<<"Particle direction: "<<PartMon.z()<<" angle: "<<ang/deg<<G4endl;
 
  fParticleGun->SetParticleEnergy(300.*MeV);
  // fParticleGun->SetParticleMomentum(300.*MeV);
 
  RadStepCounter=0;
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LADPrimaryGeneratorAction::~LADPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LADPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  G4ThreeVector MomentumDir;

  G4cout<<"IN FACT THIS IS THE BEGGINING OF THE NEW EVENT GeneratePrimaries: "<<anEvent->GetEventID() <<G4endl;
  // This function is called at the begining of event

  
  // In order to avoid dependence of LADPrimaryGeneratorAction
  // on DetectorConstruction class we get world volume
  // from G4LogicalVolumeStore
  //
  G4double worldZHalfLength = 0.;
  auto worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");

  // Check that the world volume has box shape
  G4Box* worldBox = nullptr;
  if (  worldLV ) {
    worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
  }

  if ( worldBox ) {
    worldZHalfLength = worldBox->GetZHalfLength();
  }
  else  {
    G4ExceptionDescription msg;
    msg << "World volume of box shape not found." << G4endl;
    msg << "Perhaps you have changed geometry." << G4endl;
    msg << "The gun will be place in the center.";
    G4Exception("LADPrimaryGeneratorAction::GeneratePrimaries()",
		"MyCode0002", JustWarning, msg);
  }

  // Set gun position
  fParticleGun
    //    ->SetParticlePosition(G4ThreeVector(0., 0., -worldZHalfLength));
    ->SetParticlePosition(G4ThreeVector(0., 0., 0));


   
  switch(Variables->GeneratorCase)
    {
    case 0: //a generator which scan LAD from middle to top
      {
	ScanLAD(anEvent);
        break;
      }

    case 1: //a VERY simplistic circular (just circles) raster
      {
	Raster(anEvent);
	break;
      }
    case 2: // a LUND format reader 
      {
	//	ScanLAD(anEvent); // just for test
	LUND(anEvent);
	break;
      }
    default:
      {
	
	G4cout<<"Generator Default Case"<<G4endl;
	
	G4int steps_ang = 10;
	G4double delta_ang=((150 - 123)/(steps_ang));
	G4double ID_ang=(anEvent->GetEventID()) % steps_ang;
	G4double Angle_scan = 123+ delta_ang*ID_ang;
	G4double phi = G4UniformRand()* 8.*deg -4.*deg;
	
	MomentumDir.setX( cos(phi)*sin(Angle_scan*deg) );
	MomentumDir.setY( sin(phi)*sin(Angle_scan*deg) );
	MomentumDir.setZ( cos(Angle_scan*deg) );

	fParticleGun->SetParticleMomentumDirection(MomentumDir);

	G4cout<<"Particle: "<<fParticleGun->GetParticleDefinition()->GetParticleName()<<G4endl;
	G4cout<<"Momentum: "<<fParticleGun->GetParticleMomentum()<<G4endl;
	G4cout<<"Energy: "  <<fParticleGun->GetParticleEnergy()<<G4endl;
	
	break;
      }
    }
  

  // no need to set the particle position here, when it could be something particular of the case
  //  fParticleGun -> SetParticlePosition( ParticlePosition );
<<<<<<< HEAD

  
  // I think it is better to invoke the vertex generator on each method
  // fParticleGun -> GeneratePrimaryVertex(anEvent);

}


// I think this method needs some cleaning
void LADPrimaryGeneratorAction::ScanLAD(G4Event* anEvent)
{
  G4cout<<"Generator Scan Case" <<G4endl;

  G4ThreeVector MomentumDir;
	

  particle
    = G4ParticleTable::GetParticleTable()->FindParticle(Variables->ScanParticle);



  
  // As the code was designed, these should not be necessary
  RealMomentumDir.set(0, 0, 0);
  MomDirPer.set(0, 0, 0);

  
  if( anEvent->GetEventID()==0)  StepEventTheta = 0;


  //**
  // the angle to scan the bar middle to top
  G4double PhiMax = atan(
			 (Constants->Barlength[Constants->NoOfBars-1]*cm /2.)/ //The largest bar
			 ((Variables->centralWallDistance) + Constants->WallSeparation) // the closest wall
			 );


  G4double PhiStep = PhiMax/20.; // 20 points to scan
  G4int StepEventPhi = (anEvent->GetEventID()) % 20;
       
  G4double phi = PhiStep * StepEventPhi; // start horizontally
  //**

  G4int WhichWall = ( (int)(anEvent->GetEventID())/220)%2;


  // every 220 events, a wall is scanned. With this condition,
  // which was is used 
  if((int) (anEvent->GetEventID())%220 == 0 ) StepEventTheta = 0; //another bad coding! 
    
  if ( WhichWall == 0) // this condition checks we finished a wall
    {
      // Central wall 01
      WallDist = Variables->centralWallDistance;
      WallCentAng =  Variables->centralWallAngle;
    }
  if ( WhichWall == 1) // this condition checks we finished the second wall
    {
      // Right wall 00
      WallDist = Variables->centralWallDistance + (Variables -> leftWallDistance);
      WallCentAng =  Variables->centralWallAngle + (Variables -> leftWallAngle);
    }
  

  
  // From the central angle, I move to one of the side bars.
  // This is the initial point of the horizontal scan


  
  ThetaStep = atan ( Constants->BarWidth/ WallDist);
  G4cout<<"*-*-*-*-*-*-*-THETASTEP: "<<ThetaStep/deg<<G4endl;
  ThetaMax = WallCentAng - (ThetaStep * 5);
  G4cout<<"*-*-*-*-*-*-*-THETAMAX: "<<ThetaMax/deg<<G4endl;
  theta = ThetaMax + (ThetaStep * StepEventTheta);
  G4cout<<"*-*-*-*-*-*-*-THETA: "<<theta/deg<<" StepEventTheta: "<< StepEventTheta<<G4endl;

  
  // Set the momentum vector direction in the X-Z plane to the initial point
  MomentumDir.setX( sin(theta) );
  MomentumDir.setY( 0 );
  MomentumDir.setZ( cos(theta) );

  // I coded an orthogonal vector function, because I didn't get CLHEP function to work
  MomDirPer = orthovector( MomentumDir);
  // I want to rotate wrt a vector which is perpendicular to the direction vector, but in the X-Z plane
  MomDirPer.setY(0);  

  // The step to move from bar to bar
  if((int) (anEvent->GetEventID())%20 == 19) StepEventTheta++;
  //  if((int) (anEvent->GetEventID()) == 219) StepEventTheta = 0; //another bad coding!


  
 
  
=======

  
  // I think it is better to invoke the vertex generator on each method
  // fParticleGun -> GeneratePrimaryVertex(anEvent);

}


// I think this method needs some cleaning
void LADPrimaryGeneratorAction::ScanLAD(G4Event* anEvent)
{
  G4cout<<"Generator Scan Case" <<G4endl;

  G4ThreeVector MomentumDir;
	
  // we use geantino, because this case is not for physics.
  particle
    //  = G4ParticleTable::GetParticleTable()->FindParticle("geantino");
    = G4ParticleTable::GetParticleTable()->FindParticle(Variables->ScanParticle);



  
  // As the code was designed, these should not be necessary
  RealMomentumDir.set(0, 0, 0);
  MomDirPer.set(0, 0, 0);
  if( anEvent->GetEventID()==0)  StepEventTheta =0;

  // the angle to scan the bar middle to top
  G4double PhiMax = atan(
			 (Constants->Barlength[Constants->NoOfBars-1]*cm /2.)/ //The largest bar
			 ((Variables->centralWallDistance) + Constants->WallSeparation) // the closest wall
			 );


  G4double PhiStep = PhiMax/20.; // 20 points to scan
  G4int StepEventPhi = (anEvent->GetEventID()) % 20;
       
  G4double phi = PhiStep * StepEventPhi; // start horizontally
    
  // From the central angle, I move to one of the side bars.
  // This is the initial point of the horizontal scan

  //bad coding. In principle, each wall is scanned 220 events
  // I could set calling run/beamOn 1 to run 440 events
  if( (anEvent->GetEventID()) < 220) // 
    // this is the angle separation, center to center, of each bar in a wall, seem from the vertex
    {
      WallDist = Variables->centralWallDistance;
      WallCentAng =  Variables->centralWallAngle;
    }
  else
    {
      WallDist = Variables->centralWallDistance + (Variables -> leftWallDistance);
      WallCentAng =  Variables->centralWallAngle + (Variables -> leftWallAngle);
    }
  ThetaStep = atan ( Constants->BarWidth/ WallDist);
  ThetaMax = WallCentAng - (ThetaStep * 5); 
  theta = ThetaMax + (ThetaStep * StepEventTheta);

  // Set the momentum vector direction in the X-Z plane to the initial point
  MomentumDir.setX( sin(theta) );
  MomentumDir.setY( 0 );
  MomentumDir.setZ( cos(theta) );

  // I coded an orthogonal vector function, because I didn't get CLHEP function to work
  MomDirPer = orthovector( MomentumDir);
  // I want to rotate wrt a vector which is perpendicular to the direction vector, but in the X-Z plane
  MomDirPer.setY(0);  

  // The step to move from bar to bar
  if((int) (anEvent->GetEventID())%20 == 19) StepEventTheta++;
  if((int) (anEvent->GetEventID()) == 219) StepEventTheta = 0; //another bad coding! 
>>>>>>> origin/debug
  // Define the final vector and rotate it
  RealMomentumDir = MomentumDir;
  RealMomentumDir.rotate(-phi, MomDirPer); // +phi angle goes down the hall 

  fParticleGun -> SetParticleMomentumDirection(RealMomentumDir);

  G4ThreeVector PartMon =   fParticleGun->GetParticleMomentumDirection();
  // G4cout<<"*************************************************************"<<G4endl;
  // G4cout<<"Particle direction: "<<PartMon<<G4endl;
	
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentum(Variables->ScanMomentumParticle);
  fParticleGun->GeneratePrimaryVertex(anEvent);


}


//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

// This method needs to be worked a bit, since it was designed for Nathaly
void LADPrimaryGeneratorAction::Raster(G4Event* anEvent)
{
  G4cout<<"GenerateRaster"<<G4endl;
  // This function is called at the begining of event. A full coverage will need 36*25 (36*24) events. 



  G4double EventStep    = anEvent ->GetEventID();          // The ONLY counter we can use.
  G4double EventAngStep = anEvent->GetEventID()%36 ;// The circle is divided in 36 parts, 10deg each.
  //	G4double EventRadStep = (anEvent->GetEventID())% 25 ;// The circle is divided in 36 parts, 10deg each.

  cout<<"EventStep: "<<EventStep<<" EventAngStep: "<<EventAngStep<<G4endl;

	
  if(EventAngStep == 0 || anEvent->GetEventID() == 0)
    {

      RadStepCounter ++;
	    
      if(RadStepCounter == 25) RadStepCounter = 0; // Including rad=0, needs more code, if not we will have 36 events at (0,0)
    }

  cout<<" RadStepCounter: "<<RadStepCounter<<G4endl;

	
  // We move from external circle inwards
  G4double Ang = 10*deg *EventAngStep ;//Angle
  G4double Rad = 25*mm - (1*mm* RadStepCounter);//Radius
  
  G4double Xpos = Rad * cos(Ang);
  G4double Ypos = Rad * sin(Ang);

  cout<<"Ang: "<<Ang/deg<<" Rad: "<<Rad<<" Xpos: "<<Xpos<<" Ypos: "<<Ypos<<G4endl;
	  

  // ParticlePosition.setX(Xpos);
  // ParticlePosition.setY(Ypos);
  // ParticlePosition.setZ(-1*m);
	
  fParticleGun -> SetParticlePosition( G4ThreeVector(Xpos, Ypos, 0) ); 
  fParticleGun -> SetParticleMomentumDirection( G4ThreeVector(0. ,0., 1) ); //Z-direction of the beam

  G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("e-"); // Corrected! Use different way
  //  G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("gamma"); // Corrected! Use different way
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentum(6.*MeV);
  
  //	fParticleGun->GeneratePrimaryVertex(anEvent);
 


  G4cout<<"Particle: " <<fParticleGun->GetParticleDefinition()->GetParticleName()<<G4endl;
  G4cout<<"Momentum: " <<fParticleGun->GetParticleMomentum()<<G4endl;
  G4cout<<"Energy: "   <<fParticleGun->GetParticleEnergy()<<G4endl;
  G4cout<<"Position: " <<fParticleGun->GetParticlePosition()<<G4endl;

  G4ThreeVector PartMon =   fParticleGun->GetParticleMomentumDirection();
  // G4cout<<"*************************************************************"<<G4endl;
  // G4cout<<"Particle direction: "<<PartMon<<G4endl;
	
  fParticleGun->SetParticleDefinition(particle);
  
  //	fParticleGun->GeneratePrimaryVertex(anEvent);
	
}
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-



void LADPrimaryGeneratorAction::LUND(G4Event* anEvent)
{
  G4cout<<"LUND Event : "<<anEvent->GetEventID()<<G4endl;
  // The event HAS to be read first than EventAction starts
  //  LundRead -> ReadFile();

  if(LundRead -> ReadFile() ) 
    {
      G4cout<<"NPart read: "<< LundRead -> GetnPart()  <<G4endl;
    
      for (G4int ii = 0; ii < LundRead -> GetnPart(); ii++) 
	{
	  // G4cout<< LundRead-> Getindex().at(ii)<<" "<<
	  //   LundRead-> Gett_live().at(ii)<<" "<<
	  //   LundRead-> Gettype().at(ii)<<" "<<
	  //   LundRead-> Getpid().at(ii)<<" "<<
	  //   LundRead-> GetparentInd().at(ii)<<" "<<
	  //   LundRead-> GetdaughtInd().at(ii)<<" "<<
	  //   LundRead-> Getpx().at(ii)<<" "<<
	  //   LundRead-> Getpy().at(ii)<<" "<<
	  //   LundRead-> Getpz().at(ii)<<" "<<
	  //   LundRead-> GetE().at(ii)<<" "<<
	  //   LundRead-> Getm().at(ii)<<" "<<
	  //   LundRead-> Getvx().at(ii)<<" "<<
	  //   LundRead-> Getvy().at(ii)<<" "<<
	  //   LundRead-> Getvz().at(ii)<<G4endl;
	
	  G4ThreeVector LundPartPos(LundRead -> Getvx().at(ii),
				    LundRead -> Getvy().at(ii),
				    LundRead -> Getvz().at(ii));
	
	  G4ThreeVector LundPartMom(LundRead -> Getpx().at(ii),
				    LundRead -> Getpy().at(ii),
				    LundRead -> Getpz().at(ii));
	
	  G4cout<<"PID "<<LundRead ->Getpid().at(ii) <<G4endl;
	
	  G4ParticleDefinition* Lundparticle = G4ParticleTable::GetParticleTable()->FindParticle(LundRead ->Getpid().at(ii));
	
	  G4double LundMomemtum = LundPartMom.mag();
	
	  LundPartMom /= LundMomemtum;// direction momentum vector
	
	  if(Lundparticle) // the example I was running had weird PIDs
	    {
	      fParticleGun -> SetParticleDefinition(Lundparticle);
	      fParticleGun -> SetParticleMomentum(LundMomemtum*GeV);	  
	      fParticleGun -> SetParticleMomentumDirection(LundPartMom);
	      fParticleGun -> SetParticlePosition(LundPartPos);
	    
	      fParticleGun -> GeneratePrimaryVertex(anEvent);
	    }
	}
    }
  else
    {
      G4cout<<"RETURN FALSE"<<G4endl;
<<<<<<< HEAD
      anEvent -> SetEventAborted();
=======
       
>>>>>>> origin/debug
      G4RunManager::GetRunManager() -> AbortRun(false);

    }
  


}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4ThreeVector LADPrimaryGeneratorAction::orthovector(G4ThreeVector dir_vec)
{
  G4ThreeVector dir_vec_ortho;
  dir_vec_ortho.setX( dir_vec.y() + dir_vec.z() );
  dir_vec_ortho.setY( dir_vec.z() - dir_vec.x() );
  dir_vec_ortho.setZ(-dir_vec.x() - dir_vec.y() );
  return dir_vec_ortho;
}


