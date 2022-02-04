//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//
// G4X3DFileViewer.hh
// Satoshi Tanaka and Yasuhide Sawada

#ifndef G4X3DFILE_VIEWER_HH
#define G4X3DFILE_VIEWER_HH

#include <fstream>
#include <memory>

#include "G4VViewer.hh"
#include "globals.hh"

class G4GenericMessenger;
class G4X3DFileSceneHandler;

class G4X3DFileViewer: public G4VViewer {
public:
	G4X3DFileViewer(G4X3DFileSceneHandler& scene, const G4String& name = "");
	virtual ~G4X3DFileViewer();
	void ClearView();
	void DrawView();
	void ShowView();
	void FinishView();
private:
	void SetView(); // Do nothing. SendViewParameters will do its job.
	void SendViewParameters ()  ;

private:
	G4X3DFileSceneHandler& fSceneHandler; // Reference to Graphics Scene for this view.

	std::ofstream&         fDest ;

	G4double      fViewHalfAngle ;	
	G4double      fsin_VHA       ;	
};

#endif //G4X3DFILE_VIEWER_HH
