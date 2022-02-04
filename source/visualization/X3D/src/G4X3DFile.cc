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
// G4X3DFile.cc
// Satoshi Tanaka & Yasuhide Sawada

#include <stdio.h> // sscanf
#include <stdlib.h> // getenv

#include "G4VSceneHandler.hh"

#include "G4X3DFile.hh"
#include "G4X3DFileSceneHandler.hh"
#include "G4X3DFileViewer.hh"

#include "G4GenericMessenger.hh"

#include "boost/lexical_cast.hpp"

G4X3DFile::G4X3DFile()
: G4VGraphicsSystem("X3DFILE", G4VGraphicsSystem::fileWriter)

, fX3DFileViewHalfAngle(30.0)

, fX3DFileDestDir("")
, fX3DFileNamePrefix("g4_")
, fX3DFileMaxNum(3)
, fX3DFilePickable(0)

, fX3DFileFormat("html")

, fX3DFileCssURL("https://www.x3dom.org/download/x3dom.css")
, fX3DFileJsURL("https://www.x3dom.org/download/x3dom.js")

, fX3DFileWidth(640)
, fX3DFileHeight(480)

, fX3DFileMessenger(new G4GenericMessenger(this, "/vis/x3dfile/", "X3DFILE related parameters"))

{
  fX3DFileMessenger->DeclareProperty("viewHalfAngle", fX3DFileViewHalfAngle, "set view half angle")
    .SetDefaultValue(boost::lexical_cast<std::string>(fX3DFileViewHalfAngle))
    ;

  fX3DFileMessenger->DeclareProperty("fileNamePrefix", fX3DFileNamePrefix, "set file name prefix")
    .SetDefaultValue(fX3DFileNamePrefix)
    ;

  fX3DFileMessenger->DeclareProperty("fileMaxNum", fX3DFileMaxNum, "maximum number for generated file names")
    .SetDefaultValue(boost::lexical_cast<std::string>(fX3DFileMaxNum))
    ;

  fX3DFileMessenger->DeclareProperty("pickable", fX3DFilePickable, "pickable if not zero")
    .SetDefaultValue("0")
    ;

  fX3DFileMessenger->DeclareProperty("directory", fX3DFileDestDir, "set destination directory")
    .SetDefaultValue(fX3DFileDestDir)
    ;

  fX3DFileMessenger->DeclareProperty("fileFormat", fX3DFileFormat, "set file format: html (HTML+x2dom) or xml ")
    .SetCandidates("html xml")
    .SetDefaultValue("html")
    ;

  fX3DFileMessenger->DeclareProperty("cssURL", fX3DFileCssURL, "css URL for html format instead of x3dom.org")
    .SetDefaultValue(fX3DFileCssURL)
    ;

  fX3DFileMessenger->DeclareProperty("jsURL", fX3DFileJsURL, "javascript URL for html format instead of x3dom.org")
    .SetDefaultValue(fX3DFileJsURL)
    ;

  fX3DFileMessenger->DeclareProperty("width", fX3DFileWidth, "set render width in pixels for html format")
    .SetDefaultValue(boost::lexical_cast<std::string>(fX3DFileWidth))
    ;

  fX3DFileMessenger->DeclareProperty("height", fX3DFileHeight, "set render height in pixels for html format")
    .SetDefaultValue(boost::lexical_cast<std::string>(fX3DFileHeight))
    ;
}

G4X3DFile::~G4X3DFile()
{
}


G4VSceneHandler* G4X3DFile::CreateSceneHandler(const G4String& name) 
{
	G4VSceneHandler *p = NULL;

	p = new G4X3DFileSceneHandler(*this, name);

	return p;
}

G4VViewer* G4X3DFile::CreateViewer(G4VSceneHandler& scene, const G4String& name)
{
	G4VViewer* pView = NULL;

	G4X3DFileSceneHandler* pScene = (G4X3DFileSceneHandler*)&scene;
	pView = new G4X3DFileViewer(*pScene, name);

	return pView;
}
