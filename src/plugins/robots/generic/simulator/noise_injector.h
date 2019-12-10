/**
 * @file <argos3/plugins/simulator/robots/generic/simulator/noise_injector.h>
 *
 * @author John Harwell - <john.r.harwell@gmail.com>
 */

#ifndef NOISE_INJECTOR_H
#define NOISE_INJECTOR_H

#include <string>

#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

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

     CNoiseInjector() {}

     ~CNoiseInjector() {}

     /**
      * @brief Initialize noise injection by reading XML configuration.
      */
     void Init(TConfigurationNode& t_tree);

     /**
      * @brief Returns \c TRUE if noise injection has been configured, and \c
      * FALSE otherwise.
      */
     bool Enabled(void) { return m_bEnabled; }

     /**
      * @brief Inject noise according to configuration parameters.
      *
      * If noise injection has not been configured, returns 0.0.
      */

     Real InjectNoise(void);

     /**
      * @brief Configure Uniform(a,b) noise injection from the specified range.
      */
     void InitUniform(const CRange<Real>& cUniformRange);

     /**
      * @brief Configure Gaussian(mean, stddev) noise injection from the
      * specified parameters.
      */
     void InitGaussian(Real fMean, Real fStdDev);

   private:

     /**
      * @brief Configure Uniform(a,b) noise injection by reading XML
      * configuration.
      */
     void InitUniform(TConfigurationNode& t_tree);

     /**
      * @brief Configure Gaussian(mean, stddev) noise injection by reading XML
      * configuration.
      */
     void InitGaussian(TConfigurationNode& t_tree);

      /** Random number generator */
      CRandom::CRNG* m_pcRNG{NULL};

      /** Whether to add noise or not */
      bool m_bEnabled{false};

      /** The noise model: uniform or gaussian */
      std::string m_strModel{""};

      /** Uniform noise range */
      CRange<Real> m_cUniformRange{};

      /** Gaussian noise mean */
      Real m_fGaussianMean{0.0};

      /** Gaussian noise standard deviation */
      Real m_fGaussianStdDev{1.0};
   };
}

#endif
