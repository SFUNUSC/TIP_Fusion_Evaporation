#ifndef Materials_H
#define Materials_H 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

using namespace CLHEP;

class Materials 
{
  public:

  Materials();
  ~Materials();
  
  G4Material* FindMaterial(G4String );
    
    private:

  // Elemental materials
  G4Material* Al;
  G4Material* Ge;
  G4Material* Cu;
  G4Material* C;
  G4Material* Mg;
  G4Material* al;
  G4Material* In;
  G4Material* Hf;
  G4Material* Au;
  G4Material* Ca;
  G4Material* Ta;
  
  // Compounds
  G4Material* Vacuum;
  G4Material* BGO;
  G4Material* CsI;
  G4Material* Mylar;
  G4Material* Air;
  
  //User-defined
  G4Material* UserCarbon;
  G4Material* UserGold;

};

#endif

