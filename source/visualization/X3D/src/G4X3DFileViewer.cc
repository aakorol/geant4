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
// G4X3DFileViewer.cc
// Satoshi Tanaka & Yasuhide Sawada


//#define DEBUG_X3D_VIEW

#include <cmath>

#include "G4VisManager.hh"
#include "G4Scene.hh"
#include "G4X3DFileViewer.hh"
#include "G4X3DFileSceneHandler.hh"
#include "G4X3DFile.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "G4TwoVector.hh"

G4X3DFileViewer::G4X3DFileViewer(G4X3DFileSceneHandler& sceneHandler,
				 const G4String& name) :
 G4VViewer(sceneHandler,
	   sceneHandler.IncrementViewCount(),
	   name),
 fSceneHandler(sceneHandler),
 fDest(sceneHandler.fDest)
{
  fViewHalfAngle = fSceneHandler.fSystem.fX3DFileViewHalfAngle * deg;
  fsin_VHA = std::sin ( fViewHalfAngle ) ;	
}

G4X3DFileViewer::~G4X3DFileViewer()
{}

void G4X3DFileViewer::SetView()
{
#if defined DEBUG_X3D_VIEW
  if (G4VisManager::GetVerbosity() >= G4VisManager::errors)
        G4cout << "***** G4X3DFileViewer::SetView(): No effects" << G4endl;
#endif

// Do nothing, since X3D a browser is running as a different process.
// SendViewParameters () will do this job instead.

}

void G4X3DFileViewer::DrawView()
{
#if defined DEBUG_X3D_VIEW
  if (G4VisManager::GetVerbosity() >= G4VisManager::errors)
	G4cout << "***** G4X3DFileViewer::DrawView()" << G4endl;
#endif

	fSceneHandler.X3DBeginModeling() ; 

        // Viewpoint node
        SendViewParameters(); 

	// Here is a minimal DrawView() function.
	NeedKernelVisit();
	ProcessView();
	FinishView();
}

void G4X3DFileViewer::ClearView(void)
{
#if defined DEBUG_X3D_VIEW
  if (G4VisManager::GetVerbosity() >= G4VisManager::errors)
        G4cout << "***** G4X3DFile1View::ClearView()" << G4endl;
#endif
  fSceneHandler.rewindFile();
}

void G4X3DFileViewer::ShowView(void)
{
#if defined DEBUG_X3D_VIEW
  if (G4VisManager::GetVerbosity() >= G4VisManager::errors)
        G4cout << "***** G4X3DFileViewer::ShowView()" << G4endl;
#endif
	fSceneHandler.X3DEndModeling();
}

void G4X3DFileViewer::FinishView(void)
{
#if defined DEBUG_X3D_VIEW
  if (G4VisManager::GetVerbosity() >= G4VisManager::errors)
        G4cout << "***** G4X3DFileViewer::FinishView(): No effects" << G4endl;
#endif
}

void G4X3DFileViewer::SendViewParameters () 
{
  // Calculates view representation based on extent of object being
  // viewed and (initial) direction of camera.  (Note: it can change
  // later due to user interaction via visualization system's GUI.)

#if defined DEBUG_X3D_VIEW
  if (G4VisManager::GetVerbosity() >= G4VisManager::errors)
      G4cout << "***** G4X3DFileViewer::SendViewParameters()\n";
#endif 

	// error recovery
	if ( fsin_VHA < 1.0e-6 ) { return ; } 

	// camera distance
	G4double extentRadius = fSceneHandler.GetScene()->GetExtent().GetExtentRadius();
	G4double cameraDistance = extentRadius / fsin_VHA ;

        const auto targetPoint =
          fSceneHandler.GetScene()->GetStandardTargetPoint() + fVP.GetCurrentTargetPoint();

        const auto direction   = - fVP.GetViewpointDirection().unit(); // this is direction of camera position i guess
        const auto cameraPosition = targetPoint - cameraDistance * direction;

        
#if 0
	// camera position on Z axis
	const G4Point3D&	target_point
	  = fSceneHandler.GetScene()->GetStandardTargetPoint()
	  + fVP.GetCurrentTargetPoint();
	G4double		E_z = target_point.z() + camera_distance;
	G4Point3D		E(0.0, 0.0, E_z );
#endif
	// X3D codes are generated below	
	fDest << G4endl;
	fDest << "<!-- CAMERA -->"   << G4endl;
	fDest << "<Viewpoint";
	fDest << " position=\""           ;
	fDest                 << cameraPosition.x() << " "   ;
	fDest                 << cameraPosition.y() << " "   ;
	fDest                 << cameraPosition.z() << "\"";

        // default camera direction is (0, 0, -1), and we want to orient one to `-direction`
        const auto viewAngle = std::acos(-direction.z());
        if( viewAngle>1e-2 ) {
          const G4TwoVector viewAxis = G4TwoVector(direction.y(), -direction.x()).unit();
          fDest << " orientation=\""           ;
          fDest                 << viewAxis.x() << " "   ;
          fDest                 << viewAxis.y() << " "   ;
          fDest                 <<    "0"          " "   ;
          fDest                 << viewAngle << "\"";
        }

	fDest << "/>"                           << G4endl;
	fDest << G4endl;

}
