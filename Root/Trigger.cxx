// ------------------------------------------- //
// Class/Tool to handle trigger information    //
// stored in SusyNt                            //
//                                             //
// author: Daniel Antrim                       //
// date  : April 6 2015                        //
// ------------------------------------------- //


#include <map>

#include "TH1F.h"
#include "TFile.h"
#include "TChain.h"
#include <iostream>


#include "SusyNtuple/Trigger.h"

using namespace std;

/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/* Trigger "containers"                        */
/*   Used at the SusyNt writing stage to set   */
/*   which triggers are stored in the final    */
/*   output ntuple.                            */

/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////

////////////
// Run 1
////////////
const std::vector<std::string> triggers_run1 = {
    // 2012
    // electron triggers
    "EF_e7_medium1",
    "EF_e12Tvh_loose1",
    "EF_e12Tvh_medium1",
    "EF_e24vh_medium1",
    "EF_e24vhi_medium1",
    "EF_2e12Tvh_loose1",
    "EF_e24vh_medium1_e7_medium1",

    // muon triggers
    "EF_mu4T",
    "EF_mu6",
    "EF_mu8",
    "EF_mu13",
    "EF_mu18_tight",
    "EF_mu24",
    "EF_mu24i_tight",
    "EF_2mu6",
    "EF_2mu13",
    "EF_mu18_tight_mu8_EFFS",
    "EF_e12Tvh_medium1_mu8",
    "EF_mu18_tight_e7_medium1",
    "EF_3mu6",
    "EF_e18vh_medium1_2e7T_medium1",
    "EF_mu18_tight_2mu4_EFFS",
    "EF_2e7T_medium1_mu6",
    "EF_e7T_medium1_2mu6",
    
    // photon triggers
    "EF_g20_loose",
    "EF_g40_loose",
    "EF_g60_loose",
    "EF_g80_loose",
    "EF_g100_loose",
    "EF_g120_loose",

    // tau triggers
    "EF_tau20_medium1",
    "EF_tau20Ti_medium1",
    "EF_tau29Ti_medium1",
    "EF_tau29Ti_medium1_tau20Ti_medium1",
    "EF_tau20Ti_medium1_e18vh_medium1",
    "EF_tau20_medium1_mu15",

    // lep-tau matching
    "EF_e18vh_medium1",
    "EF_mu15",

    // MET trigger
    "EF_2mu8_EFxe40wMu_tclcw",
    "EF_xe80_tclcw_loose",
    
    // jet + met
    "EF_j110_aftchad_xe90_tclcw_loose",
    "EF_j80_a4tchad_xe100_tclcw_loose",
    "EF_j80_a4tchad_xe70_tclcw_dphi2j45xe10",

    // more triggers for ISR analysis
    "EF_mu4T_j65_a4tchad_xe70_tclcw_veryloose",
    "EF_2mu4T_xe60_tclcw",
    "EF_2mu8_EFxe40_tclcw",
    "EF_e24vh_medium1_EFxe35_tclcw",
    "EF_mu24_j65_a4tchad_EFxe40_tclcw",
    "EF_mu24_j65_a4tchad_EFxe40wMu_tclcw"
};

