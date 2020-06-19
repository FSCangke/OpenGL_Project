//
// Created by Dark burster on 2020/6/19.
//

#ifndef OPENGL_EDITION_H
#define OPENGL_EDITION_H

#include "variable.h"

// ** NEW ** in this assignment, is an animation of a teapot
// Hitting p will pause this animation; see keyboard callback

void initall() {
    rotamount = 45.0;
    teapotlocX = -0.05;
    robotlocX = -0.05;
    shoulderl = 210;
    elbowl = -5;
    shoulderr = 150;
    elbowr = -65;
    legl = 150;
    kneel = 30;
    legr = 210;
    kneer = 0;
    maxlimit = 210;
    minlimit = 150;
    ramount = 2.5;
    slamount = 0.5;
    kneeamount = 0.25;
}

void initloc() {
    rotamount = 45.0;
    teapotlocX = -0.05;
    robotlocX = -0.05;
}

void animation(void) {
    teapotlocX = teapotlocX + 0.0025;
    robotlocX = robotlocX + 0.0025;
    rotamount = rotamount + ramount;
    if (teapotlocX > 1.0) teapotlocX = -0.5;
    if (robotlocX > 1.0) robotlocX = -0.5;
    if (rotamount > 360.0) rotamount = 0.0;
    if (shoulderr > maxlimit) {
        sign = -1;
    } else if (shoulderr < minlimit) {
        sign = 1;
    }
    shoulderr = shoulderr + sign * slamount;
    elbowr = elbowr + sign * slamount;
    shoulderl = shoulderl - sign * slamount;
    elbowl = elbowl - sign * slamount;
    legr = legr - sign * slamount;
    kneer = kneer + sign * kneeamount;
    legl = legl + sign * slamount;
    kneel = kneel - sign * kneeamount;

    glutPostRedisplay();
}

void setteapotloc() {
    printf("%.2f;%.2f;%.2f;%.2f\n", ramount, teapotlocX, teapotlocY, teapotlocZ);
    printf("input numbers\n");
    scanf("%f/%f/%f/%f", &ramount, &teapotlocX, &teapotlocY, &teapotlocZ);
    printf("%.2f;%.2f;%.2f;%.2f\n", ramount, teapotlocX, teapotlocY, teapotlocZ);
}

void setrobotmove() {
    printf("%.2f;%.2f;\n", slamount, kneeamount);
    printf("input ramount\n");
    scanf("%f/%f", &slamount, &kneeamount);
    printf("%.2f;%.2f;\n", slamount, kneeamount);
}

void setrobotloc() {
    printf("%.2f;%.2f;%.2f\n", robotlocX, robotlocY, robotlocZ);
    printf("input ramount\n");
    scanf("%f/%f/%f", &robotlocX, &robotlocY, &robotlocZ);
    printf("%.2f;%.2f;%.2f\n", robotlocX, robotlocY, robotlocZ);
}

void setrobotlimit() {
    printf("%d;%d\n", maxlimit, minlimit);
    printf("input ramount\n");
    scanf("%d/%d", &maxlimit, &minlimit);
    printf("%d;%d\n", maxlimit, minlimit);
}

#endif //OPENGL_EDITION_H
