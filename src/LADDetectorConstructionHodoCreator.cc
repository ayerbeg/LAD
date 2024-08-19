#include "LADDetectorConstructionHodoCreator.hh"

#include "LADConstants.hh"
#include "LADVariables.hh"

#include "G4VisAttributes.hh"
#include "LADMaterials.hh"
//#include "G4SDManager.hh"


LADDetectorConstructionHodoCreator::LADDetectorConstructionHodoCreator()
{
  G4cout << "<LADDetectorConstructionHodoCreator> Info" << G4endl; 
}


void LADDetectorConstructionHodoCreator::BuildHodo(G4LogicalVolume *worldLV, LADMaterials *Materials)
{

  if ( ! Materials->defaultMaterial || ! Materials->absorberMaterial || ! Materials->gapMaterial ) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined.";
    G4Exception("LADDetectorConstruction::DefineVolumes()",
		"MyCode0001", FatalException, msg);
  }

    
  // 11 Bars, but one frame has 10
  // Order changed for the construction. I could do it in the constructor, but this simpler

  thick  = Constants -> BarThick;
  width  = Constants -> BarWidth ;
  length = Constants -> Barlength[Constants ->NoOfBars-1]*cm;
  //length = Constants -> Barlength[0]*cm;
    
  //CONSTRUCTION OF THE WALL CONTAINING THE BARS

  // This extra cm is not well used. But in the future with more precise measurements,
  // the whole construction could be improved
  WallWidth = (Constants -> NoOfBars * width) + 1*cm; //The total width of the Bars WITHOUT separation + 1 cm spac
  WallThick = thick + 1*cm;
  WallLength = length + 1*cm; // not cool! this only works if we have the Bars are sorted from long to short


  G4cout<<"WallWidth: "<<WallWidth<<" WallLength: "<<WallLength<<G4endl;


  // This is VERY simplistic construction. In fact, the ID of each panel is not quite
  // well determined, thus it needs extra work on how to ID each Bar
  
  // In principle, wall containers have the same dimensions 
  wall
    = new G4Box("Wall",                                  // its name
		WallWidth/2 ,WallLength/2, WallThick/2); // its size 
  
  G4cout<<"WL: "<< WallLength/m<<" WW: "<< WallWidth/m<<" WT: "<< WallThick/m<<G4endl;
    
  
  wallSep = Constants -> WallSeparation;
  
  // The reference system is:
  // Z: direction of the beam (downstream)
  // Y: up to the hall
  // X: to the left looking downstream 
 
  angleW[0] = (Variables -> centralWallAngle) + (Variables -> leftWallAngle);
  angleW[1] =  Variables -> centralWallAngle;
  angleW[2] = (Variables -> centralWallAngle) - (Variables -> rightWallAngle);
   
  distanceW[0] = (Variables -> centralWallDistance) + (Variables -> leftWallDistance);
  distanceW[1] =  Variables -> centralWallDistance;
  distanceW[2] = (Variables -> centralWallDistance) + (Variables -> rightWallDistance);


  
  for(G4int ii = 0; ii < 3; ii++)
    {
      // Angles are measured from Z -> X direction

      vSeparationX.push_back(distanceW[ii]*sin(angleW[ii]) );
      vSeparationY.push_back(0);
      vSeparationZ.push_back( distanceW[ii]*cos(angleW[ii]) );

      vSpaceX.push_back( wallSep*sin(angleW[ii]) );
      vSpaceY.push_back(0);
      vSpaceZ.push_back( wallSep*cos(angleW[ii]) );
    }


  // Each wall has to face the target, so they need to be rotated
  // the same angle as the wall was displaced. Due to the nature
  // of the rotation function, it is necesary to do a rotation per wall
  // independently.

  // I think this part of the code could be compacted, but not sure (CA)
    
  // rmW0.rotateY(-30.*deg);
  // rmW1.rotateY(-30.*deg);
  // rmW2.rotateY(-(23+30)*deg);
  // rmW3.rotateY(-(23+30)*deg);
  // rmW4.rotateY(-(23+23+30)*deg);
  
  rmW0.rotateY(angleW[0]);
  rmW1.rotateY(angleW[0]);
  rmW2.rotateY(angleW[1]);
  rmW3.rotateY(angleW[1]);
  rmW4.rotateY(angleW[2]);

  
  rmW.push_back(rmW0);
  rmW.push_back(rmW1);
  rmW.push_back(rmW2);
  rmW.push_back(rmW3);
  rmW.push_back(rmW4);
   
  // NOMENCLATURE FOR LAD:
  // 3 WALLS with
  // 2 SUBWALLS for a total of
  // 5 PANELS

  G4LogicalVolume *wallLV[Constants ->NoOfPanels];
  
  // probably this line is stupid, since the construction of this is quite rigid regarding the name of subpanels.
  // nevertheless, I'll keep the same philosophy
  G4int SubWalls = Constants -> NoOfSubWalls;

  for (G4int ww = 0; ww < Constants ->NoOfWalls; ww++)
    {
      if(ww>1) SubWalls = 1; // because we have 2 double walls and 1 single.
      //Maybe a while loop could be more efficient
      
      G4cout<<"Wall "<<ww<<G4endl;

      for (G4int ws = 0; ws < SubWalls; ws++)
	{ 
	  G4cout<<"SubWall "<<ws<<G4endl;

	  wallLV[ww+ws+ww]
	    = new G4LogicalVolume(
				  wall,                       // its solid
				  Materials->defaultMaterial, // its material
				  "SciWall");                 // its name
	  
	  wallPV = new G4PVPlacement(G4Transform3D(
						   rmW[ww+ws+ww],
						   (G4ThreeVector( vSeparationX[ww], vSeparationY[ww], vSeparationZ[ww]) +
						    G4ThreeVector( vSpaceX[ww]*ws, vSpaceY[ww]*ws, vSpaceZ[ww]*ws  )
						    )),
				     wallLV[ww+ws+ww],               // its logical volume
				     "SciWall",                      // its name
				     worldLV,                        // its mother  volume
				     false,                          // no boolean operation
				     ww+ws+ww,                       // copy number
				     fCheckOverlaps);

	  
	  // There is something magic calling with the same name each solid and logic
	  // volume, although they are different on each loop, without deleting the
	  // previous one
	  
	  for (G4int pp = 0; pp < Constants ->NoOfBars; pp++)
	    {
	      BarS = new G4Box("Bar_box", // its name
			       width/2, Constants ->Barlength[pp]/2*cm,  thick/2); // its size
	      
	      BarLV = new G4LogicalVolume(BarS,       // its solid
					  Materials->BC408,  // its material
					  "Bar_LV");         // its name
	      
	      //     G4cout<<"pos: "<<width*pp<<" wall: "<<ww+ws<<G4endl;
	      
	      new G4PVPlacement(nullptr,                                                    // no rotation
				G4ThreeVector( ((WallWidth/2 - width/2)- width*pp),0, 0),  // its position
				BarLV,                                                      // its logical volume
				"Bar_phy",                                                      // its name
				wallLV[ww+ws+ww],                                           // its mother volume
				false,                                                      // no boolean operation
				(ww*10000)+(ws*100)+pp,                                     // copy number
				fCheckOverlaps);                                            // checking overlaps
	      //NOTE: with this copy number, each Bar has an unique ID
	      
	    }
	  if(ww+ws+ww == 5) break; // just 5 walls
	}


   
      //      wallLV[ww]->SetVisAttributes(G4VisAttributes::GetInvisible()); //(G4VisAttributes(G4Colour::Green()));
      
    }

      wallLV[0]->SetVisAttributes(G4VisAttributes(G4Colour::Green()));
      wallLV[1]->SetVisAttributes(G4VisAttributes(G4Colour::Blue()));
      wallLV[2]->SetVisAttributes(G4VisAttributes(G4Colour::Red()));
      wallLV[3]->SetVisAttributes(G4VisAttributes(G4Colour::Yellow()));
      wallLV[4]->SetVisAttributes(G4VisAttributes(G4Colour::White()));


      return;
}


void LADDetectorConstructionHodoCreator::ConstructSDandField()
{

  G4cout<<" LADDetectorConstructionHodoCreator::ConstructSDandField() START"<<G4endl;
  /*
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  //
  // Sensitive detectors
  //
 LADHodoSD *barSD
   = new LADHodoSD("barSD", "LADbarsHitsCollection"); 
  G4SDManager::GetSDMpointer()->AddNewDetector(barSD);
  BarLV-> SetSensitiveDetector(barSD);

  */
}


LADDetectorConstructionHodoCreator::~LADDetectorConstructionHodoCreator()
{
    G4cout << "<~LADDetectorConstructionHodoCreator> --> DONE" << G4endl; 
}
