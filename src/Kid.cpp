#include "GentooEngine.h"
#include "Kid.h"

#define SPRITE_IDLE                             "assets/img/kid/idle.png"
// #define SPRITE_WALK                             "assets/img/kid/walk.png"
#define SPRITE_RUN                              "assets/img/kid/run.png"
// #define SPRITE_BREAK                            "assets/img/kid/break.png"
#define SPRITE_JUMP                             "assets/img/kid/jump.png"
#define SPRITE_FALL                             "assets/img/kid/fall.png"
// #define SPRITE_LAND                             "assets/img/kid/land.png"
// #define SPRITE_DAMAGE                           "assets/img/kid/land.png"
#define SPRITE_ATTACK_MELEEGROUND0              "assets/img/kid/attackmeleeground1.png"
// #define SPRITE_ATTACK_MELEEGROUND0END           "assets/img/kid/attackmeleeground1end.png"
// #define SPRITE_ATTACK_MELEEGROUND1              "assets/img/kid/attackmeleeground2.png"
// #define SPRITE_ATTACK_MELEEGROUND1END           "assets/img/kid/attackmeleeground2end.png"

#define SPRITE_IDLE_FRAMES                      6, 0.1f
// #define SPRITE_WALK_FRAMES                      1, 0.1f
#define SPRITE_RUN_FRAMES                       8, 0.075f
// #define SPRITE_BRAKE_FRAMES                     3, 0.1f
#define SPRITE_JUMP_FRAMES                      5, 0.1f
#define SPRITE_FALL_FRAMES                      3, 0.1f
// #define SPRITE_LAND_FRAMES                      3, 0.1f
// #define SPRITE_DAMAGE_FRAMES                    1, 0.1f
#define SPRITE_ATTACK_MELEEGROUND0_FRAMES       6, 0.1f
// #define SPRITE_ATTACK_MELEEGROUND0END_FRAMES    1, 0.1f
// #define SPRITE_ATTACK_MELEEGROUND1_FRAMES       1, 0.1f
// #define SPRITE_ATTACK_MELEEGROUND1END_FRAMES    1, 0.1f

#define ACCELERATION_RUN                        800.0f
#define SPEED_RUN                               120.0f
#define SPEED_ONAIR                             100.0f
#define SPEED_ATTACK                            300.0f
#define FORCE_JUMP                              300.0f
#define FORCE_MASS                              460.0f
#define IMPULSE_ATTACK_SWORD_X                  40.0f

#define ATTACK_SWORD_FORCE                      400.0f, 0.0f
#define ATTACK_SWORD_IMPULSE                    70.0f
#define ATTACK_SWORD_DAMAGE                     1

#define COLLIDER_POSITION_ONGROUND              0.0f, 9.0f
#define COLLIDER_POSITION_ONAIR                 0.0f, 5.0f
#define COLLIDER_BOX_SIZE                       16.0f, 29.0f

#define CAMERABOX_RECT                          16.0f, 19.0f, 16.0f, 28.0f
#define CAMERABOX_SPACING                       0.0f, 12.0f
#define CINEMACHINE_LENGTH                      14.0f, 86.0f
#define CINEMACHINE_SLICES                      8, 32, 2, 24
#define CINEMACHINE_OFFSET                      0.0f, -12.0f
#define CINEMACHINE_SETUP                       true, true, true, false, true, true, false, false

#define CAMERA_GROUNDED_RESET_TIME              1.5f

// float coisaLinda = 1000.0f; // remover

