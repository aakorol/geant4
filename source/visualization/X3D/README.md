# X3DFILE Visualization graphics system for Geant4

This is a minimal rewrite of VRML2FILE driver which stores scene
as X3D XML file: plain or HTML-X3DOM page wrapped.

The motivation for this rewrite is to have trivial geant4 visualization in
Jupyter notebook for simple educational Geant4 exercises.

Some enhancement besides the rewrite:

  * set create viewer/scenehandler parameters with UI messenger (directory `/vis/x3dfile`);
  * use BOX element instead of MultiFacet for G4Box solid.

UI commands:

  * /vis/x3dfile/viewHalfAngle <double>  - set view half angle, for calculation initial camera position, default=30deg
  * /vis/x3dfile/fileNamePrefix <string> - set file name prefix, default="g4_"
  * /vis/x3dfile/fileMaxNum <int>        - set maximum number for generated file names, default=3
  * /vis/x3dfile/pickable <0|1>          - pickable flag (replaces getenv variable)
  * /vis/x3dfile/directory <string>      - set destination directory (replaces getenv variable), default=""
  * /vis/x3dfile/fileFormat <xml|html>   - set file format: html (HTML+x2dom) or xml, default="html"
  * /vis/x3dfile/cssURL <string>         - set css URL for html format, default="https://www.x3dom.org/download/x3dom.css"
  * /vis/x3dfile/jsURL  <string>         - set javascript URL for html format, default="https://www.x3dom.org/download/x3dom.js"
  * /vis/x3dfile/width  <int>            - set render width in pixels for html format, default=640
  * /vis/x3dfile/height <int>            - set render height in pixels for html format, default=480


Example cells for Jupyter notebook:

* set graphics system
```
Geant4.gApplyUICommand("/vis/open X3DFILE")
```
* create scene, add elements, enable visualization, generate events
* show scene
```
import IPython.display
with open("g4_00.html", "r") as x3dfile:
        data = x3dfile.read()
IPython.display.HTML(data)   
```

Simple example at https://github.com/aakorol/korol-examples/blob/main/geant4-x3d.ipynb 
