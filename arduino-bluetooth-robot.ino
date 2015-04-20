#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 4); // RX, TX

// Keyboard ASCII code direction mapping
int buttonLeft[12] = {113, 119, 101, 114, 97, 115, 100, 122, 120, 99}; // q, w, e, r, a, s, d, f, z, x, c
int buttonRight[12] = {117, 105, 111, 112, 104, 106, 107, 108, 98, 110, 109}; // u, i, o, p, h, j, k, l, b, n, m
int buttonStart[4] = {116, 121, 103}; // t, y, g
int buttonStop = 32; // Space

// Pin configuration
const int LEFT_MOTORPIN = 0;
const int RIGHT_MOTORPIN = 1;

// Other constants
const int MOTOR_SPEED = 64;
const int DIRECTION_LEFT = 0;
const int DIRECTION_RIGHT = 1;

const int COMMAND_START = 0;
const int COMMAND_STOP = 1;
const int COMMAND_LEFT = 2;
const int COMMAND_RIGHT = 3;
const int COMMAND_NOT_RECOGNIZED = 4;

int currentCommand = 0;

void setup()
{
    mySerial.begin(9600);

    pinMode(RIGHT_MOTORPIN, OUTPUT);
    pinMode(LEFT_MOTORPIN, OUTPUT);
}

void loop()
{
    // Check if we got a serial command comming in
    if (mySerial.available()) {
        int r = mySerial.read();

        mySerial.println(r);
        // Detect which command has been sent
        switch (detectCommand(r)) {
            case COMMAND_START:
                mySerial.println("MOVE");
                move();
                break;
            case COMMAND_STOP:
                mySerial.println("STOP");
                stop();
                break;
            case COMMAND_LEFT:
                mySerial.println("LEFT");
                turnLeft();
                break;
            case COMMAND_RIGHT:
                mySerial.println("RIGHT");
                turnRight();
                break;
            default:
                mySerial.println("Command not recognized!");
                break;
        }
    }
}

/*
 * Detect which command has been sent
 * int r Request ASCII code
 * return int Command to execute
 */
int detectCommand(int r)
{
    if (r == buttonStop) {
        return COMMAND_STOP;
    } else {
        for (int i = 0; i < (sizeof(buttonLeft) / sizeof(int)) - 1; i++) {
            if (buttonLeft[i] == r) {
                return COMMAND_LEFT;
            } else if (buttonRight[i] == r) {
                return COMMAND_RIGHT;
            } else if (i < (sizeof(buttonStart) / sizeof(int)) - 1 && buttonStart[i] == r) {
                return COMMAND_START;
            }
        }
    }

    return COMMAND_NOT_RECOGNIZED;
}

/**
 * Turn
 * int direction What direction to turn to
 */
void turn(int direction) {
    stop();

    if (direction == DIRECTION_LEFT) {
        analogWrite(LEFT_MOTORPIN, MOTOR_SPEED);
    } else {
        analogWrite(RIGHT_MOTORPIN, MOTOR_SPEED);
    }
}

/**
 * Turn left
 * int turnTime How long to turn for
 */
void turnLeft() {
    turn(DIRECTION_LEFT);
}

/**
 * Turn right
 * int turnTime How long to turn for
 */
void turnRight() {
    turn(DIRECTION_RIGHT);
}

/**
 * Move
 */
void move() {
    analogWrite(LEFT_MOTORPIN, MOTOR_SPEED);
    analogWrite(RIGHT_MOTORPIN, MOTOR_SPEED);
}

/**
 * Stop
 */
void stop() {
    analogWrite(LEFT_MOTORPIN, 0);
    analogWrite(RIGHT_MOTORPIN, 0);
}
