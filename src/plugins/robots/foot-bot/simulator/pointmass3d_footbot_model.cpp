/**
 * @file <argos3/plugins/robots/foot-bot/simulator/pointmass3d_footbot_model.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "pointmass3d_footbot_model.h"
#include <argos3/core/utility/math/cylinder.h>

namespace argos {

   static const Real FOOTBOT_RADIUS                   = 0.085036758f;
   static const Real FOOTBOT_INTERWHEEL_DISTANCE      = 0.14f;
   static const Real FOOTBOT_HEIGHT                   = 0.146899733f;

   enum FOOTBOT_WHEELS {
      FOOTBOT_LEFT_WHEEL = 0,
      FOOTBOT_RIGHT_WHEEL = 1
   };

   /****************************************/
   /****************************************/

   CPointMass3DFootBotModel::CPointMass3DFootBotModel(CPointMass3DEngine& c_engine,
                                                      CFootBotEntity& c_footbot) :
      CPointMass3DModel(c_engine, c_footbot.GetEmbodiedEntity()),
      m_cWheeledEntity(c_footbot.GetWheeledEntity()),
      m_fCurrentWheelVelocity(m_cWheeledEntity.GetWheelVelocities()) {
      /* Register the origin anchor update method */
      RegisterAnchorMethod(GetEmbodiedEntity().GetOriginAnchor(),
                           &CPointMass3DFootBotModel::UpdateOriginAnchor);
   }

   /****************************************/
   /****************************************/

   void CPointMass3DFootBotModel::Reset() {
      CPointMass3DModel::Reset();
      m_fAngularVelocity = 0.0;
   }

   /****************************************/
   /****************************************/


   void CPointMass3DFootBotModel::UpdateFromEntityStatus() {
     m_cVelocity.Set((m_fCurrentWheelVelocity[FOOTBOT_RIGHT_WHEEL] + m_fCurrentWheelVelocity[FOOTBOT_LEFT_WHEEL]) * 0.5,
                     0.0,
                     0.0);
      m_cVelocity.Rotate(m_cOrientation);
      m_fAngularVelocity = (m_fCurrentWheelVelocity[FOOTBOT_RIGHT_WHEEL] -
                            m_fCurrentWheelVelocity[FOOTBOT_LEFT_WHEEL]) / FOOTBOT_INTERWHEEL_DISTANCE;
   }

   /****************************************/
   /****************************************/

   void CPointMass3DFootBotModel::Step() {
      m_cPosition += m_cVelocity * m_cPM3DEngine.GetPhysicsClockTick();

      auto pitch_angle = ATan2(-m_cVelocity.GetX(),
                               Sqrt(Square(m_cVelocity.GetY()) + Square(m_cVelocity.GetZ())));
      auto roll_angle = ATan2(m_cVelocity.GetY(), m_cVelocity.GetZ());

      argos::CRadians cYaw(m_fAngularVelocity * m_cPM3DEngine.GetPhysicsClockTick());
      argos::CRadians cPitch(pitch_angle * m_cPM3DEngine.GetPhysicsClockTick());
      argos::CRadians cRoll(roll_angle * m_cPM3DEngine.GetPhysicsClockTick());
      m_cOrientation.FromEulerAngles(cYaw, cPitch, cRoll);
   }

   /****************************************/
   /****************************************/

   void CPointMass3DFootBotModel::CalculateBoundingBox() {
      GetBoundingBox().MinCorner.Set(
         GetEmbodiedEntity().GetOriginAnchor().Position.GetX() - FOOTBOT_RADIUS,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetY() - FOOTBOT_RADIUS,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetZ());
      GetBoundingBox().MaxCorner.Set(
         GetEmbodiedEntity().GetOriginAnchor().Position.GetX() + FOOTBOT_RADIUS,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetY() + FOOTBOT_RADIUS,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetZ() + FOOTBOT_HEIGHT);
   }

   /****************************************/
   /****************************************/

   bool CPointMass3DFootBotModel::CheckIntersectionWithRay(Real& f_t_on_ray,
                                                           const CRay3& c_ray) const {
      CCylinder m_cShape(FOOTBOT_RADIUS,
                         FOOTBOT_HEIGHT,
                         m_cPosition,
                         CVector3::Z);
      bool bIntersects = m_cShape.Intersects(f_t_on_ray, c_ray);
      return bIntersects;
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_POINTMASS3D_OPERATIONS_ON_ENTITY(CFootBotEntity, CPointMass3DFootBotModel);

   /****************************************/
   /****************************************/

}
