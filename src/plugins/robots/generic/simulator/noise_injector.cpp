/**
 * @file <argos3/plugins/robots/generic/noise_injector.cpp>
 *
 * @author John Harwell - <john.r.harwell@gmail.com>
 */

#include <argos3/plugins/robots/generic/simulator/noise_injector.h>
#include <argos3/plugins/robots/generic/simulator/gaussian_noise_injector.h>
#include <argos3/plugins/robots/generic/simulator/uniform_noise_injector.h>

namespace argos {

   /****************************************/
   /****************************************/

   CNoiseInjector::CNoiseInjector() :
       m_pcRNG(CRandom::CreateRNG("argos")),
       m_bEnabled(false) {}

   /****************************************/
   /****************************************/

   bool CNoiseInjector::BernoulliEvent() {
     return m_pcRNG->Bernoulli(InjectNoise());
   }

   /****************************************/
   /****************************************/

   std::string CNoiseInjector::GetQueryDocumentation(
       const SDocumentationQuerySpec& c_spec) {
     std::string doc =
         "If the '" + c_spec.strXMLTag + "' child tag is added to the " + c_spec.strDocName + " " +
         c_spec.strSAAType + " configuration,\n"
         "then the 'model' attribute is required and is used to specify the\n"
         "noise model used to calculate the amount of noise applied each timestep the\n" +
         c_spec.strDocName + " is enabled/active. If the '" + c_spec.strXMLTag + "' child tag does not exist,\n"
         "or the " + c_spec.strDocName + " " + c_spec.strSAAType + " is disabled no noise is injected.\n\n"

         "The following noise models can be specified:\n\n" +

         "- 'none' - Do not inject any noise; this model exists to allow for the inclusion of\n"
         "           the '" + c_spec.strXMLTag + "' tag without necessarily enabling noise injection.\n\n"

         "- 'uniform' - Injects uniformly distributed noise Uniform(-'level','level). The 'level'\n"
         "              attribute is required for this noise model.\n\n"

         "- 'gaussian' - Injects Gaussian('mean','stddev') noise. Both the 'mean' and 'stddev'\n"
         "               attributes are optional, and default to 0.0 and 1.0, respectively,\n"
         "               if omitted.\n\n";

     std::string strExamples =
         "Example XML configurations:\n\n"

         "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <" + c_spec.strSAAType +"s>\n"
                   "        ...\n"
                   "        <!-- Uniformly distributed noise -->\n"
                   "        <" + c_spec.strXMLParent + " implementation=\"default\">\n"
                   "          <" + c_spec.strXMLTag + " model=\"uniform\"\n"
                   "                 level=\"0.1\" />\n"
                   "        </" + c_spec.strXMLParent + ">\n"
                   "        <!-- Gaussian noise -->\n"
                   "        <" + c_spec.strXMLParent + " implementation=\"default\">\n"
                   "          <" + c_spec.strXMLTag + " model=\"gaussian\"\n"
                   "                 stddev=\"0.1\"\n"
                   "                 mean=\"0.1\" />\n"
                   "        </" + c_spec.strXMLParent + ">\n"
                   "        ...\n"
                   "      </" + c_spec.strSAAType +">\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
         "  </controllers>\n\n";
     if (!c_spec.bShowExamples) {
       return doc;
     } else {
       return doc + strExamples;
     }
   }
} // namespace argos