Kid::Kid (GameObject& associated): EntityMachine(associated) {
    type = type | ComponentType::_Kid;
    associated.layer = LayerDistance::_Player;
    associated.label = "Player";
    jumpTimer.SetResetTime(0.8f);
    invincibilityTimer.SetResetTime(1.25f);
    isInvincible = false;
    hp = 4;

    runSpeedIncrease = 0.0f;
    runSpeedReset = true;
    jumpSpeedDecrease = 0.0f;
    attackPerforming = false;
    attackImpulseCancel = false;
    lastDirectionX = 1;

    GameObject* attack = new GameObject(LayerDistance::_Player_Front);
    swordAttackOnGround = new KidAttackMelee(*attack, &associated);
    swordAttackOnGround->SetProperties(
        Vec2(ATTACK_SWORD_FORCE), ATTACK_SWORD_IMPULSE, ATTACK_SWORD_DAMAGE, IMPULSE_ATTACK_SWORD_X
    ); attack->AddComponent(swordAttackOnGround);
    Game::GetInstance().GetCurrentState().AddObject(attack);

    isGrounded = false;
    hitCeiling = false;
    hitWall = false;

    associated.pixelColliderFix1 = true;
}

void Kid::Awaken () {
    Sprite* spriteIdle = new Sprite(associated, SPRITE_IDLE, SPRITE_IDLE_FRAMES);
    Sprite* spriteRun = new Sprite(associated, SPRITE_RUN, SPRITE_RUN_FRAMES);
    Sprite* spriteJump = new Sprite(associated, SPRITE_JUMP, SPRITE_JUMP_FRAMES, SPRITE_ONESHOT_TRUE);
    Sprite* spriteFall = new Sprite(associated, SPRITE_FALL, SPRITE_FALL_FRAMES, SPRITE_ONESHOT_TRUE);
    Sprite* spriteDamage = new Sprite(associated, SPRITE_IDLE, SPRITE_IDLE_FRAMES);
    Sprite* spriteAttackSwordOnGround = new Sprite(
        associated, SPRITE_ATTACK_MELEEGROUND0, SPRITE_ATTACK_MELEEGROUND0_FRAMES, SPRITE_ONESHOT_TRUE);

    AddSpriteState(EntityState::Idle, spriteIdle);
    AddSpriteState(EntityState::Running, spriteRun);
    AddSpriteState(EntityState::Jumping, spriteJump);
    AddSpriteState(EntityState::Falling, spriteFall);
    AddSpriteState(EntityState::Injured, spriteDamage);
    AddSpriteState(EntityState::AttackingSwordOnGround_0, spriteAttackSwordOnGround);

    rigidBody = new RigidBody(associated);
    associated.AddComponent(rigidBody);

    collider = new Collider(associated);
    collider->SetBox(Vec2(COLLIDER_POSITION_ONGROUND), Vec2(COLLIDER_BOX_SIZE));
    associated.AddComponent(collider);

    cameraBox = new GameObject(associated.layer);
    CameraBox* cameraBoxComp = new CameraBox(*cameraBox, &associated, CAMERABOX_SPACING);
    cameraBoxComp->focusBoxOffset = Rect(CAMERABOX_RECT);
    cameraBox->AddComponent(cameraBoxComp);
    cameraBoxComp->AddMethod(this, std::bind(&CameraEffects, this));
    Game::GetInstance().GetCurrentState().AddObject(cameraBox);
};

void Kid::Start () {
    state = EntityState::Idle;

    Camera::Follow(
        cameraBox, Vec2(CINEMACHINE_LENGTH), CINEMACHINE_SLICES,
        Camera::RIGHT, Camera::UP, Vec2(CINEMACHINE_OFFSET));
    Camera::cinemachine.Setup(CINEMACHINE_SETUP);
    Camera::offset.y = 0;

    cameraGroundedTimer.SetResetTime(CAMERA_GROUNDED_RESET_TIME);
};

// keep it empty so LateUpdateEntity is not called
void Kid::LateUpdate (float dt) {};

