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

#include "graphics/irr_driver.hpp"
#include "guiengine/message_queue.hpp"
#include "items/powerup.hpp"
#include "karts/abstract_kart.hpp"
#include "karts/controller/player_controller.hpp"
#include "karts/kart.hpp"
#include "modes/linear_world.hpp"
#include "physics/btKart.hpp"
#include "savestate.hpp"
#include "tracks/track.hpp"
#include "utils/log.hpp"

void SaveState::reset()
{
    m_valid = false;
    m_linear_world = NULL;
    m_player_kart = NULL;
    m_tick = 0;
    m_time = 0.;
}

void SaveState::create(uint64_t tick, Stats stats, LinearWorld *linear_world, AbstractKart *player_kart)
{
    if (!linear_world || !dynamic_cast<Kart*>(player_kart))
    {
        Log::error("Save States", "Invalid world or kart!");
        return;
    }

    m_linear_world = linear_world;
    m_player_kart = dynamic_cast<Kart*>(player_kart);

    m_tick = tick;
    m_time_ticks = linear_world->m_time_ticks;
    m_time = linear_world->m_time;
    m_stats = stats;

    // SmoothNetworkBody attributes
    kartStartSmoothingPostion = m_player_kart->m_start_smoothing_postion;
    kartAdjustPosition = m_player_kart->m_adjust_position;
    kartAdjustControlPoint = m_player_kart->m_adjust_control_point;
    kartPrevPositionData = m_player_kart->m_prev_position_data;
    kartSmoothedTransform = m_player_kart->m_smoothed_transform;
    kartAdjustTime = m_player_kart->m_adjust_time;
    kartAdjustTimeDt = m_player_kart->m_adjust_time_dt;
    kartSmoothing = m_player_kart->m_smoothing;
    kartEnabled = m_player_kart->m_enabled;
    kartSmoothRotation = m_player_kart->m_smooth_rotation;
    kartAdjustVerticalOffset = m_player_kart->m_adjust_vertical_offset;
    kartMinAdjustLength = m_player_kart->m_min_adjust_length;
    kartMaxAdjustLength = m_player_kart->m_max_adjust_length;
    kartMinAdjustSpeed = m_player_kart->m_min_adjust_speed;
    kartMaxAdjustTime = m_player_kart->m_max_adjust_time;
    kartAdjustLengthThreshold = m_player_kart->m_adjust_length_threshold;

    // Moveable attributes
    kartVelocityLC = m_player_kart->m_velocityLC;
    kartHeading = m_player_kart->m_heading;
    kartPitch = m_player_kart->m_pitch;
    kartRoll = m_player_kart->m_roll;
    kartTransform = m_player_kart->m_transform;
    // UserPointer kartUserPointer;
    // irr::scene::ISceneNode *kartNode;
    // std::unique_ptr<btRigidBody> m_body;
        // btCollisionObject
        kartBodyWorldTransform = m_player_kart->m_body->m_worldTransform;
        kartBodyInterpolationWorldTransform = m_player_kart->m_body->m_interpolationWorldTransform;
        kartBodyInterpolationLinearVelocity = m_player_kart->m_body->m_interpolationLinearVelocity;
        kartBodyInterpolationAngularVelocity = m_player_kart->m_body->m_interpolationAngularVelocity;
        kartBodyAnisotropicFriction = m_player_kart->m_body->m_anisotropicFriction;
        kartBodyHasAnisotropicFriction = m_player_kart->m_body->m_hasAnisotropicFriction;
        kartBodyContactProcessingThreshold = m_player_kart->m_body->m_contactProcessingThreshold;
        /*btBroadphaseProxy* kartBodyBroadphaseHandle;
        btCollisionShape* kartBodyCollisionShape;
        void* kartBodyExtensionPointer;
        btCollisionShape* kartBodyRootCollisionShape;*/
        kartBodyCollisionFlags = m_player_kart->m_body->m_collisionFlags;
        kartBodyIslandTag1 = m_player_kart->m_body->m_islandTag1;
        kartBodyCompanionId = m_player_kart->m_body->m_companionId;
        kartBodyActivationState1 = m_player_kart->m_body->m_activationState1;
        kartBodyDeactivationTime = m_player_kart->m_body->m_deactivationTime;
        kartBodyFriction = m_player_kart->m_body->m_friction;
        kartBodyRestitution = m_player_kart->m_body->m_restitution;
        kartBodyInternalType = m_player_kart->m_body->m_internalType;
        // void* kartBodyUserObjectPointer;
        kartBodyHitFraction = m_player_kart->m_body->m_hitFraction;
        kartBodyCcdSweptSphereRadius = m_player_kart->m_body->m_ccdSweptSphereRadius;
        kartBodyCcdMotionThreshold = m_player_kart->m_body->m_ccdMotionThreshold;
        kartBodyCheckCollideWith = m_player_kart->m_body->m_checkCollideWith;
        // btRigidBody
        kartBodyInvInertiaTensorWorld = m_player_kart->m_body->m_invInertiaTensorWorld;
        kartBodyLinearVelocity = m_player_kart->m_body->m_linearVelocity;
        kartBodyAngularVelocity = m_player_kart->m_body->m_angularVelocity;
        kartBodyInverseMass = m_player_kart->m_body->m_inverseMass;
        kartBodyinearFactor = m_player_kart->m_body->m_linearFactor;
        kartBodyGravity = m_player_kart->m_body->m_gravity;
        kartBodyGravity_acceleration = m_player_kart->m_body->m_gravity_acceleration;
        kartBodyInvInertiaLocal = m_player_kart->m_body->m_invInertiaLocal;
        kartBodyTotalForce = m_player_kart->m_body->m_totalForce;
        kartBodyTotalTorque = m_player_kart->m_body->m_totalTorque;
        kartBodyLinearDamping = m_player_kart->m_body->m_linearDamping;
        kartBodyAngularDamping = m_player_kart->m_body->m_angularDamping;
        kartBodyAdditionalDamping = m_player_kart->m_body->m_additionalDamping;
        kartBodyAdditionalDampingFactor = m_player_kart->m_body->m_additionalDampingFactor;
        kartBodyAdditionalLinearDampingThresholdSqr = m_player_kart->m_body->m_additionalLinearDampingThresholdSqr;
        kartBodyAdditionalAngularDampingThresholdSqr = m_player_kart->m_body->m_additionalAngularDampingThresholdSqr;
        kartBodyAdditionalAngularDampingFactor = m_player_kart->m_body->m_additionalAngularDampingFactor;
        kartBodyLinearSleepingThreshold = m_player_kart->m_body->m_linearSleepingThreshold;
        kartBodyAngularSleepingThreshold = m_player_kart->m_body->m_angularSleepingThreshold;
       *kartBodyOptionalMotionState = *m_player_kart->m_body->m_optionalMotionState;
        //btAlignedObjectArray<btTypedConstraint*> m_constraintRefs;
        kartBodyRigidbodyFlags = m_player_kart->m_body->m_rigidbodyFlags;
        kartBodyDebugBodyId = m_player_kart->m_body->m_debugBodyId;
        kartBodyDeltaLinearVelocity = m_player_kart->m_body->m_deltaLinearVelocity;
        kartBodyDeltaAngularVelocity = m_player_kart->m_body->m_deltaAngularVelocity;
        kartBodyAngularFactor = m_player_kart->m_body->m_angularFactor;
        kartBodyInvMass = m_player_kart->m_body->m_invMass;
        kartBodyPushVelocity = m_player_kart->m_body->m_pushVelocity;
        kartBodyTurnVelocity = m_player_kart->m_body->m_turnVelocity;
    kartMotionState = *m_player_kart->m_motion_state;
        kartMotionStateCenterOfMass = m_player_kart->m_motion_state->m_center_of_mass;

    // AbstractKart attributes
    /*float m_kart_length;
    float m_kart_width;
    float m_kart_height;
    float m_kart_highest_point;
    const Vec3* m_wheel_graphics_position;
    unsigned int m_world_kart_id;*/
    kartStartingTransform = m_player_kart->m_starting_transform;
    /*int m_live_join_util;
    std::unique_ptr<KartProperties> m_kart_properties;
    PerPlayerDifficulty m_difficulty;
    std::unique_ptr<KartModel> m_kart_model;
    std::unique_ptr<Attachment> m_attachment;*/
    kartControls = m_player_kart->m_controls;
    /*AbstractKartAnimation *m_kart_animation;
    irr::scene::IDummyTransformationSceneNode    *m_wheel_box;*/

    // Kart attributes
    /*int m_network_finish_check_ticks;
    int m_network_confirmed_finish_ticks;*/
    kartGraphicalYOffset = m_player_kart->m_graphical_y_offset;
    kartXyzFront = m_player_kart->m_xyz_front;
    // const float XYZ_HISTORY_TIME = 0.25f;
    kartXyzHistorySize = m_player_kart->m_xyz_history_size;
    kartPreviousXyz = m_player_kart->m_previous_xyz;
    kartPreviousXyzTimes = m_player_kart->m_previous_xyz_times;
    kartTimePreviousCounter = m_player_kart->m_time_previous_counter;
    kartJumping = m_player_kart->m_is_jumping;
    // bool m_enabled_network_spectator;
    kartBubblegumTorqueSign = m_player_kart->m_bubblegum_torque_sign;
    kartBounceBackTicks = m_player_kart->m_bounce_back_ticks;
    kartMaxSpeed = *m_player_kart->m_max_speed;
    kartTerrainInfo = *m_player_kart->m_terrain_info;
    // Powerup *m_powerup;
        kartPowerUpType = m_player_kart->m_powerup->getType();
        kartPowerUpCount = m_player_kart->m_powerup->getNum();
   *kartVehicleRaycaster = *m_player_kart->m_vehicle_raycaster;
    // std::unique_ptr<btKart> m_vehicle;
        kartVehicleForwardWS = m_player_kart->m_vehicle->m_forwardWS;
        kartVehicleAxle = m_player_kart->m_vehicle->m_axle;
        kartVehicleForwardImpulse = m_player_kart->m_vehicle->m_forwardImpulse;
        kartVehicleSideImpulse = m_player_kart->m_vehicle->m_sideImpulse;
        kartVehicleUserConstraintType = m_player_kart->m_vehicle->m_userConstraintType;
        kartVehicleUserConstraintId = m_player_kart->m_vehicle->m_userConstraintId;
        kartVehicleDamping = m_player_kart->m_vehicle->m_damping;
       *kartVehicleVehicleRaycaster = *m_player_kart->m_vehicle->m_vehicleRaycaster;
        kartVehicleAllowSliding = m_player_kart->m_vehicle->m_allow_sliding;
        kartVehicleAdditionalImpulse = m_player_kart->m_vehicle->m_additional_impulse;
        kartVehicleTicks_additionalImpulse = m_player_kart->m_vehicle->m_ticks_additional_impulse;
        kartVehicleAdditionalRotation = m_player_kart->m_vehicle->m_additional_rotation;
        kartVehicleTicks_additionalRotation = m_player_kart->m_vehicle->m_ticks_additional_rotation;
        kartVehicleChassisBody = *m_player_kart->m_vehicle->m_chassisBody;
        kartVehicleNum_wheels_on_ground = m_player_kart->m_vehicle->m_num_wheels_on_ground;
        kartVehicleIndexRightAxis = m_player_kart->m_vehicle->m_indexRightAxis;
        kartVehicleIndexUpAxis = m_player_kart->m_vehicle->m_indexUpAxis;
        kartVehicleIndexForwardAxis = m_player_kart->m_vehicle->m_indexForwardAxis;
        kartVehicleMin_speed = m_player_kart->m_vehicle->m_min_speed;
        kartVehicleMax_speed = m_player_kart->m_vehicle->m_max_speed;
        kartVehicleVisual_wheels_touch_ground = m_player_kart->m_vehicle->m_visual_wheels_touch_ground;
        kartVehicleWheelInfo = m_player_kart->m_vehicle->m_wheelInfo;
    kartSkidding = *m_player_kart->getSkidding();
    /*std::unique_ptr<Stars> m_stars_effect;
    std::unique_ptr<Shadow> m_shadow;
    std::unique_ptr<SkidMarks> m_skidmarks;
    std::unique_ptr<KartGFX> m_kart_gfx;
    std::unique_ptr<SlipStream> m_slipstream;*/
    kartChassis = *m_player_kart->m_kart_chassis;
    /*ParticleEmitter *m_collision_particles;
    Controller  *m_controller;
    Controller  *m_saved_controller;
    PowerupManager::PowerupType m_last_used_powerup;*/
    kartFlying = m_player_kart->m_flying;
    /*bool m_has_caught_nolok_bubblegum;
    bool m_race_result;
    bool m_eliminated;
    int m_initial_position;
    int m_race_position;
    float        m_max_gear_rpm;*/
    kartBrakeTicks = m_player_kart->m_brake_ticks;
    /*int16_t      m_invulnerable_ticks;
    int16_t      m_bubblegum_ticks;
    int16_t       m_view_blocked_by_plunger;*/
    kartCurrentLean = m_player_kart->m_current_lean;
    kartMinNitroTick = m_player_kart->m_min_nitro_ticks;
    /*bool         m_fire_clicked;
    bool         m_boosted_ai;
    bool            m_finished_race;
    float           m_finish_time;*/
    kartCollectedEnergy = m_player_kart->m_collected_energy;
    // float         m_consumption_per_tick;
    kartEnergyToMinRatio = m_player_kart->m_energy_to_min_ratio;
    kartStartupBoost = m_player_kart->m_startup_boost;
    kartFallingTime = m_player_kart->m_falling_time;
    // float           m_weight;
    kartSpeed = m_player_kart->m_speed;
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
    kartTicksLastCrash = m_player_kart->m_ticks_last_crash;
    //RaceManager::KartType m_type;

    m_valid = true;
}

