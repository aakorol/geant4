//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: XrayFluoPlanePrimaryGeneratorMessenger.hh
// GEANT4 tag $Name:
//
// Author: Alfonso Mantero (Alfonso.Mantero@ge.infn.it)
//
// History:
// -----------
//  02 Sep 03  Alfonso Mantero created  
//
// -------------------------------------------------------------------


#ifndef XrayFluoPlanePrimaryGeneratorMessenger_h
#define XrayFluoPlanePrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"
//#include "XrayFluoPlanePrimaryGeneratorAction.hh"

class XrayFluoPlanePrimaryGeneratorAction;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class XrayFluoPlanePrimaryGeneratorMessenger: public G4UImessenger
{
  public:
  XrayFluoPlanePrimaryGeneratorMessenger(XrayFluoPlanePrimaryGeneratorAction*);
  ~XrayFluoPlanePrimaryGeneratorMessenger();
  
  void SetNewValue(G4UIcommand*, G4String);
  
private:

  XrayFluoPlanePrimaryGeneratorAction* XrayFluoAction; 

  //command to set a random impact point
  G4UIcmdWithAString*          RndmCmd;

 //command to choose a plane circular source
  G4UIcmdWithAString*          RndmVert;
 
  //command to shot particles according to certain spectra
  G4UIcmdWithAString*        spectrum;

 //command to shot particles from an isotropic source
  G4UIcmdWithAString*        isoVert;
};

#endif
