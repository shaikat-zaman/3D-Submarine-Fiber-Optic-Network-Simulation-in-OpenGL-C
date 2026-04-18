#include "common.h"

// drawSupertree: Draws Supertree Grove structure.
void drawSupertree(float cx, float cz, float h, float scale){
    glPushMatrix();
    glTranslatef(cx, 0, cz);
    glScalef(scale, scale, scale);

    mat(0.3f, 0.1f, 0.3f, 0.2f, 0.2f);
    cyl(0, 0, 0, 0, h*0.7f, 0, 0.3f, 12);

    if(!isDay) matE(0.8f, 0.1f, 0.6f, 0.5f, 0.1f, 0.4f);
    else mat(0.3f, 0.7f, 0.2f, 0.4f, 0.4f);
    glPushMatrix();
    glTranslatef(0, h*0.8f, 0);
    glRotatef(90, 1, 0, 0);
    glutSolidCone(1.2f, h*0.5f, 12, 4);
    glPopMatrix();

    if(!isDay) matE(1.0f, 0.3f, 0.9f, 0.8f, 0.2f, 0.7f);
    else mat(0.1f, 0.6f, 0.2f, 0.4f, 0.4f);
    glPushMatrix();
    glTranslatef(0, h*0.85f, 0);
    glRotatef(90, 1, 0, 0);
    glutWireCone(1.8f, h*0.6f, 16, 6);
    glPopMatrix();

    mat(0.7f, 0.7f, 0.75f, 0.8f, 0.8f);
    glPushMatrix();
    glTranslatef(0, h*0.5f, 0);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(0.04f, 1.0f, 8, 16);
    glPopMatrix();

    if(!isDay) matE(1.0f, 0.8f, 0.9f, 0.9f, 0.6f, 0.8f);
    else mat(0.8f, 0.4f, 0.6f, 0.2f, 0.2f);
    sph(0, h*0.85f, 0, 0.3f, 10);
    noE();

    glPopMatrix();
}

// drawArtScience: Draws ArtScience Museum lotus shape.
void drawArtScience(float cx, float cz){
    glPushMatrix(); glTranslatef(cx, 0, cz);

    mat(0.85f, 0.85f, 0.85f, 0.8f, 0.8f);
    cyl(0, 0, 0, 0, 0.5f, 0, 0.5f, 16);

    for(int i=0; i<10; i++){
        mat(0.9f, 0.9f, 0.95f, 1.0f, 1.0f);
        if(!isDay) matE(0.8f, 0.9f, 1.0f, 0.3f, 0.3f, 0.5f);

        glPushMatrix();
        glTranslatef(0, 0.3f, 0);
        glRotatef(i*36.0f, 0, 1, 0);
        glRotatef(40.0f, 1, 0, 0);
        glScalef(0.3f, 1.4f, 0.1f);

        sph(0, 0.0f, 0, 1.0f, 8);

        mat(0.4f, 0.6f, 0.8f, 0.9f, 1.0f);
        if(!isDay) matE(0.4f, 0.7f, 1.0f, 0.8f, 0.9f, 1.0f);
        glTranslatef(0, 0.1f, 0.05f);
        glutSolidCone(0.8f, 1.8f, 8, 4);
        noE();
        glPopMatrix();
    }

    mat(0.7f, 0.8f, 0.9f, 0.5f, 0.5f);
    sph(0, 0.3f, 0, 0.65f, 10);
    noE(); glPopMatrix();
}

