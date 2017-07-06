#include "OTDetectorConstruction.hh"
#include "G4SubtractionSolid.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UniformElectricField.hh"
//#include "G4VisAttributes.hh"

OTDetectorConstruction::OTDetectorConstruction()
: G4VUserDetectorConstruction()
{
}

OTDetectorConstruction::~OTDetectorConstruction()
{
}

G4VPhysicalVolume* OTDetectorConstruction::Construct()
{  
  G4NistManager* nist = G4NistManager::Instance();


  // -----------------------------------------------------
  // World

  G4Material* world_mat = nist -> FindOrBuildMaterial("G4_AIR");
  G4double world_size = 1000*mm;

  G4Box* solidWorld =    
    new G4Box("World",                       // its name
              5*world_size,                // half x
              5*world_size,                // half y
              5*world_size);               // half z
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      true);                 //overlaps checking


  // -----------------------------------------------------
  // Detector

  G4Material* scintillator_mat = nist -> FindOrBuildMaterial("G4_XYLENE");
  G4Material* veto_mat = nist -> FindOrBuildMaterial("G4_POLYETHYLENE");
  G4Material* tube_mat = nist -> FindOrBuildMaterial("G4_Pyrex_Glass");
  G4double detector_size = 1*mm;
  G4double scintillator_offset_z = (63.5/2+6)*mm;
  G4double veto_offset_z = 5*mm;
  G4double tube_offset_z = 6*mm;

  
  G4double sizeX = 1000 * detector_size;
  G4double sizeY = 76.2 * detector_size;
  G4double sizeZ = 63.5 * detector_size;
//hollow pyrex glass tube
  G4Box* outerTube =    
    new G4Box("outerTube",
              sizeX/2,
              sizeY/2,
              sizeZ/2);

  G4Box* innerTube = 
  	new G4Box("innerTube",
			  (1000*mm-2*mm)/2,
			  (76.2*mm-2*mm)/2,
			  (63.5*mm-2*mm)/2);

  G4SubtractionSolid*hollowTube = 
  	new G4SubtractionSolid("hollowTube",
							outerTube,
							innerTube);
  
  G4LogicalVolume* logicalDetector = 
    new G4LogicalVolume(hollowTube,
                        tube_mat,
                        "hollowTube");
    new G4PVPlacement(0,
                      G4ThreeVector(0,0,tube_offset_z),
                      logicalDetector,
                      "hollowTube",
                      logicWorld,
                      false,
                      1,  
                      true);
//liquide Scintillator
  G4Box* lScintillator =
    new G4Box("lScintillator",
              (1000*mm-2*mm)/2,
              (76.2*mm-2*mm)/2,
              (63.5*mm-2*mm)/2);     
 
  G4LogicalVolume* logicalDetector1 =                         
    new G4LogicalVolume(lScintillator,
                        scintillator_mat,
                        "lScintillator");
                                   
    new G4PVPlacement(0,
                      G4ThreeVector(0,0,scintillator_offset_z),
                      logicalDetector1,
                      "lScintillator",
                      logicWorld,
                      false,
                      1,
                      true);
//Veto Wall
  G4Box* veto =    
    new G4Box("veto",
              sizeX/2,
              sizeY/2,
              10*mm/2);
      
  G4LogicalVolume* logicalDetector2 =                         
    new G4LogicalVolume(veto,
                        veto_mat,
                        "veto");
                                   
    new G4PVPlacement(0,
                      G4ThreeVector(0,0,veto_offset_z),
                      logicalDetector2,
                      "veto",
                      logicWorld,
                      false,
                      12,
                      true);

  return physWorld;
}
