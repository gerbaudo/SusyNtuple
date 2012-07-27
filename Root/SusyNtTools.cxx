#include "SusyNtuple/SusyNtTools.h"

// Common Packages
#include "Mt2/mt2_bisect.h" 

using namespace std;
using namespace Susy;


/*--------------------------------------------------------------------------------*/
// Constructor
/*--------------------------------------------------------------------------------*/
SusyNtTools::SusyNtTools() : 
        m_anaType(Ana_3Lep)
{
}

/*--------------------------------------------------------------------------------*/
// Get event weight, combine gen, pileup, xsec, and lumi weights
// Default weight uses ICHEP dataset, A-B14 lumi
// You can supply a different luminosity, but the pileup weights will still correspond to A-B14
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getEventWeight(const Event* evt, float lumi)
{
  if(!evt->isMC) return 1;
  else return evt->w * evt->wPileup * evt->xsec * lumi / evt->sumw;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getEventWeight1fb(const Event* evt)
{
  if(!evt->isMC) return 1;
  else return evt->w * evt->wPileup1fb * evt->xsec * LUMI_A_B3 / evt->sumw;
}

/*--------------------------------------------------------------------------------*/
// 'Overview' functions to make it easy for user to grab all objects
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getBaselineObjects(SusyNtObject* susy_nt, ElectronVector &elecs,
                                     MuonVector &muons, JetVector &jets, SusyNtSys sys)
{
  // Get all objects
  elecs = getPreElectrons(susy_nt, sys);
  muons = getPreMuons(susy_nt, sys);
  jets  = getPreJets(susy_nt, sys);

  // Now perform the overlap removals
  performOverlap(elecs, muons, jets);

  // Do SFOS removal for Mll < 12 
  removeSFOSPair(elecs, MLL_MIN);
  removeSFOSPair(muons, MLL_MIN);

}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getSignalObjects(ElectronVector bsElecs, MuonVector bsMuons, JetVector bsJets,
				   ElectronVector &sigElecs, MuonVector &sigMuons, JetVector &sigJets,
                                   uint nVtx, bool isMC)
{
  // Set signal objects
  sigElecs = getSignalElectrons(bsElecs, nVtx, isMC);
  sigMuons = getSignalMuons(bsMuons, nVtx, isMC);
  sigJets  = getSignalJets(bsJets);
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getSignalObjects(SusyNtObject* susy_nt, ElectronVector &sigElecs, 
				   MuonVector &sigMuons, JetVector &sigJets, SusyNtSys sys,
                                   uint nVtx, bool isMC)
{
  // Temprorary vectors for baseline objects
  ElectronVector elecs = getPreElectrons(susy_nt, sys);
  MuonVector     muons = getPreMuons(susy_nt, sys);
  JetVector       jets = getPreJets(susy_nt, sys);

  // Perform overlap removal
  performOverlap(elecs, muons, jets);

  // Now set the signal objects
  sigElecs = getSignalElectrons(elecs, nVtx, isMC);
  sigMuons = getSignalMuons(muons, nVtx, isMC);
  sigJets  = getSignalJets(jets);
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::performOverlap(ElectronVector &elecs, MuonVector &muons, JetVector &jets)
{
  // Remove electrons from electrons
  e_e_overlap(elecs, E_E_DR);

  // Remove jets from electrons
  e_j_overlap(elecs, jets, J_E_DR, true);
  
  // Remove electrons from jets
  e_j_overlap(elecs, jets, E_J_DR, false);

  // Remove muons from jets
  m_j_overlap(muons, jets, M_J_DR);

  // Remove electrons and muons that overlap
  e_m_overlap(elecs, muons, E_M_DR);

  // Remove muons from muons
  m_m_overlap(muons, M_M_DR);
}

/*--------------------------------------------------------------------------------*/
// Methods to grab the Baseline objects
/*--------------------------------------------------------------------------------*/
ElectronVector SusyNtTools::getPreElectrons(SusyNtObject* susy_nt, SusyNtSys sys)
{
  // Not sure if I want to pass SusyNt object around or not... but just do it this way
  // for now for lack of a more creative idea.
  ElectronVector elecs;
  for(uint ie=0; ie<susy_nt->ele()->size(); ++ie){
    Electron* e = &(susy_nt->ele()->at(ie));
    e->setState(sys);
    
    // Apply any additional Selection
    if(e->Pt() < ELECTRON_PT_CUT) continue;

    // Save
    elecs.push_back(e);
  }

  return elecs;
}
/*--------------------------------------------------------------------------------*/
MuonVector SusyNtTools::getPreMuons(SusyNtObject* susy_nt, SusyNtSys sys)
{
  MuonVector muons;
  for(uint im=0; im<susy_nt->muo()->size(); ++im){
    Muon* m = &(susy_nt->muo()->at(im));
    m->setState(sys);

    // Apply any additional selection
    if(m->Pt() < MUON_PT_CUT) continue;
    
    // Save
    muons.push_back(m);
  }

  return muons;
}
/*--------------------------------------------------------------------------------*/
JetVector SusyNtTools::getPreJets(SusyNtObject* susy_nt, SusyNtSys sys)
{
  JetVector jets;
  for(uint ij=0; ij<susy_nt->jet()->size(); ++ij){
    Jet* j = &(susy_nt->jet()->at(ij));
    j->setState(sys);

    // Apply any additional selection
    // At least jets of 20 GeV
    if(j->Pt() < JET_PT_CUT_3L) continue;
    jets.push_back(j);
  }
  
  return jets;
}

/*--------------------------------------------------------------------------------*/
// Get Signal objects
/*--------------------------------------------------------------------------------*/
ElectronVector SusyNtTools::getSignalElectrons(ElectronVector bsElecs, uint nVtx, bool isMC)
{
  ElectronVector sigElecs;
  for(uint ie=0; ie<bsElecs.size(); ++ie){
    const Electron* e = bsElecs.at(ie);
    if(isSignalElectron(e, nVtx, isMC)){
      sigElecs.push_back(e);
    }
  }

  return sigElecs;
}
/*--------------------------------------------------------------------------------*/
MuonVector SusyNtTools::getSignalMuons(MuonVector bsMuons, uint nVtx, bool isMC)
{
  MuonVector sigMuons;
  for(uint im=0; im<bsMuons.size(); ++im){
    const Muon* m = bsMuons.at(im);
    if(isSignalMuon(m, nVtx, isMC)){
      sigMuons.push_back(m);
    }
  }

  return sigMuons;
}
/*--------------------------------------------------------------------------------*/
JetVector SusyNtTools::getSignalJets(JetVector bsJets)
{
  JetVector sigJets;
  for(uint ij=0; ij<bsJets.size(); ++ij){
    const Jet* j = bsJets.at(ij);
    if(isSignalJet(j)){
      sigJets.push_back(j);
    }
  }

  return sigJets;
}
/*--------------------------------------------------------------------------------*/
PhotonVector SusyNtTools::getSignalPhotons(SusyNtObject* susy_nt)
{
  // Currently only storing signal photons, so just a conv way to get them.

  PhotonVector sigPho;
  for(uint ip=0; ip<susy_nt->pho()->size(); ip++){
    sigPho.push_back( &(susy_nt->pho()->at(ip)) );
  }

  return sigPho;  
}

/*--------------------------------------------------------------------------------*/
// Get Met
/*--------------------------------------------------------------------------------*/
Susy::Met* SusyNtTools::getMet(SusyNtObject* susy_nt, SusyNtSys sys)
{
  // Right now not being clever. Could maybe make sys index correspond to 
  // index on the array.
  Met* met = NULL;
  vector<Met>* metTmp = susy_nt->met();
  for(uint i=0; i<metTmp->size(); i++){
    if(metTmp->at(i).sys == sys){
      met = &(metTmp->at(i));
      return met;
    }
  }
  if(!met){
    cout << "Error: Unable to find met for given systematic!  Returning NULL!!" << endl;
  }
  
  return met;
}

/*--------------------------------------------------------------------------------*/
// Check if Lepton is Signal Lepton
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalLepton(const Lepton* l, uint nVtx, bool isMC){
  if( l->isEle() ) return isSignalElectron( (Electron*) l, nVtx, isMC);
  else             return isSignalMuon( (Muon*) l, nVtx, isMC );
};
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalElectron(const Electron* ele, uint nVtx, bool isMC)
{
  if(!ele->tightPP) return false;

  // Relative ptcone iso
  if(ele->ptcone30/ele->Pt() >= ELECTRON_PTCONE30_PT_CUT) return false;

  // Sliding topo etcone isolation cut, slope differs between data/mc
  if(elEtTopoConeCorr(ele,nVtx,isMC)/ele->Pt() >= ELECTRON_TOPOCONE30_PT_CUT ) return false;

  // Impact parameter
  if(fabs(ele->d0Sig()) >= ELECTRON_D0SIG_CUT) return false;
  if(fabs(ele->z0SinTheta()) >= ELECTRON_Z0_SINTHETA_CUT) return false;

  return true;
}

/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalMuon(const Muon* mu, uint nVtx, bool isMC)
{
  // Sliding ptcone isolation cut, slope differs between data/mc
  if(muPtConeCorr(mu,nVtx,isMC)/mu->Pt() >= MUON_PTCONE30_PT_CUT ) return false;

  // etcone isolation cut
  // DON'T APPLY THIS FOR NOW - just here for studies
  //if(muEtConeCorr(mu,nVtx,isMC)/mu->Pt() >= MUON_ETCONE30_PT_CUT) return false;

  // Impact parameter
  if(fabs(mu->d0Sig()) >= MUON_D0SIG_CUT) return false;
  if(fabs(mu->z0SinTheta()) >= MUON_Z0_SINTHETA_CUT) return false;

  return true;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::elEtTopoConeCorr(const Electron* e, uint nVtx, bool isMC){
  float slope = isMC? ELECTRON_TOPOCONE30_SLOPE_MC : ELECTRON_TOPOCONE30_SLOPE_DATA;
  return e->topoEtcone30Corr - slope*nVtx;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::muPtConeCorr(const Muon* m, uint nVtx, bool isMC){
    float slope = isMC? MUON_PTCONE30_SLOPE_MC : MUON_PTCONE30_SLOPE_DATA;
    return m->ptcone30 - slope*nVtx;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::muEtConeCorr(const Muon* m, uint nVtx, bool isMC){
  float k1 = isMC? MUON_ETCONE30_K1_MC : MUON_ETCONE30_K1_DATA;
  float k2 = isMC? MUON_ETCONE30_K2_MC : MUON_ETCONE30_K2_DATA;
  return m->etcone30 - k1*nVtx - k2*nVtx*nVtx;
}

/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalJet(const Jet* jet)
{
  // Two lep and three lep have different requirements
  // TODO: These will likely be merged
  float JET_PT_CUT = m_anaType==Ana_2Lep? JET_PT_CUT_2L : JET_PT_CUT_3L;
  
  if(jet->Pt() < JET_PT_CUT)         return false;
  if(fabs(jet->Eta()) > JET_ETA_CUT) return false;
  if(jet->jvf < JET_JVF_CUT)         return false;
  return true;
}

/*--------------------------------------------------------------------------------*/
// Overlap Methods (Commandeered from Anyes)
/*--------------------------------------------------------------------------------*/
void SusyNtTools::e_e_overlap(ElectronVector &elecs, float minDr)
{
  if(elecs.size() < 2) return;

  for(int ie1=elecs.size()-1; ie1>=0; ie1--){
    const Electron* e1 = elecs.at(ie1);
    for(int ie2=ie1-1; ie2>=0; ie2--){
      const Electron* e2 = elecs.at(ie2);
      if(e1->DeltaR(*e2) > minDr) continue;
      
      if(e1->Pt() < e2->Pt()){
	elecs.erase( elecs.begin() + ie1 );
	break;
      }
      else
	elecs.erase( elecs.begin() + ie2 );
    }
  }
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::e_j_overlap(ElectronVector &elecs, JetVector &jets, float minDr, bool removeJets)
{
  if( elecs.size() == 0 || jets.size() == 0 ) return;

  for(int ie=elecs.size()-1; ie>=0; ie--){
    const Electron* e = elecs.at(ie);
    for(int ij=jets.size()-1; ij>=0; ij--){
      const Jet* j = jets.at(ij);
      if(e->DeltaR(*j) > minDr) continue;
      
      if(removeJets)
	jets.erase( jets.begin() + ij );
      else{
	elecs.erase( elecs.begin() + ie);
	break;
      }
    }// end loop over jets
  }// end loop over electrons	  
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::m_j_overlap(MuonVector &muons, JetVector jets, float minDr)
{
  if( muons.size() == 0 || jets.size() == 0 ) return;

  for(int im=muons.size()-1; im>=0; im--){
    const Muon* m = muons.at(im);
    for(int ij=jets.size()-1; ij>=0; ij--){
      const Jet* j = jets.at(ij);
      if(m->DeltaR(*j) > minDr) continue;

      muons.erase( muons.begin() + im );
      break;

    }// end loop over jets
  }// end loop over muons
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::e_m_overlap(ElectronVector &elecs, MuonVector &muons, float minDr)
{
  if( elecs.size() == 0 || muons.size() == 0 ) return;

  //In this case we will want to remove both the electron and the muon
  for(int ie=elecs.size()-1; ie>=0; ie--){
    const Electron* e = elecs.at(ie);
    for(int im=muons.size()-1; im>=0; im--){
      const Muon* m = muons.at(im);
      if(e->DeltaR(*m) > minDr) continue;
      
      elecs.erase( elecs.begin() + ie );
      muons.erase( muons.begin() + im );
      break;

    }// end loop over muons
  }// end loop over jets
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::m_m_overlap(MuonVector& muons, float minDr)
{
  if(muons.size() < 2) return;

  // If 2 muons overlap, toss them both!
  for(int iMu=muons.size()-1; iMu>=0; iMu--){
    const Muon* mu1 = muons.at(iMu);
    for(int jMu=iMu-1; jMu>=0; jMu--){
      const Muon* mu2 = muons.at(jMu);
      if(mu1->DeltaR(*mu2) > minDr) continue;
      
      muons.erase( muons.begin() + iMu );
      muons.erase( muons.begin() + jMu );
      iMu--;

      break;

    } 
  } 
}

/*--------------------------------------------------------------------------------*/
// Methods to handle signal region cuts
/*--------------------------------------------------------------------------------*/
void SusyNtTools::removeSFOSPair(ElectronVector &elecs, float MllCut)
{
  if(elecs.size() < 2) return;

  // Now removing all combinations of SFOS pairs with mll < cut
  ElectronVector electronsPass;
  uint nEle = elecs.size();

  // Use a double loop to check all combinatorics
  for(uint i=0; i<nEle; i++){
    const Electron* e1 = elecs[i];
    bool pass = true;
    for(uint j=0; j<nEle; j++){
      if(i==j) continue;
      const Electron* e2 = elecs[j];
      if(isSFOS(e1,e2) && Mll(e1,e2) < MllCut){
        pass = false;
        break;
      }
    }
    if(pass) electronsPass.push_back(e1);
  }

  // Now replace the supplied vector with this cleaned vector
  elecs = electronsPass;
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::removeSFOSPair(MuonVector &muons, float MllCut)
{
  if( muons.size() < 2 ) return;

  // Now removing all combinations of SFOS pairs with mll < cut
  MuonVector muonsPass;
  uint nMu = muons.size();

  // Use a double loop to check all combinatorics
  for(uint i=0; i<nMu; i++){
    const Muon* m1 = muons[i];
    bool pass = true;
    for(uint j=0; j<nMu; j++){
      if(i==j) continue;
      const Muon* m2 = muons[j];
      if(isSFOS(m1,m2) && Mll(m1,m2) < MllCut){
        pass = false;
        break;
      }
    }
    if(pass) muonsPass.push_back(m1);
  }

  // Now replace the supplied vector with this cleaned vector
  muons = muonsPass;
}

/*--------------------------------------------------------------------------------*/
// Lepton flavor methods (moved from SusyDefs)
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSameFlav(const Lepton* l1, const Lepton* l2)
{ return l1->isEle()==l2->isEle() && l1->isMu()==l2->isMu(); }
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSFOS(const Lepton* l1, const Lepton* l2)
{ return isSameFlav(l1,l2) && (l1->q*l2->q < 0); }
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSFSS(const Lepton* l1, const Lepton* l2)
{ return isSameFlav(l1,l2) && (l1->q*l2->q > 0); }
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasSFOS(const LeptonVector& leps){
  uint nLep = leps.size();
  for(uint i=0; i<nLep; i++){
    for(uint j=i+1; j<nLep; j++){
      if(isSFOS(leps[i],leps[j])) return true;
    }
  }
  return false;
}

/*--------------------------------------------------------------------------------*/
// Mass calculation methods (moved from SusyDefs)
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Mll(const Lepton* l1, const Lepton* l2)
{ return (*l1 + *l2).M(); }
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Mlll(const Lepton* l1, const Lepton* l2, const Lepton* l3)
{ return (*l1 + *l2 + *l3).M(); }
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Mt(const Lepton* lep, const Met* met)
{ return sqrt( 2.*lep->Pt()*met->Et*(1 - cos(lep->DeltaPhi((met->lv())))) ); }
/*--------------------------------------------------------------------------------*/
float SusyNtTools::Meff(const LeptonVector& leps, const JetVector& jets, const Met* met)
{
  float meff = 0;
  for(uint i=0; i<leps.size(); i++) meff += leps[i]->Pt();
  for(uint i=0; i<jets.size(); i++){
    if(jets[i]->Pt() > 40) meff += jets[i]->Pt();
  }
  meff += met->Et;
  return meff;
}

/*--------------------------------------------------------------------------------*/
// Z selection methods
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isZ(const Lepton* l1, const Lepton* l2, float massWindow)
{ return isSFOS(l1,l2) && fabs( Mll(l1,l2)-MZ ) < massWindow; }
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isZWindow(const Lepton* l1, const Lepton* l2, float minMll, float maxMll)
{
  float mll=Mll(l1,l2);
  return (isSFOS(l1,l2) && mll>minMll && mll<maxMll);
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasZ(const LeptonVector& leps, float massWindow)
{
  for(uint i=0; i<leps.size(); i++){
    for(uint j=i+1; j<leps.size(); j++){
      if( isZ(leps[i],leps[j],massWindow) ) return true;
    }
  }
  return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasZWindow(const LeptonVector& leps, float minMll, float maxMll)
{
  for(uint i=0; i<leps.size(); i++){
    for(uint j=i+1; j<leps.size(); j++){
      if( isZWindow(leps[i],leps[j],minMll, maxMll) ) return true;
    }
  }
  return false;
}

/*--------------------------------------------------------------------------------*/
// Finds indices for the lepton pair closest to the Z mass
/*--------------------------------------------------------------------------------*/
void SusyNtTools::bestZ(uint& l1, uint& l2, const LeptonVector& leps)
{
  float minDM = -1;
  uint nLep = leps.size();
  for(uint i=0; i < nLep; i++){
    for(uint j=i+1; j < nLep; j++){

      if( !isSFOS(leps[i],leps[j]) ) continue;
      float dM = fabs( Mll(leps[i],leps[j]) - MZ );

      if(minDM<0 || dM<minDM){
        minDM = dM;
        l1 = i;
        l2 = j;
      }
    }
  }
  if(minDM<0){
    cout << "bestZ : WARNING : No SFOS candidates!" << endl;
    abort();
  }
}

/*--------------------------------------------------------------------------------*/
// B-Jet methods
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::hasBJet(const JetVector& jets, float weight)
{
  uint nJet = jets.size();
  for(uint i=0; i<nJet; i++){
    if(isBJet(jets[i])) return true;
  }
  return false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isBJet(const Jet* jet, float weight)
{
  // Switching to MV1
  return jet->mv1 > weight;
}

/*--------------------------------------------------------------------------------*/
// MissingET Rel
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getMetRel(const Met* met, const LeptonVector& leptons, const JetVector& jets, float minJetPt)
{
  const TLorentzVector metLV = met->lv();
  float dPhi = TMath::Pi()/2.;

  for(uint il=0; il<leptons.size(); ++il)
    if( fabs(metLV.DeltaPhi( *leptons.at(il) )) < dPhi )
      dPhi = fabs(metLV.DeltaPhi( *leptons.at(il) ));
  for(uint ij=0; ij<jets.size(); ++ij){
    const Jet* jet = jets.at(ij);
    if(jet->Pt()<minJetPt) continue;
    if( fabs(metLV.DeltaPhi( *jet )) < dPhi )
      dPhi = fabs(metLV.DeltaPhi( *jet ));    
  }// end loop over jets
  
  return metLV.Et() * sin(dPhi);
}

/*--------------------------------------------------------------------------------*/
// Calculate MT2
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getMT2(const LeptonVector& leptons, const Susy::Met* met)
{
  if( leptons.size() < 2 ) return -999;

  // necessary variables
  TLorentzVector metlv = met->lv();
  TLorentzVector l0    = *leptons.at(0);
  TLorentzVector l1    = *leptons.at(1);

  double pTMiss[3] = {0.0, metlv.Px(), metlv.Py()};
  double pA[3]     = {0.0, l0.Px(), l0.Py()};
  double pB[3]     = {0.0, l1.Px(), l1.Py()};
  
  // Create Mt2 object
  mt2_bisect::mt2 mt2_event;
  mt2_event.set_momenta(pA,pB,pTMiss);
  mt2_event.set_mn(0); // LSP mass = 0 is Generic
  
  return mt2_event.get_mt2();
}

/*--------------------------------------------------------------------------------*/
// Top Tagging methods borrowed from SUSYTools. Yes this is duplication of code...
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::passTopTag(const LeptonVector& leptons, const JetVector& jets, const Met* met,
                             int opt, float ptJetCut, float mEffCut)
{
  // Defaults opt = 0, ptJetCut = 30 GeV, mEffCut = 100 GeV
  // Tagging is based on number of jets and at least two leptons
  if( leptons.size() < 2 ) return false;

  // Define some necessary vars
  uint njets         = jets.size();
  uint njetscan      = njets < 3 ? njets : 3; // Only consider up to 3 jets 
  TLorentzVector v1l = *leptons.at(0);
  TLorentzVector v2l = *leptons.at(1);
  double phi         = met->phi;
  double Et          = met->Et;
  TVector2 metv2 (Et * cos(phi), Et * sin(phi));

  // Check to see if our leptons and met is
  // consistent with a W candidate
  bool maybeW = toptag0j(v1l,v2l,metv2);
  if(!maybeW)            return true;
  if( jets.size() < 2 )  return true;
  
  // Check to see if we have top
  int npairs    = 0;
  for(uint i1=0; i1<njetscan-1; ++i1){
    if(i1==0 || opt==0){ // opt=0 implies compare all jets
      for(uint i2=i1+1; i2<njetscan; ++i2){
	TLorentzVector v1j = *jets.at(i1);
	TLorentzVector v2j = *jets.at(i2);
	bool maybeTop = toptag2j(ptJetCut,mEffCut,v1l,v2l,v1j,v2j,metv2,opt);
	if(maybeTop) npairs++;
      }// end loop over second jet
    }// end if options
  }// end loop over first jet

  if( npairs > 0 ) return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::toptag0j(TLorentzVector v1l, TLorentzVector v2l, TVector2 met)
{
  // Taken from TopTag.cxx in SUSYTools

  static const Double_t wmass = 80.4;
  double metx = met.X();
  double mety = met.Y();

  // First mct(ll)
  //---------------
  double mctll=calcMCT(v1l,v2l);

  double pxus=v1l.Px()+v2l.Px()+metx;
  double pyus=v1l.Py()+v2l.Py()+mety;
  double rr=sqrt(pxus*pxus+pyus*pyus)/(2.*wmass);
  double fact=rr+sqrt(1+rr*rr);

  //cout << "mctll " << mctll << " cut value " << wmass*fact << std::endl;
  return mctll < wmass*fact ? true: false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::toptag2j(double ptjcut, double meffcut, 
			   TLorentzVector v1l, TLorentzVector v2l,
			   TLorentzVector v1j, TLorentzVector v2j,
			   TVector2 met, int iopt ) 
{
  // Taken from TopTag.cxx in SUSYTools

  //
  const double tmass = 172.5;
  const double mljcut = 155.;
  const double mctjcut = 137.;
  //
  TLorentzVector vjb[2];
  double ml1j[2];
  double ml2j[2];
  //
  double metx = met.X();  double mety = met.Y();
  double ptl1=v1l.Pt();   double ptl2=v2l.Pt();
  double ptj1=v1j.Pt();   double ptj2=v2j.Pt();
  double pxus=v1l.Px()+v2l.Px()+metx;
  double pyus=v1l.Py()+v2l.Py()+mety;
  double mefftop=ptj1+ptj2+ptl1+ptl2;

  if(ptj2 < ptjcut) return false;
  if(iopt == 0 && mefftop < meffcut) return false;

  // First part:
  //-------------
  vjb[0]=v1j;
  vjb[1]=v2j;
  double mctjj=calcMCT(vjb[0],vjb[1]);
  double pxusjl=vjb[0].Px()+vjb[1].Px()+pxus;
  double pyusjl=vjb[0].Py()+vjb[1].Py()+pyus;
  double rrj=sqrt(pxusjl*pxusjl+pyusjl*pyusjl)/(2.*tmass);
  double factj=rrj+sqrt(1+rrj*rrj);

  bool imct = mctjj < mctjcut*factj ? true : false;
  if(iopt==1) return imct;

  // Second part:
  //--------------
  for(int i=0;i<2;++i) {
    ml1j[i]=(v1l+vjb[i]).M();
    ml2j[i]=(v2l+vjb[i]).M();
  }
  int ncou=0;
  int icou1[2];
  int icou2[2];
  for(int i=0;i<2;++i) {
    for(int j=0;j<2;++j) {
      if(i != j) {
	if(ml1j[i]<mljcut && ml2j[j]<mljcut) {
	  icou1[ncou]=i;
	  icou2[ncou]=j;
	  ncou++;
	}
      }
    }
  }
  bool imjl = ncou>0 ? true : false;

  // Third part: finally mct(ql,ql) for each coupling passing the mjl cut
  //-------------exploit the dependence of mct((jl),(jl)) on the maximum
  //             mass^2 of the two jl combinations 
  int ngcou=0;
  for(int i=0;i<ncou;++i) {
    int ij1=icou1[i];
    int ij2=icou2[i];
    TLorentzVector vtot1=vjb[ij1]+v1l;
    TLorentzVector vtot2=vjb[ij2]+v2l;
    double mctjl=calcMCT(vtot1,vtot2);
    double mm1=(vjb[ij1]+v1l).M();
    double mm2=(vjb[ij2]+v2l).M();
    double mmax2 = mm1>mm2 ? (mm1*mm1) : (mm2*mm2);
    double upl=mmax2/(tmass)+(tmass);
    //cout << " i " << " mctjl " << mctjl << " mmax2 " << mmax2 << 
    //" upl " << upl << endl;
    if(mctjl < upl*factj )  ngcou++;
  }

  bool imctjl = ngcou>0 ? true : false;
  //cout << " ll " << imctll << " ct " << imct << " jl " << imjl <<
  //" ctlj " << imctjl << endl;

  return imct & imjl & imctjl;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::calcMCT(TLorentzVector v1, TLorentzVector v2)
{
  float mct = (v1.Mt() + v2.Mt())*(v1.Mt() + v2.Mt()) - (v1-v2).Perp2();
  mct = (mct >= 0.) ? sqrt(mct) : sqrt(-mct);
  return mct;
}

