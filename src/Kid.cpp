#include "GentooEngine.h"
#include "Kid.h"

#define SPRITE_IDLE                 "assets/img/kid/idle.png"
#define SPRITE_WALK                 "assets/img/kid/walk.png"
#define SPRITE_RUN                  "assets/img/kid/run.png"
#define SPRITE_JUMP                 "assets/img/kid/jump.png"
#define SPRITE_FALL                 "assets/img/kid/fall.png"
#define SPRITE_ATTACK               "assets/img/kid/attack.png"

#define SPRITE_IDLE_FRAMES          6, 0.05f
// #define SPRITE_WALK_FRAMES          8, 0.05f
#define SPRITE_RUN_FRAMES           8, 0.05f
#define SPRITE_BRAKE_FRAMES         3, 0.05f
#define SPRITE_JUMP_FRAMES          5, 0.05f
#define SPRITE_FALL_FRAMES          3, 0.05f
#define SPRITE_LAND_FRAMES          3, 0.05f
#define SPRITE_ATTACK_FRAMES        1, 0.05f
#define SPRITE_DAMAGE_FRAMES        1, 0.05f

#define ACCELERATION_RUN            800.0f
#define SPEED_RUN                   140.0f
#define SPEED_ONAIR                 120.0f
#define FORCE_JUMP                  300.0f
#define FORCE_MASS                  460.0f
#define FORCE_DAMAGE_X              -400.0f
#define FORCE_DAMAGE_Y              -140.0f
#define IMPULSE_DAMAGE              70.0f

#define COLLIDER_POSITION           0.0f, 9.0f
#define COLLIDER_BOX_SIZE           16.0f, 29.0f

#define CAMERABOX_RECT              16.0f, 19.0f, 16.0f, 28.0f
#define CAMERABOX_SPACING           0.0f, 12.0f
#define CINEMACHINE_LENGTH          14.0f, 86.0f
#define CINEMACHINE_SLICES          8, 32, 2, 24
#define CINEMACHINE_OFFSET          0.0f, -12.0f
#define CINEMACHINE_SETUP           true, true, true, false, true, true, false, false

#define CAMERA_SHAKE_COUNT          6
#define CAMERA_SHAKE_RANGE          3
#define CAMERA_SHAKE_RESET_TIME     0.03f
#define CAMERA_GROUNDED_RESET_TIME  1.5f

Kid::Kid (GameObject& associated): EntityMachine(associated) {
    type = type | ComponentType::_Kid;
    associated.layer = LayerDistance::_Player;
    associated.label = "Player";
    jumpTimer.SetResetTime(0.8f);
    invincibilityTimer.SetResetTime(1.25f);
    isInvincible = false;
    hp = 4;

    speedRunIncrease = 0.0f;
    speedJumpDecrease = 0.0f;
    lastDirectionX = 1;

    GameObject* attack = new GameObject(LayerDistance::_Player_Front);
    attackMelee = new KidAttackMelee(*attack, &associated);
    attack->AddComponent(attackMelee);
    Game::GetInstance().GetCurrentState().AddObject(attack);

    isGrounded = false;
    hitCeiling = false;
    hitWall = false;
}

void Kid::Awaken () {
    Sprite* spriteIdle = new Sprite(associated, SPRITE_IDLE, SPRITE_IDLE_FRAMES);
    // Sprite* spriteWalk = new Sprite(associated, SPRITE_WALK, 6, 0.15);
    Sprite* spriteRun = new Sprite(associated, SPRITE_RUN, SPRITE_RUN_FRAMES);
    Sprite* spriteJump = new Sprite(associated, SPRITE_JUMP, SPRITE_JUMP_FRAMES, SPRITE_ONESHOT_TRUE);
    Sprite* spriteFall = new Sprite(associated, SPRITE_FALL, SPRITE_FALL_FRAMES, SPRITE_ONESHOT_TRUE);
    // Sprite* spriteAttack = new Sprite(associated, SPRITE_ATTACK, 5, 0.12, true);
    Sprite* spriteDamage = new Sprite(associated, SPRITE_IDLE, SPRITE_IDLE_FRAMES);

    AddSpriteState(Idle, spriteIdle);
    // AddSpriteState(Walking, spriteWalk);
    AddSpriteState(Running, spriteRun);
    AddSpriteState(Jumping, spriteJump);
    AddSpriteState(Falling, spriteFall);
    // AddSpriteState(Attacking, spriteAttack);
    AddSpriteState(Injured, spriteDamage);

    rigidBody = new RigidBody(associated);
    associated.AddComponent(rigidBody);

    collider = new Collider(associated);
    collider->SetBox(Vec2(COLLIDER_POSITION), Vec2(COLLIDER_BOX_SIZE));
    associated.AddComponent(collider);

    cameraBox = new GameObject(associated.layer);
    CameraBox* cameraBoxComp = new CameraBox(*cameraBox, &associated, CAMERABOX_SPACING);
    cameraBoxComp->focusBoxOffset = Rect(CAMERABOX_RECT);
    cameraBox->AddComponent(cameraBoxComp);
    cameraBoxComp->AddMethod(this, std::bind(&CameraEffects, this));
    Game::GetInstance().GetCurrentState().AddObject(cameraBox);
};

