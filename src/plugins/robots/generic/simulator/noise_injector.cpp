/**
 * @file <argos3/plugins/robots/generic/noise_injector.cpp>
 *
 * @author John Harwell - <john.r.harwell@gmail.com>
 */

#include <argos3/plugins/robots/generic/simulator/noise_injector.h>

#include "light_default_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   void CNoiseInjector::Init(TConfigurationNode& t_tree) {
      try {
         /* parse noise model */
         GetNodeAttribute(t_tree, "model", m_strModel);

         if ("none" == m_strModel) {
           /* no noise injection */
         }
         else if ("uniform" == m_strModel) {
           InitUniform(t_tree);
           m_bEnabled = true;
           m_pcRNG = CRandom::CreateRNG("argos");
         }
         else if ("gaussian" == m_strModel) {
           InitGaussian(t_tree);
           m_bEnabled = true;
           m_pcRNG = CRandom::CreateRNG("argos");
         }
         else {
           THROW_ARGOSEXCEPTION("Bad noise model specified");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in noise injection", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CNoiseInjector::InitUniform(TConfigurationNode& t_tree) {
     /* Parse noise level */
     Real fNoiseLevel = 0.0f;
     GetNodeAttribute(t_tree, "level", fNoiseLevel);
     if (fNoiseLevel < 0.0f) {
       THROW_ARGOSEXCEPTION("Uniform noise level must be specified as non-negative value");
     }
     else if (fNoiseLevel > 0.0f) {
       m_cUniformRange.Set(-fNoiseLevel, fNoiseLevel);
     }
   } /* InitUniform() */

   /****************************************/
   /****************************************/

   void CNoiseInjector::InitUniform(const CRange<Real>& cUniformRange) {
     m_strModel = "uniform";
     m_cUniformRange = cUniformRange;
     m_bEnabled = true;
   } /* InitUniform() */

   /****************************************/
   /****************************************/

   void CNoiseInjector::InitGaussian(TConfigurationNode& t_tree) {
     /* Parse stddev */
     GetNodeAttributeOrDefault(t_tree,
                               "stddev",
                               m_fGaussianStdDev,
                               m_fGaussianStdDev);
     if (m_fGaussianStdDev < 0.0f) {
       THROW_ARGOSEXCEPTION("Gaussian standard deviation cannot be negative");
     }
     /* parse mean */
     GetNodeAttributeOrDefault(t_tree,
                               "mean",
                               m_fGaussianMean,
                               m_fGaussianMean);
   } /* InitGaussian() */

   /****************************************/
   /****************************************/

   void CNoiseInjector::InitGaussian(Real fMean, Real fStdDev) {
     m_strModel = "gaussian";
     m_fGaussianMean = fMean;
     m_fGaussianStdDev = fStdDev;
     m_bEnabled = true;
   } /* InitGaussian() */

   /****************************************/
   /****************************************/

   Real CNoiseInjector::InjectNoise(void) {
     if ("none" == m_strModel) {
       /* no noise injection */
       return 0.0;
     } else if ("uniform" == m_strModel) {
       return m_pcRNG->Uniform(m_cUniformRange);
     }
     else if ("gaussian" == m_strModel) {
       return m_pcRNG->Gaussian(m_fGaussianStdDev, m_fGaussianMean);
     }
     else {
       THROW_ARGOSEXCEPTION("Bad noise model specified");
     }
   } /* InjectNoise() */

} // namespace argos
