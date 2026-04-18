#include "common.h"

// drawRealTree: Draws procedural blocky/cylindrical trees.
void drawRealTree(float x, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    glScalef(scale, scale, scale);
    mat(0.40f, 0.25f, 0.15f, 0.1f, 0.1f);
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    GLUquadric* qt = gluNewQuadric();
    gluCylinder(qt, 0.15f, 0.08f, 1.2f, 8, 1);
    gluDeleteQuadric(qt);
    glPopMatrix();
    cyl(0, 1.0f, 0, -0.3f, 1.6f, 0.2f, 0.05f, 6);
    cyl(0, 1.0f, 0,  0.3f, 1.5f, 0.2f, 0.05f, 6);
    cyl(0, 1.0f, 0,  0.0f, 1.7f,-0.3f, 0.05f, 6);
    float b = isDay ? 1.0f : 0.4f;
    mat(0.2f*b, 0.6f*b, 0.2f*b, 0.1f, 0.1f);
    sph( 0.0f, 1.4f,  0.0f, 0.5f,  10);
    sph(-0.3f, 1.6f,  0.2f, 0.45f, 10);
    sph( 0.3f, 1.5f,  0.2f, 0.45f, 10);
    sph( 0.0f, 1.7f, -0.3f, 0.45f, 10);
    sph(-0.15f, 1.8f, 0.0f, 0.4f, 10);
    glPopMatrix();
}

// drawPalm: Draws curved palm tree trunks and fronds.
void drawPalm(float x,float z){
    mat(.52f,.37f,.13f,.1f,.1f);
    float py=0,px=x;
    for(int i=0;i<10;i++){ float ox=sinf(i*.18f)*.12f; cyl(px+ox*i,py,z,px+ox*(i+1),py+.22f,z,.055f,6); py+=.22f; }
    float topX=px+sinf(10*.18f)*.12f*10;
    mat(.12f,.72f,.12f,.2f,.1f);
    for(int i=0;i<8;i++){ float a=i*(TAU/8); cyl(topX,py,z,topX+cosf(a)*.9f,py-.25f,z+sinf(a)*.9f,.025f,4); }
}

// drawWindmill: Draws animated rotating wind turbines.
void drawWindmill(float wx,float wz){
    float h=3.4f;
    mat(0.4f, 0.4f, 0.45f, 0.2f, 0.2f);
    box(wx-0.3f, 0.0f, wz-0.3f, 0.6f, 0.3f, 0.6f);
    mat(0.85f, 0.85f, 0.9f, 0.4f, 0.4f);
    glPushMatrix(); glTranslatef(wx, 0.3f, wz); glRotatef(-90, 1, 0, 0);
    GLUquadric* q = gluNewQuadric(); gluCylinder(q, 0.12f, 0.06f, h-0.3f, 16, 1); gluDeleteQuadric(q);
    glPopMatrix();
    mat(0.9f, 0.9f, 0.95f, 0.5f, 0.5f);
    box(wx, h, wz-0.15f, 0.3f, 0.25f, 0.4f);
    mat(0.4f, 0.4f, 0.45f, 0.6f, 0.6f);
    sph(wx, h+0.12f, wz+0.15f, 0.1f, 10);
    mat(0.95f, 0.95f, 0.98f, 0.6f, 0.6f);
    glPushMatrix(); glTranslatef(wx, h+0.12f, wz+0.15f); glRotatef(windAngle, 0, 0, 1);
    for(int b=0; b<3; b++){
        glPushMatrix(); glRotatef(b*120.0f, 0, 0, 1); glTranslatef(0.0f, 0.7f, 0.0f);
        glScalef(0.12f, 0.8f, 0.02f); glutSolidSphere(1.0f, 12, 4); glPopMatrix();
    }
    glPopMatrix();
}