// drawCapitaSpring: Draws building. Algorithms: DDA Line (wireframe), Bresenham (roof).
void drawCapitaSpring(float cx, float cz, float h){
    mat(0.3f, 0.4f, 0.5f, 0.9f, 1.0f);
    if(!isDay) matE(0.8f, 0.9f, 1.0f, 0.1f, 0.1f, 0.2f);
    box(cx, 0, cz, 1.4f, h, 1.4f); noE();

    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);
    glColor3f(0.0f, 1.0f, 1.0f);
    float halfW = 0.7f;

    drawLineDDA(cx - halfW, 0.0f, cx - halfW, h, cz - halfW);
    drawLineDDA(cx + halfW, 0.0f, cx + halfW, h, cz - halfW);
    drawLineDDA(cx - halfW, 0.0f, cx - halfW, h, cz + halfW);
    drawLineDDA(cx + halfW, 0.0f, cx + halfW, h, cz + halfW);

    for(float y = 0.0f; y <= h; y += 2.0f) {
        drawLineDDA(cx - halfW, y, cx + halfW, y, cz - halfW);
        drawLineDDA(cx - halfW, y, cx + halfW, y, cz + halfW);
        drawLineDDA(cx - halfW, y, cx - halfW, y, cz - halfW);
    }

    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);

    mat(0.1f, 0.6f, 0.2f, 0.1f, 0.1f);
    for(int i=0; i<8; i++){ sph(cx-0.4f+(i%4)*0.25f, h*0.42f+(i/4)*0.2f, cz+0.75f, 0.15f, 8); }

    if(!isDay) matE(0.0f, 0.8f, 0.3f, 0.4f, 0.6f, 0.3f);
    else mat(0.1f, 0.6f, 0.2f, 0.1f, 0.1f);
    for(int i=0; i<6; i++){ sph(cx-0.3f+(i%3)*0.3f, h*0.42f, cz+0.72f, 0.08f, 8); }
    noE();

    mat(0.1f, 0.6f, 0.2f, 0.1f, 0.1f);
    for(int i=0; i<6; i++){ sph(cx-0.4f+(i%3)*0.4f, h+0.1f, cz-0.3f+(i/3)*0.6f, 0.2f, 8); }

    mat(0.7f, 0.7f, 0.75f, 0.6f, 0.6f);
    cyl(cx-0.4f, h, cz-0.3f, cx-0.4f, h+0.8f, cz-0.3f, 0.03f, 4);
    cyl(cx+0.4f, h, cz+0.3f, cx+0.4f, h+1.2f, cz+0.3f, 0.03f, 4);

    glDisable(GL_LIGHTING);
    glColor3f(0.2f, 0.2f, 0.2f);
    //drawLineBresenham(cx-0.6f, h+0.1f, cz, cx+0.6f, h+0.1f, cz);
    //drawLineBresenham(cx, h+0.1f, cz-0.6f, cx, h+0.1f, cz+0.6f);
    glEnable(GL_LIGHTING);
}

// drawSailTower: Draws twisted Marina Bay Sail tower.
void drawSailTower(float cx, float cz, float h){
    glPushMatrix(); glTranslatef(cx, 0, cz);
    for(int i=0; i<12; i++){
        float y = i * (h/12.0f), sc = 1.0f - (i*0.04f);
        mat(0.2f, 0.6f, 0.85f, 0.95f, 1.0f);
        if(!isDay) matE(0.9f, 0.9f, 0.6f, 0.2f, 0.3f, 0.1f);

        glPushMatrix();
        glTranslatef(0, y, 0);
        float twist_mult = (h-y)/h;
        glScalef(sc, 1.0f, sc*0.8f);
        glRotatef(i*1.8f * twist_mult, 0, 1, 0);
        box(0, 0, 0, 1.2f, h/12.0f, 1.2f);
        glPopMatrix();
    }
    noE();

    mat(0.85f, 0.85f, 0.9f, 0.8f, 0.8f);
    cyl(0.2f, h, 0, 0.2f, h+1.5f, 0, 0.04f, 4);
    mat(0.2f, 0.6f, 0.85f, 0.9f, 1.0f);
    sph(0.2f, h+1.6f, 0, 0.08f, 6);
    mat(0.85f, 0.85f, 0.9f, 0.8f, 0.8f);
    cyl(-0.2f, h, 0.3f, -0.2f, h+0.8f, 0.3f, 0.03f, 4);

    glPopMatrix();
}

// drawGlassTower: Draws tiered glass skyscraper.
void drawGlassTower(float cx, float cz, float h){
    float base_w = 1.2f, mid_w = 0.9f, top_w = 0.6f;
    float base_h = h*0.4f, mid_h = h*0.3f, top_h = h*0.3f;

    mat(0.2f, 0.45f, 0.7f, 0.9f, 1.0f);
    if(!isDay) matE(0.8f, 0.9f, 1.0f, 0.2f, 0.3f, 0.4f);
    box(cx, 0, cz, base_w, base_h, base_w); noE();

    glPushMatrix(); glTranslatef(0, base_h, 0);
    mat(0.4f, 0.7f, 0.9f, 0.95f, 1.0f);
    if(!isDay) matE(0.9f, 0.95f, 1.0f, 0.3f, 0.4f, 0.5f);
    box(cx, 0, cz, mid_w, mid_h, mid_w); noE();

    glTranslatef(0, mid_h, 0);
    mat(0.6f, 0.8f, 1.0f, 0.9f, 1.0f);
    if(!isDay) matE(0.95f, 1.0f, 1.0f, 0.4f, 0.5f, 0.6f);
    box(cx, 0, cz, top_w, top_h, top_w); noE();
    glPopMatrix();

    mat(0.8f, 0.8f, 0.8f, 0.4f, 0.4f);
    for(float y=0.5f; y<h; y+=1.2f) box(cx, y, cz, 1.25f, 0.05f, 1.25f);

    box(cx-0.6f, 0, cz-0.6f, 0.05f, h, 0.05f); box(cx+0.6f, 0, cz-0.6f, 0.05f, h, 0.05f);
    box(cx-0.6f, 0, cz+0.6f, 0.05f, h, 0.05f); box(cx+0.6f, 0, cz+0.6f, 0.05f, h, 0.05f);
}

