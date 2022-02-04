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
// G4X3DFile.hh
// Satoshi Tanaka & Yasuhide Sawada

#ifndef G4X3DFILE_HH
#define G4X3DFILE_HH

#include <memory>

#include "G4VGraphicsSystem.hh"
#include "G4String.hh"

class G4VSceneHandler;

#include "G4GenericMessenger.hh"

class G4X3DFile: public G4VGraphicsSystem {
public:

	G4X3DFile();

	virtual ~G4X3DFile();
	G4VSceneHandler* CreateSceneHandler(const G4String& name = "");
	G4VViewer*  CreateViewer(G4VSceneHandler&, const G4String& name = "");

private:

	G4X3DFile(const G4X3DFile&) = delete;
	// Create parameters for this graphics system artefacts

	friend class G4X3DFileSceneHandler;
	friend class G4X3DFileViewer;

	// viewer create parameters

	G4double fX3DFileViewHalfAngle;

	// scene handler create parameters

	G4String fX3DFileDestDir;
	G4String fX3DFileNamePrefix;
	G4int    fX3DFileMaxNum;
	G4int    fX3DFilePickable;

	G4String fX3DFileFormat; // xml || html
  
	G4String fX3DFileCssURL; // css URL
	G4String fX3DFileJsURL; // javascript URL
  
        G4int    fX3DFileWidth;      // canvas size fX3DFileor html fX3DFileormat
        G4int    fX3DFileHeight;

	std::unique_ptr<G4GenericMessenger> fX3DFileMessenger;
};

#endif //G4X3DFILE_HH
