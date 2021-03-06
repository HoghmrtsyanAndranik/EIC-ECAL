#include "DetectorConstruction.hh"
#include "DetectorSD.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"


#include <vector>
#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4GenericMessenger.hh"
G4ThreadLocal 
G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = 0;


DetectorConstruction::DetectorConstruction()
:   G4VUserDetectorConstruction(),
    fCheckOverlaps(true),
    NofLayers(-1)
{
}

DetectorConstruction::~DetectorConstruction()
{ 
}  

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define materials 
  DefineMaterials();
  
  // Define volumes
  return DefineVolumes();
}
void DetectorConstruction::DefineMaterials()
{
// auto nistManager = G4NistManager::Instance();
  G4String name, symbol;
  G4double a;  // mass of a mole;
  G4double z;  // z=mean number of protons;  
  G4double density; 
  G4int ncomponents,natoms;

  a = 16.00 * g / mole;
    G4Element *elO = new G4Element(name = "Oxygen", symbol = "O", z = 8., a);

  a = 183.84 * g / mole;
    G4Element *elW = new G4Element(name = "Tungsten", symbol = "W", z = 74., a);

  a = 207.20 * g / mole;
    G4Element *elPb = new G4Element(name = "Lead", symbol = "Pb", z = 82., a);

  a = 28.09 * g / mole;
    G4Element *elSi = new G4Element(name = "Silicone", symbol = "Si", z = 14., a);
  
  a = 137.33 * g / mole;
    G4Element *elBa = new G4Element(name = "Barium", symbol = "Ba", z = 56., a);

  // Vacuum
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                  kStateGas, 2.73*kelvin, 3.e-18*pascal);
  auto PbWO4 = new G4Material("PbWO4",density = 8.280 * g / cm3,ncomponents = 3);
  PbWO4->AddElement(elO, natoms = 4);
  PbWO4->AddElement(elW, natoms = 1);
  PbWO4->AddElement(elPb, natoms = 1);

  auto DSBCe = new G4Material(name = "DSBCe", density = 3.8 * g / cm3, ncomponents = 3);
    DSBCe->AddElement(elO, natoms = 5);
    DSBCe->AddElement(elBa, natoms = 1);
    DSBCe->AddElement(elSi, natoms = 2);
}

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{




    G4double RIn = 15 * cm;
    G4double ROut = 120 *cm;
    //G4double ROutshift = 5 * cm;
    G4double Thickness = 40 * cm;
    //double SizeZ;
    G4double PosZ = 0;

    //............... EMCAL Crystals modules ......................
    G4double PWO_Thickness = 40. * cm;
    G4double PWO_Width = 2. * cm;
    G4double PWO_Gap = 0.01 * mm;
    G4double PWO_InnerR = 15. * cm;
    G4double PWO_OuterR = 82. * cm;
    G4double PWO_PosZ = 0;

 /*   //............... EMCAL Glass modules ......................
    double Glass_Thickness = 40. * cm;
    double Glass_Width = 4. * cm;
    double Glass_Gap = 0.01 * mm;
    double Glass_InnerR = 15.*cm;
    double Glass_OuterR = 82.*cm;
    double Glass_PosZ = 0.;
    G4GenericMessenger *Messenger;

            // Set geant options
            Messenger = new G4GenericMessenger(this,"/src/");
            Messenger->DeclareProperty("pwoThickness", PWO_Thickness, "Thikness (z direction dimention) of PWO crystals ");
            Messenger->DeclareProperty("pwoWidth", PWO_Width, "Width (and higth) of each PWO crystal");
            Messenger->DeclareProperty("pwoGap", PWO_Gap, "Gap between PWO crystals ");
            Messenger->DeclareProperty("pwoInnerR", PWO_InnerR, "Inner radius or beam hole for PWO block");
            Messenger->DeclareProperty("pwoOuterR", PWO_OuterR, "Outer radius of PWO block");

            Messenger->DeclareProperty("glassThickness", Glass_Thickness, "Thikness (z direction dimention) of Glass modules");
            Messenger->DeclareProperty("glassWidth", Glass_Width, "Width (and higth) of each Glass modules");
            Messenger->DeclareProperty("glassGap", Glass_Gap, "Gap between Glass modules ");
        
*/
    

    auto worldSizeXY = 2*ROut;
    double worldSizeZ = 3.*m;
    //................Get Materials.................................

    auto PWO_Material = G4Material::GetMaterial("PbWO4");
    auto Glass_Material = G4Material::GetMaterial("DSBCe");
    auto defaultMaterial = G4Material::GetMaterial("Galactic");
    
    if ( ! PWO_Material || ! Glass_Material || ! defaultMaterial ) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined."; 
    G4Exception("DetectorConstruction::DefineVolumes()",
      "MyCode0001", FatalException, msg);
  }  
    //World

    auto worldS = new G4Box("World",worldSizeXY/2,worldSizeXY/2,worldSizeZ/2);
    auto worldLV = new G4LogicalVolume(worldS,defaultMaterial,"World");
    auto worldPV = new G4PVPlacement(0,G4ThreeVector(),worldLV,"World",0,false,0,fCheckOverlaps);

   /* //Calorimeter
    auto CalorS = new G4Tubs("Calorimeter",RIn,ROut,Thickness/2.,0.,360* deg);
    auto CalorLV = new G4LogicalVolume(CalorS,PWO_Material,"CalorimeterLV");
    new G4PVPlacement(0,G4ThreeVector(0,0,PosZ),CalorLV,"Calorimeter",worldLV,false,0);*/

    

