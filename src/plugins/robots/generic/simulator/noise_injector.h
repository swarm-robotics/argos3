/**
 * @file <argos3/plugins/simulator/robots/generic/simulator/noise_injector.h>
 *
 * @author John Harwell - <john.r.harwell@gmail.com>
 */

#ifndef NOISE_INJECTOR_H
#define NOISE_INJECTOR_H

#include <string>
#include <memory>

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/configuration/argos_configuration.h>

namespace argos {

/**
 * @brief Utility class to standardize noise injection in robot sensors and
 * actuators. Has the following noise models:
 *
 * - uniform - A Uniform(a,b) distribution
 * - gaussian - A Guassian(mean, stddev) distribution
 */

class CNoiseInjector {

   public:

     struct SDocumentationQuerySpec {
       std::string strDocName;
       std::string strXMLParent;
       std::string strXMLTag;
       std::string strSAAType;
       bool bShowExamples;
     };

     CNoiseInjector();

     virtual ~CNoiseInjector() {}

     /**
      * @brief Get the documentation for this class should appear whenever a
      * query involving a noise-injectable entity is requested.
      */
     static std::string GetQueryDocumentation(const SDocumentationQuerySpec& c_spec);

     /**
      * @brief Inject noise according to configuration parameters.
      *
      * If noise injection has not been configured it should return 0.0.
      */
     virtual Real InjectNoise() = 0;

     virtual void Init(TConfigurationNode& t_tree) = 0;

     /**
      * @brief Returns \c TRUE if noise injection has been configured, and \c
      * FALSE otherwise.
      */
     inline bool Enabled(void) { return m_bEnabled; }

     /**
      * @brief Compute a Bernoulli event based on the configured noise model.
      */
     bool BernoulliEvent(void);

   protected:

     inline void SetEnable(bool b_state) { m_bEnabled = b_state; }

     inline CRandom::CRNG* GetRNG() {
       return m_pcRNG;
     }

   private:

     /** Random number generator */
     CRandom::CRNG* m_pcRNG;

     /** Whether to add noise or not */
     bool m_bEnabled;
   };
}

#endif