// drawSGMiniBld: Draws mini building. Algorithm: DDA Line (tower).
void drawSGMiniBld(float cx, float cz, float h){
    glPushMatrix(); glTranslatef(cx, 0, cz);

    mat(0.85f, 0.85f, 0.8f, 0.5f, 0.5f);
    box(-0.9f, 0, 0, 1.8f, h*0.5f, 1.0f);

    mat(0.2f, 0.2f, 0.2f, 0.1f, 0.1f);
    for(int i=0; i<3; i++){
        float x = -0.6f + i*0.6f;
        box(x, h*0.15f, 0.52f, 0.25f, 0.4f, 0.05f);
        box(x, h*0.35f, 0.52f, 0.25f, 0.3f, 0.05f);
    }
    mat(0.8f, 0.3f, 0.1f, 0.3f, 0.3f);
    box(-0.9f, h*0.5f, 0, 1.9f, 0.1f, 1.1f);
    glPushMatrix(); glTranslatef(0, h*0.5f, 0); glRotatef(180, 1, 0, 0); glutSolidCone(1.0f, 0.3f, 4, 1); glPopMatrix();

    glTranslatef(0, h*0.5f, 0);
    mat(0.3f, 0.5f, 0.7f, 0.9f, 0.8f);
    if(!isDay) matE(0.9f, 0.9f, 0.8f, 0.2f, 0.2f, 0.1f);
    box(-0.2f, 0, 0, 1.4f, h*0.5f, 0.8f); noE();

    mat(0.8f, 0.8f, 0.85f, 0.4f, 0.4f);
    for(float y=0.0f; y<h*0.5f; y+=0.3f) box(-0.2f, y, 0.42f, 1.45f, 0.02f, 0.05f);

    glDisable(GL_LIGHTING);
    glColor3f(0.8f, 0.2f, 0.2f);
    drawLineDDA(-0.6f, h*0.5f, 0.6f, h*0.5f + 1.2f, 0.0f);
    glEnable(GL_LIGHTING);

    mat(0.1f, 0.6f, 0.2f, 0.1f, 0.1f); sph(-0.6f, 0.1f, 0.3f, 0.15f, 6); sph(0.6f, h*0.5f+0.1f, 0, 0.15f, 6); noE();
    glPopMatrix();
}

// drawFlyer: Draws Singapore Flyer observation wheel.
void drawFlyer(float cx, float cz){
    glPushMatrix();
    glTranslatef(cx, 4.0f, cz);

    mat(0.9f, 0.95f, 1.0f, 0.6f, 0.6f);
    glPushMatrix();
    glTranslatef(0, -4.0f, 0.2f);
    glRotatef(-90, 1, 0, 0);
    GLUquadric* qs = gluNewQuadric();
    gluCylinder(qs, 0.35f, 0.15f, 4.0f, 16, 1);
    gluDeleteQuadric(qs);
    glPopMatrix();

    glPushMatrix();
    float wheelRot = gTime * 10.0f;
    glRotatef(wheelRot, 0, 0, 1);

    mat(0.8f, 0.8f, 0.9f, 0.5f, 0.5f);
    if(!isDay) matE(0.8f, 0.8f, 1.0f, 0.3f, 0.3f, 0.5f);
    glutWireTorus(0.12f, 2.5f, 12, 36);

    glutWireTorus(0.04f, 1.8f, 8, 24);

    mat(0.6f, 0.6f, 0.65f, 0.6f, 0.6f);
    for(int i=0; i<16; i++){
        float a1 = i * (PI / 8.0f);
        float a2 = (i+1) * (PI / 8.0f);

        float ca1 = cosf(a1), sa1 = sinf(a1);
        float ca2 = cosf(a2), sa2 = sinf(a2);

        cyl(0, 0, 0, ca1*2.5f, sa1*2.5f, 0, 0.02f, 4);

        cyl(ca1*1.8f, sa1*1.8f, 0, ca2*2.5f, sa2*2.5f, 0, 0.015f, 4);
        cyl(ca2*1.8f, sa2*1.8f, 0, ca1*2.5f, sa1*2.5f, 0, 0.015f, 4);
    }

    for(int i=0; i<12; i++){
        float a = i * (PI / 6.0f);
        glPushMatrix();
        glTranslatef(cosf(a)*2.5f, sinf(a)*2.5f, 0.15f);
        glRotatef(-wheelRot, 0, 0, 1);

        if(!isDay) matE(0.9f, 0.95f, 1.0f, 0.6f, 0.6f, 0.8f);
        else mat(0.8f, 0.9f, 1.0f, 0.8f, 0.9f);

        box(-0.1f, -0.15f, -0.1f, 0.2f, 0.3f, 0.2f);
        glPopMatrix();
    }
    noE();
    glPopMatrix();

    mat(0.3f, 0.3f, 0.35f, 0.8f, 0.8f);
    sph(0, 0, 0, 0.35f, 16);
    mat(0.9f, 0.9f, 1.0f, 0.5f, 0.5f);
    sph(0, 0, 0.35f, 0.15f, 12);

    glPopMatrix();
}

