// Dear emacs, this is -*- c++ -*-
#ifndef SUSYNTUPLE_LEPTON_H
#define SUSYNTUPLE_LEPTON_H

#include "SusyNtuple/Particle.h"

namespace Susy
{
/// Lepton class, common to electrons and muons
class Lepton : public Particle
{
public:
    Lepton(){ clear(); }
    virtual ~Lepton(){};
    /** Copy constructor */
    Lepton(const Lepton &);
    /** Assignment operator */
    Lepton& operator=(const Lepton &);

    // public member vars
    int q;                    ///< Charge
    bool isBaseline; ///< flag computed by SUSYTools_xAOD
    bool isSignal; ///< flag computed by SUSYTools_xAOD
    float etcone20;           ///< etcone20 for mu, topoEtcone20_corrected for el
    float ptcone20;           ///< ptcone20 isolation
    float ptcone30;           ///< ptcone30 isolation
    //\todo: move additional iso variables here??

    float d0;                 ///< d0 extrapolated to PV
    float errD0;              ///< Uncertainty on d0
    float z0;                 ///< z0 extrapolated to PV
    float errZ0;              ///< Uncertainty on z0

    unsigned int mcType;      ///< MCTruthClassifier particle type
    unsigned int mcOrigin;    ///< MCTruthClassifier particle origin

    bool matched2TruthLepton; ///< flag from RecoTruthMatch::Matched2TruthLepton
    int truthType;            ///< RecoTruthMatch::type

    float effSF;              ///< Efficiency scale factor
    float errEffSF;           ///< Uncertainty on the efficiency scale factor

    //unsigned int trigFlags; ///< Bit word representing matched trigger chains
    long long trigFlags;      ///< Bit word representing matched trigger chains

    /// Methods to return impact parameter variables
    /** Note that these are not absolute valued! */
    float d0Sig() const {
      return d0/errD0;
    }
    float z0SinTheta() const {
      return z0 * sin(Theta());
    }

    /// Trigger matching
    /** Provide the trigger chain via bit mask, e.g. TRIG_mu18 */
    bool matchTrig(unsigned int mask) const {
      return (trigFlags & mask) == mask;
    }

    // Polymorphism, baby!!
    virtual bool isEle() const { return false; }
    virtual bool isMu()  const { return false; }
    void setState(int sys){ resetTLV(); }

    /// Print method
    virtual void print() const {};

    /// Clear vars
    void clear(){
      q = etcone20 = ptcone20 = ptcone30 = 0;
      d0 = errD0 = z0 = errZ0 = 0;
      mcType = mcOrigin = 0;
      matched2TruthLepton = false;
      truthType = -1;
      effSF = 1;
      errEffSF = 0;
      trigFlags = 0;
      isBaseline = isSignal = false;
      Particle::clear();
    }

    ClassDef(Lepton, 13);
};
} // Susy
#endif
