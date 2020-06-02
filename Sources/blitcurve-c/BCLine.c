// BCLine.c: Line type
// ©2020 DrewCrawfordApps LLC
#import "BCLine.h"

BCLine BCLineMakeWithPointAndAngle(simd_float2 a, float angle, float distance) {
    BCLine l;
    l.a = a;
    l.b.x = distance * cos(angle) + a.x;
    l.b.y = distance * sin(angle) + a.y;
    return l;
}