// drawBeach: Draws procedural flat sand and overlapping wet sand mesh.
void drawBeach(float xC,float zC,float w,float d,int isBD){
    float brt=isDay?1:.4f;
    glDisable(GL_LIGHTING);
    float NX=16,NZ=10;
    for(int i=0;i<(int)NX-1;i++){
        glBegin(GL_QUAD_STRIP);
        for(int j=0;j<(int)NZ;j++){
            for(int k=0;k<2;k++){
                float fx=xC-w/2+(i+k)*(w/NX);
                float fz=zC-d/2+j*(d/NZ);
                float wet=(isBD)?(fx-(xC-w/2))/w:(1-(fx-(xC-w/2))/w);
                float sandy=.88f-.15f*wet;
                glColor3f(sandy*brt,(sandy-.06f)*brt,(sandy-.22f)*brt);
                glVertex3f(fx,.005f,fz);
            }
        }
        glEnd();
    }
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    float surfX=isBD?(xC+w/2):(xC-w/2);
    for(int j=0;j<(int)NZ-1;j++){
        float fz=zC-d/2+j*(d/NZ);
        float wa=.3f+.2f*sinf(gTime*2+j*.8f);
        glColor4f(.9f*brt,.97f*brt,1.f*brt,wa);
        glBegin(GL_QUADS);
        glVertex3f(surfX-.15f,.01f,fz); glVertex3f(surfX+.25f,.01f,fz);
        glVertex3f(surfX+.25f,.01f,fz+d/NZ); glVertex3f(surfX-.15f,.01f,fz+d/NZ);
        glEnd();
    }
    glDisable(GL_BLEND); glEnable(GL_LIGHTING);
}

// drawSatellite: Draws hovering orbital satellite with rotating dishes.
void drawSatellite(void){
    float satX = -20.0f + 8.0f * sinf(gTime * 0.15f);
    float satY = 13.5f;
    float satZ = -10.0f + 8.0f * cosf(gTime * 0.15f);
    glPushMatrix();
    glTranslatef(satX, satY, satZ);
    glRotatef(gTime * 15.0f, 0, 1, 0);
    glRotatef(15.0f, 1, 0, 0);
    mat(0.85f, 0.75f, 0.1f, 0.8f, 0.8f);
    box(-0.35f, -0.4f, -0.35f, 0.7f, 0.8f, 0.7f);
    mat(0.1f, 0.15f, 0.5f, 0.9f, 0.9f);
    box(-2.8f, -0.05f, -0.5f, 2.45f, 0.1f, 1.0f);
    box(0.35f, -0.05f, -0.5f, 2.45f, 0.1f, 1.0f);
    mat(0.8f, 0.8f, 0.85f, 0.5f, 0.5f);
    cyl(-0.35f, 0, 0, -2.8f, 0, 0, 0.04f, 6);
    cyl(0.35f, 0, 0, 2.8f, 0, 0, 0.04f, 6);
    for(int i=0; i<4; i++){
        box(-2.5f + i*0.5f, 0.02f, -0.51f, 0.03f, 0.1f, 1.02f);
        box(0.6f + i*0.5f, 0.02f, -0.51f, 0.03f, 0.1f, 1.02f);
    }
    mat(0.9f, 0.9f, 0.95f, 0.6f, 0.6f);
    glPushMatrix();
    glTranslatef(0.0f, -0.4f, 0.0f);
    glRotatef(90, 1, 0, 0);
    GLUquadric* q = gluNewQuadric();
    gluCylinder(q, 0.0f, 0.45f, 0.3f, 16, 1);
    gluDisk(q, 0, 0.45f, 16, 1);
    gluDeleteQuadric(q);
    glPopMatrix();
    if(!isDay) matE(1.0f, 0.1f, 0.1f, sinf(gTime*5)>0?1:0, 0, 0);
    else mat(1.0f, 0.0f, 0.0f, 0.5f, 0.5f);
    sph(0.0f, 0.45f, 0.0f, 0.08f, 8);
    noE();
    glPopMatrix();
}

// draw2DCloud: Draws a cloud. Algorithm: Midpoint Circle.
void draw2DCloud(float x, float y, float scale, float alpha) {
    glPushMatrix();
    glTranslatef(x, y, -22.0f);
    glScalef(scale, scale, 1.0f);
    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    drawCircleMidpoint(0.0f, 0.0f, 0.0f, 0.6f);
    drawCircleMidpoint(0.5f, 0.2f, 0.0f, 0.5f);
    drawCircleMidpoint(-0.5f, 0.1f, 0.0f, 0.4f);
    drawCircleMidpoint(0.8f, -0.1f, 0.0f, 0.35f);
    drawCircleMidpoint(-0.8f, -0.2f, 0.0f, 0.3f);
    glPopMatrix();
}

