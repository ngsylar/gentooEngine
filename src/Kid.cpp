#include "GentooEngine.h"
#include "Kid.h"

#define SPRITE_IDLE                             "assets/img/kid/idle.png"
// #define SPRITE_WALK                             "assets/img/kid/walk.png"
#define SPRITE_RUN                              "assets/img/kid/run.png"
// #define SPRITE_BREAK                            "assets/img/kid/break.png"
#define SPRITE_JUMP                             "assets/img/kid/jump.png"
#define SPRITE_FALL                             "assets/img/kid/fall.png"
#define SPRITE_LAND                             "assets/img/kid/land.png"
#define SPRITE_DAMAGE                           "assets/img/kid/damage.png"
#define SPRITE_ATTACK_SWORDGROUND0              "assets/img/kid/attackmeleeground1.png"
#define SPRITE_ATTACK_SWORDGROUND1              "assets/img/kid/attackmeleeground2.png"
#define SPRITE_ATTACK_SWORDSTRONGC              "assets/img/kid/attackstrongcharge.png"
#define SPRITE_ATTACK_SWORDSTRONGF              "assets/img/kid/attackstrongfull.png"
#define SPRITE_ATTACK_SWORDSTRONGP              "assets/img/kid/attackstrongperform.png"
#define SPRITE_SPELL0                           "assets/img/kid/magic.png"
#define SPRITE_DEATH                            "assets/img/kid/death.png"

#define SPRITE_IDLE_FRAMES                      10, 0.1f
// #define SPRITE_WALK_FRAMES                      1, 0.1f
#define SPRITE_RUN_FRAMES                       8, 0.075f
// #define SPRITE_BRAKE_FRAMES                     3, 0.1f
#define SPRITE_JUMP_FRAMES                      5, 0.1f
#define SPRITE_FALL_FRAMES                      3, 0.1f
#define SPRITE_LAND_FRAMES                      3, 0.1f
#define SPRITE_ATTACK_SWORDGROUND0_FRAMES       10, 0.1f
#define SPRITE_ATTACK_SWORDGROUND1_FRAMES       10, 0.1f
#define SPRITE_ATTACK_SWORDSTRONGC_FRAMES       19, 0.1f
#define SPRITE_ATTACK_SWORDSTRONGF_FRAMES       4, 0.1f
#define SPRITE_ATTACK_SWORDSTRONGP_FRAMES       10, 0.1f
#define SPRITE_SPELL0_FRAMES                    12, 0.1f
#define SPRITE_DEATH_FRAMES                     12, 0.1f

#define ACCELERATION_RUN                        800.0f
#define SPEED_RUN                               120.0f
#define SPEED_ONAIR                             100.0f
#define SPEED_ATTACK                            300.0f
#define FORCE_JUMP                              300.0f
#define FORCE_JUMP_MASS                         460.0f
#define FORCE_DAMAGE_MASS                       1748.0f
#define IMPULSE_ATTACK_SWORD_X                  40.0f

#define ATTACK_SWORD_FORCE                      400.0f, 0.0f
#define ATTACK_SWORD_IMPULSE                    50.0f
#define ATTACK_SWORD_DAMAGE                     1
#define ATTACK_SWORD_TIME_HIT                   0.3f
#define ATTACK_SWORD_TIME_CANCEL                0.6f
#define ATTACK_SWORD_TIME_END                   1.0f

#define INVINCIBLE_FLICK_SPEED                  1200.0f

#define COLLIDER_POSITION_ONGROUND              0.0f, 9.0f
#define COLLIDER_POSITION_ONAIR                 0.0f, 5.0f
#define COLLIDER_BOX_SIZE                       16.0f, 29.0f
#define COLLIDER_POSITION_Y_REPULSION           4.0f

#define CAMERABOX_RECT                          16.0f, 19.0f, 16.0f, 28.0f
#define CAMERABOX_SPACING                       0.0f, 12.0f
#define CINEMACHINE_LENGTH                      14.0f, 86.0f
#define CINEMACHINE_SLICES                      8, 32, 2, 24
#define CINEMACHINE_OFFSET                      0.0f, -12.0f
#define CINEMACHINE_SETUP                       true, true, true, false, true, true, false, false

#define CAMERA_GROUNDED_RESET_TIME              1.5f

GameObject* Kid::instance = nullptr;

