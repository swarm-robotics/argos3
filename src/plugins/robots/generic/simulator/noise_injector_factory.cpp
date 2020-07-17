/**
 * @file <argos3/plugins/robots/generic/noise_injector_factory.cpp>
 *
 * @author John Harwell - <john.r.harwell@gmail.com>
 */

#include <argos3/plugins/robots/generic/simulator/gaussian_noise_injector.h>
#include <argos3/plugins/robots/generic/simulator/uniform_noise_injector.h>
#include <argos3/plugins/robots/generic/simulator/noise_injector_factory.h>

namespace argos {

   /****************************************/
   /****************************************/

   std::unique_ptr<CNoiseInjector> CNoiseInjectorFactory::Create(TConfigurationNode& t_tree) {
      try {
        /* parse noise model */
        std::string strModel;
        GetNodeAttribute(t_tree, "model", strModel);
        if ("uniform" == strModel) {
           return std::make_unique<CUniformNoiseInjector>();
         } else if ("gaussian" == strModel) {
           return std::make_unique<CGaussianNoiseInjector>();
         } else if ("none" != strModel) {
           THROW_ARGOSEXCEPTION("Bad noise model specified");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error while initializing noise injection", ex);
      }
   }

} // namespace argos
