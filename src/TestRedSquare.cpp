#include "GentooEngine.h"
#include "TestObjects.h"

#define RIGIDBODY_GRAVITY_MAX       800.0f

#define CINEMACHINE_LENGTH          25.0f, 175.0f
#define CINEMACHINE_SLICES          8, 32
#define CINEMACHINE_DEADSLICES      2, 28
#define CINEMACHINE_OFFSET          0.0f, 25.0f
#define CINEMACHINE_SETUP           true, true, true, false, true, true, false, false

#define ASSISTANT_OFFSET_Y          25.0f
#define ASSISTANT_OFFSET_Y_CUBIC    15625.0f

Ball::Ball (GameObject& associated): Component(associated) {
    associated.label = "Player";
    runSpeed = 300.0f;
    jumpForce = 400.0f;
    jumpHeightMax = 140.0f;
    jumpHeight = 0.0f;
    isJumping = false;
    isFalling = false;
    cameraDelay.SetResetTime(0.25f);
    cameraAcceleration = 0.0f;
    cameraOffset = 0.0f;
}

void Ball::Start () {
    Camera::Follow(
        &associated, Vec2(CINEMACHINE_LENGTH), CINEMACHINE_SLICES, CINEMACHINE_DEADSLICES,
        Camera::RIGHT, Camera::UP, Vec2(CINEMACHINE_OFFSET)
    ); Camera::cinemachine.Setup(CINEMACHINE_SETUP);
    Camera::offset.y = 0.0f;

    rigidBody = new RigidBody(associated);
    associated.AddComponent(rigidBody);
    associated.AddComponent(new Collider(associated));
    rigidBody->velocityMax.y = RIGIDBODY_GRAVITY_MAX;
}

void Ball::Update (float dt) {
    InputManager& input = InputManager::GetInstance();
    CameraCheckTracking(dt);

    if (isFalling)
        CameraHandleFall(dt);
    if (isJumping)
        HandleJump(input.IsKeyDown(KEY_ARROW_UP), dt);

    if (input.KeyPress(KEY_ARROW_UP) and rigidBody->IsGrounded())
        StartJump(dt);
    if (input.IsKeyDown(KEY_ARROW_LEFT))
        rigidBody->Translate(Vec2(-runSpeed,0)*dt);
    if (input.IsKeyDown(KEY_ARROW_RIGHT))
        rigidBody->Translate(Vec2(runSpeed,0)*dt);

    // // editar: remover
    // rigidBody->gravityEnabled = false;
    // if (input.IsKeyDown(KEY_ARROW_UP))
    //     rigidBody->Translate(Vec2(0,-runSpeed)*dt);
    // if (input.IsKeyDown(KEY_ARROW_DOWN))
    //     rigidBody->Translate(Vec2(0,runSpeed)*dt);
}

void Ball::StartJump (float dt) {
    cameraOffset = cameraDistance.y;
    float jumpDisplacement = jumpForce * dt;
    rigidBody->Translate(Vec2(0,-jumpDisplacement));
    jumpHeight += jumpDisplacement;
    rigidBody->gravityEnabled = false;
    isJumping = true;
}

void Ball::HandleJump (bool isKeyDown, float dt) {
    // if the button continues to be pressed increases the vertical jump height
    if (isKeyDown and (jumpHeight < jumpHeightMax)) {
        float jumpDisplacement = jumpForce * dt;
        rigidBody->Translate(Vec2(0,-jumpDisplacement));
        jumpHeight += jumpDisplacement;
        if (jumpHeight > jumpHeightMax)
            jumpHeight = jumpHeightMax;
    }
    // applies a constant force that gradually decreases due to the gravity force
    else {
        rigidBody->AddForce(Vec2(0,-((jumpForce*jumpHeight)/jumpHeightMax)));
        rigidBody->gravityEnabled = true;
        if (cameraOffset > 0.0f)
            isFalling = true;
        else jumpHeight = 0.0f;
        isJumping = false;
    }
    // if it hits the ceiling cancels the jump or the force applied in previous conditions
    if (rigidBody->IsColliding(RigidBody::UP)) {
        rigidBody->CancelForces(RigidBody::VERTICAL);
        rigidBody->gravityEnabled = true;
        jumpHeight = 0.0f;
        isJumping = false;
    }
}

void Ball::CameraHandleFall (float dt) {
    // if the jump height is less than 0.67 of the max jump height, disables camera acceleration
    if (jumpHeight < (jumpHeightMax * 0.67f)) {
        jumpHeight = 0.0f;
        isFalling = false;
        return;
    }
    // else slows down the camera climb
    cameraAcceleration += (jumpForce * jumpForce) / ASSISTANT_OFFSET_Y_CUBIC;
    Camera::screenOffset.y -= cameraAcceleration * dt;
    if (Camera::screenOffset.y <= 0.0f) {
        Camera::screenOffset.y = 0.0f;
        cameraAcceleration = 0.0f;
        jumpHeight = 0.0f;
        isFalling = false;
    }
}

void Ball::CameraCheckTracking (float dt) {
    if (not isFalling and (Camera::screenOffset.y < ASSISTANT_OFFSET_Y)) {
        
        // if it is falling speeds up the camera drop after some delay
        cameraDelay.Update(dt);
        if (cameraDelay.IsOver()) {
            cameraAcceleration += (jumpForce * jumpForce) / ASSISTANT_OFFSET_Y_CUBIC;
            Camera::screenOffset.y += cameraAcceleration * dt;
        }
        // if it hits the ground adjusts the camera offset value
        if (rigidBody->IsGrounded() and (Camera::screenOffset.y < ASSISTANT_OFFSET_Y)) {
            Camera::pos.y += ASSISTANT_OFFSET_Y - Camera::screenOffset.y;
            Camera::screenOffset.y = ASSISTANT_OFFSET_Y;
            cameraDelay.Reset();
        }
        // limits the camera offset value
        else if (Camera::screenOffset.y >= ASSISTANT_OFFSET_Y) {
            Camera::screenOffset.y = ASSISTANT_OFFSET_Y;
            cameraAcceleration = 0.0f;
            cameraDelay.Reset();
        }
    } // resets to default setting if camera loses the track of it
    Vec2 windowHalfSize = Game::GetInstance().GetWindowSize();
    cameraDistance = Camera::GetPosition() - associated.box.GetPosition();
    if ((fabs(cameraDistance.x) > windowHalfSize.x) or (fabs(cameraDistance.y) > windowHalfSize.y)) {
        Camera::Follow(
            &associated, Vec2(CINEMACHINE_LENGTH), CINEMACHINE_SLICES, CINEMACHINE_DEADSLICES,
            Camera::RIGHT, Camera::UP, Vec2(CINEMACHINE_OFFSET)
        ); Camera::cinemachine.Setup(CINEMACHINE_SETUP);
    }
}