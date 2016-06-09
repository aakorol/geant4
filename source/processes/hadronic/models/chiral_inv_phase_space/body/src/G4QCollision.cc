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
// $Id: G4QCollision.cc,v 1.3 2005/02/17 17:13:55 mkossov Exp $
// GEANT4 tag $Name: geant4-07-00-patch-01 $
//
//      ---------------- G4QCollision class -----------------
//                 by Mikhail Kossov, December 2003.
// G4QCollision class of the CHIPS Simulation Branch in GEANT4
// ---------------------------------------------------------------
// ****************************************************************************************
// ********** This CLASS is temporary moved from the photolepton_hadron directory *********
// ******* DO NOT MAKE ANY CHANGE! With time it'll move back to photolepton...(M.K.) ******
// ****************************************************************************************

//#define debug
//#define pdebug

#include "G4QCollision.hh"

G4QCollision::G4QCollision(const G4String& processName) : G4VDiscreteProcess(processName)
{
#ifdef debug
  G4cout<<"G4QCollision::Constructor is called"<<G4endl;
#endif
  if (verboseLevel>0) G4cout << GetProcessName() << " process is created "<< G4endl;
  G4QCHIPSWorld::Get()->GetParticles(234);           // Create CHIPS World of 234 particles
  G4QNucleus::SetParameters(0.,0.,1.,1.);            // Nuclear clusterization parameters
  G4Quasmon::SetParameters(180.,.09,.3);             // Temperature, s-antis, eta suppress
  G4QEnvironment::SetParameters(.5);                 // SolAngle (pbar-A secondary capture)
  //@@ Initialize here the G4QuasmonString parameters
}

// Destructor

G4QCollision::~G4QCollision() {}


G4LorentzVector G4QCollision::GetEnegryMomentumConservation()
{
  return EnMomConservation;
}

G4int G4QCollision::GetNumberOfNeutronsInTarget()
{
  return nOfNeutrons;
}

G4double G4QCollision::GetMeanFreePath(const G4Track& aTrack,G4double,G4ForceCondition* Fc)
{
  *Fc = NotForced;
  const G4DynamicParticle* incidentParticle = aTrack.GetDynamicParticle();
  G4ParticleDefinition* incidentParticleDefinition=incidentParticle->GetDefinition();
  if( !IsApplicable(*incidentParticleDefinition))
    G4cout<<"-W-G4QCollision::GetMeanFreePath called for not implemented particle"<<G4endl;
  // Calculate the mean Cross Section for the set of Elements(*Isotopes) in the Material
  G4double Momentum = incidentParticle->GetTotalMomentum(); // 3-momentum of the Particle
  const G4Material* material = aTrack.GetMaterial();        // Get the current material
  const G4double* NOfNucPerVolume = material->GetVecNbOfAtomsPerVolume();
  const G4ElementVector* theElementVector = material->GetElementVector();
  G4int nE=material->GetNumberOfElements();
#ifdef debug
  G4cout<<"G4QCollision::GetMeanFreePath:"<<nE<<" Elem's in theMaterial"<<G4endl;
#endif
  G4bool leptoNuc=false;       // By default the reaction is not lepto-nuclear
  G4VQCrossSection* CSmanager=G4QProtonNuclearCrossSection::GetPointer();
  if(incidentParticleDefinition == G4Proton::Proton())
      CSmanager=G4QProtonNuclearCrossSection::GetPointer();
  else if(incidentParticleDefinition == G4Gamma::Gamma())
      CSmanager=G4QPhotonNuclearCrossSection::GetPointer();
  else if(incidentParticleDefinition == G4Electron::Electron() ||
          incidentParticleDefinition == G4Positron::Positron())
  {
    CSmanager=G4QElectronNuclearCrossSection::GetPointer();
    leptoNuc=true;
  }
  else if(incidentParticleDefinition == G4MuonPlus::MuonPlus() ||
          incidentParticleDefinition == G4MuonMinus::MuonMinus())
  {
    CSmanager=G4QMuonNuclearCrossSection::GetPointer();
    leptoNuc=true;
  }
  else G4cout<<"G4QCollision::GetMeanFreePath:Particle isn't implemented in CHIPS"<<G4endl;
  
  G4QIsotope* Isotopes = G4QIsotope::Get(); // Pointer to the G4QIsotopes singelton
  G4double sigma=0.;
  for(G4int i=0; i<nE; ++i)
  {
    G4int Z = static_cast<G4int>((*theElementVector)[i]->GetZ()); // Z of the Element
    std::vector<std::pair<G4int,G4double>*>* cs= Isotopes->GetCSVector(Z); // Pointer to CS
    G4int nIs=cs->size();                         // A#Of Isotopes in the Element
    if(nIs) for(G4int j=0; j<nIs; j++)            // Calculate CS for eachIsotope of El
    {
      std::pair<G4int,G4double>* curIs=(*cs)[j];  // A pointer, which is used twice
      G4int N=curIs->first;                       // #ofNeuterons in the isotope
      curIs->second = CSmanager->GetCrossSection(Momentum, Z, N); // CS calculation
    } // End of temporary initialization of the cross sections in the G4QIsotope singeltone
    sigma+=Isotopes->GetMeanCrossSection(Z)*NOfNucPerVolume[i]; // SUM(MeanCS*NOFNperV)
  } // End of LOOP over Elements

  // Check that cross section is not zero and return the mean free path
  if(sigma > 0.) return 1./sigma;                 // Mean path [distance] 
  return DBL_MAX;
}