////////////
// Run 2
////////////
const std::vector<std::string> triggers_run2 = {

    // Trigger list last update:
    //     dantrim July 30 2015

    ////////////////////////////
    // MUON
    ////////////////////////////

    // muon trig
    "HLT_mu26_imedium",
    "HLT_mu24_imedium",
    "HLT_mu24_iloose_L1MU15",
    "HLT_mu20_iloose_L1MU15",
    "HLT_mu50",
    "HLT_mu60_0eta105_msonly",

    // dimuon trig
    "HLT_2mu14",
    "HLT_2mu10",
    "HLT_mu24_mu8noL1",
    "HLT_mu22_mu8noL1",
    "HLT_mu20_mu8noL1",
    "HLT_mu18_mu8noL1",

    ////////////////////////////
    // ELECTRON
    ////////////////////////////

    // electronTrigCut
    "HLT_e26_tight_iloose",
    "HLT_e60_medium",
    "HLT_e24_tight_iloose",
    "HLT_e24_medium_iloose_L1EM20VH",
    "HLT_e24_medium_iloose_L1EM18VH",

    // electronTrigLH
    "HLT_e26_lhtight_iloose",
    "HLT_e60_lhmedium",
    "HLT_e24_lhtight_iloose",
    "HLT_e24_lhmedium_iloose_L1EM20VH",
    "HLT_e24_lhmedium_iloose_L1EM18VH",

    // dielectronTrigCut
    "HLT_2e17_loose",
    "HLT_2e15_loose_L12EM13VH",
    "HLT_2e12_loose_L12EM10VH",

    // dielectronTrigLH
    "HLT_2e17_lhloose",
    "HLT_2e15_lhloose_L12EM13VH",
    "HLT_2e12_lhloose_L12EM10VH",

    ////////////////////////////
    // ELE/MUON
    ////////////////////////////

    // elemuonTrigCut
    "HLT_e17_loose_mu14",
    "HLT_e7_medium_mu24",
    "HLT_e26_medium_L1EM22VHI_mu8noL1",
    "HLT_e24_medium_L1EM20VHI_mu8noL1",

    // elemuonTrigLH
    "HLT_e17_lhloose_mu14",
    "HLT_e7_lhmedium_mu24",
    "HLT_e26_lhmedium_L1EM22VHI_mu8noL1",
    "HLT_e24_lhmedium_L1EM20VHI_mu8noL1",

    ////////////////////////////
    // MET
    ////////////////////////////

    "HLT_xe35",
    "HLT_xe35_pueta",
    "HLT_xe50",
    "HLT_xe50_pueta",
    "HLT_xe70",
    "HLT_xe70_pueta",
    "HLT_xe100",
    "HLT_xe100_pueta"

/*    // DC14
    // electron triggers
    "HLT_e5_etcut",
    "HLT_e5_lhtight",
    "HLT_e9_etcut",
    "HLT_e9_lhtight",
    "HLT_e14_etcut",
    "HLT_e14_lhtight",
    "HLT_e20_medium",
    "HLT_e24_medium1_iloose",
    "HLT_e24_loose1",
    "HLT_e24_tight_iloose",
    "HLT_e25_lhvloose_L1EM15",
    "HLT_e25_etcut_L1EM15",
    "HLT_e26_tight_iloose",
    "HLT_e26_lhtight_iloose",
    "HLT_e28_tight1_iloose",
    "HLT_e30_etcut_L1EM15",
    "HLT_e40_etcut_L1EM15",
    "HLT_e60_medium",
    "HLT_e60_loose1",
    "HLT_e60_medium1",
    "HLT_e60_lhmedium",

    // dielectron triggers
    "HLT_2e12_loose1",
    "HLT_2e12_loose_L12EM10VH",
    "HLT_2e17_loose1",
    "HLT_2e17_lhloose",

    // muon triggers
    "HLT_mu4",
    "HLT_mu6",
    "HLT_mu10",
    "HLT_mu14",
    "HLT_mu18",
    "HLT_mu20",
    "HLT_mu22",
    "HLT_mu24",
    "HLT_mu24_imedium",
    "HLT_mu26",
    "HLT_mu26_imedium",
    "HLT_mu50",
    "HLT_mu60_0eta105_msonly",
    
    // dimuon triggers
    "HLT_2mu4",
    "HLT_2mu6",
    "HLT_2mu10",
    "HLT_2mu14",
    "HLT_mu24_mu8noL1",
    "HLT_mu14_iloose_mu14",
    "HLT_mu20_imedium_mu8noL1",
    "HLT_mu20_iloose_mu8noL1",

    // el/mu triggers
    "HLT_e17_loose_mu14",
    "HLT_e17_medium_mu12",
    
    // multi-lepton triggers
    "HLT_3mu6",
    "HLT_e12loose_2mu10",

    // photon triggers
    "HLT_g120_loose1",
    "HLT_g140_loose1",
    
    // tau triggers
    "HLT_e18_loose1_tau25_medium1_calo",
    "HLT_e18_lhloose1_tau25_medium1_calo",
    "HLT_mu14_tau25_medium1_calo",
    "HLT_tau35_medium1_calo_tau25_medium1_calo",

    // met triggers
    "HLT_xe50_cell",
    "HLT_xe60",
    "HLT_xe70",
    "HLT_xe100",
    "HLT_xe100_cell",

    // jet triggers
    "HLT_j400",
    "HLT_3j175"
*/
};

