// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_ELECTRON_H
#define SUSYNTUPLE_ELECTRON_H

#include "SusyNtuple/Lepton.h"

#include "SusyNtuple/SusyNtSys.h"

namespace Susy
{
/// Electron class
class Electron : public Lepton
{
public:
    Electron(){ clear(); }
    virtual ~Electron(){};
    Electron(const Electron &);
    /** Assignment operator */
    Electron& operator=(const Electron &);

    // Cluster/track variables
    float clusE;              ///< CaloCluster energy
    float clusEta;            ///< CaloCluster eta
    float clusPhi;            ///< CaloCluster phi
    float trackPt;            ///< ID track pt

    //LLH quality flags
    bool veryLooseLLH;        ///< veryLooseLLH
    bool looseLLH;            ///< looseLLH
    bool mediumLLH;           ///< mediumLLH
    bool tightLLH;            ///< tightLLH

    //Without d0 cut
    bool looseLLH_nod0;       ///< looseLLH
    bool mediumLLH_nod0;      ///< mediumLLH
    bool tightLLH_nod0;       ///< tightLLH

    bool isChargeFlip;        ///< Charge flip flag from RecoTruthMatch

    //AT: This is insane >50!
    float res_all_dn;
    float res_all_up;
/*
    float res_matCalo_dn;
    float res_matCalo_up;
    float res_matCryo_dn;
    float res_matCryo_up;
    float res_matGap_dn;
    float res_matGap_up;
    float res_matId_dn;
    float res_matId_up;
    float res_nom;
    float res_none;
    float res_pileup_dn;
    float res_pileup_up;
    float res_sampTerm_dn;
    float res_sampTerm_up;
    float res_z_dn;
    float res_z_up;
*/
    float scale_all_dn;
    float scale_all_up;
/*
    float scale_G4_dn;
    float scale_G4_up;
    float scale_L1_dn;
    float scale_L1_up;
    float scale_L2_dn;
    float scale_L2_up;
    float scale_LArCalib_dn;
    float scale_LArCalib_up;
    float scale_LArECalib_dn;
    float scale_LArECalib_up;
    float scale_LArEunconv_dn;
    float scale_LArEunconv_up;
    float scale_LArUnconv_dn;
    float scale_LArUnconv_up;
    float scale_last;
    float scale_matCalo_dn;
    float scale_matCalo_up;
    float scale_matCryo_dn;
    float scale_matCryo_up;
    float scale_matId_dn;
    float scale_matId_up;
    float scale_nom;
    float scale_none;
    float scale_ped_dn;
    float scale_ped_up;
    float scale_ps_dn;
    float scale_ps_up;
    float scale_s12_dn;
    float scale_s12_up;
    float scale_ZeeStat_dn;
    float scale_ZeeStat_up;
    float scale_ZeeSys_dn;
    float scale_ZeeSys_up;
*/
    float eff_corr_dn;
    float eff_corr_up;
    float eff_uncorr_dn;
    float eff_uncorr_up;
/*
    float scale_mom_dn;
    float scale_mom_up;
*/
    // Polymorphism, baby!!
    bool isEle() const { return true;  }
    bool isMu()  const { return false; }

    /// Shift energy up/down for systematic
    void setState(int sys);

    /// Print method
    void print() const;

    /// Clear vars
    void clear(){
      clusE = clusEta = clusPhi = trackPt = 0;
      veryLooseLLH = looseLLH= mediumLLH = tightLLH = false;
      looseLLH_nod0= mediumLLH_nod0 = tightLLH_nod0 = false;
      isChargeFlip = false;
      
      res_all_dn= res_all_up= 0;
      /*
      res_matCalo_dn= res_matCalo_up=0;
      res_matCryo_dn= res_matCryo_up= res_matGap_dn= res_matGap_up=0;
      res_matId_dn= res_matId_up= res_nom= res_none=0;
      res_pileup_dn= res_pileup_up=0;
      res_sampTerm_dn=res_sampTerm_up= res_z_dn=res_z_up=0;
      */
      scale_all_dn=scale_all_up=0;
/*
      scale_G4_dn=scale_G4_up=0;
      scale_L1_dn=scale_L1_up=scale_L2_dn=scale_L2_up=0;
      scale_LArCalib_dn= scale_LArCalib_up=
      scale_LArECalib_dn=scale_LArECalib_up=0;
      scale_LArEunconv_dn=scale_LArEunconv_up=
      scale_LArUnconv_dn=scale_LArUnconv_up=0;
      scale_last=0;
      scale_matCalo_dn=scale_matCalo_up=
      scale_matCryo_dn=scale_matCryo_up=scale_matId_dn=0;
      scale_matId_up=scale_nom=scale_none=0;
      scale_ped_dn=scale_ped_up=scale_ps_dn=
      scale_ps_up=scale_s12_dn=scale_s12_up=0;
      scale_ZeeStat_dn=scale_ZeeStat_up=
      scale_ZeeSys_dn=scale_ZeeSys_up=scale_mom_dn=scale_mom_up=0;
*/
      eff_corr_dn = eff_corr_up =0;
      eff_uncorr_dn = eff_uncorr_up =0;

      Lepton::clear();
    }

    ClassDef(Electron, 7);
};
} //Susy
#endif