// drawMBS: Draws Marina Bay Sands hotel and skypark.
void drawMBS(void){
    float tx=14.0f, tz=3.0f, h=6.0f; // SHIFTED RIGHT (+3.5)
    for(int t=0; t<3; t++){
        float bx=tx+t*1.8f;
        mat(0.2f, 0.6f, 0.85f, 0.95f, 1.0f);
        if(!isDay) matE(0.9f, 0.9f, 0.6f, 0.2f, 0.3f, 0.1f);

        glPushMatrix(); glTranslatef(bx, 0, tz);
        box(0, h*0.25f, 0.2f, 0.9f, h*0.5f, 0.8f);
        box(0, h*0.75f, 0.05f, 0.8f, h*0.5f, 0.7f);

        mat(0.85f, 0.85f, 0.85f, 0.8f, 0.8f); cyl(0, 0, 0, 0, h, 0, 0.15f, 4);
        mat(0.8f, 0.8f, 0.85f, 0.6f, 0.6f);
        box(0, 0, 0.62f, 0.92f, h, 0.01f); box(0, 0, -0.32f, 0.87f, h, 0.01f);
        glPopMatrix();
    }
    noE();

    glPushMatrix();
    glTranslatef(tx+1.8f, 6.2f, tz+0.1f);

    mat(0.88f, 0.88f, 0.9f, 0.8f, 0.8f);
    glPushMatrix(); glScalef(6.0f, 0.4f, 1.2f); glutSolidSphere(1.0f, 16, 8); glPopMatrix();

    glPushMatrix(); glTranslatef(0, 0.15f, 0.25f);
    mat(0.2f, 0.7f, 1.0f, 0.9f, 1.0f);
    if(!isDay) matE(0.2f, 0.7f, 1.0f, 0.6f, 0.8f, 1.0f);
    glScalef(5.8f, 0.06f, 0.8f); glutSolidSphere(1.0f, 16, 8); glPopMatrix(); noE();

    glPushMatrix(); glTranslatef(0, 0.15f, -0.5f);
    mat(0.1f, 0.6f, 0.2f, 0.1f, 0.1f);
    if(!isDay) matE(0.0f, 1.0f, 0.5f, 0.4f, 0.6f, 0.3f);
    for(int i=0; i<12; i++){ sph(-1.2f+i*0.6f, 0.0f, 0.1f, 0.15f, 8); }
    mat(0.7f, 0.7f, 0.75f, 0.6f, 0.6f);
    cyl(2.4f, -0.1f, 0.1f, 2.4f, 0.4f, 0.1f, 0.06f, 4);
    box(-2.8f, -0.1f, 0.1f, 0.6f, 0.2f, 0.6f);
    glPopMatrix();

    glPopMatrix();
}

// drawSGDataCenter: Draws Singapore server building.
void drawSGDataCenter(void){
    float cx=12.7f, cz=0.5f; // SHIFTED RIGHT (+3.5)
    mat(0.1f, 0.12f, 0.15f, 0.6f, 0.5f); box(cx-1.5f, 0, cz-1.5f, 3.0f, 0.5f, 3.0f);
    if(!isDay) matE(0.0f, 0.5f, 1.0f, 0.0f, 0.4f, 0.8f); else mat(0.1f, 0.6f, 0.9f, 0.8f, 0.9f);
    box(cx-1.2f, 0.5f, cz-1.2f, 2.4f, 2.0f, 2.4f); noE();
    mat(0.2f, 0.22f, 0.25f, 0.4f, 0.4f);
    box(cx-1.3f, 0.5f, cz-1.3f, 0.4f, 2.2f, 0.4f); box(cx+0.9f, 0.5f, cz-1.3f, 0.4f, 2.2f, 0.4f);
    box(cx-1.3f, 0.5f, cz+0.9f, 0.4f, 2.2f, 0.4f); box(cx+0.9f, 0.5f, cz+0.9f, 0.4f, 2.2f, 0.4f);
    box(cx-1.3f, 2.5f, cz-1.3f, 2.6f, 0.4f, 2.6f);
    mat(0.5f, 0.5f, 0.6f, 0.8f, 0.8f); cyl(cx, 2.9f, cz, cx, 3.5f, cz, 0.2f, 12);
    if(!isDay) matE(0.0f, 1.0f, 0.5f, 0.0f, 0.8f, 0.4f); else mat(0.0f, 1.0f, 0.5f, 0.6f, 0.6f);
    sph(cx, 3.6f, cz, 0.3f, 12); noE();
    matE(0.0f, 0.8f, 1.0f, 0.0f, 0.5f, 0.8f); box(cx-1.0f, 1.2f, cz+1.2f, 2.0f, 0.4f, 0.05f); noE();
}