//    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, PWO_PosZ),PWO_LV,"anunik",CalorLV, false, fCheckOverlaps);


    //Visualization
    auto worldVisAttr = new G4VisAttributes(G4Color(1.0, 1.0, 1.0));
    worldVisAttr->SetLineWidth(1);
    worldVisAttr->SetForceSolid(false);
    worldLV->SetVisAttributes(worldVisAttr);
   /* 
    auto CalorVisAttr = new G4VisAttributes(G4Color(1.0, 1.0, 0.9));
    CalorVisAttr->SetLineWidth(1);
    CalorVisAttr->SetForceSolid(false);
    CalorLV->SetVisAttributes(CalorVisAttr);

*/

    //  Crystalner arandzin


    
    auto PWO_Solid = new G4Box("Crystal",PWO_Width * 0.5 , PWO_Width * 0.5 ,PWO_Thickness * 0.5);
    auto PWO_LV = new G4LogicalVolume(PWO_Solid,PWO_Material,"CrystalLV");

    
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
                    std::stringstream sstm;
                    sstm << "pwo_phys_" << i*j;
                    std::string name = sstm.str();
                    double x = i*PWO_Width;
                    double y = j*PWO_Width;
                    new G4PVPlacement(nullptr, G4ThreeVector(x-PWO_Width,y-PWO_Width, PWO_PosZ), PWO_LV,name, worldLV, false, fCheckOverlaps);
        }
    }

    
    /*
    static char abname[256];
    PWO_PosZ = Thickness / 2 - PWO_Thickness / 2;

    auto PWO_Solid = new G4Box("Crystal",PWO_Width * 0.5 , PWO_Width * 0.5, PWO_Thickness * 0.5);
    auto PWO_LV = new G4LogicalVolume(PWO_Solid,PWO_Material,"CrystalLV");

    //Visualization
    
    auto CrystalVisAttr = new G4VisAttributes(G4Color(0.3, 0.5, 0.9, 0.9));
    CrystalVisAttr->SetLineWidth(1);
    CrystalVisAttr->SetForceSolid(false);
    PWO_LV->SetVisAttributes(CrystalVisAttr);
    
    //Crystals
    double y_C = 0;
    double x_C;
    G4int k = -1;
    
    for (int j = 0; j < 50; j++) {
        y_C = j==0 ? PWO_Gap : y_C - PWO_Width + PWO_Gap;
        x_C = (PWO_Width + PWO_Gap) * 0.5;

        for (int i = 0; i < 50 ; i++){
            double R = sqrt(x_C * x_C + y_C * y_C);

            if(R < PWO_OuterR - PWO_Width + PWO_Gap && R > PWO_InnerR){
                
                double x,y;
                k++;
                sprintf(abname, "PW_Ph_%d", k);
                    x=x_C; y=y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, PWO_PosZ),PWO_LV,abname,CalorLV, false, fCheckOverlaps);

                k++;
                sprintf(abname, "PW_Ph_%d", k);
                    x=-x_C; y=y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, PWO_PosZ),PWO_LV,abname,CalorLV, false, fCheckOverlaps);

                k++;
                sprintf(abname, "PW_Ph_%d", k);
                    x=x_C; y=-y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, PWO_PosZ),PWO_LV,abname,CalorLV, false, fCheckOverlaps);

                k++;
                sprintf(abname, "PW_Ph_%d", k);
                    x=-x_C; y=-y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, PWO_PosZ),PWO_LV,abname,CalorLV, false, fCheckOverlaps);

            }
        }
    }

    static char abname2[256];
    Glass_OuterR = ROut -1*cm;
    Glass_InnerR = PWO_OuterR + 3*cm;
    Glass_PosZ = Thickness/2 - Glass_Thickness/2.;
    
    auto GLASS_S = new G4Box("Glass_solid",Glass_Width * 0.5,Glass_Width * 0.5,Glass_Thickness *0.5);
    auto GLASS_LV = new G4LogicalVolume(GLASS_S,Glass_Material,"Glass_LV");
    
    //Visualization
    auto GlassVisAttr = new G4VisAttributes(G4Color(0.3, 0.4, 1., 0.5));
    GlassVisAttr->SetLineWidth(1);
    GlassVisAttr->SetForceSolid(false);
    GLASS_LV->SetVisAttributes(GlassVisAttr);
    
    y_C = 0;
    x_C = 0.;
    k = -1;

    for (int j = 0; j < 50; j++) {
            if (j == 0) { y_C = Glass_Gap; }
            else { y_C -= Glass_Width + Glass_Gap; }
            x_C = (Glass_Width + Glass_Gap) * 0.5;

        for (int i = 0; i < 50; i++) {
            double R = sqrt(x_C * x_C + y_C * y_C);

         if (R < Glass_OuterR - Glass_Width + Glass_Gap && R > Glass_InnerR) {
                    double x,y;
                    k++;
                    sprintf(abname2, "GLA_Ph_%d", k);
                    x=x_C; y=y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, Glass_PosZ),GLASS_LV,abname2,CalorLV, false, fCheckOverlaps);
                    
                    k++;
                    sprintf(abname2, "GLA_Ph_%d", k);
                    x=x_C; y=-y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, Glass_PosZ),GLASS_LV,abname2,CalorLV, false, fCheckOverlaps);

                    k++;
                    sprintf(abname2, "GLA_Ph_%d", k);
                    x=x_C; y=-y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, Glass_PosZ),GLASS_LV,abname2,CalorLV, false, fCheckOverlaps);

                    k++;
                    sprintf(abname2, "GLA_Ph_%d", k);
                    x=-x_C; y=-y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, Glass_PosZ),GLASS_LV,abname2,CalorLV, false, fCheckOverlaps);
        }
    }
    }
      PWO_PosZ = Thickness / 2 - PWO_Thickness / 2;  

      PWO_Solid = new G4Box("Crystal", PWO_Width * 0.5, PWO_Width * 0.5, PWO_Thickness * 0.5);
    //  PWO_LV = new G4LogicalVolume(PWO_Solid,PWO_Material,"CrystalLV");

      double diameter = 2 * PWO_OuterR;

      int towersInRow = std::ceil((diameter + PWO_Gap) /  (PWO_Width + PWO_Gap));

      double leftTowerPos, topTowerPos;
        if(towersInRow%2) {
            int towersInHalfRow = std::ceil(towersInRow/2.0);
            leftTowerPos = topTowerPos = -towersInHalfRow * (PWO_Width + PWO_Gap);
        }
        else{
            int towersInHalfRow = towersInRow/2;
            leftTowerPos = topTowerPos = -(towersInHalfRow - 0.5) * (PWO_Width + PWO_Gap);

        }
       int towerIndex = 0;
       for(int colIndex=0; colIndex < towersInRow; colIndex++) {
            for(int rowIndex=0; rowIndex < towersInRow; rowIndex++) {
                double x = leftTowerPos + colIndex * (PWO_Width + PWO_Gap);
                double y = topTowerPos + rowIndex * (PWO_Width + PWO_Gap);

                if ((std::abs(y) < PWO_OuterR && std::abs(x) < PWO_OuterR) &&
                    (std::abs(y) > PWO_InnerR || std::abs(x) > PWO_InnerR))
                {
                    int code = 1 * rowIndex + colIndex;
                  //  std::string name(fmt::format("pwo_phys_{}", code));
              
                 //   std::string name = "pwo_ph_" + std::to_string(code);
                    std::stringstream sstm;
                    sstm << "pwo_phys_" << code;
                    std::string name = sstm.str();
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, PWO_PosZ), PWO_LV,name, CalorLV, false, fCheckOverlaps);
                    towerIndex++;
                }
            }
        }


    using namespace std;

    RIn = 15 * cm;
    ROut = 100 *cm;
   // G4double ROutshift = 5 * cm;
    Thickness = 40 * cm;
    //double SizeZ;
    PosZ = 0;

    //............... EMCAL Crystals modules ......................
    PWO_Thickness = 20. * cm;
    PWO_Width = 2. * cm;
    PWO_Gap = 0.01 * mm;
    PWO_InnerR = 15. * cm;
    PWO_OuterR = 82. * cm;
    PWO_PosZ = 0.;

    //............... EMCAL Glass modules ......................
    Glass_Thickness = 40. * cm;
    Glass_Width = 4. * cm;
    Glass_Gap = 0.01 * mm;

    //G4double Glass_InnerR = 30.*cm;
    //G4double Glass_OuterR = 100.*cm;
    //G4double Glass_PosZ = 0.;
    
    Glass_OuterR = ROut - 1*cm ;
    Glass_InnerR = PWO_OuterR + 3*cm;
    Glass_PosZ = Thickness / 2. - Glass_Thickness / 2.;

    GLASS_S = new G4Box("Glass_solid",Glass_Width * 0.5,Glass_Width * 0.5,Glass_Thickness *0.5);
    GLASS_LV = new G4LogicalVolume(GLASS_S,Glass_Material,"Glass_LV");

    diameter = 2 * Glass_OuterR;

    towersInRow = std::ceil((diameter + Glass_Gap) /  (Glass_Width + Glass_Gap));

    if(towersInRow%2) {
            int towersInHalfRow = std::ceil(towersInRow/2.0);
            topTowerPos = leftTowerPos = -towersInHalfRow * (Glass_Width + Glass_Gap);

        } else {
            int towersInHalfRow = towersInRow/2;
            topTowerPos = leftTowerPos = -(towersInHalfRow - 0.5) * (Glass_Width + Glass_Gap);
        }
   // int moduleIndex = 0;

    towerIndex = 0;
        for(int rowIndex=0; rowIndex < towersInRow; rowIndex++) {
            for(int colIndex=0; colIndex < towersInRow; colIndex++) {
                double x = leftTowerPos + colIndex * (Glass_Width + Glass_Gap);
                double y = topTowerPos + rowIndex * (Glass_Width + Glass_Gap);
                double r = sqrt(x * x + y * y);


                if (r < Glass_OuterR && (std::abs(x) > Glass_InnerR || std::abs(y) > Glass_InnerR)) {
                    int code = 1 * rowIndex + colIndex;

                   // std::string name(fmt::format("glass_phys_{}", code));

                  //  fastformat::write(name, "gla_phys_", code);
                   // std::string name = "gla_phys_" + std::to_string(code);

                    std::stringstream sstm;
                    sstm << "glass_phys_" << code;
                    std::string name = sstm.str();
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, Glass_PosZ), GLASS_LV, name, CalorLV, false, fCheckOverlaps);
                    towerIndex++;
                }
            }
        }
*/
NofLayers = 100;

return worldPV;
}
void DetectorConstruction::ConstructSDandField()
{

  auto CrystalSD= new DetectorSD("CrystalSD", "CrystalHitsCollection",NofLayers);
  G4SDManager::GetSDMpointer()->AddNewDetector(CrystalSD);
  SetSensitiveDetector("CrystalLV",CrystalSD);
  

  G4ThreeVector fieldValue;
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);

  G4AutoDelete::Register(fMagFieldMessenger);
}