void Kid::Start () {
    state = Idle;

    Camera::Follow(
        cameraBox, Vec2(CINEMACHINE_LENGTH), CINEMACHINE_SLICES,
        Camera::RIGHT, Camera::UP, Vec2(CINEMACHINE_OFFSET));
    Camera::cinemachine.Setup(CINEMACHINE_SETUP);
    Camera::offset.y = 0;

    cameraGroundedTimer.SetResetTime(CAMERA_GROUNDED_RESET_TIME);
    cameraShakeTimer.SetResetTime(CAMERA_SHAKE_RESET_TIME);
    cameraShakeTimer.FalseStart();
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
    bool directionXhasChanged = (directionX != 0) and (directionX != lastDirectionX);
    if (directionXhasChanged) lastDirectionX = directionX;

    // tolerance before start falling
    if ((state != Falling) and (rigidBody->GetSpeed().y > 100)) {
        isGrounded = false;
        state = Falling;
    }

    // remover
    if (input.KeyPress(Key::attack)) {
        attackMelee->direction = (lastDirectionX == 1)? KidAttackMelee::RIGHT : KidAttackMelee::LEFT;
        attackMelee->Enable();
    }

    switch (state) {
        case Idle:
            // prevent movement when opposite directions are active
            if (directionX != 0)
                state = Running;
            
            // start jump
            if (input.KeyPress(Key::jump)) {
                state = Jumping;
                rigidBody->SetGravity(0.0f);
                isGrounded = false;
            }
            break;
        
        // case Walking:
        //     break;
        
        case Running:
            // movement is performed
            if (directionX == 0) {
                state = Idle;
                rigidBody->SetSpeedOnX(0.0f);
                speedRunIncrease = 0.0f;
            }
            // movement is performing
            else {
                speedRunIncrease = (speedRunIncrease < SPEED_RUN)?
                    speedRunIncrease + (ACCELERATION_RUN * dt) : SPEED_RUN;
                rigidBody->SetSpeedOnX(directionX * speedRunIncrease);
            }
            // start jump
            if (input.KeyPress(Key::jump)) {
                state = Jumping;
                rigidBody->SetGravity(0.0f);
                isGrounded = false;
            }
            break;
        
        case Jumping:
            // jump is performing
            rigidBody->SetSpeedOnY(-FORCE_JUMP + speedJumpDecrease);
            speedJumpDecrease += FORCE_MASS * dt;
            if (directionX != 0)
                rigidBody->SetSpeedOnX(directionX * SPEED_ONAIR);
            jumpTimer.Update(dt);

            // jump is performed
            if (input.KeyRelease(Key::jump) or jumpTimer.IsOver() or hitCeiling) {
                state = Falling;
                rigidBody->SetSpeedOnY(0.0f);
                rigidBody->ResetGravity();
                jumpTimer.Reset();
                speedJumpDecrease = 0.0f;
                hitCeiling = false;
            }
            break;
        
        case Falling:
            // movement on air
            if (directionX != 0)
                rigidBody->SetSpeedOnX(directionX * SPEED_ONAIR);
            else rigidBody->SetSpeedOnX(0.0f);

            break;
        
        // case Attacking:
        //     break;
        
        case Injured:
            if (collider->box.GetPosition().DistanceTo(damageOrigin) > IMPULSE_DAMAGE) {
                rigidBody->SetSpeed(Vec2(lastDirectionX * FORCE_DAMAGE_X * 0.5f, FORCE_DAMAGE_Y));
                state = Falling;
            }
            if (hitWall) {
                state = Falling;
                hitWall = false;
            }
            break;

        default: break;
    }

    if (directionXhasChanged)
        textureFlip = (directionX == 1)? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
}

bool Kid::NewStateRule (EntityState newState) {
    if (newState == state)
        return false;

    switch (newState) {
        case Injured:
            if (isInvincible) return false;
            damageOrigin = collider->box.GetPosition();
            rigidBody->SetSpeed(Vec2(lastDirectionX * FORCE_DAMAGE_X, FORCE_DAMAGE_Y));
            rigidBody->ResetGravity();
            isInvincible = true;
            rigidBody->triggerLabels.push_back("Enemy");
            return true;

        default: return true;
    }
}

void Kid::NotifyCollision (GameObject& other) {
    rigidBody->NotifyCollision(other);

    switch (state) {
        case Falling:
            // hits the ground
            if (rigidBody->ImpactDown()) {
                state = Idle;
                rigidBody->SetSpeedOnX(0.0f);
                isGrounded = true;
            } break;

        case Jumping:
            // hits the ceiling
            if (rigidBody->ImpactUp())
                hitCeiling = true;
            break;
        
        case Injured:
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

void Kid::CameraStartShake () {
    cameraShakeReset = Vec2();
    int shakeRange = (CAMERA_SHAKE_RANGE << 1) + 1;
    for (int i=0; i < CAMERA_SHAKE_COUNT; i++) {
        Vec2 shake = Vec2(
            rand()%shakeRange-CAMERA_SHAKE_RANGE,
            rand()%shakeRange-CAMERA_SHAKE_RANGE
        ); cameraShakeQueue.push(shake);
    }
}

void* Kid::CameraEffects () {
    /*--------------------------------------------------------------------------------------------------*/
    // shakes the camera after a hit
    /*--------------------------------------------------------------------------------------------------*/

    if (not cameraShakeQueue.empty()) {
        if (cameraShakeTimer.IsOver()) {
            Camera::masterOffset += cameraShakeQueue.front() - cameraShakeReset;
            cameraShakeReset = cameraShakeQueue.front();
            cameraShakeQueue.pop();
            if (cameraShakeQueue.empty()) {
                Camera::masterOffset -= cameraShakeReset;
                cameraShakeTimer.FalseStart();
            }
        } else cameraShakeTimer.Update(Game::GetInstance().GetDeltaTime());
    }

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