// drawSky: Draws screen-space background gradient, stars, and clouds.
void drawSky(void){
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); glOrtho(-1,1,-1,1,-1,1);
    glMatrixMode(GL_MODELVIEW);  glPushMatrix(); glLoadIdentity();
    glBegin(GL_QUADS);
    if(isDay){
        glColor3f(0.3f, 0.65f, 0.95f); glVertex2f(-1,1); glVertex2f(1,1);
        glColor3f(0.7f, 0.85f, 1.0f);  glVertex2f(1,0);  glVertex2f(-1,0);
        glColor3f(0.7f, 0.85f, 1.0f);  glVertex2f(-1,0); glVertex2f(1,0);
        glColor3f(0.15f, 0.12f, 0.08f); glVertex2f(1,-1); glVertex2f(-1,-1);
    } else {
        glColor3f(0.02f, 0.02f, 0.1f); glVertex2f(-1,1); glVertex2f(1,1);
        glColor3f(0.05f, 0.05f, 0.2f); glVertex2f(1,0);  glVertex2f(-1,0);
        glColor3f(0.05f, 0.05f, 0.2f); glVertex2f(-1,0); glVertex2f(1,0);
        glColor3f(0.02f, 0.02f, 0.05f); glVertex2f(1,-1); glVertex2f(-1,-1);
    }
    glEnd();
    glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW); glPopMatrix();

    if(!isDay){
        glPointSize(2.2f); glBegin(GL_POINTS);
        unsigned rng=12345;
        for(int s=0;s<420;s++){
            rng=rng*1664525+1013904223; float sx=(float)(rng&0xFFFF)/65535*60-30;
            rng=rng*1664525+1013904223; float sy=(float)(rng&0xFFFF)/65535*20;
            float br=.52f+.48f*sinf(s*.7f+gTime*.75f);
            glColor3f(br,br,br*.87f); glVertex3f(sx, sy, -25.0f);
        }
        glEnd(); glPointSize(1);
    }
    if(isDay){
        glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        float cloudData[][3] = {
            {-20.0f, 14.0f, 2.5f}, {-10.0f, 16.0f, 1.8f}, {0.0f, 13.0f, 3.0f},
            {12.0f, 17.0f, 2.2f}, {22.0f, 15.0f, 1.5f}, {-15.0f, 18.0f, 2.0f}
        };
        for(int c=0; c<6; c++){
            float cx = cloudData[c][0] + fmodf(gTime * 0.5f + c * 5.0f, 60.0f) - 15.0f;
            float cy = cloudData[c][1] + sinf(gTime * 0.2f + c) * 0.5f;
            float scale = cloudData[c][2];
            draw2DCloud(cx, cy, scale, 0.85f);
        }
        glDisable(GL_BLEND);
    }
    glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
}

// drawSunMoon: Draws sun/moon. Algorithms: Midpoint Circle (body), DDA Line (sun rays).
void drawSunMoon(void){
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    if(isDay){
        float sx = -5.0f, sy = 15.0f, sz = -25.0f;
        glColor3f(1.0f, 0.95f, 0.2f);
        drawCircleMidpoint(sx, sy, sz, 2.5f);
        float rayRot = gTime * 5.0f;
        glPushMatrix();
        glTranslatef(sx, sy, sz);
        glRotatef(rayRot, 0, 0, 1);
        glColor4f(1.0f, 0.9f, 0.1f, 0.6f);
        for(int r=0; r<12; r++){
            float a = r * (TAU / 12);
            float pulse = 0.5f + 0.2f * sinf(gTime * 2.0f + r);
            float startX = cosf(a)*2.6f;
            float startY = sinf(a)*2.6f;
            float endX = cosf(a)* (4.0f + pulse);
            float endY = sinf(a)* (4.0f + pulse);
            float perpX = -sinf(a) * 0.05f;
            float perpY = cosf(a) * 0.05f;
            for(int w = -3; w <= 3; w++) {
                drawLineDDA(startX + perpX*w, startY + perpY*w, endX, endY, 0.0f);
            }
        }
        glPopMatrix();
    } else {
        float sx = 15.0f, sy = 14.0f, sz = -22.0f;
        glColor3f(0.9f, 0.9f, 0.95f);
        drawCircleMidpoint(sx, sy, sz, 2.0f);
        glColor4f(0.75f, 0.75f, 0.8f, 0.8f);
        drawCircleMidpoint(sx + 0.5f, sy + 0.5f, sz + 0.1f, 0.4f);
        drawCircleMidpoint(sx - 0.6f, sy - 0.3f, sz + 0.1f, 0.5f);
        drawCircleMidpoint(sx + 0.2f, sy - 0.8f, sz + 0.1f, 0.3f);
        glColor4f(0.8f, 0.8f, 1.0f, 0.15f);
        for(float glowR = 2.1f; glowR <= 3.5f; glowR += 0.1f) {
            drawCircleOutline(sx, sy, sz - 0.1f, glowR);
        }
    }
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}