Kid::Kid (GameObject& associated): EntityMachine(associated) {
    instance = &associated;
    type = type | ComponentType::_Kid;
    associated.layer = LayerDistance::_Player;
    associated.label = "Player";
    jumpTimer.SetResetTime(0.8f);
    spellTimer.SetResetTime(1.2f);
    damageTimer.SetResetTime(0.4f);
    invincibilityTimer.SetResetTime(1.25f);
    isInvincible = false;
    isDead = false;
    hp = 6;

    runSpeedIncrease = 0.0f;
    runSpeedReset = true;
    jumpSpeedDecrease = 0.0f;
    attackPerforming = false;
    chargePerforming = false;
    attackImpulseCancel = false;
    damagePerforming = false;
    flickFactor = 255.0f;
    lastDirectionX = 1;

    GameObject* attack = new GameObject(LayerDistance::_Player_Front);
    swordAttackOnGround = new KidAttackMelee(*attack, &associated);
    swordAttackOnGround->SetProperties(
        Vec2(ATTACK_SWORD_FORCE), ATTACK_SWORD_IMPULSE, ATTACK_SWORD_DAMAGE, IMPULSE_ATTACK_SWORD_X);
    attack->AddComponent(swordAttackOnGround);
    Game::GetInstance().GetCurrentState().AddObject(attack);

    attack = new GameObject(LayerDistance::_Player_Front);
    swordAttackStrong = new KidAttackStrong(*attack, &associated);
    attack->AddComponent(swordAttackStrong);
    Game::GetInstance().GetCurrentState().AddObject(attack);

    attack = new GameObject(LayerDistance::_Player_Front);
    foxAttack = new FoxAttack(*attack, &associated);
    attack->AddComponent(foxAttack);
    Game::GetInstance().GetCurrentState().AddObject(attack);

    isGrounded = false;
    hitCeiling = false;
    hitWall = false;

    // melius colliders' pixel correction
    associated.pixelColliderFix1 = true;
}

Kid::~Kid () {
    instance = nullptr;
}

GameObject* Kid::GetInstance () {
    return instance;
}

void Kid::Awaken () {
    Sprite* spriteIdle = new Sprite(associated, SPRITE_IDLE, SPRITE_IDLE_FRAMES);
    Sprite* spriteRun = new Sprite(associated, SPRITE_RUN, SPRITE_RUN_FRAMES);
    Sprite* spriteJump = new Sprite(associated, SPRITE_JUMP, SPRITE_JUMP_FRAMES, true);
    Sprite* spriteFall = new Sprite(associated, SPRITE_FALL, SPRITE_FALL_FRAMES);
    Sprite* spriteLand = new Sprite(associated, SPRITE_LAND, SPRITE_LAND_FRAMES, true);
    Sprite* spriteDamage = new Sprite(associated, SPRITE_DAMAGE);
    Sprite* spriteSwordOnGround0 = new Sprite(
        associated, SPRITE_ATTACK_SWORDGROUND0, SPRITE_ATTACK_SWORDGROUND0_FRAMES, true);
    Sprite* spriteSwordOnGround1 = new Sprite(
        associated, SPRITE_ATTACK_SWORDGROUND1, SPRITE_ATTACK_SWORDGROUND1_FRAMES, true);
    Sprite* spriteSwordStrongCharge = new Sprite(
        associated, SPRITE_ATTACK_SWORDSTRONGC, SPRITE_ATTACK_SWORDSTRONGC_FRAMES, true);
    Sprite* spriteSwordStrongFull = new Sprite(
        associated, SPRITE_ATTACK_SWORDSTRONGF, SPRITE_ATTACK_SWORDSTRONGF_FRAMES);
    Sprite* spriteSwordStrongPerform = new Sprite(
        associated, SPRITE_ATTACK_SWORDSTRONGP, SPRITE_ATTACK_SWORDSTRONGP_FRAMES, true);
    Sprite* spriteSpell0 = new Sprite(associated, SPRITE_SPELL0, SPRITE_SPELL0_FRAMES, true);
    Sprite* spriteDeath = new Sprite(associated, SPRITE_DEATH, SPRITE_DEATH_FRAMES, true);

    AddSpriteState(EntityState::Idle, spriteIdle);
    AddSpriteState(EntityState::Running, spriteRun);
    AddSpriteState(EntityState::Jumping, spriteJump);
    AddSpriteState(EntityState::Falling, spriteFall);
    AddSpriteState(EntityState::Landing, spriteLand);
    AddSpriteState(EntityState::Injured, spriteDamage);
    AddSpriteState(EntityState::AttackingSwordOnGround_0, spriteSwordOnGround0);
    AddSpriteState(EntityState::AttackingSwordOnGround_1, spriteSwordOnGround1);
    AddSpriteState(EntityState::AttackingSwordStrong_Charge, spriteSwordStrongCharge);
    AddSpriteState(EntityState::AttackingSwordStrong_Full, spriteSwordStrongFull);
    AddSpriteState(EntityState::AttackingSwordStrong_Perform, spriteSwordStrongPerform);
    AddSpriteState(EntityState::CastingSpell_0, spriteSpell0);
    AddSpriteState(EntityState::Dead, spriteDeath);

    rigidBody = new RigidBody(associated);
    associated.AddComponent(rigidBody);
    rigidBody->triggerLabels.push_back("Enemy");
    rigidBody->triggerLabels.push_back("Boss");

    collider = new Collider(associated);
    collider->SetBox(Vec2(COLLIDER_POSITION_ONGROUND), Vec2(COLLIDER_BOX_SIZE));
    associated.AddComponent(collider);

    cameraBox = new GameObject(associated.layer);
    CameraBox* cameraBoxComp = new CameraBox(*cameraBox, &associated, CAMERABOX_SPACING);
    cameraBoxComp->focusBoxOffset = Rect(CAMERABOX_RECT);
    cameraBox->AddComponent(cameraBoxComp);
    cameraBoxComp->AddMethod(this, std::bind(&CameraEffects, this));
    Game::GetInstance().GetCurrentState().AddObject(cameraBox);
}