void Kid::UpdateEntity (float dt) {
    InputManager& input = InputManager::GetInstance();

    if (isInvincible) {
        invincibilityTimer.Update(dt);
        if (invincibilityTimer.IsOver()) {
            invincibilityTimer.Reset();
            isInvincible = false;
            rigidBody->triggerLabels.clear();
        }
    }
    // prevent movement when opposite directions are active
    int directionX = input.IsKeyDown(Key::moveRight) - input.IsKeyDown(Key::moveLeft);
    bool changeDirectionX = (not attackPerforming) and (directionX != 0) and (directionX != lastDirectionX);
    if (changeDirectionX) lastDirectionX = directionX;
    
    // tolerance before reset run speed
    runSpeedReset = (directionX == 0)? true : false;
    if (runSpeedReset) runSpeedIncrease = 0.0f;

    // tolerance before start falling
    if ((state != EntityState::Falling) and (rigidBody->GetSpeed().y > 100)) {
        FormatState(EntityState::Falling);
        isGrounded = false;
    }
    // // remover
    // if (rigidBody->GetSpeed().x != coisaLinda) {
    //     coisaLinda = rigidBody->GetSpeed().x;
    //     SDL_Log("%f", coisaLinda);
    // }

    switch (state) {
        case EntityState::Idle:
            // prevent movement when opposite directions are active
            if (directionX != 0)
                state = EntityState::Running;

            // start melee attack on ground
            if (input.KeyPress(Key::attack)) {
                FormatState(EntityState::AttackingSwordOnGround_0);
                break;
            }
            // start jump
            else if (input.KeyPress(Key::jump))
                FormatState(EntityState::Jumping);

            break;
        
        // case EntityState::Walking:
        //     break;
        
        case EntityState::Running:
            // movement is performed
            if (directionX == 0)
                FormatState(EntityState::Idle);

            // movement is performing
            else {
                runSpeedIncrease = (runSpeedIncrease < SPEED_RUN)?
                    runSpeedIncrease + (ACCELERATION_RUN * dt) : SPEED_RUN;
                rigidBody->SetSpeedOnX(directionX * runSpeedIncrease);
            }
            // start melee attack on ground
            if (input.KeyPress(Key::attack)) {
                FormatState(EntityState::AttackingSwordOnGround_0);
                break;
            }
            // start jump
            else if (input.KeyPress(Key::jump))
                FormatState(EntityState::Jumping);
            
            break;
        
        case EntityState::Jumping:
            // jump is performing
            rigidBody->SetSpeedOnY(-FORCE_JUMP + jumpSpeedDecrease);
            jumpSpeedDecrease += FORCE_MASS * dt;
            if (directionX != 0)
                rigidBody->SetSpeedOnX(directionX * SPEED_ONAIR);
            jumpTimer.Update(dt);

            // jump is performed
            if (input.KeyRelease(Key::jump) or jumpTimer.IsOver() or hitCeiling) {
                FormatState(EntityState::Falling);
                rigidBody->SetSpeedOnY(0.0f);
                rigidBody->ResetGravity();
                jumpTimer.Reset();
                hitCeiling = false;
            }
            break;
        
        case EntityState::Falling:
            // movement on air
            rigidBody->SetSpeedOnX(SPEED_ONAIR * directionX);
            break;
        
        case EntityState::AttackingSwordOnGround_0:
            if ((not attackImpulseCancel) and
            (fabs(associated.box.x - swordAttackOnGround->originPositionX) >= IMPULSE_ATTACK_SWORD_X)) {
                rigidBody->SetSpeedOnX(0.0f);
                attackImpulseCancel = true;
            }
            if (sprites[state]->OneshotIsOver())
                FormatState(EntityState::Idle);

            break;
        
        case EntityState::Injured:
            if (collider->box.GetPosition().DistanceTo(damageOrigin) > damageImpulse) {
                rigidBody->SetSpeed(Vec2(lastDirectionX * damageForce.x * 0.5f, damageForce.y));
                FormatState(EntityState::Falling);
            }
            if (hitWall) {
                FormatState(EntityState::Falling);
                hitWall = false;
            }
            break;

        default: break;
    }

    if (changeDirectionX)
        textureFlip = (directionX == 1)? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
}

