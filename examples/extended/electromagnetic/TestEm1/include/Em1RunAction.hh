// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: Em1RunAction.hh,v 1.5 2001/02/21 14:00:34 maire Exp $
// GEANT4 tag $Name: geant4-03-01 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef Em1RunAction_h
#define Em1RunAction_h 1

#include "G4UserRunAction.hh"
#include "ProcessesCount.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class G4Run;
class Em1RunActionMessenger;

#ifndef G4NOHIST
 class HepTupleManager;
 class HepHistogram;
#endif

class Em1RunAction : public G4UserRunAction
{
  public:
    Em1RunAction();
   ~Em1RunAction();

  public:
    void BeginOfRunAction(const G4Run*);
    void   EndOfRunAction(const G4Run*);
    
    void CountTraks0(G4int nt) { NbOfTraks0 += nt;}
    void CountTraks1(G4int nt) { NbOfTraks1 += nt;}
    void CountSteps0(G4int ns) { NbOfSteps0 += ns;}
    void CountSteps1(G4int ns) { NbOfSteps1 += ns;}
    void CountProcesses(G4String);
    
  public:
    void  SetRndmFreq(G4int val) {saveRndm = val;}
    G4int GetRndmFreq()          {return saveRndm;}
        
#ifndef G4NOHIST   
    HepHistogram* GetHisto(G4int id) {return histo[id];}
#endif
           
  private:  
    void bookHisto();
    void cleanHisto();
          
  private:
    G4int NbOfTraks0, NbOfTraks1;
    G4int NbOfSteps0, NbOfSteps1;
    ProcessesCount*   ProcCounter;

    Em1RunActionMessenger* runMessenger;    
    G4int saveRndm;
    
#ifndef G4NOHIST       
    HepTupleManager* hbookManager;
    HepHistogram* histo[3];
#endif                     
};

#endif