// drawSGBuildingLinks: Draws power poles and animated packet links.
void drawSGBuildingLinks(void){
    float dcX=12.7f, dcY=3.6f, dcZ=0.5f; // SHIFTED RIGHT (+3.5)

    float blds[8][3]={{15.2f,5.2f,3.0f},{20.0f,6.5f,4.5f},{12.3f,1.5f,3.0f},{18.0f,9.0f,-3.5f},{20.5f,9.5f,-4.0f},{19.0f,3.0f,2.5f},{21.0f,4.0f,1.5f},{16.0f,10.0f,-4.5f}};
    float poles[8][2]={{15.2f,1.8f},{20.0f,1.8f},{12.3f,1.8f},{18.0f,-1.8f},{20.5f,-1.8f},{19.0f,1.8f},{21.0f,1.8f},{16.0f,-1.8f}};
    float poleH = 2.5f;
    float cols[8][3]={{0,.95f,.4f},{.85f,0,1},{0,.85f,1},{.85f,0,1},{.0f,1,.6f},{1,.5f,.0f},{.4f,.8f,1},{.9f,.2f,.5f}};

    glDisable(GL_LIGHTING); glLineWidth(1.0f);
    for(int i=0;i<8;i++){
        mat(0.3f, 0.3f, 0.35f, 0.5f, 0.6f); cyl(poles[i][0], 0, poles[i][1], poles[i][0], poleH, poles[i][1], 0.04f, 6);

        glColor3f(.1f,.1f,.1f);
        glBegin(GL_LINES); glVertex3f(dcX,dcY,dcZ); glVertex3f(poles[i][0],poleH,poles[i][1]); glVertex3f(poles[i][0],poleH,poles[i][1]); glVertex3f(blds[i][0],blds[i][1],blds[i][2]); glEnd();

        if (cableOK) {
            float t_anim=fmodf(gTime*(dataspd*1.4f+i*.12f)+i*.37f,1.f);
            float px, py, pz;
            if (t_anim < 0.5f) { float nt = t_anim / 0.5f; px = dcX + (poles[i][0] - dcX) * nt; py = dcY + (poleH - dcY) * nt; pz = dcZ + (poles[i][1] - dcZ) * nt; }
            else { float nt = (t_anim - 0.5f) / 0.5f; px = poles[i][0] + (blds[i][0] - poles[i][0]) * nt; py = poleH + (blds[i][1] - poleH) * nt; pz = poles[i][1] + (blds[i][2] - poles[i][1]) * nt; }
            glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE);
            glColor4f(cols[i][0],cols[i][1],cols[i][2],.95f); sph(px,py,pz,.05f,8);
            glColor4f(cols[i][0],cols[i][1],cols[i][2],.3f);  sph(px,py,pz,.13f,8);
            glDisable(GL_BLEND);
        }
    }
    glLineWidth(1); glEnable(GL_LIGHTING);
}