void Kid::Start () {
    state = EntityState::Idle;
    sprites[state].get()->SetFrame(0);

    Camera::Follow(
        cameraBox, Vec2(CINEMACHINE_LENGTH), CINEMACHINE_SLICES,
        Camera::RIGHT, Camera::UP, Vec2(CINEMACHINE_OFFSET));
    Camera::cinemachine.Setup(CINEMACHINE_SETUP);
    Camera::offset.y = 0;

    cameraGroundedTimer.SetResetTime(CAMERA_GROUNDED_RESET_TIME);
}

// keep it empty so LateUpdateEntity is not called
void Kid::LateUpdate (float dt) {}

void Kid::UpdateEntity (float dt) {
    if (state == EntityState::Dead) return;
    InputManager& input = InputManager::GetInstance();

    if (isInvincible)
        InvincibleUpdate(dt);

    // prevent movement when opposite directions are active
    int directionX = input.IsKeyDown(Key::moveRight) - input.IsKeyDown(Key::moveLeft);
    bool lockedDirection = attackPerforming or chargePerforming or damagePerforming;
    bool changeDirectionX = (not lockedDirection) and (directionX != 0) and (directionX != lastDirectionX);
    if (changeDirectionX) lastDirectionX = directionX;

    // tolerance before reset run speed
    runSpeedReset = (directionX == 0)? true : false;
    if (runSpeedReset) runSpeedIncrease = 0.0f;

    // tolerance before start falling
    bool isNotFalling = (state != EntityState::Falling) and (state != EntityState::Injured);
    bool attackIsOver = attackPerforming and (attackTimer.GetTime() >= ATTACK_SWORD_TIME_HIT);
    bool isNotAttacking = (not attackPerforming) or attackIsOver;
    if (isNotFalling and isNotAttacking and (rigidBody->GetSpeed().y > 100)) {
        FormatState(EntityState::Falling);
        isGrounded = false;
    }

    // waits the falling before die
    else if ((isNotFalling and (hp <= 0)) or isDead) {
        Die();
        return;
    }

    switch (state) {
        case EntityState::Idle:
            // prevent movement when opposite directions are active
            if (directionX != 0)
                FormatState(EntityState::Running);

            // start melee attack on ground
            if (input.KeyPress(Key::attack)) {
                FormatState(EntityState::AttackingSwordOnGround_0);
                break;
            }
            // start magic attack
            else if (input.KeyPress(Key::magic)) {
                FormatState(EntityState::CastingSpell_0);
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
            // start magic attack
            else if (input.KeyPress(Key::magic)) {
                FormatState(EntityState::CastingSpell_0);
            }
            // start jump
            else if (input.KeyPress(Key::jump))
                FormatState(EntityState::Jumping);

            break;
        
        case EntityState::Jumping:
            // jump is performing
            rigidBody->SetSpeedOnY(-FORCE_JUMP + jumpSpeedDecrease);
            jumpSpeedDecrease += FORCE_JUMP_MASS * dt;
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
            AttackUpdate(dt);

            if (chargePerforming) {
                if (not input.IsKeyDown(Key::attack)) {
                    chargePerforming = false;

                } else if (attackTimer.GetTime() >= (ATTACK_SWORD_TIME_CANCEL - 0.04f)) {
                    FormatState(EntityState::AttackingSwordStrong_Charge);
                }
            }
            if (attackTimer.IsBetween(ATTACK_SWORD_TIME_HIT, ATTACK_SWORD_TIME_CANCEL)
            and input.KeyPress(Key::attack)) {
                FormatState(EntityState::AttackingSwordOnGround_1);
            }
            break;

        case EntityState::AttackingSwordOnGround_1:
            AttackUpdate(dt);
            break;

        case EntityState::AttackingSwordStrong_Charge:
            if (input.IsKeyDown(Key::attack)) {
                if (sprites[state]->OneshotIsOver())
                    FormatState(EntityState::AttackingSwordStrong_Full);
            } else {
                chargePerforming = false;
                FormatState(EntityState::Idle);
            } break;

        case EntityState::AttackingSwordStrong_Full:
            if (input.KeyRelease(Key::attack))
                FormatState(EntityState::AttackingSwordStrong_Perform);
            break;

        case EntityState::AttackingSwordStrong_Perform:
            AttackUpdate(dt);
            break;

        case EntityState::CastingSpell_0:
            spellTimer.Update(dt);
            if (spellTimer.IsOver())
                FormatState(Idle);
            break;

        case EntityState::Injured:
            damageTimer.Update(dt);
            if (damageTimer.IsOver())
                FormatState(EntityState::Falling);

            if (collider->box.GetPosition().DistanceTo(damageOrigin) > damageImpulse) {
                if (rigidBody->GetSpeed().y == 0.0f)
                    rigidBody->ResetGravity();

                jumpSpeedDecrease += FORCE_DAMAGE_MASS * dt * (-lastDirectionX);
                rigidBody->SetSpeed(
                    Vec2((damageForce.x * lastDirectionX) - jumpSpeedDecrease, rigidBody->GetSpeed().y)
                );
                if ((std::signbit(rigidBody->GetSpeed().x)==std::signbit(lastDirectionX)) and (hp > 0)) {
                    rigidBody->SetSpeedOnX(0.0f);
                    FormatState(EntityState::Falling);
                }
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
        case EntityState::AttackingSwordOnGround_0: case EntityState::AttackingSwordOnGround_1:
            if ((newState == EntityState::Injured) and (not swordAttackOnGround->lifetime.IsOver())
            and (lastDirectionX == ((argsv[AttackGeneric::_OriginX] < collider->box.x)? -1 : 1)))
                return false;
            attackImpulseCancel = false;
            attackPerforming = false;
            break;

        case EntityState::AttackingSwordStrong_Full:
            if (newState == EntityState::Injured)
                chargePerforming = false;
            break;

        case EntityState::AttackingSwordStrong_Perform:
            chargePerforming = false;
            attackPerforming = false;
            break;
        
        case EntityState::CastingSpell_0:
            attackPerforming = false;
            break;

        case EntityState::Injured:
            jumpSpeedDecrease = 0.0f;
            damagePerforming = false;
            break;

        default: break;
    }

    sprites[newState]->SetTextureColorMod(flickFactor, flickFactor, flickFactor);

    switch (newState) {
        case EntityState::Idle:
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
        
        // editar: usar uma altura minima para entrar nesse estado
        // para isso usar o ponto y a partir de falling e medir ate o ponto de impact down
        case EntityState::Landing:
            rigidBody->SetSpeedOnX(0.0f);
            runSpeedIncrease = 0.0f;
            return true;

        case EntityState::AttackingSwordOnGround_0:
            AttackStart();
            chargePerforming = true;
            return true;

        case EntityState::AttackingSwordOnGround_1:
            AttackStart();
            return true;

        case EntityState::AttackingSwordStrong_Perform:
            AttackStrongStart();
            return true;

        case EntityState::CastingSpell_0:
            spellTimer.Reset();
            rigidBody->SetSpeedOnX(0.0f);
            runSpeedIncrease = 0.0f;
            attackPerforming = true;
            foxAttack->Perform((lastDirectionX < 0)? FoxAttack::LEFT : FoxAttack::RIGHT);
            return true;

        case EntityState::Injured:
            if (isInvincible) return false;

            damageOrigin = collider->box.GetPosition();
            damageForce = Vec2(-argsv[AttackGeneric::_ForceX], -argsv[AttackGeneric::_ForceY]);
            damageImpulse = argsv[AttackGeneric::_Impulse] + argsv[AttackGeneric::_Displacement];
            jumpSpeedDecrease = 0.0f;
            damageTimer.Reset();

            lastDirectionX = (argsv[AttackGeneric::_OriginX] < collider->box.x)? -1 : 1;
            textureFlip = (lastDirectionX == 1)? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

            rigidBody->SetSpeed(Vec2(damageForce.x * lastDirectionX, damageForce.y));
            rigidBody->ResetGravity();
            damagePerforming = true;

            isInvincible = true;
            hp -= argsv[AttackGeneric::_Damage];

            ColliderReset();
            return true;

        default: return true;
    }
}

void Kid::AttackStart () {
    KidAttackMelee::AttackDirection direction = (lastDirectionX == 1)?
        KidAttackMelee::RIGHT : KidAttackMelee::LEFT;
    swordAttackOnGround->Perform(direction);
    attackOriginX = associated.box.x;

    if (runSpeedReset or swordAttackOnGround->ImpulseIsCanceled())
        attackImpulseCancel = true;
    if (not attackImpulseCancel)
        rigidBody->SetSpeedOnX(SPEED_ATTACK * lastDirectionX);
    else rigidBody->SetSpeedOnX(0.0f);
    
    attackPerforming = true;
    attackTimer.Reset();
}

void Kid::AttackStrongStart () {
    KidAttackStrong::AttackDirection direction = (lastDirectionX == 1)?
        KidAttackStrong::RIGHT : KidAttackStrong::LEFT;
    swordAttackStrong->Perform(direction);
    attackOriginX = associated.box.x;

    // if (runSpeedReset or swordAttackOnGround->ImpulseIsCanceled())
    //     attackImpulseCancel = true;
    // if (not attackImpulseCancel)
    //     rigidBody->SetSpeedOnX(SPEED_ATTACK * lastDirectionX);
    // else rigidBody->SetSpeedOnX(0.0f);
    
    attackPerforming = true;
    attackTimer.Reset();
}

void Kid::AttackUpdate (float dt) {
    if ((not attackImpulseCancel) and
    (fabs(associated.box.x - attackOriginX) >= IMPULSE_ATTACK_SWORD_X)) {
        rigidBody->SetSpeedOnX(0.0f);
        attackImpulseCancel = true;
    }
    attackTimer.Update(dt);
    if (sprites[state]->OneshotIsOver())
        FormatState(EntityState::Idle);
}

void Kid::InvincibleUpdate (float dt) {
    // invincibility time
    invincibilityTimer.Update(dt);
    if (invincibilityTimer.IsOver()) {
        invincibilityTimer.Reset();
        isInvincible = false;
        flickFactor = 255.0f;
        sprites[state]->SetTextureColorMod(255, 255, 255);
        return;
    }
    // flick animation
    flickFactor += flickDirection * INVINCIBLE_FLICK_SPEED * dt;
    if (flickFactor >= 255.0f) {
        flickDirection = -1.0f;
        flickFactor = 255.0f;
    } else if (flickFactor <= 0.0f) {
        flickDirection = 1.0f;
        flickFactor = 0.0f;
    } sprites[state]->SetTextureColorMod(flickFactor, flickFactor, flickFactor);
}

void Kid::Die () {
    associated.RemoveComponent(rigidBody);
    associated.RemoveComponent(collider);

    isInvincible = false;
    FormatState(EntityState::Dead);
    sprites[state]->SetTextureColorMod(255, 255, 255);
}

void Kid::NotifyCollision (GameObject& other) {
    rigidBody->NotifyCollision(other);

    switch (state) {
        case EntityState::Falling:
            // hits the ground
            if (rigidBody->ImpactDown()) {
                ColliderReset();
                FormatState(EntityState::Idle);
                isGrounded = true;
            } break;

        case EntityState::Jumping:
            // hits the ceiling
            if (rigidBody->ImpactUp())
                hitCeiling = true;
            break;

        case EntityState::AttackingSwordOnGround_0: case EntityState::AttackingSwordOnGround_1:
            if (rigidBody->ImpactLeft() or rigidBody->ImpactRight()) {
                rigidBody->SetSpeedOnX(0.0f);
                attackImpulseCancel = true;
            } break;

        case EntityState::Injured:
            // hits a wall
            if (rigidBody->ImpactLeft() or rigidBody->ImpactRight())
                hitWall = true;
            // hits the ground and dies
            else if ((hp <= 0) and rigidBody->ImpactDown())
                isDead = true;
            break;

        default: break;
    }
}

void Kid::ColliderReset () {
    collider->SetBox(Vec2(COLLIDER_POSITION_ONGROUND), Vec2(COLLIDER_BOX_SIZE));
    associated.box.y -= COLLIDER_POSITION_Y_REPULSION;
}

bool Kid::IsInvincible () {
    return isInvincible;
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