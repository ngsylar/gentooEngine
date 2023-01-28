#ifndef CONTROLLER_H
#define CONTROLLER_H

class Key {
    public:
        // menus
        static int accept;
        static int decline;
        static int back;
        static int left;
        static int right;
        static int up;
        static int down;

        // player actions
        static int switchMovement;
        static int moveLeft;
        static int moveRight;
        static int jump;
        static int attack;
        static int magic;
};

#endif