// drawSGTerrain: Seamlessly fades into the sky on the right AND back!
void drawSGTerrain(void){
    int NX=40,NZ=30;

    // FIX: x0 is exactly 10.0f so it seamlessly connects to the Ocean wall
    float x0=10.0f,x1=28.0f,z0=-10.0f,z1=35.0f;

    float skyR = isDay ? 0.7f : 0.05f;
    float skyG = isDay ? 0.85f : 0.05f;
    float skyB = isDay ? 1.0f : 0.2f;

    glDisable(GL_LIGHTING);

    // 1. MAIN TERRAIN (Fades on the right edge seamlessly)
    for(int i=0;i<NX-1;i++){
        glBegin(GL_QUAD_STRIP);
        for(int j=0;j<NZ;j++) for(int k=0;k<2;k++){
            float fx=x0+(i+k)*(x1-x0)/(NX-1);
            float fz=z0+j*(z1-z0)/(NZ-1);

            float brt=isDay?1:.25f;
            float r = .46f*brt, g = .46f*brt, b = .50f*brt;

            // X-Axis Right Side Fade (Shifted to 23.5)
            float pX = 0.0f;
            if(fx > 23.5f) {
                pX = (fx - 23.5f) / 4.5f;
                if(pX > 1.0f) pX = 1.0f;
            }

            glColor3f(r*(1-pX) + skyR*pX, g*(1-pX) + skyG*pX, b*(1-pX) + skyB*pX);
            glVertex3f(fx, 0, fz); // Always at exactly Y=0
        }
        glEnd();
    }

    // 2. NEW VANISHING EXTENSION (Fades both back AND right)
    glBegin(GL_QUAD_STRIP);
    for(int st = 0; st <= 20; st++) {
        float pZ = st / 20.0f; // depth fade
        float curZ = -10.0f - (30.5f * pZ);

        // FIX: Left corner exactly matches the 10.0f boundary with no gaps
        float curX0 = 10.0f * (1.0f - pZ) + (8.5f) * pZ;
        float curX1 = 28.0f * (1.0f - pZ) + (16.5f) * pZ;

        float brt = isDay ? 1 : .25f;
        float baseR = 0.46f * brt, baseG = 0.46f * brt, baseB = 0.50f * brt;

        // Left vertex (only depth fade)
        float pLeft = pZ;
        glColor3f(baseR*(1-pLeft) + skyR*pLeft, baseG*(1-pLeft) + skyG*pLeft, baseB*(1-pLeft) + skyB*pLeft);
        glVertex3f(curX0, 0.0f, curZ);

        // Right vertex (max of depth fade and x-edge fade)
        float pXRight = 0.0f;
        if(curX1 > 23.5f) pXRight = (curX1 - 23.5f) / 4.5f;
        if(pXRight > 1.0f) pXRight = 1.0f;

        float pRight = (pZ > pXRight) ? pZ : pXRight;
        glColor3f(baseR*(1-pRight) + skyR*pRight, baseG*(1-pRight) + skyG*pRight, baseB*(1-pRight) + skyB*pRight);
        glVertex3f(curX1, 0.0f, curZ);
    }
    glEnd();

    glEnable(GL_LIGHTING);
}