bool Kid::NewStateRule (EntityState newState, int argsc, float argsv[]) {
    if (newState == state)
        return false;
    
    switch (state) {
        case EntityState::AttackingSwordOnGround_0:
            attackImpulseCancel = false;
            attackPerforming = false;
            break;

        default: break;
    }

    switch (newState) {
        case EntityState::Idle:
            collider->SetBox(Vec2(COLLIDER_POSITION_ONGROUND), Vec2(COLLIDER_BOX_SIZE));
            rigidBody->SetSpeedOnX(0.0f);
            return true;

        case EntityState::Jumping:
            collider->SetBox(Vec2(COLLIDER_POSITION_ONAIR), Vec2(COLLIDER_BOX_SIZE));
            rigidBody->SetGravity(0.0f);
            jumpSpeedDecrease = 0.0f;
            isGrounded = false;
            return true;
        
        case EntityState::Falling:
            collider->SetBox(Vec2(COLLIDER_POSITION_ONAIR), Vec2(COLLIDER_BOX_SIZE));
            return true;

        case EntityState::AttackingSwordOnGround_0:
            swordAttackOnGround->direction = (lastDirectionX == 1)?
                KidAttackMelee::RIGHT : KidAttackMelee::LEFT;
            swordAttackOnGround->Perform(associated.box.x);
            
            if (runSpeedReset)
                attackImpulseCancel = true;
            if (not attackImpulseCancel)
                rigidBody->SetSpeedOnX(SPEED_ATTACK * lastDirectionX);
            else rigidBody->SetSpeedOnX(0.0f);
            
            attackPerforming = true;
            return true;

        case EntityState::Injured:
            if (isInvincible) return false;
            damageOrigin = collider->box.GetPosition();
            damageForce = Vec2(-argsv[Attack::_ForceX], -argsv[Attack::_ForceY]);
            damageImpulse = argsv[Attack::_Impulse];
            rigidBody->SetSpeed(Vec2(lastDirectionX * damageForce.x, damageForce.y));
            rigidBody->ResetGravity();
            isInvincible = true;
            rigidBody->triggerLabels.push_back("Enemy");
            hp -= argsv[Attack::_Damage];
            return true;

        default: return true;
    }
}

void Kid::NotifyCollision (GameObject& other) {
    rigidBody->NotifyCollision(other);

    switch (state) {
        case EntityState::Falling:
            // hits the ground
            if (rigidBody->ImpactDown()) {
                FormatState(EntityState::Idle);
                isGrounded = true;
            } break;

        case EntityState::Jumping:
            // hits the ceiling
            if (rigidBody->ImpactUp())
                hitCeiling = true;
            break;

        case EntityState::AttackingSwordOnGround_0:
            if (rigidBody->ImpactLeft() or rigidBody->ImpactRight()) {
                rigidBody->SetSpeedOnX(0.0f);
                attackImpulseCancel = true;
            }
            break;

        case EntityState::Injured:
            // hits a wall
            if (rigidBody->ImpactLeft() or rigidBody->ImpactRight())
                hitWall = true;
            break;

        default: break;
    }
}

bool Kid::Is (ComponentType type) {
    return type & this->type;
}

void* Kid::CameraEffects () {
    /*--------------------------------------------------------------------------------------------------*/
    // returns the camera to original focus offset when kid is grounded
    /*--------------------------------------------------------------------------------------------------*/

    if (not isGrounded) {
        cameraGroundedTimer.Reset();
        return nullptr;
    }
    float dt = Game::GetInstance().GetDeltaTime();
    if (not cameraGroundedTimer.IsOver()) {
        cameraGroundedTimer.Update(dt);
        return nullptr;
    }

    float cameraBoxY = collider->box.y - (cameraBox->box.h - collider->box.h);
    if (cameraBox->box.y > cameraBoxY) {
        cameraBox->box.y -= (cameraBox->box.y - cameraBoxY) * dt;
        if (cameraBox->box.y < cameraBoxY) {
            cameraBox->box.y = cameraBoxY;
            cameraGroundedTimer.Reset();
        }
    } return nullptr;
}