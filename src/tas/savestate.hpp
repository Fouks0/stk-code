//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2019 Fouks
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_SAVESTATE_HPP
#define HEADER_SAVESTATE_HPP

#include <memory>

#include "items/powerup_manager.hpp"
#include "LinearMath/btTransform.h"
#include "LinearMath/btAlignedObjectArray.h"
#include "physics/kart_motion_state.hpp"
#include "karts/max_speed.hpp"
#include "karts/skidding.hpp"
#include "tracks/terrain_info.hpp"
#include "ISceneNode.h"
#include "stats.hpp"

class AbstractKart;
class Kart;
class btMotionState;
class btVehicleRaycaster;
class LinearWorld;

// Stores a Save State (for now, only a kart infos and a bit more like the time)
// Basically saving most attributes from the kart, which is not really trivial Lol... But as there are pointers and Non Copyables everywhere....
// Most of the times, the state is well restored, but sometimes, the save will be a bit off for some reason, so it is needed to restart the race once a while...
// Attachments and Thunderbird are not saved so if you use a Save State just after such events, just wait that they disappear...
class SaveState
{
private:
    LinearWorld *m_linear_world; // Current World
    Kart *m_player_kart; // Current Player Kart. For now, only support Single Player TASes

    bool m_valid;
    uint64_t m_tick;
    uint64_t m_time_ticks; // For World
    double m_time;
    Stats m_stats;

    // Moveable attributes
    Vec3  kartVelocityLC;
    float kartHeading;
    float kartPitch;
    float kartRoll;
    btTransform kartTransform;
    // UserPointer kartUserPointer;
    irr::scene::ISceneNode *kartNode;
    // std::unique_ptr<btRigidBody> m_body;
        // btCollisionObject attributes
        btTransform kartBodyWorldTransform;
        btTransform kartBodyInterpolationWorldTransform;
        btVector3 kartBodyInterpolationLinearVelocity;
        btVector3 kartBodyInterpolationAngularVelocity;
        btVector3 kartBodyAnisotropicFriction;
        int kartBodyHasAnisotropicFriction;
        btScalar kartBodyContactProcessingThreshold;
        /*btBroadphaseProxy* kartBodyBroadphaseHandle;
        btCollisionShape* kartBodyCollisionShape;
        void* kartBodyExtensionPointer;
        btCollisionShape* kartBodyRootCollisionShape;*/
        int kartBodyCollisionFlags;
        int kartBodyIslandTag1;
        int kartBodyCompanionId;
        int kartBodyActivationState1;
        btScalar kartBodyDeactivationTime;
        btScalar kartBodyFriction;
        btScalar kartBodyRestitution;
        int kartBodyInternalType;
        // void* kartBodyUserObjectPointer;
        btScalar kartBodyHitFraction;
        btScalar kartBodyCcdSweptSphereRadius;
        btScalar kartBodyCcdMotionThreshold;
        int kartBodyCheckCollideWith;
        // btRigidBody attributes
        btMatrix3x3	kartBodyInvInertiaTensorWorld;
        btVector3 kartBodyLinearVelocity;
        btVector3 kartBodyAngularVelocity;
        btScalar kartBodyInverseMass;
        btVector3 kartBodyinearFactor;
        btVector3 kartBodyGravity;
        btVector3 kartBodyGravity_acceleration;
        btVector3 kartBodyInvInertiaLocal;
        btVector3 kartBodyTotalForce;
        btVector3 kartBodyTotalTorque;
        btScalar kartBodyLinearDamping;
        btScalar kartBodyAngularDamping;
        bool  kartBodyAdditionalDamping;
        btScalar kartBodyAdditionalDampingFactor;
        btScalar kartBodyAdditionalLinearDampingThresholdSqr;
        btScalar kartBodyAdditionalAngularDampingThresholdSqr;
        btScalar kartBodyAdditionalAngularDampingFactor;
        btScalar kartBodyLinearSleepingThreshold;
        btScalar kartBodyAngularSleepingThreshold;
        btMotionState *kartBodyOptionalMotionState;
        btMotionState* m_optionalMotionState;
        // btAlignedObjectArray<btTypedConstraint*> m_constraintRefs;
        int kartBodyRigidbodyFlags;
        int kartBodyDebugBodyId;
        btVector3 kartBodyDeltaLinearVelocity;
        btVector3 kartBodyDeltaAngularVelocity;
        btVector3 kartBodyAngularFactor;
        btVector3 kartBodyInvMass;
        btVector3 kartBodyPushVelocity;
        btVector3 kartBodyTurnVelocity;
    KartMotionState kartMotionState;
        btTransform kartMotionStateCenterOfMass;   // btTransform m_center_of_mass;

    // AbstractKart attributes
    /*float m_kart_length;
    float m_kart_width;
    float m_kart_height;
    float m_kart_highest_point;
    const Vec3* m_wheel_graphics_position;
    unsigned int m_world_kart_id;*/
    btTransform kartStartingTransform;
    /*int m_live_join_util;
    std::unique_ptr<KartProperties> m_kart_properties;
    PerPlayerDifficulty m_difficulty;
    std::unique_ptr<KartModel> m_kart_model;
    std::unique_ptr<Attachment> m_attachment;*/
    KartControl kartControls;
    /*AbstractKartAnimation *m_kart_animation;
    irr::scene::IDummyTransformationSceneNode    *m_wheel_box;*/