// drawStreetLight: Draws street lamp. Algorithm: Midpoint Circle (bulb).
void drawStreetLight(float x,float z){
    mat(.38f,.38f,.42f,.3f,.3f); cyl(x,0,z,x,1.8f,z,.04f,6); cyl(x,1.8f,z,x+.22f,1.9f,z,.03f,4);

    glDisable(GL_LIGHTING);
    if(!isDay) glColor3f(1.0f, 0.9f, 0.5f); else glColor3f(0.8f, 0.8f, 0.8f);
    glPushMatrix();
    glTranslatef(x+.22f, 1.95f, z);
    glRotatef(90, 0, 1, 0);
    drawCircleMidpoint(0.0f, 0.0f, 0.0f, 0.08f);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

// drawTollPlaza: A structure at the end of the road to hide the car vanish point
void drawTollPlaza(float cx, float cz) {
    glPushMatrix();
    glTranslatef(cx, 0.0f, cz);

    // Main Roof/Canopy (Wide enough to cover both lanes)
    mat(0.2f, 0.4f, 0.7f, 0.5f, 0.5f); // Blueish metallic
    box(0.0f, 1.2f, 0.0f, 1.5f, 0.2f, 2.5f); // Roof

    // Side Pillars
    mat(0.7f, 0.7f, 0.75f, 0.3f, 0.3f); // Light grey concrete/metal
    box(-0.2f, 0.0f, 1.6f, 0.6f, 1.2f, 0.4f); // Front Left Pillar
    box( 0.2f, 0.0f, 1.6f, 0.6f, 1.2f, 0.4f); // Back Left Pillar
    box(-0.2f, 0.0f,-1.6f, 0.6f, 1.2f, 0.4f); // Front Right Pillar
    box( 0.2f, 0.0f,-1.6f, 0.6f, 1.2f, 0.4f); // Back Right Pillar

    // Center divider/booth
    mat(0.8f, 0.8f, 0.8f, 0.4f, 0.4f); // White booth
    box(0.0f, 0.0f, 0.0f, 0.8f, 0.8f, 0.4f);

    // Glass windows on booth
    mat(0.1f, 0.8f, 0.9f, 0.9f, 0.9f); // Glass
    box(0.0f, 0.3f, 0.0f, 0.85f, 0.3f, 0.45f);

    glPopMatrix();
}


// drawSingapore: Assembles SG terrain, landmarks, roads, and cars.
void drawSingapore(void){
    float skyR = isDay ? 0.7f : 0.05f;
    float skyG = isDay ? 0.85f : 0.05f;
    float skyB = isDay ? 1.0f : 0.2f;

    drawSGTerrain();

    // CROP: Road now starts from X = 13.0 instead of 11.0 (in front of data center)
    glDisable(GL_LIGHTING);
    float roadR = 0.18f, roadG = 0.18f, roadB = 0.2f;
    glBegin(GL_QUAD_STRIP);
    for(int x_step = 0; x_step <= 20; x_step++) {
        // Starts at 13.0, ends at 28.5 (total width 15.5)
        float x = 13.0f + (15.5f * x_step / 20.0f);
        float p = 0.0f;

        // Left Edge Fog (Fade in)
        if (x < 15.0f) {
            p = 1.0f - ((x - 13.0f) / 2.0f);
            if (p < 0.0f) p = 0.0f;
            if (p > 1.0f) p = 1.0f;
        }
        // Right Edge Fog (Fade out)
        else if (x > 23.5f) {
            p = (x - 23.5f) / 4.5f;
            if (p > 1.0f) p = 1.0f;
        }

        float r = roadR * (1-p) + skyR * p;
        float g = roadG * (1-p) + skyG * p;
        float b = roadB * (1-p) + skyB * p;

        glColor3f(r, g, b);
        glVertex3f(x, 0.02f, -1.0f);
        glVertex3f(x, 0.02f,  1.0f);
    }
    glEnd();

    // CROP: Lane dividers now start from X = 13.5
    float brt=isDay?1:.55f;
    float laneR = .9f*brt, laneG = .9f*brt, laneB = .28f*brt;
    // Reduced count from 15 to 13 because the road is shorter on the left
    for(int i=0;i<13;i++){
        float rx=13.5f+i*1.2f;
        float p = 0.0f;

        // Left Edge Fog
        if (rx < 15.0f) {
            p = 1.0f - ((rx - 13.0f) / 2.0f);
            if (p < 0.0f) p = 0.0f;
            if (p > 1.0f) p = 1.0f;
        }
        // Right Edge Fog
        else if (rx > 23.5f) {
            p = (rx - 23.5f) / 4.5f;
            if (p > 1.0f) p = 1.0f;
        }

        float r = laneR*(1-p) + skyR*p;
        float g = laneG*(1-p) + skyG*p;
        float b = laneB*(1-p) + skyB*p;

        glColor3f(r, g, b);
        glBegin(GL_QUADS);
        glVertex3f(rx,.035f,-0.05f); glVertex3f(rx+.6f,.035f,-0.05f);
        glVertex3f(rx+.6f,.035f,0.05f); glVertex3f(rx,.035f,0.05f);
        glEnd();
    }
    glEnable(GL_LIGHTING);

    // Landmarks
    drawMBS();
    drawArtScience(12.3f, 3.0f);
    drawFlyer(20.0f, 4.5f);

    drawSupertree(10.3f, -4.5f, 3.5f, 1.0f);
    drawSupertree(12.0f, -6.5f, 4.5f, 1.2f);
    drawSupertree(14.0f, -5.5f, 4.0f, 1.1f);

    drawCapitaSpring(16.0f, -4.5f, 10.0f);
    drawGlassTower(18.0f, -3.5f, 9.0f);
    drawSailTower(20.5f, -4.0f, 9.5f);

    drawSGMiniBld(19.0f, 2.5f, 3.0f);
    drawSGMiniBld(21.0f, 1.5f, 4.0f);

    drawSGDataCenter();
    drawSGBuildingLinks();

    drawPalm(14.5f, 7.5f); drawPalm(17.5f, 8.5f); drawPalm(22.5f, 10.0f);

    // Distant Landmarks in SG (Safe from Wireframe Overflow)
    drawGlassTower(16.5f, -14.0f, 8.0f);
    drawSupertree(12.0f, -15.0f, 3.5f, 0.9f);
    drawSupertree(13.5f, -18.0f, 4.0f, 1.1f);
    drawSailTower(19.0f, -22.0f, 9.0f);

    drawPalm(15.0f, -14.0f);
    drawPalm(18.0f, -18.0f);

    // ==========================================
    // DEEP HORIZON ADDITIONS
    // ==========================================
    drawPalm(12.5f, -22.0f);
    drawPalm(21.0f, -25.0f);
    drawPalm(15.0f, -27.0f);
    drawPalm(18.5f, -30.0f);
    drawPalm(23.0f, -33.0f);
    drawPalm(10.5f, -28.0f);
    drawPalm(26.0f, -31.0f);
    drawPalm(13.5f, -34.0f);
    // ==========================================

    float sl[][2]={{14.0f,.0f},{16.5f,.72f},{19.0f,.0f},{21.5f,.72f},{24.0f,.0f}};
    for(int i=0;i<5;i++) drawStreetLight(sl[i][0],sl[i][1]);

    // Toll Plaza exactly at X = 26.5f
    drawTollPlaza(26.5f, 0.0f);

    // ==========================================
    // PERFECT CAR LOGIC (Reset DEEP inside the Toll Plaza at X = 27.2)
    // ==========================================
    for(int i=0;i<MAX_CARS;i++){
        float speed = 2.5f + (i % 4) * 0.4f;
        float offset = i * 3.8f;
        float cx, carZ;
        int visualDir;

        if (i % 2 == 0) {
            visualDir = 1;
            carZ = 0.5f;
            // Cars start at 13.0 and travel exactly 14.2 units to reach 27.2 (Deep inside Plaza)
            cx = 13.0f + fmodf(gTime * speed + offset, 14.2f);
        } else {
            visualDir = -1;
            carZ = -0.5f;
            // Cars spawn at 27.2 (Deep inside Plaza) and travel left exactly 14.2 units to reach 13.0
            cx = 27.2f - fmodf(gTime * speed + offset, 14.2f);
        }

        glPushMatrix();
        glTranslatef(cx, 0.0f, carZ);

        float pFade = 0.0f;
        // Fade in smoothly on the Left (from X=13.0 to X=14.5) to hide spawn near Data Center
        if (cx < 14.5f) {
            pFade = 1.0f - ((cx - 13.0f) / 1.5f);
            if(pFade < 0.0f) pFade = 0.0f;
            if(pFade > 1.0f) pFade = 1.0f;
        }
        // No Right Side Fade -> The building completely covers the car before it reaches 27.2

        // Draw if not fully faded AND within boundaries
        // Only draw when cx is between 13.0 and 27.2 so it doesn't pop outside limits
        if (pFade < 0.99f && cx >= 13.0f && cx <= 27.2f) {
            float rBody = sgCar[i].r * (1-pFade) + skyR * pFade;
            float gBody = sgCar[i].g * (1-pFade) + skyG * pFade;
            float bBody = sgCar[i].b * (1-pFade) + skyB * pFade;
            mat(rBody, gBody, bBody, .5f*(1-pFade), .6f*(1-pFade));
            box(0, .10f, 0, .52f, .20f, .27f);

            float rWin = .45f * (1-pFade) + skyR * pFade;
            float gWin = .72f * (1-pFade) + skyG * pFade;
            float bWin = .92f * (1-pFade) + skyB * pFade;
            mat(rWin, gWin, bWin, .85f*(1-pFade), .9f*(1-pFade));
            box(.16f*visualDir, .20f, 0, .14f, .15f, .22f);

            if(!isDay){
                float eFade = 1.0f - pFade;
                matE(1,1,.8f,1*eFade,1*eFade,.5f*eFade);
                sph(.25f*visualDir, .15f, -.1f, .04f, 6);
                sph(.25f*visualDir, .15f,  .1f, .04f, 6);
                noE();
            }

            float wCol = .1f * (1-pFade) + skyR * pFade;
            mat(wCol, wCol, wCol, .2f*(1-pFade), .1f*(1-pFade));
            for(int w=0;w<4;w++){
                float wx=(w<2)?-.18f:.18f, wz=(w%2==0)?-.11f:.11f;
                sph(wx, .06f, wz, .07f, 8);
            }
        }
        glPopMatrix();
    }
    // ==========================================

    drawBeach(10.0f,12.5f,.7f,45.0f,0);

    float fc=isDay?1:.45f;
    mat(.45f,.45f,.45f,.2f,.2f); cyl(18.0f,0,10.5f,18.0f,3.0f,10.5f,.025f,6);
    glDisable(GL_LIGHTING);
    glColor3f(.82f*fc,.10f,.10f); glVertex3f(18.0f,3.0f,10.5f); glVertex3f(18.9f,3.0f,10.5f); glVertex3f(18.9f,2.75f,10.5f); glVertex3f(18.0f,2.75f,10.5f);
    glColor3f(.96f*fc,.96f*fc,.96f*fc); glVertex3f(18.0f,2.75f,10.5f); glVertex3f(18.9f,2.75f,10.5f); glVertex3f(18.9f,2.50f,10.5f); glVertex3f(18.0f,2.50f,10.5f); glEnd();
    glEnable(GL_LIGHTING);

    float lc=isDay?1:.45f;
    glDisable(GL_LIGHTING);
    glColor3f(.15f,.15f,.75f*lc); drawStr3(16.5f,11.5f,0," SINGAPORE",GLUT_BITMAP_TIMES_ROMAN_24);

    glEnable(GL_LIGHTING);
}