G4bool G4QCollision::IsApplicable(const G4ParticleDefinition& particle) 
{
  if      (particle == *(     G4MuonPlus::MuonPlus()     )) return true;
  else if (particle == *(    G4MuonMinus::MuonMinus()    )) return true; 
  else if (particle == *(      G4TauPlus::TauPlus()      )) return true;
  else if (particle == *(     G4TauMinus::TauMinus()     )) return true;
  else if (particle == *(     G4Electron::Electron()     )) return true;
  else if (particle == *(     G4Positron::Positron()     )) return true;
  else if (particle == *(        G4Gamma::Gamma()        )) return true;
  else if (particle == *(       G4Proton::Proton()       )) return true;
  //else if (particle == *(      G4Neutron::Neutron()      )) return true;
  //else if (particle == *(    G4PionMinus::PionMinus()    )) return true;
  //else if (particle == *(     G4PionPlus::PionPlus()     )) return true;
  //else if (particle == *(     G4KaonPlus::KaonPlus()     )) return true;
  //else if (particle == *(    G4KaonMinus::KaonMinus()    )) return true;
  //else if (particle == *( G4KaonZeroLong::KaonZeroLong() )) return true;
  //else if (particle == *(G4KaonZeroShort::KaonZeroShort())) return true;
  //else if (particle == *(       G4Lambda::Lambda()       )) return true;
  //else if (particle == *(    G4SigmaPlus::SigmaPlus()    )) return true;
  //else if (particle == *(   G4SigmaMinus::SigmaMinus()   )) return true;
  //else if (particle == *(    G4SigmaZero::SigmaZero()    )) return true;
  //else if (particle == *(      G4XiMinus::XiMinus()      )) return true;
  //else if (particle == *(       G4XiZero::XiZero()       )) return true;
  //else if (particle == *(   G4OmegaMinus::OmegaMinus()   )) return true;
  //else if (particle == *(  G4AntiNeutron::AntiNeutron()  )) return true;
  //else if (particle == *(   G4AntiProton::AntiProton()   )) return true;
#ifdef debug
  G4cout<<"***G4QCollision::IsApplicable: PDG="<<particle.GetPDGEncoding()<<G4endl;
#endif
  return false;
}