std::vector<std::string> getTrigNames(string set)
{ 
    if(set.compare("run1")==0) { 
        std::cout << std::endl;
        std::cout << " ------------------------ " << std::endl;
        std::cout << " Storing Run1 trigger set " << std::endl;
        std::cout << " ------------------------ " << std::endl;
        std::cout << std::endl;
        return triggers_run1;
    }
    else if(set.compare("run2")==0) {
        std::cout << std::endl;
        std::cout << " ------------------------ " << std::endl;
        std::cout << " Storing Run2 trigger set " << std::endl;
        std::cout << " ------------------------ " << std::endl;
        std::cout << std::endl;
        return triggers_run2;
    }
    else {
        std::cout << "getTrigNames error: Requested set of triggers ("<< set << ") not available. Storing Run-2 triggers instead." << std::endl;
        return triggers_run2;
    }
}

// ---------- !! PARADIGM SHIFT !! ----------- //
// ---------- !! PARADIGM SHIFT !! ----------- //
// ---------- !! PARADIGM SHIFT !! ----------- //



/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/* Trigger tool                                */
/*    To be used at the analysis level of      */
/*    SusyNt                                   */

/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////

// ------------------------------------------- //
// Constructor                                 //
// ------------------------------------------- //
Trigger::Trigger(TChain* input_chain, bool dbg) :
    m_dbg(dbg),
    m_trigHisto(NULL)
{
    if(m_dbg)
        cout << " ------------------ " << endl
             << "Initializing Trigger" << endl
             << " ------------------ " << endl;
    m_trigHisto = static_cast<TH1F*>(input_chain->GetFile()->Get("trig"));
    m_triggerMap.clear();
    buildTriggerMap();
    if(m_dbg)
        cout << " ------------------ " << endl;
}
// ------------------------------------------- //
// Build trigger-map 
// ------------------------------------------- //
// BinLabels <---> trigger name
// BinNumber <---> trigger bit number

//void Trigger::buildTriggerMap(TChain* susyNt, bool dbg)
void Trigger::buildTriggerMap()
{
    for(int trigBin = 1; trigBin < m_trigHisto->GetNbinsX(); trigBin++) {
        string triggerChainName = m_trigHisto->GetXaxis()->GetBinLabel(trigBin);
        m_triggerMap[triggerChainName] = trigBin-1;
        
        if(m_dbg) {
            cout << "Trigger " << triggerChainName << " at bit " << m_triggerMap[triggerChainName] << endl;
        }
    }
}

// ------------------------------------------- //
// Test whether a given trigger has fired
// ------------------------------------------- //
bool Trigger::passTrigger(TBits& triggerbits, std::string triggerName)
{
    if(m_triggerMap.find(triggerName)!=m_triggerMap.end()){
        return triggerbits.TestBitNumber(m_triggerMap[triggerName]);
    }
    else {
        std::cout << "Trigger " << triggerName << " not available!!" << std::endl;
        std::cout << "Dumping available triggers and exitting." << std::endl;
        dumpTriggerInfo();
        exit(1);
    }
//    std::map<std::string, int>::const_iterator trig_it = m_triggerMap.begin();
//    std::map<std::string, int>::const_iterator trig_end = m_triggerMap.end(); 
//    return triggerbits.TestBitNumber(m_triggerMap[triggerName]);
}

// ------------------------------------------- //
// Dump information about what triggers are
// stored in the SusyNt
// ------------------------------------------- //
void Trigger::dumpTriggerInfo()
{
   
    // remember: bit is stored as (bin # - 1) 
    cout << " // ---------------------------------- // " << endl;
    cout << "    Available triggers                    " << endl;
    cout << "    Name : Bit                            " << endl;
    for(int trigBin = 1; trigBin < m_trigHisto->GetNbinsX(); trigBin++) {
        string triggerChainName = m_trigHisto->GetXaxis()->GetBinLabel(trigBin);
        cout << "    " << m_trigHisto->GetXaxis()->GetBinLabel(trigBin) << " : " << trigBin-1 << endl;
    }
    cout << " // ---------------------------------- // " << endl;
}
        
    
    





