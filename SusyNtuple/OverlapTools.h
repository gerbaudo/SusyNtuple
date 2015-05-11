#ifndef SUSYNTUPLE_OVERLAPTOOLS_H
#define SUSYNTUPLE_OVERLAPTOOLS_H

#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNt.h"
#include "SusyNtuple/AnalysisType.h"

namespace Susy {

    /// A class to  perform the overlap removal
    

    class OverlapTools
    {
    public :
        OverlapTools();
        /// call this to use the Run-2 overlap procedure
        /**
            Set the analysis type. Controls whether to use the
            harmonized OR and any potential analysis-specific
            OR procedure.
        */
        OverlapTools& setAnalysis(const AnalysisType& a);
        /// check whether running Run-2 procedure
        /**
            Calling this returns a boolean, m_doHarmonization, which
            is set true depending on AnalysisType. Call 'doHarmonization'
            when deciding to perform OR on baseline objects (Run-1) or 
            signal objects (Run-2) in the code where the object-selection
            is taking place.
        */
        bool doHarmonization();

        /// main overlap removal function
        void performOverlap(ElectronVector& electrons, MuonVector& muons,
                            TauVector& taus, JetVector& jets);
        /// remove jets from electrons
        void j_e_overlap(ElectronVector& electrons, JetVector& jets);
        /// remove electrons from jets
        void e_j_overlap(ElectronVector& electrons, JetVector& jets);
        /// remove muons from jets
        void m_j_overlap(MuonVector& muons, JetVector& jets);
        /// remove electrons and muons that overlap with each-other
        void e_m_overlap(ElectronVector& electrons, MuonVector& muons);
        /// remove (sub-lead) electron from (lead) electron
        void e_e_overlap(ElectronVector& electrons);

        /// remove (pairs) overlapping muons
        void m_m_overlap(MuonVector& muons);
        /// remove taus overlapping with electrons
        void t_e_overlap(TauVector& taus, ElectronVector& electrons);
        /// remove taus overlapping with muons
        void t_m_overlap(TauVector& taus, MuonVector& muons);
        /// remove jets overlapping with taus
        void j_t_overlap(TauVector& taus, JetVector& jets);



    protected :
        /** This flag sets whether to perform the Run-2 overlap removal procedure.
            If true the Run-2 procedure is used. */
        bool m_doHarmonization;
        bool m_verbose;

        
        ////////////////////////////////
        // Cuts for overlap
        ////////////////////////////////
        float E_E_DR  =0.05; ///< dR cone for performing e-e overlap (remove sub-leading electron)
        float J_E_DR  =0.2;  ///< dR cone for performing jet-electron overlap (remove jet)
        float T_E_DR  =0.2;  ///< dR cone for performing tau-electron overlap (remove tau)
        float T_M_DR  =0.2;  ///< dR cone for performing tau-muon overlap (remove tau)
        float J_T_DR  =0.2;  ///< dR cone for performing jet-muon overlap (remove jet)
        float E_J_DR  =0.4;  ///< dR cone for performing electron-jet overlap (remove electron)
        float M_J_DR  =0.4;  ///< dR cone for performing muon-jet overlap (remove muon)
        float E_M_DR  =0.01; ///< dR cone for performing electron-muon overlap (remove both the electron AND muon)
        float M_M_DR  =0.05; ///< dR cone for performing muon-muon overlap (remove both muons)
        
    




    }; // class OverlapTools





}







#endif