G4VParticleChange* G4QCollision::PostStepDoIt(const G4Track& track, const G4Step& step)
{
  static const G4double mu=G4MuonMinus::MuonMinus()->GetPDGMass(); // muon mass
  static const G4double mu2=mu*mu;                                 // squared muon mass
  //static const G4double dpi=M_PI+M_PI;   // 2*pi (for Phi distr.) ***changed to twopi***
  static const G4double mNeut= G4QPDGCode(2112).GetMass();
  static const G4double mProt= G4QPDGCode(2212).GetMass();
  static const G4double dM=mProt+mNeut;                            // doubled nucleon mass
  //static const G4double mPi0 = G4QPDGCode(111).GetMass();
  //static const G4double mDeut= G4QPDGCode(2112).GetNuclMass(1,1,0);
  //static const G4double mPi  = G4QPDGCode(211).GetMass();
  //static const G4double mMu  = G4QPDGCode(13).GetMass();
  //static const G4double mTau = G4QPDGCode(15).GetMass();
  //static const G4double mEl  = G4QPDGCode(11).GetMass();
  const G4DynamicParticle* projHadron = track.GetDynamicParticle();
  const G4ParticleDefinition* particle=projHadron->GetDefinition();
  G4LorentzVector proj4M=projHadron->Get4Momentum();
  G4double momentum = projHadron->GetTotalMomentum(); // 3-momentum of the Particle
  G4double Momentum=proj4M.rho();
  if(fabs(Momentum-momentum)>.001)G4cerr<<"G4QC::PSDI P="<<Momentum<<"="<<momentum<<G4endl;
#ifdef debug
  G4cout<<"G4QCollision::PostStepDoIt is called, P="<<Momentum<<"="<<momentum<<G4endl;
#endif
  if (!IsApplicable(*particle))  // Check applicability
  {
    G4cerr<<"G4QCollision::PostStepDoIt:Only gam,e+,e-,mu+,mu-,t+,t-,p are implemented."
          <<G4endl;
    return 0;
  }
  const G4Material* material = track.GetMaterial();      // Get the current material
  G4int Z=0;
  const G4ElementVector* theElementVector = material->GetElementVector();
  G4int i=0;
  G4double sum=0.;
  G4int nE=material->GetNumberOfElements();
#ifdef debug
  G4cout<<"G4QCollision::PostStepDoIt: "<<nE<<" elements in the material."<<G4endl;
#endif
  G4int projPDG=0;                           // PDG Code prototype for the captured hadron
  // Not all these particles are implemented yet (see Is Applicable)
  if      (particle ==      G4MuonPlus::MuonPlus()     ) projPDG=  -13;
  else if (particle ==     G4MuonMinus::MuonMinus()    ) projPDG=   13;
  else if (particle ==      G4Electron::Electron()     ) projPDG=   11;
  else if (particle ==      G4Positron::Positron()     ) projPDG=  -11;
  else if (particle ==         G4Gamma::Gamma()        ) projPDG=   22;
  else if (particle ==        G4Proton::Proton()       ) projPDG= 2212;
  else if (particle ==       G4Neutron::Neutron()      ) projPDG= 2112;
  else if (particle ==     G4PionMinus::PionMinus()    ) projPDG= -211;
  else if (particle ==      G4PionPlus::PionPlus()     ) projPDG=  211;
  else if (particle ==      G4KaonPlus::KaonPlus()     ) projPDG= 2112;
  else if (particle ==     G4KaonMinus::KaonMinus()    ) projPDG= -321;
  else if (particle ==  G4KaonZeroLong::KaonZeroLong() ) projPDG=  130;
  else if (particle == G4KaonZeroShort::KaonZeroShort()) projPDG=  310;
  else if (particle ==       G4TauPlus::TauPlus()      ) projPDG=  -15;
  else if (particle ==      G4TauMinus::TauMinus()     ) projPDG=   15;
  else if (particle ==        G4Lambda::Lambda()       ) projPDG= 3122;
  else if (particle ==     G4SigmaPlus::SigmaPlus()    ) projPDG= 3222;
  else if (particle ==    G4SigmaMinus::SigmaMinus()   ) projPDG= 3112;
  else if (particle ==     G4SigmaZero::SigmaZero()    ) projPDG= 3212;
  else if (particle ==       G4XiMinus::XiMinus()      ) projPDG= 3312;
  else if (particle ==        G4XiZero::XiZero()       ) projPDG= 3322;
  else if (particle ==    G4OmegaMinus::OmegaMinus()   ) projPDG= 3334;
  else if (particle ==   G4AntiNeutron::AntiNeutron()  ) projPDG=-2112;
  else if (particle ==    G4AntiProton::AntiProton()   ) projPDG=-2212;
#ifdef debug
  G4int prPDG=particle->GetPDGEncoding();
		G4cout<<"G4QCollision::PostStepRestDoIt: projPDG="<<projPDG<<",stPDG="<<prPDG<<G4endl;
#endif
  if(!projPDG)
  {
    G4cerr<<"---Worning---G4QCollision::PostStepDoIt: Undefined captured hadron"<<G4endl;
    return 0;
  }
  // @@ It's a standard randomization procedure, which can be placed in G4QMaterial class
  std::vector<G4double> sumfra;
  for(i=0; i<nE; ++i)
  {
	   G4double frac=material->GetFractionVector()[i];
    sum+=frac;
    sumfra.push_back(sum);             // remember the summation steps
  }
  G4double rnd = sum*G4UniformRand();
  for(i=0; i<nE; ++i)
  {
    G4int cZ=static_cast<G4int>((*theElementVector)[i]->GetZ());
    sum=sumfra[i];
    if (rnd<sum)  
    { 
	     Z = cZ;
      break;
	   }
  }
  if(Z<=0)
  {
    G4cerr<<"---Worning---G4QCollision::PostStepDoIt:Element with Z="<<Z<< G4endl;
    if(Z<0) return 0;
  }
  G4int N = G4QIsotope::Get()->GetNeutrons(Z);
  nOfNeutrons=N;                                       // Remember it for energy-mom. check
  if(Z+N>20) G4QNucleus::SetParameters(.18,.06,6.,1.); // HeavyNuclei NuclearClusterization
  else       G4QNucleus::SetParameters(0.0,0.0,1.,1.); // LightNuclei NuclearClusterization
  if(N) // @@ Temporary suppression of Deuterons @@ !
  {
#ifdef debug
    G4cout<<"*TMP*G4QCollision::PostStepDoIt: N="<<N<<" for element with Z="<<Z<<G4endl;
#endif
    N=0;
  }
#ifdef debug
  G4cout<<"G4QCollision::PostStepDoIt: N="<<N<<" for element with Z="<<Z<<G4endl;
#endif
  if(N<0)
  {
    G4cerr<<"---Worning---G4QCollision::PostStepDoIt:Element with N="<<N<< G4endl;
    return 0;
  }
		if(projPDG==11||projPDG==-11||projPDG==13||projPDG==-13||projPDG==15||projPDG==-15)
		{ // Lepto-nuclear case with the equivalent photon algorithm. @@InFuture + neutrino & QE
    G4double kinEnergy= projHadron->GetKineticEnergy();
    G4ParticleMomentum dir = projHadron->GetMomentumDirection();
    G4VQCrossSection* CSmanager=G4QElectronNuclearCrossSection::GetPointer();
    G4int aProjPDG=abs(projPDG);
    if(aProjPDG==13) CSmanager=G4QMuonNuclearCrossSection::GetPointer();
    if(aProjPDG==15) CSmanager=G4QTauNuclearCrossSection::GetPointer();
    G4double xSec=CSmanager->GetCrossSection(Momentum, Z, N);// Recalculate Cross Section
    // @@ check a possibility to separate p, n, or alpha (!)
    if(xSec <= 0.) // The cross-section iz 0 -> Do Nothing
    {
      //Do Nothing Action insead of the reaction
      aParticleChange.ProposeEnergy(kinEnergy);
      aParticleChange.ProposeLocalEnergyDeposit(0.);
      aParticleChange.ProposeMomentumDirection(dir) ;
      return G4VDiscreteProcess::PostStepDoIt(track,step);
    }
    G4double photonEnergy = CSmanager->GetExchangeEnergy(); // Energy of EqivExchangePart
    if( kinEnergy < photonEnergy )
    {
      //Do Nothing Action insead of the reaction
      G4cerr<<"G4QCollision::PSDoIt: photE="<<photonEnergy<<">leptE="<<kinEnergy<<G4endl;
      aParticleChange.ProposeEnergy(kinEnergy);
      aParticleChange.ProposeLocalEnergyDeposit(0.);
      aParticleChange.ProposeMomentumDirection(dir) ;
      return G4VDiscreteProcess::PostStepDoIt(track,step);
    }
    G4double photonQ2 = CSmanager->GetExchangeQ2(photonEnergy);// Q2(t) of EqivExchangePart
    G4double W=photonEnergy-photonQ2/dM;// HadronicEnergyFlow (W-energy) for virtual photon
    if(W<0.) 
    {
      //Do Nothing Action insead of the reaction
      G4cout << "G4QCollision::PostStepDoIt:(lN) negative equivalent energy W="<<W<<G4endl;
      aParticleChange.ProposeEnergy(kinEnergy);
      aParticleChange.ProposeLocalEnergyDeposit(0.);
      aParticleChange.ProposeMomentumDirection(dir) ;
      return G4VDiscreteProcess::PostStepDoIt(track,step);
    }
    // Update G4VParticleChange for the scattered muon
    G4VQCrossSection* thePhotonData=G4QPhotonNuclearCrossSection::GetPointer();
    G4double sigNu=thePhotonData->GetCrossSection(photonEnergy, Z, N);// IntegratedCrossSec
    G4double sigK =thePhotonData->GetCrossSection(W, Z, N);           // Real CrossSec
    G4double rndFraction = CSmanager->GetVirtualFactor(photonEnergy, photonQ2);
    if(sigNu*G4UniformRand()>sigK*rndFraction) 
    {
      //Do NothingToDo Action insead of the reaction
      G4cout << "G4QCollision::PostStepDoIt: probability correction - DoNothing"<<G4endl;
      aParticleChange.ProposeEnergy(kinEnergy);
      aParticleChange.ProposeLocalEnergyDeposit(0.);
      aParticleChange.ProposeMomentumDirection(dir) ;
      return G4VDiscreteProcess::PostStepDoIt(track,step);
    }
    G4double iniE=kinEnergy+mu;          // Initial total energy of the muon
    G4double finE=iniE-photonEnergy;     // Final total energy of the muon
    if(finE>0) aParticleChange.ProposeEnergy(finE) ;
    else
    {
      aParticleChange.ProposeEnergy(0.) ;
      aParticleChange.ProposeTrackStatus(fStopAndKill);
    }
    // Scatter the muon
    G4double EEm=iniE*finE-mu2;          // Just an intermediate value to avoid "2*"
    G4double iniP=sqrt(iniE*iniE-mu2);   // Initial momentum of the electron
    G4double finP=sqrt(finE*finE-mu2);   // Final momentum of the electron
    G4double cost=(EEm+EEm-photonQ2)/iniP/finP; // cos(theta) for the electron scattering
    if(cost>1.) cost=1.;                 // To avoid the accuracy of calculation problem
    //else if(cost>1.001)                // @@ error report can be done, but not necessary
    if(cost<-1.) cost=-1.;               // To avoid the accuracy of calculation problem
    //else if(cost<-1.001)               // @@ error report can be done, but not necessary
    // --- Example from electromagnetic physics --
    //G4ThreeVector newMuonDirection(dirx,diry,dirz);
    //newMuonDirection.rotateUz(dir);
    //aParticleChange.ProposeMomentumDirection(newMuonDirection1) ;
    // The scattering in respect to the derection of the incident muon is made impicitly:
    G4ThreeVector ort=dir.orthogonal();  // Not normed orthogonal vector (!) (to dir)
    G4ThreeVector ortx = ort.unit();     // First unit vector orthogonal to the direction
    G4ThreeVector orty = dir.cross(ortx);// Second unit vector orthoganal to the direction
    G4double sint=sqrt(1.-cost*cost);    // Perpendicular component
    G4double phi=twopi*G4UniformRand();  // phi of scattered electron
    G4double sinx=sint*sin(phi);         // x-component
    G4double siny=sint*cos(phi);         // y-component
    G4ThreeVector findir=cost*dir+sinx*ortx+siny*orty;
    aParticleChange.ProposeMomentumDirection(findir); // new direction for the muon
    const G4ThreeVector photon3M=iniP*dir-finP*findir;
    projPDG=22;
    proj4M=G4LorentzVector(photon3M,photon3M.mag());
  }
  G4int targPDG=90000000+Z*1000+N;       // PDG Code of the target nucleus
  G4QPDGCode targQPDG(targPDG);
  G4double tM=targQPDG.GetMass();
  EnMomConservation=proj4M+G4LorentzVector(0.,0.,0.,tM);    // Total 4-mom of the reaction
  G4QHadronVector* output=new G4QHadronVector; // Prototype of the output G4QHadronVector
  // @@@@@@@@@@@@@@ Temporary for the testing purposes --- Begin
  G4bool elF=false; // Flag of the ellastic scattering is "false" by default
  //G4double eWei=1.;
  // @@@@@@@@@@@@@@ Temporary for the testing purposes --- End  
#ifdef debug
  G4cout<<"G4QCollision::PostStepDoIt: projPDG="<<projPDG<<", targPDG="<<targPDG<<G4endl;
#endif
  G4QHadron* pH = new G4QHadron(projPDG,proj4M);                // ---> DELETED -->---+
  G4QuasmonString* pan= new G4QuasmonString(pH,false,targPDG,false);//-> DELETED --+  |
  delete pH;                                                    // --------<-------+--+
#ifdef debug
  G4double mp=G4QPDGCode(projPDG).GetMass();   // Mass of the projectile particle  |
  G4cout<<"G4QCollision::PostStepDoIt: pPDG="<<projPDG<<", pM="<<mp<<G4endl; //    |
#endif
  G4int tNH=0;                      // Prototype of the number of secondaries inOut|
  try                                                           //                 |
	 {                                                             //                 |
				// delete output;                                           //                 |
    //output = pan->Fragment();// DESTROYED in the end of the LOOP work space      |
    // @@@@@@@@@@@@@@ Temporary for the testing purposes --- Begin                 |
    tNH=pan->GetNOfHadrons();
    if(tNH==2)                      // At least 2 hadrons are in the Constr.Output |
				{//                                                                            |
      elF=true;                     // Just put a flag for the ellastic Scattering |
	     delete output;                // Delete a prototype of dummy G4QHadronVector |
      output = pan->GetHadrons();   // DESTROYED in the end of the LOOP work space |
    }//                                                                            |
    //eWei=pan->GetWeight();        // Just an example for the weight of the event |
#ifdef debug
    G4cout<<"=====>>G4QCollision::PostStepDoIt: elF="<<elF<<",n="<<tNH<<G4endl; // |
#endif
    // @@@@@@@@@@@@@@ Temporary for the testing purposes --- End                   |
  }                                                             //                 |
  catch (G4QException& error)//                                                    |
	 {                                                             //                 |
	   //#ifdef pdebug
    G4cerr<<"***G4QCollision::PostStepDoIt: GEN Exception is catched"<<G4endl; //  |
	   //#endif
    G4Exception("G4QCollision::AtRestDoIt:","27",FatalException,"QString Excep");//|
  }                                                             //                 |
  delete pan;                              // Delete the Nuclear Environment ---<--+
  aParticleChange.Initialize(track);
  G4double localtime = track.GetGlobalTime();
  G4ThreeVector position = track.GetPosition();
  // ------------- From here the secondaries are filled -------------------------
  //@@@  G4int tNH = output->size();       // A#of hadrons in the output
  aParticleChange.SetNumberOfSecondaries(tNH); 
  // Now add nuclear fragments
#ifdef debug
  G4cout<<"G4QCollision::PostStepDoIt: "<<tNH<<" particles are generated"<<G4endl;
#endif
  G4int nOut=output->size();               // Real length of the output @@ Temporary
  if(tNH==1) tNH=0;                        // @@ Temporary
  if(tNH==2&&2!=nOut) G4cout<<"--Warning--G4QCollision::PostStepDoIt: 2 # "<<nOut<<G4endl;
  // Deal with ParticleChange final state interface to GEANT4 output of the process
  if(tNH==2) for(i=0; i<tNH; i++) // @@ Temporary tNH==2 instead of just tNH
  {
    // Note that one still has to take care of Hypernuclei (with Lambda or Sigma inside)
    // Hypernucleus mass calculation and ion-table interface upgrade => work for Hisaya @@
    // The decau process for hypernuclei must be developed in GEANT4 (change CHIPS body)
    G4QHadron* hadr=output->operator[](i);   // Pointer to the output hadron    
    G4int PDGCode = hadr->GetPDGCode();
    G4int nFrag   = hadr->GetNFragments();
#ifdef pdebug
    G4cout<<"G4QCollision::AtRestDoIt: H#"<<i<<",PDG="<<PDGCode<<",nF="<<nFrag<<G4endl;
#endif
    if(nFrag)                // Skip intermediate (decayed) hadrons
    {
#ifdef debug
	     G4cout<<"G4QCollision::PostStepDoIt: Intermediate particle is found i="<<i<<G4endl;
#endif
      delete hadr;
      continue;
    }
    G4DynamicParticle* theSec = new G4DynamicParticle;  
    G4ParticleDefinition* theDefinition;
    if     (PDGCode==90000001) theDefinition = G4Neutron::Neutron();
    else if(PDGCode==90001000) theDefinition = G4Proton::Proton();//While it can be in ions
    else if(PDGCode==91000000) theDefinition = G4Lambda::Lambda();
    else if(PDGCode==311 || PDGCode==-311)
    {
      if(G4UniformRand()>.5) theDefinition = G4KaonZeroLong::KaonZeroLong();   // K_L
						else                   theDefinition = G4KaonZeroShort::KaonZeroShort(); // K_S
    }
    else if(PDGCode==91000999) theDefinition = G4SigmaPlus::SigmaPlus();
    else if(PDGCode==90999001) theDefinition = G4SigmaMinus::SigmaMinus();
    else if(PDGCode==91999000) theDefinition = G4XiMinus::XiMinus();
    else if(PDGCode==91999999) theDefinition = G4XiZero::XiZero();
    else if(PDGCode==92998999) theDefinition = G4OmegaMinus::OmegaMinus();
	   else if(PDGCode >80000000) // Defines hypernuclei as normal nuclei (N=N+S Correction!)
    {
      G4int aZ = hadr->GetCharge();
      G4int aA = hadr->GetBaryonNumber();
#ifdef pdebug
						G4cout<<"G4QCollision::AtRestDoIt:Ion Z="<<aZ<<", A="<<aA<<G4endl;
#endif
      theDefinition = G4ParticleTable::GetParticleTable()->FindIon(aZ,aA,0,aZ);
    }
    else theDefinition = G4ParticleTable::GetParticleTable()->FindParticle(PDGCode);
    if(!theDefinition)
    {
      G4cout<<"---Worning---G4QCollision::PostStepDoIt: drop PDG="<<PDGCode<<G4endl;
      delete hadr;
      continue;
    }
#ifdef pdebug
    G4cout<<"G4QCollision::PostStepDoIt:Name="<<theDefinition->GetParticleName()<<G4endl;
#endif
    theSec->SetDefinition(theDefinition);
    G4LorentzVector h4M=hadr->Get4Momentum();
#ifdef debug
    G4cout<<"G4QCollision::PostStepDoIt:#"<<i<<",PDG="<<PDGCode<<",4M="<<h4M<<G4endl;
#endif
    theSec->Set4Momentum(h4M);
    delete hadr; // <-----<-----------<-------------<---------------------<---------<-----+
#ifdef debug
    G4ThreeVector curD=theSec->GetMomentumDirection();              //                    ^
    G4double curM=theSec->GetMass();                                //                    |
    G4double curE=theSec->GetKineticEnergy()+curM;                  //                    ^
    G4cout<<"G4QCapAtR::PSDoIt:p="<<curD<<curD.mag()<<",e="<<curE<<",m="<<curM<<G4endl;// |
#endif
    G4Track* aNewTrack = new G4Track(theSec, localtime, position ); //                    ^
    aParticleChange.AddSecondary( aNewTrack ); //                                         |
#ifdef debug
    G4cout<<"G4QCollision::PostStepDoIt:#"<<i<<" is done."<<G4endl; //                    |
#endif
  } //                                                                                    |
  delete output; // instances of the G4QHadrons from the output are already deleted above +
  aParticleChange.ProposeTrackStatus(fStopAndKill);        // Kill the absorbed particle
  //return &aParticleChange;                               // This is not enough (ClearILL)
#ifdef debug
    G4cout<<"G4QCollision::PostStepDoIt: **** PostStepDoIt is done ****"<<G4endl;
#endif
  return G4VDiscreteProcess::PostStepDoIt(track, step);
}