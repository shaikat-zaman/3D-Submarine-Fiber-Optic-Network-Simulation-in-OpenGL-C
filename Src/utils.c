#include "common.h"

// mat: Sets basic material properties (diffuse, specular, shininess).
void mat(float r,float g,float b,float sp,float sh){
    glColor3f(r,g,b);
    float s[]={sp,sp,sp,1}, e[]={0,0,0,1}, ss[]={sh*128};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,s);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,ss);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,e);
}

// matE: Sets material emission color (for glowing objects).
void matE(float r,float g,float b,float er,float eg,float eb){
    glColor3f(r,g,b);
    float e[]={er,eg,eb,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,e);
}

// noE: Disables material emission.
void noE(void){
    float e[]={0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,e);
}

// box: Draws a scaled cube.
void box(float cx,float cy,float cz,float w,float h,float d){
    glPushMatrix(); glTranslatef(cx,cy+h*.5f,cz); glScalef(w,h,d); glutSolidCube(1); glPopMatrix();
}

// sph: Draws a solid sphere.
void sph(float x,float y,float z,float r,int sl){
    glPushMatrix(); glTranslatef(x,y,z); glutSolidSphere(r,sl,sl); glPopMatrix();
}

// cyl: Draws a cylinder between two points in 3D space.
void cyl(float x1,float y1,float z1,float x2,float y2,float z2,float rad,int sl){
    float dx=x2-x1,dy=y2-y1,dz=z2-z1;
    float len=sqrtf(dx*dx+dy*dy+dz*dz);
    if(len<1e-4f) return;
    dx/=len; dy/=len; dz/=len;
    float cx2=dy-dz*0, cy2=dz*0-dx, cz2=dx*0-dy;
    float cl=sqrtf(cx2*cx2+cy2*cy2+cz2*cz2);
    float ang=acosf(dz)*180/PI;
    glPushMatrix(); glTranslatef(x1,y1,z1);
    if(cl>1e-4f) glRotatef(ang,cx2/cl,cy2/cl,cz2/cl);
    else if(dz<0) glRotatef(180,1,0,0);
    GLUquadric*q=gluNewQuadric(); gluCylinder(q,rad,rad,len,sl,1); gluDeleteQuadric(q);
    glPopMatrix();
}

// disk3: Draws a flat circular disk.
void disk3(float x,float y,float z,float r,int sl){
    glPushMatrix(); glTranslatef(x,y,z); glRotatef(-90,1,0,0);
    GLUquadric*q=gluNewQuadric(); gluDisk(q,0,r,sl,1); gluDeleteQuadric(q);
    glPopMatrix();
}

// drawStr3: Draws a 3D text string at specified coordinates.
void drawStr3(float x,float y,float z,const char*s,void*f){
    glRasterPos3f(x,y,z); for(;*s;s++) glutBitmapCharacter(f,*s);
}

// drawLineDDA: Draws a line between two points. Algorithm: Digital Differential Analyzer (DDA).
void drawLineDDA(float x1, float y1, float x2, float y2, float z) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float steps = (fabs(dx) > fabs(dy)) ? fabs(dx) : fabs(dy);
    steps *= 40.0f;
    if(steps < 1.0f) steps = 1.0f;
    float xInc = dx / steps;
    float yInc = dy / steps;
    float x = x1;
    float y = y1;
    glBegin(GL_POINTS);
    for(int i = 0; i <= (int)steps; i++) {
        glVertex3f(x, y, z);
        x += xInc;
        y += yInc;
    }
    glEnd();
}

// drawCircleMidpoint: Draws a filled circle using DDA scanlines. Algorithm: Midpoint Circle.
void drawCircleMidpoint(float cx, float cy, float cz, float r) {
    float density = 40.0f;
    int R = (int)(r * density);
    int x = 0;
    int y = R;
    int d = 1 - R;
    while (x <= y) {
        float fx = x / density;
        float fy = y / density;
        drawLineDDA(cx - fx, cy + fy, cx + fx, cy + fy, cz);
        drawLineDDA(cx - fx, cy - fy, cx + fx, cy - fy, cz);
        drawLineDDA(cx - fy, cy + fx, cx + fy, cy + fx, cz);
        drawLineDDA(cx - fy, cy - fx, cx + fy, cy - fx, cz);
        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

// drawCircleOutline: Draws the perimeter of a circle. Algorithm: Midpoint Circle.
void drawCircleOutline(float cx, float cy, float cz, float r) {
    float density = 40.0f;
    int R = (int)(r * density);
    int x = 0;
    int y = R;
    int d = 1 - R;
    glBegin(GL_POINTS);
    while (x <= y) {
        float fx = x / density;
        float fy = y / density;
        glVertex3f(cx + fx, cy + fy, cz);
        glVertex3f(cx - fx, cy + fy, cz);
        glVertex3f(cx + fx, cy - fy, cz);
        glVertex3f(cx - fx, cy - fy, cz);
        glVertex3f(cx + fy, cy + fx, cz);
        glVertex3f(cx - fy, cy + fx, cz);
        glVertex3f(cx + fy, cy - fx, cz);
        glVertex3f(cx - fy, cy - fx, cz);
        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
    glEnd();
}
