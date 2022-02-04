#------------------------------------------------------------------------------
# Module : G4X3D
# Package: Geant4.src.G4visualization.G4X3D
#------------------------------------------------------------------------------

#
# Module has optional sources
#

# List those always built
set(G4VIS_X3D_MODULE_HEADERS
  G4X3DFile.hh
  G4X3DFileSceneHandler.hh
  G4X3DFileViewer.hh)

set(G4VIS_X3D_MODULE_SOURCES
  G4X3DFile.cc
  G4X3DFileSceneHandler.cc
  G4X3DFileViewer.cc)

#
# Define the Geant4 Module.
#
geant4_define_module(NAME G4X3D
  HEADERS
    ${G4VIS_X3D_MODULE_HEADERS}
  SOURCES
    ${G4VIS_X3D_MODULE_SOURCES}
  GRANULAR_DEPENDENCIES
    G4csg
    G4geometrymng
    G4globman
    G4graphics_reps
    G4hits
    G4intercoms
    G4modeling
    G4specsolids
    G4vis_management
  GLOBAL_DEPENDENCIES
    G4digits_hits
    G4geometry
    G4global
    G4graphics_reps
    G4intercoms
    G4modeling
    G4vis_management
)

# List any source specific properties here
