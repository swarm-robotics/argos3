/**
 * @file <argos3/plugins/simulator/sensors/positioning_default_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>

#include "positioning_default_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   CPositioningDefaultSensor::CPositioningDefaultSensor() :
       m_pcEmbodiedEntity(NULL) {}


   /****************************************/
   /****************************************/

   void CPositioningDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
      m_sReading.Position = m_pcEmbodiedEntity->GetOriginAnchor().Position;
      m_sReading.Orientation = m_pcEmbodiedEntity->GetOriginAnchor().Orientation;
   }

   /****************************************/
   /****************************************/

   void CPositioningDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_PositioningSensor::Init(t_tree);
         /* Parse noise injection */
         if(NodeExists(t_tree, "pos_noise")) {
           TConfigurationNode& tNode = GetNode(t_tree, "pos_noise");
           m_cPosNoiseInjector.Init(tNode);
         }
         if(NodeExists(t_tree, "angle_noise")) {
           TConfigurationNode& tNode = GetNode(t_tree, "angle_noise");
           m_cAngleNoiseInjector.Init(tNode);
         }
         if(NodeExists(t_tree, "axis_noise")) {
           TConfigurationNode& tNode = GetNode(t_tree, "axis_noise");
           m_cAxisNoiseInjector.Init(tNode);
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in default positioning sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CPositioningDefaultSensor::Update() {
      m_sReading.Position = m_pcEmbodiedEntity->GetOriginAnchor().Position;
      if (m_cPosNoiseInjector.Enabled() ||
          m_cAngleNoiseInjector.Enabled() ||
          m_cAxisNoiseInjector.Enabled()) {
        m_sReading.Position += CVector3(m_cPosNoiseInjector.InjectNoise(),
                                        m_cPosNoiseInjector.InjectNoise(),
                                        m_cPosNoiseInjector.InjectNoise());
        m_pcEmbodiedEntity->GetOriginAnchor().Orientation.ToAngleAxis(m_cAngle,
                                                                      m_cAxis);
        m_cAngle += CRadians(ToRadians(CDegrees(m_cPosNoiseInjector.InjectNoise())));
        m_cAxis += CVector3(m_cAxisNoiseInjector.InjectNoise(),
                            m_cAxisNoiseInjector.InjectNoise(),
                            m_cAxisNoiseInjector.InjectNoise());
        m_sReading.Orientation.FromAngleAxis(m_cAngle, m_cAxis);
      }
      else {
         m_sReading.Orientation = m_pcEmbodiedEntity->GetOriginAnchor().Orientation;
      }
   }

   /****************************************/
   /****************************************/

   void CPositioningDefaultSensor::Reset() {
      m_sReading.Position = m_pcEmbodiedEntity->GetOriginAnchor().Position;
      m_sReading.Orientation = m_pcEmbodiedEntity->GetOriginAnchor().Orientation;
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CPositioningDefaultSensor,
                   "positioning", "default",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "A generic positioning sensor.",

                   "This sensor returns the current position and orientation of a robot. This sensor\n"
                   "can be used with any robot, since it accesses only the body component. In\n"
                   "controllers, you must include the ci_positioning_sensor.h header.\n\n"

                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <positioning implementation=\"default\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"

                   "OPTIONAL XML CONFIGURATION\n\n"

                   "It is possible to add different types of noise to the sensor, thus matching the\n"
                   "characteristics of a real robot better. This can be done by adding one or more of\n"
                   "the 'pos_noise', 'angle_noise', 'axis_noise' child tags (all noise types are\n"
                   "independent). Tag 'pos_noise' controls the noise applied to the position returned\n"
                   "by the sensor. Tag 'angle_noise' controls the noise applied to the angle (values\n"
                   "expressed in degrees) returned by the sensor. Tag 'axis_noise_range' controls the\n"
                   "noise applied to the rotation axis returned by the sensor. Note that the angle\n"
                   "AND axis sensor values are jointly used are used to calculate a quaternion, which\n"
                   "is the actual returned value for rotation, so the 'angle_noise' and 'axis_noise'\n"
                   "tags should (probably) appear together, if they appear at all.\n\n"

                   "For all tags, if the tag exists, then the 'model' attribute is required and is\n"
                   "used to specify noise model to be applied to the sensor each timestep it is\n"
                   "enabled/active:\n\n"

                   "- 'none' - Does not inject any noise; this model exists to allow for the\n"
                   "           inclusion of the 'noise' tag without necessarily enabling noise\n"
                   "           injection.\n\n"

                   "- 'uniform' - Injects uniformly distributed noise Uniform(-'level', 'level')\n"
                   "              into the sensor. The 'level' attribute is required for this noise\n"
                   "              model.\n\n"

                   "- 'gaussian' - Injects Gaussian('mean','stddev') noise into the sensor\n"
                   "               Both the 'mean' and 'stddev' attributes are optional, and\n"
                   "               default to 0.0 and 1.0, respectively, if omitted.\n\n"

                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <!-- Uniformly distributed position noise -->\n"
                   "        <positioning implementation=\"default\">\n"
                   "          <pos_noise model=\"uniform\"\n"
                   "                     level=\"0.1\" />\n"
                   "        </positioning>\n"
                   "        <!-- Angle Gaussian noise -->\n"
                   "        <positioning implementation=\"default\">\n"
                   "          <angle_noise model=\"gaussian\"\n"
                   "                      stddev=\"0.1\"\n"
                   "                      mean=\"0.1\" />\n"
                   "        </positioning>\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"

                   "None.\n",

                   "Usable"
		  );

}