void SaveState::restore(LinearWorld *linear_world, AbstractKart *player_kart)
{
    if (!linear_world || !player_kart)
    {
        Log::error("Save States", "Invalid world or kart!");
        return;
    }
    if (linear_world != m_linear_world || dynamic_cast<Kart*>(player_kart) != m_player_kart)
    {
        Log::error("Save States", "Wrong world or kart!");
        return;
    }

    m_time_ticks = linear_world->m_time_ticks;
    linear_world->m_time = m_time;
    World::getWorld()->setTime(m_time);

    // SmoothNetworkBody attributes
    m_player_kart->m_start_smoothing_postion = kartStartSmoothingPostion;
    m_player_kart->m_adjust_position = kartAdjustPosition;
    m_player_kart->m_adjust_control_point = kartAdjustControlPoint;
    m_player_kart->m_prev_position_data = kartPrevPositionData;
    m_player_kart->m_smoothed_transform = kartSmoothedTransform;
    m_player_kart->m_adjust_time = kartAdjustTime;
    m_player_kart->m_adjust_time_dt = kartAdjustTimeDt;
    m_player_kart->m_smoothing = kartSmoothing;
    m_player_kart->m_enabled = kartEnabled;
    m_player_kart->m_smooth_rotation = kartSmoothRotation;
    m_player_kart->m_adjust_vertical_offset = kartAdjustVerticalOffset;
    m_player_kart->m_min_adjust_length = kartMinAdjustLength;
    m_player_kart->m_max_adjust_length = kartMaxAdjustLength;
    m_player_kart->m_min_adjust_speed = kartMinAdjustSpeed;
    m_player_kart->m_max_adjust_time = kartMaxAdjustTime;
    m_player_kart->m_adjust_length_threshold = kartAdjustLengthThreshold;

    // Moveable attributes
    m_player_kart->m_velocityLC = kartVelocityLC;
    m_player_kart->m_heading = kartHeading;
    m_player_kart->m_pitch = kartPitch;
    m_player_kart->m_roll = kartRoll;
    m_player_kart->m_transform = kartTransform;
    // UserPointer kartUserPointer;
    // irr::scene::ISceneNode *kartNode;
    // std::unique_ptr<btRigidBody> m_body;
        // btCollisionObject
        m_player_kart->m_body->m_worldTransform = kartBodyWorldTransform;
        m_player_kart->m_body->m_interpolationWorldTransform = kartBodyInterpolationWorldTransform;
        m_player_kart->m_body->m_interpolationLinearVelocity = kartBodyInterpolationLinearVelocity;
        m_player_kart->m_body->m_interpolationAngularVelocity = kartBodyInterpolationAngularVelocity;
        m_player_kart->m_body->m_anisotropicFriction = kartBodyAnisotropicFriction;
        m_player_kart->m_body->m_hasAnisotropicFriction = kartBodyHasAnisotropicFriction;
        m_player_kart->m_body->m_contactProcessingThreshold = kartBodyContactProcessingThreshold;
        /*btBroadphaseProxy* kartBodyBroadphaseHandle;
        btCollisionShape* kartBodyCollisionShape;
        void* kartBodyExtensionPointer;
        btCollisionShape* kartBodyRootCollisionShape;*/
        m_player_kart->m_body->m_collisionFlags = kartBodyCollisionFlags;
        m_player_kart->m_body->m_islandTag1 = kartBodyIslandTag1;
        m_player_kart->m_body->m_companionId = kartBodyCompanionId;
        m_player_kart->m_body->m_activationState1 = kartBodyActivationState1;
        m_player_kart->m_body->m_deactivationTime = kartBodyDeactivationTime;
        m_player_kart->m_body->m_friction = kartBodyFriction;
        m_player_kart->m_body->m_restitution = kartBodyRestitution;
        m_player_kart->m_body->m_internalType = kartBodyInternalType;
        // void* kartBodyUserObjectPointer;
        m_player_kart->m_body->m_hitFraction = kartBodyHitFraction;
        m_player_kart->m_body->m_ccdSweptSphereRadius = kartBodyCcdSweptSphereRadius;
        m_player_kart->m_body->m_ccdMotionThreshold = kartBodyCcdMotionThreshold;
        m_player_kart->m_body->m_checkCollideWith = kartBodyCheckCollideWith;
        // btRigidBody
        m_player_kart->m_body->m_invInertiaTensorWorld = kartBodyInvInertiaTensorWorld;
        m_player_kart->m_body->m_linearVelocity = kartBodyLinearVelocity;
        m_player_kart->m_body->m_angularVelocity = kartBodyAngularVelocity;
        m_player_kart->m_body->m_inverseMass = kartBodyInverseMass;
        m_player_kart->m_body->m_linearFactor = kartBodyinearFactor;
        m_player_kart->m_body->m_gravity = kartBodyGravity;
        m_player_kart->m_body->m_gravity_acceleration = kartBodyGravity_acceleration;
        m_player_kart->m_body->m_invInertiaLocal = kartBodyInvInertiaLocal;
        m_player_kart->m_body->m_totalForce = kartBodyTotalForce;
        m_player_kart->m_body->m_totalTorque = kartBodyTotalTorque;
        m_player_kart->m_body->m_linearDamping = kartBodyLinearDamping;
        m_player_kart->m_body->m_angularDamping = kartBodyAngularDamping;
        m_player_kart->m_body->m_additionalDamping = kartBodyAdditionalDamping;
        m_player_kart->m_body->m_additionalDampingFactor = kartBodyAdditionalDampingFactor;
        m_player_kart->m_body->m_additionalLinearDampingThresholdSqr = kartBodyAdditionalLinearDampingThresholdSqr;
        m_player_kart->m_body->m_additionalAngularDampingThresholdSqr = kartBodyAdditionalAngularDampingThresholdSqr;
        m_player_kart->m_body->m_additionalAngularDampingFactor = kartBodyAdditionalAngularDampingFactor;
        m_player_kart->m_body->m_linearSleepingThreshold = kartBodyLinearSleepingThreshold;
        m_player_kart->m_body->m_angularSleepingThreshold = kartBodyAngularSleepingThreshold;
       *m_player_kart->m_body->m_optionalMotionState = *kartBodyOptionalMotionState;
        //btAlignedObjectArray<btTypedConstraint*> m_constraintRefs;
        m_player_kart->m_body->m_rigidbodyFlags =kartBodyRigidbodyFlags;
        m_player_kart->m_body->m_debugBodyId = kartBodyDebugBodyId;
        m_player_kart->m_body->m_deltaLinearVelocity = kartBodyDeltaLinearVelocity;
        m_player_kart->m_body->m_deltaAngularVelocity = kartBodyDeltaAngularVelocity;
        m_player_kart->m_body->m_angularFactor = kartBodyAngularFactor;
        m_player_kart->m_body->m_invMass = kartBodyInvMass;
        m_player_kart->m_body->m_pushVelocity = kartBodyPushVelocity;
        m_player_kart->m_body->m_turnVelocity = kartBodyTurnVelocity;
    *m_player_kart->m_motion_state = kartMotionState;
        m_player_kart->m_motion_state->m_center_of_mass = kartMotionStateCenterOfMass;

    // AbstractKart attributes
    /*float m_kart_length;
    float m_kart_width;
    float m_kart_height;
    float m_kart_highest_point;
    const Vec3* m_wheel_graphics_position;
    unsigned int m_world_kart_id;*/
    m_player_kart->m_starting_transform = kartStartingTransform;
    /*int m_live_join_util;
    std::unique_ptr<KartProperties> m_kart_properties;
    PerPlayerDifficulty m_difficulty;
    std::unique_ptr<KartModel> m_kart_model;
    std::unique_ptr<Attachment> m_attachment;*/
    m_player_kart->m_controls = kartControls;
    /*AbstractKartAnimation *m_kart_animation;
    irr::scene::IDummyTransformationSceneNode    *m_wheel_box;*/

    // Kart attributes
    /*int m_network_finish_check_ticks;
    int m_network_confirmed_finish_ticks;*/
    m_player_kart->m_graphical_y_offset = kartGraphicalYOffset;
    m_player_kart->m_xyz_front = kartXyzFront;
    // const float XYZ_HISTORY_TIME = 0.25f;
    m_player_kart->m_xyz_history_size = kartXyzHistorySize;
    m_player_kart->m_previous_xyz = kartPreviousXyz;
    m_player_kart->m_previous_xyz_times = kartPreviousXyzTimes;
    m_player_kart->m_time_previous_counter = kartTimePreviousCounter;
    m_player_kart->m_is_jumping = kartJumping;
    // bool m_enabled_network_spectator;
    m_player_kart->m_bubblegum_torque_sign = kartBubblegumTorqueSign;
    m_player_kart->m_bounce_back_ticks = kartBounceBackTicks;
   *m_player_kart->m_max_speed = kartMaxSpeed;
   *m_player_kart->m_terrain_info = kartTerrainInfo;
    // Powerup *m_powerup;
        m_player_kart->m_powerup->set(PowerupManager::POWERUP_NOTHING, 0);
        m_player_kart->m_powerup->set(kartPowerUpType, kartPowerUpCount);
   *m_player_kart->m_vehicle_raycaster = *kartVehicleRaycaster;
    // std::unique_ptr<btKart> m_vehicle;
        m_player_kart->m_vehicle->m_forwardWS = kartVehicleForwardWS;
        m_player_kart->m_vehicle->m_axle = kartVehicleAxle;
        m_player_kart->m_vehicle->m_forwardImpulse = kartVehicleForwardImpulse;
        m_player_kart->m_vehicle->m_sideImpulse = kartVehicleSideImpulse;
        m_player_kart->m_vehicle->m_userConstraintType = kartVehicleUserConstraintType;
        m_player_kart->m_vehicle->m_userConstraintId = kartVehicleUserConstraintId;
        m_player_kart->m_vehicle->m_damping = kartVehicleDamping;
       *m_player_kart->m_vehicle->m_vehicleRaycaster = *kartVehicleVehicleRaycaster;
        m_player_kart->m_vehicle->m_allow_sliding = kartVehicleAllowSliding;
        m_player_kart->m_vehicle->m_additional_impulse = kartVehicleAdditionalImpulse;
        m_player_kart->m_vehicle->m_ticks_additional_impulse = kartVehicleTicks_additionalImpulse;
        m_player_kart->m_vehicle->m_additional_rotation = kartVehicleAdditionalRotation;
        m_player_kart->m_vehicle->m_ticks_additional_rotation = kartVehicleTicks_additionalRotation;
       *m_player_kart->m_vehicle->m_chassisBody = kartVehicleChassisBody;
        m_player_kart->m_vehicle->m_num_wheels_on_ground = kartVehicleNum_wheels_on_ground;
        m_player_kart->m_vehicle->m_indexRightAxis = kartVehicleIndexRightAxis;
        m_player_kart->m_vehicle->m_indexUpAxis = kartVehicleIndexUpAxis;
        m_player_kart->m_vehicle->m_indexForwardAxis = kartVehicleIndexForwardAxis;
        m_player_kart->m_vehicle->m_min_speed = kartVehicleMin_speed;
        m_player_kart->m_vehicle->m_max_speed = kartVehicleMax_speed;
        m_player_kart->m_vehicle->m_visual_wheels_touch_ground = kartVehicleVisual_wheels_touch_ground;
        m_player_kart->m_vehicle->m_wheelInfo = kartVehicleWheelInfo;
   *m_player_kart->m_skidding = kartSkidding;
    /*std::unique_ptr<Stars> m_stars_effect;
    std::unique_ptr<Shadow> m_shadow;
    std::unique_ptr<SkidMarks> m_skidmarks;
    std::unique_ptr<KartGFX> m_kart_gfx;
    std::unique_ptr<SlipStream> m_slipstream;*/
   *m_player_kart->m_kart_chassis = kartChassis;
    /*ParticleEmitter *m_collision_particles;
    Controller  *m_controller;
    Controller  *m_saved_controller;
    PowerupManager::PowerupType m_last_used_powerup;*/
    m_player_kart->m_flying = kartFlying;
    /*bool m_has_caught_nolok_bubblegum;
    bool m_race_result;
    bool m_eliminated;
    int m_initial_position;
    int m_race_position;
    float        m_max_gear_rpm;*/
    m_player_kart->m_brake_ticks = kartBrakeTicks;
    /*int16_t      m_invulnerable_ticks;
    int16_t      m_bubblegum_ticks;
    int16_t       m_view_blocked_by_plunger;*/
    m_player_kart->m_current_lean = kartCurrentLean;
    m_player_kart->m_min_nitro_ticks = kartMinNitroTick;
    /*bool         m_fire_clicked;
    bool         m_boosted_ai;
    bool            m_finished_race;
    float           m_finish_time;*/
    m_player_kart->m_collected_energy = kartCollectedEnergy;
    // float         m_consumption_per_tick;
    m_player_kart->m_energy_to_min_ratio = kartEnergyToMinRatio;
    m_player_kart->m_startup_boost = kartStartupBoost;
    m_player_kart->m_falling_time = kartFallingTime;
    // float           m_weight;
    m_player_kart->m_speed = kartSpeed;
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
    m_player_kart->m_ticks_last_crash = kartTicksLastCrash;
    //RaceManager::KartType m_type;
}