    // Kart attributes
    /*int m_network_finish_check_ticks;
    int m_network_confirmed_finish_ticks;*/
    float kartGraphicalYOffset;
    Vec3 kartXyzFront;
    // const float XYZ_HISTORY_TIME = 0.25f;
    int kartXyzHistorySize;
    std::vector<Vec3> kartPreviousXyz;
    std::vector<float> kartPreviousXyzTimes;
    float kartTimePreviousCounter;
    bool kartJumping;
    // bool m_enabled_network_spectator;
    bool kartBubblegumTorqueSign;
    uint8_t kartBounceBackTicks;
    MaxSpeed kartMaxSpeed;
    TerrainInfo kartTerrainInfo;
    // Powerup *m_powerup;
        PowerupManager::PowerupType kartPowerUpType;
        int kartPowerUpCount;
    btVehicleRaycaster *kartVehicleRaycaster;
    // std::unique_ptr<btKart> m_vehicle;
        btAlignedObjectArray<btVector3> kartVehicleForwardWS;
        btAlignedObjectArray<btVector3> kartVehicleAxle;
        btAlignedObjectArray<btScalar>  kartVehicleForwardImpulse;
        btAlignedObjectArray<btScalar>  kartVehicleSideImpulse;
        int kartVehicleUserConstraintType;
        int kartVehicleUserConstraintId;
        btScalar kartVehicleDamping;
        btVehicleRaycaster *kartVehicleVehicleRaycaster;
        bool kartVehicleAllowSliding;
        btVector3 kartVehicleAdditionalImpulse;
        uint16_t kartVehicleTicks_additionalImpulse;
        float kartVehicleAdditionalRotation;
        uint16_t kartVehicleTicks_additionalRotation;
        btRigidBody kartVehicleChassisBody;
        int kartVehicleNum_wheels_on_ground;
        int kartVehicleIndexRightAxis;
        int kartVehicleIndexUpAxis;
        int kartVehicleIndexForwardAxis;
        btScalar kartVehicleMin_speed;
        btScalar kartVehicleMax_speed;
        bool kartVehicleVisual_wheels_touch_ground;
        btAlignedObjectArray<btWheelInfo> kartVehicleWheelInfo;
    Skidding kartSkidding;
    /*std::unique_ptr<Stars> m_stars_effect;
    std::unique_ptr<Shadow> m_shadow;
    std::unique_ptr<SkidMarks> m_skidmarks;
    std::unique_ptr<KartGFX> m_kart_gfx;
    std::unique_ptr<SlipStream> m_slipstream;*/
    btCompoundShape kartChassis;
    /*ParticleEmitter *m_collision_particles;
    Controller  *m_controller;
    Controller  *m_saved_controller;
    PowerupManager::PowerupType m_last_used_powerup;*/
    bool kartFlying;
    /*bool m_has_caught_nolok_bubblegum;
    bool m_race_result;
    bool m_eliminated;
    int m_initial_position;
    int m_race_position;
    float        m_max_gear_rpm;*/
    int kartBrakeTicks;
    /*int16_t      m_invulnerable_ticks;
    int16_t      m_bubblegum_ticks;
    int16_t       m_view_blocked_by_plunger;*/
    float kartCurrentLean;
    int8_t kartMinNitroTick;
    /*bool         m_fire_clicked;
    bool         m_boosted_ai;
    bool            m_finished_race;
    float           m_finish_time;*/
    float kartCollectedEnergy;
    // float         m_consumption_per_tick;
    float kartEnergyToMinRatio;
    float kartStartupBoost;
    float kartFallingTime;
    // float           m_weight;
    float kartSpeed;
    /*float         m_last_factor_engine_sound;
    float         m_default_suspension_force;
    btTransform  m_reset_transform;
    std::vector<SFXBase*> m_custom_sounds;
    int m_emitter_id = 0;
    static const int EMITTER_COUNT = 3;
    SFXBase      *m_emitters[EMITTER_COUNT];
    SFXBase      *m_engine_sound;
    SFXBase      *m_terrain_sound;
    const Material *m_last_sound_material;
    SFXBase      *m_nitro_sound;
    SFXBase      *m_previous_terrain_sound;
    SFXBase      *m_skid_sound;
    SFXBuffer    *m_horn_sound;
    static const int CRASH_SOUND_COUNT = 3;
    SFXBuffer    *m_crash_sounds[CRASH_SOUND_COUNT];
    SFXBuffer    *m_goo_sound;
    SFXBuffer    *m_boing_sound;*/
    int kartTicksLastCrash;
    //RaceManager::KartType m_type;

public:
    SaveState() {reset();}
    void reset();
    void create(uint64_t, Stats, LinearWorld*, AbstractKart*);
    void restore(LinearWorld*, AbstractKart*);

    uint64_t getTick() const {return m_tick;}
    bool isValid() const {return m_valid;}
    Stats getStats() const {return m_stats;}
};

#endif
