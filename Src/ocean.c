#include "common.h"
#define MAX_STONE 35
#define MAX_CORAL 40
#define MAX_TRASH 20
#define MAX_WRECK 3
#define MAX_KELP 35

static float sf_sx[MAX_STONE], sf_sz[MAX_STONE], sf_ss[MAX_STONE];
static float sf_cx[MAX_CORAL], sf_cz[MAX_CORAL]; int sf_cs[MAX_CORAL];
static float sf_tx[MAX_TRASH], sf_tz[MAX_TRASH]; int sf_tt[MAX_TRASH];
static float sf_wx[MAX_WRECK], sf_wz[MAX_WRECK], sf_wr[MAX_WRECK];
static float sf_kx[MAX_KELP],  sf_kz[MAX_KELP];
static int floorInit = 0;

void initSeaFloor() {
    if(floorInit) return;
    for(int i=0; i<MAX_STONE; i++) { sf_sx[i] = -9.5f + (rand()%1900)/100.0f; sf_sz[i] = -38.0f + (rand()%7000)/100.0f; sf_ss[i] = 0.15f + (rand()%100)/300.0f; }
    for(int i=0; i<MAX_CORAL; i++) { sf_cx[i] = -9.5f + (rand()%1900)/100.0f; sf_cz[i] = -38.0f + (rand()%7000)/100.0f; sf_cs[i] = rand(); }
    for(int i=0; i<MAX_TRASH; i++) { sf_tx[i] = -9.5f + (rand()%1900)/100.0f; sf_tz[i] = -38.0f + (rand()%7000)/100.0f; sf_tt[i] = rand()%3; }
    for(int i=0; i<MAX_WRECK; i++) { sf_wx[i] = -8.0f + (rand()%1600)/100.0f; sf_wz[i] = -35.0f + (rand()%6000)/100.0f; sf_wr[i] = rand()%360; }
    for(int i=0; i<MAX_KELP; i++)  { sf_kx[i] = -9.5f + (rand()%1900)/100.0f; sf_kz[i] = -38.0f + (rand()%7000)/100.0f; }
    floorInit = 1;
}

//height of the sand at any X, Z coordinate
float getSeaFloorY(float x, float z) {
    float cdist = fabsf(x);
    float depth = -5.2f - 2.6f*(1.0f - cdist/10.0f);
    float ripple = 0.12f * sinf(x*3.0f + z*2.5f) + 0.05f * cosf(x*6.0f);
    float noise = 0.04f * sinf(x*5.0f + z*3.0f);
    return depth + ripple + noise;
}

//  Draw vibrant coral clusters
void drawCoral(float x, float y, float z, int seed) {
    glPushMatrix();
    glTranslatef(x, y + 0.1f, z);

    int rMod = seed % 4;
    float shade = isDay ? 1.0f : 0.4f;
    if(rMod == 0) mat(0.9f*shade, 0.3f*shade, 0.4f*shade, 0.2f, 0.2f); // Pink/Red
    else if(rMod == 1) mat(0.9f*shade, 0.6f*shade, 0.1f*shade, 0.2f, 0.2f); // Orange
    else if(rMod == 2) mat(0.6f*shade, 0.2f*shade, 0.8f*shade, 0.2f, 0.2f); // Purple
    else mat(0.2f*shade, 0.8f*shade, 0.5f*shade, 0.2f, 0.2f); // Bright Green

    // Brain coral bases
    glPushMatrix(); glScalef(1.2f, 0.8f, 1.0f); glutSolidSphere(0.18f, 10, 8); glPopMatrix();
    glPushMatrix(); glTranslatef(0.15f, 0.05f, 0.1f); glutSolidSphere(0.12f, 8, 8); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.1f, 0.08f, -0.15f); glutSolidSphere(0.15f, 8, 8); glPopMatrix();

    // Tube corals
    if(seed % 2 == 0) {
        mat(0.8f*shade, 0.8f*shade, 0.2f*shade, 0.2f, 0.2f); // Yellow tubes
        glPushMatrix(); glTranslatef(-0.05f, 0.1f, 0.0f); glRotatef(-20, 0,0,1); glRotatef(-90, 1,0,0); glutSolidCone(0.04f, 0.3f, 6, 2); glPopMatrix();
        glPushMatrix(); glTranslatef(0.05f, 0.1f, 0.05f); glRotatef(15, 1,0,0); glRotatef(-90, 1,0,0); glutSolidCone(0.03f, 0.25f, 6, 2); glPopMatrix();
    }
    glPopMatrix();
}

// Draw ocean trash (Barrels, Crates, Tires)
void drawTrash(float x, float y, float z, int type) {
    glPushMatrix();
    glTranslatef(x, y + 0.1f, z);
    float shade = isDay ? 0.8f : 0.3f;

    if(type == 0) { // Rusty Barrel
        mat(0.4f*shade, 0.3f*shade, 0.25f*shade, 0.2f, 0.2f);
        glRotatef(45, 1, 0, 1);
        glPushMatrix(); glScalef(0.15f, 0.25f, 0.15f); glutSolidCube(1.0f); glPopMatrix();
    } else if(type == 1) { // Sunken Wood Crate
        mat(0.3f*shade, 0.2f*shade, 0.1f*shade, 0.1f, 0.1f);
        glRotatef(15, 0, 1, 0);
        glRotatef(20, 1, 0, 0); // Stuck in sand
        glutSolidCube(0.25f);
    } else { // Rubber Tire
        mat(0.1f*shade, 0.1f*shade, 0.1f*shade, 0.1f, 0.1f);
        glRotatef(75, 1, 0, 0); // Flat on sand
        glutSolidTorus(0.04f, 0.12f, 8, 12);
    }
    glPopMatrix();
}

// Draw Sunken Shipwrecks
void drawShipwreck(float x, float y, float z, float rot) {
    glPushMatrix();
    glTranslatef(x, y + 0.15f, z);
    glRotatef(rot, 0, 1, 0);
    glRotatef(15.0f, 0, 0, 1); // Tilted and buried in sand

    float shade = isDay ? 0.7f : 0.3f;
    mat(0.25f*shade, 0.18f*shade, 0.12f*shade, 0.1f, 0.1f); // Dark rotting wood

    // Main broken hull
    glPushMatrix(); glScalef(1.5f, 0.4f, 0.7f); glutSolidSphere(1.0f, 16, 10); glPopMatrix();

    // Hollow interior illusion (Dark center)
    mat(0.05f, 0.05f, 0.05f, 0.1f, 0.1f);
    glPushMatrix(); glTranslatef(0.0f, 0.1f, 0.0f); glScalef(1.3f, 0.35f, 0.5f); glutSolidSphere(1.0f, 12, 8); glPopMatrix();

    // Broken Mast
    mat(0.2f*shade, 0.15f*shade, 0.1f*shade, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(-0.3f, 0.3f, 0.0f);
    glRotatef(55.0f, 1, 0, 1); // Snapped
    GLUquadric* q = gluNewQuadric();
    gluCylinder(q, 0.06f, 0.03f, 1.2f, 8, 1);
    gluDeleteQuadric(q);
    glPopMatrix();

    glPopMatrix();
}

//  Draw Organic Swaying Kelp / Sea Plants
void drawKelpPlant(float bx, float by, float bz) {
    glDisable(GL_LIGHTING);
    float dc = isDay ? 1.0f : 0.35f;
    glColor3f(0.1f * dc, 0.6f * dc, 0.2f * dc);

    // 3 Stalks per plant
    for(int s=0; s<3; s++) {
        glBegin(GL_QUAD_STRIP);
        float offset = (bx + bz) * 5.0f + s; // Random phase
        for(int seg=0; seg<=10; seg++) {
            float height = seg * 0.35f;
            // Snaking sway motion based on time and height
            float swayX = sinf(gTime * 1.5f + height + offset) * 0.15f;
            float swayZ = cosf(gTime * 1.2f + height + offset) * 0.1f;

            float px = bx + swayX + (s*0.1f);
            float py = by + height;
            float pz = bz + swayZ;

            float width = 0.06f * (1.0f - (float)seg/12.0f); // Tapers at top

            glVertex3f(px - width, py, pz);
            glVertex3f(px + width, py, pz);
        }
        glEnd();
    }
    glEnable(GL_LIGHTING);
}

// EXTERNS needed for specular colors and emmissive toggles
extern void matE(float r,float g,float b,float er,float eg,float eb);
extern void noE(void);

// =========================================================================
//   CRUISE SHIP ASSETS & HELPERS
// =========================================================================
static void shipMat(float dr,float dg,float db, float ar,float ag,float ab, float sr,float sg,float sb, float sh) {
    GLfloat d[4]={dr,dg,db,1.0f};
    GLfloat a[4]={ar,ag,ab,1.0f};
    GLfloat s[4]={sr,sg,sb,1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   d);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   a);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  s);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, sh);
    glColor3f(dr, dg, db);
}

#define S_M_WHITE()  shipMat(1.97f,.97f,.98f, .38f,.38f,.39f, .90f,.90f,.92f,  80)
#define S_M_CREAM()  shipMat(.98f,.95f,.84f, .38f,.37f,.32f, .60f,.60f,.50f,  50)
#define S_M_RED()    shipMat(.84f,.08f,.10f, .30f,.04f,.04f, .70f,.28f,.28f,  60)
#define S_M_DARK()   shipMat(.12f,.12f,.14f, .05f,.05f,.06f, .25f,.25f,.30f,  35)
#define S_M_BLACK()  shipMat(.06f,.06f,.07f, .02f,.02f,.03f, .18f,.18f,.22f,  25)
#define S_M_ORANGE() shipMat(1.0f,.46f,.00f, .40f,.20f,.00f, .70f,.50f,.20f,  60)
#define S_M_BLUE()   shipMat(.18f,.42f,.88f, .08f,.18f,.40f, .50f,.65f,.95f, 100)
#define S_M_LTBLUE() shipMat(.55f,.82f,.98f, .22f,.33f,.40f, .60f,.78f,.95f,  90)
#define S_M_YELLOW() shipMat(1.0f,.88f,.10f, .40f,.35f,.05f, .80f,.80f,.30f,  70)
#define S_M_CHROME() shipMat(.75f,.76f,.80f, .28f,.28f,.30f, 1.0f,1.0f,1.0f, 128)
#define S_M_FUNNEL() shipMat(.80f,.10f,.10f, .30f,.05f,.05f, .65f,.25f,.25f,  65)
#define S_M_WOOD()   shipMat(.56f,.38f,.18f, .22f,.15f,.07f, .25f,.18f,.08f,  20)
#define S_M_POOL()   shipMat(.10f,.62f,.82f, .05f,.25f,.35f, .50f,.80f,.92f, 110)

static GLUquadric* getShipQ() {
    static GLUquadric* q = NULL;
    if(!q) { q = gluNewQuadric(); gluQuadricNormals(q, GLU_SMOOTH); }
    return q;
}

static void shipBox(float x,float y,float z, float w,float h,float d) {
    glPushMatrix(); glTranslatef(x,y,z); glScalef(w,h,d); glutSolidCube(1.0); glPopMatrix();
}
static void shipCyl(float x,float y,float z, float r,float len,int sl) {
    glPushMatrix(); glTranslatef(x,y,z); glRotatef(-90.0f,1,0,0);
    gluCylinder(getShipQ(),r,r,len,sl,1); gluDisk(getShipQ(),0,r,sl,1);
    glTranslatef(0,0,len); gluDisk(getShipQ(),0,r,sl,1); glPopMatrix();
}
static void shipTcyl(float x,float y,float z, float r1,float r2,float len,int sl) {
    glPushMatrix(); glTranslatef(x,y,z); glRotatef(-90.0f,1,0,0);
    gluCylinder(getShipQ(),r1,r2,len,sl,1); gluDisk(getShipQ(),0,r1,sl,1);
    glTranslatef(0,0,len); gluDisk(getShipQ(),0,r2,sl,1); glPopMatrix();
}
static void shipSph(float x,float y,float z,float r,int sl) {
    glPushMatrix(); glTranslatef(x,y,z); glutSolidSphere(r,sl,sl); glPopMatrix();
}

#define S_HL       7.2f
#define S_HW       1.55f
#define S_HT       1.15f
#define S_HD       1.05f
#define S_BOW_Z    (S_HL-2.2f)
#define S_BASE     S_HT

static void ship_hullSide(float sx) {
    float bL = -S_HL, bOW = S_BOW_Z, bT = S_HL, w = sx * S_HW;
    glBegin(GL_QUADS);
      glNormal3f(sx,0,0);
      glVertex3f(w, S_HT, bL); glVertex3f(w, S_HT, bOW);
      glVertex3f(w,-S_HD, bOW); glVertex3f(w,-S_HD, bL);
    glEnd();
    glBegin(GL_TRIANGLES);
      glNormal3f(sx*0.70f, 0, 0.71f);
      glVertex3f(w, S_HT, bOW); glVertex3f(0, S_HT, bT); glVertex3f(w,-S_HD, bOW);
      glNormal3f(sx*0.70f, 0, 0.71f);
      glVertex3f(0, S_HT, bT); glVertex3f(0,-S_HD, bT); glVertex3f(w,-S_HD, bOW);
    glEnd();
}

static void ship_drawHull(void) {
    S_M_WHITE(); ship_hullSide( 1.0f); ship_hullSide(-1.0f);
    glBegin(GL_QUADS);
      glNormal3f(0,0,-1);
      glVertex3f(-S_HW, S_HT, -S_HL); glVertex3f( S_HW, S_HT, -S_HL);
      glVertex3f( S_HW,-S_HD, -S_HL); glVertex3f(-S_HW,-S_HD, -S_HL);
    glEnd();
    S_M_CREAM();
    glBegin(GL_QUADS);
      glNormal3f(0,1,0);
      glVertex3f(-S_HW, S_HT, -S_HL); glVertex3f( S_HW, S_HT, -S_HL);
      glVertex3f( S_HW, S_HT, S_BOW_Z); glVertex3f(-S_HW, S_HT, S_BOW_Z);
    glEnd();
    glBegin(GL_TRIANGLES);
      glNormal3f(0,1,0);
      glVertex3f(-S_HW, S_HT, S_BOW_Z); glVertex3f( S_HW, S_HT, S_BOW_Z); glVertex3f(0, S_HT, S_HL);
    glEnd();

    S_M_RED();
    float sy = -S_HD+0.22f, ey = -S_HD+0.52f, e = 0.004f;
    glBegin(GL_QUADS); glNormal3f(1,0,0); glVertex3f(S_HW+e, ey, -S_HL); glVertex3f(S_HW+e, ey, S_BOW_Z); glVertex3f(S_HW+e, sy, S_BOW_Z); glVertex3f(S_HW+e, sy, -S_HL); glEnd();
    glBegin(GL_TRIANGLES); glNormal3f(0.70f,0,0.71f); glVertex3f(S_HW+e, ey, S_BOW_Z); glVertex3f(e, ey, S_HL); glVertex3f(S_HW+e, sy, S_BOW_Z); glVertex3f(e, ey, S_HL); glVertex3f(e, sy, S_HL); glVertex3f(S_HW+e, sy, S_BOW_Z); glEnd();
    glBegin(GL_QUADS); glNormal3f(-1,0,0); glVertex3f(-S_HW-e, ey, -S_HL); glVertex3f(-S_HW-e, ey, S_BOW_Z); glVertex3f(-S_HW-e, sy, S_BOW_Z); glVertex3f(-S_HW-e, sy, -S_HL); glEnd();
    glBegin(GL_TRIANGLES); glNormal3f(-0.70f,0,0.71f); glVertex3f(-S_HW-e, ey, S_BOW_Z); glVertex3f(-e, ey, S_HL); glVertex3f(-S_HW-e, sy, S_BOW_Z); glVertex3f(-e, ey, S_HL); glVertex3f(-e, sy, S_HL); glVertex3f(-S_HW-e, sy, S_BOW_Z); glEnd();
    glBegin(GL_QUADS); glNormal3f(0,0,-1); glVertex3f(-S_HW, ey,-S_HL-e); glVertex3f(S_HW, ey,-S_HL-e); glVertex3f( S_HW, sy,-S_HL-e); glVertex3f(-S_HW, sy,-S_HL-e); glEnd();

    S_M_DARK();
    float ky = -S_HD+0.22f;
    glBegin(GL_QUADS); glNormal3f(1,0,0); glVertex3f(S_HW, ky, -S_HL); glVertex3f(S_HW, ky, S_BOW_Z); glVertex3f(S_HW,-S_HD, S_BOW_Z); glVertex3f(S_HW,-S_HD, -S_HL); glEnd();
    glBegin(GL_QUADS); glNormal3f(-1,0,0); glVertex3f(-S_HW, ky, -S_HL); glVertex3f(-S_HW, ky, S_BOW_Z); glVertex3f(-S_HW,-S_HD, S_BOW_Z); glVertex3f(-S_HW,-S_HD,-S_HL); glEnd();
    glBegin(GL_QUADS); glNormal3f(0,-1,0); glVertex3f(-S_HW,-S_HD,-S_HL); glVertex3f(S_HW,-S_HD,-S_HL); glVertex3f( S_HW,-S_HD, S_BOW_Z); glVertex3f(-S_HW,-S_HD, S_BOW_Z); glEnd();
    glBegin(GL_QUADS); glNormal3f(0,0,-1); glVertex3f(-S_HW, ky,-S_HL); glVertex3f(S_HW, ky,-S_HL); glVertex3f( S_HW,-S_HD,-S_HL); glVertex3f(-S_HW,-S_HD,-S_HL); glEnd();
    glBegin(GL_TRIANGLES); glNormal3f(0.70f,0,0.71f); glVertex3f(S_HW, ky, S_BOW_Z); glVertex3f(0, ky, S_HL); glVertex3f(S_HW,-S_HD, S_BOW_Z); glVertex3f(0, ky, S_HL); glVertex3f(0,-S_HD, S_HL); glVertex3f(S_HW,-S_HD, S_BOW_Z); glEnd();
    glBegin(GL_TRIANGLES); glNormal3f(-0.70f,0,0.71f); glVertex3f(-S_HW, ky, S_BOW_Z); glVertex3f(0, ky, S_HL); glVertex3f(-S_HW,-S_HD, S_BOW_Z); glVertex3f(0, ky, S_HL); glVertex3f(0,-S_HD, S_HL); glVertex3f(-S_HW,-S_HD, S_BOW_Z); glEnd();
    glBegin(GL_TRIANGLES); glNormal3f(0,-1,0); glVertex3f(-S_HW,-S_HD, S_BOW_Z); glVertex3f(S_HW,-S_HD, S_BOW_Z); glVertex3f(0,-S_HD, S_HL); glEnd();

    for(int i=0;i<11;i++){
        float pz = -5.8f + i*1.05f; if(pz > S_BOW_Z-0.5f) break;
        S_M_BLUE(); shipBox(S_HW+0.004f, 0.48f, pz, 0.006f, 0.15f, 0.17f); shipBox(S_HW+0.004f, 0.08f, pz, 0.006f, 0.15f, 0.17f);
        S_M_CHROME(); shipBox(S_HW+0.002f, 0.48f, pz, 0.004f, 0.21f, 0.23f); shipBox(S_HW+0.002f, 0.08f, pz, 0.004f, 0.21f, 0.23f);
        S_M_BLUE(); shipBox(-S_HW-0.004f, 0.48f, pz, 0.006f, 0.15f, 0.17f); shipBox(-S_HW-0.004f, 0.08f, pz, 0.006f, 0.15f, 0.17f);
        S_M_CHROME(); shipBox(-S_HW-0.002f, 0.48f, pz, 0.004f, 0.21f, 0.23f); shipBox(-S_HW-0.002f, 0.08f, pz, 0.004f, 0.21f, 0.23f);
    }
}

typedef struct { float z0, z1, hw, dh; } ShipDeckData;
static void ship_drawSuperstructure(void) {
    ShipDeckData decks[5] = {
        { -5.6f,  4.4f, 1.32f, 1.10f }, { -5.1f,  3.9f, 1.22f, 1.10f },
        { -4.6f,  3.4f, 1.12f, 1.10f }, { -4.1f,  2.9f, 1.02f, 1.10f }, { -3.6f,  2.4f, 0.92f, 1.10f },
    };
    float yBot = S_BASE;
    for(int i=0;i<5;i++){
        ShipDeckData *dk = &decks[i];
        float yMid = yBot + dk->dh * 0.5f, zMid = (dk->z0 + dk->z1) * 0.5f, zLen = dk->z1 - dk->z0;
        S_M_WHITE(); shipBox(0, yMid, zMid, dk->hw*2, dk->dh, zLen);
        S_M_CHROME(); float rail_y = yBot + dk->dh + 0.10f;
        shipBox(-dk->hw, rail_y, zMid, 0.035f, 0.22f, zLen); shipBox( dk->hw, rail_y, zMid, 0.035f, 0.22f, zLen);
        shipBox(0, rail_y, dk->z0, dk->hw*2, 0.22f, 0.035f); shipBox(0, rail_y, dk->z1, dk->hw*2, 0.22f, 0.035f);
        int nPost = (int)(zLen / 0.5f); float postStep = zLen / nPost;
        for(int p=0;p<=nPost;p++){ float pz = dk->z0 + p*postStep; shipCyl(-dk->hw, yBot+dk->dh, pz, 0.018f, 0.22f, 6); shipCyl( dk->hw, yBot+dk->dh, pz, 0.018f, 0.22f, 6); }
        int nWin = (int)((zLen - 0.3f) / 0.52f);
        for(int j=0;j<nWin;j++){
            float wz = dk->z0 + 0.38f + j*0.52f, wx = dk->hw + 0.006f;
            S_M_BLUE(); shipBox( wx, yMid+0.08f, wz, 0.008f, 0.25f, 0.28f); shipBox(-wx, yMid+0.08f, wz, 0.008f, 0.25f, 0.28f);
            S_M_CHROME(); shipBox( wx, yMid+0.08f, wz, 0.004f, 0.31f, 0.34f); shipBox(-wx, yMid+0.08f, wz, 0.004f, 0.31f, 0.34f);
        }
        S_M_WOOD(); shipBox(0, yBot+dk->dh+0.003f, zMid, dk->hw*2-0.12f, 0.02f, zLen-0.06f); yBot += dk->dh;
    }
    S_M_CREAM(); float bhy = yBot; shipBox(0, bhy+0.62f, 0.4f,  1.85f, 1.24f, 5.2f);
    shipBox( 1.30f, bhy+0.55f, 0.4f, 0.80f, 0.90f, 3.8f); shipBox(-1.30f, bhy+0.55f, 0.4f, 0.80f, 0.90f, 3.8f);
    float wy = bhy+0.70f;
    for(int k=0;k<5;k++){ float wz = -0.8f + k*0.5f; S_M_LTBLUE(); shipBox(0.928f+0.004f, wy, wz, 0.008f, 0.30f, 0.32f); shipBox(-0.928f-0.004f,wy, wz, 0.008f, 0.30f, 0.32f); }
    for(int k=0;k<5;k++){ float wx = -0.75f + k*0.38f; S_M_LTBLUE(); shipBox(wx, wy, 2.608f+0.004f, 0.32f, 0.30f, 0.008f); }
    yBot += 1.24f; S_M_WHITE(); float why = yBot; shipBox(0, why+0.50f, 0.8f, 1.65f, 1.0f, 3.6f); yBot += 1.0f;
    S_M_CHROME(); shipCyl(0.0f, yBot, 0.9f, 0.045f, 1.8f, 10); shipCyl(0.5f, yBot, 0.9f, 0.020f, 1.0f, 8); shipCyl(-0.5f,yBot, 0.9f, 0.020f, 1.0f, 8); shipBox(0, yBot+1.0f, 0.9f, 1.2f, 0.04f, 0.04f);
    S_M_DARK(); glPushMatrix(); glTranslatef(0, yBot+1.8f, 0.9f); glRotatef((float)(gTime * 45.0), 0,1,0); glScalef(0.80f, 0.06f, 0.38f); glutSolidSphere(1.0, 18, 8); glPopMatrix();
    S_M_CHROME(); shipCyl(0, yBot+1.6f, 0.9f, 0.025f, 0.22f, 8);
    float topY = S_BASE + 5*1.10f + 0.003f;
    S_M_CREAM(); shipBox(0, topY+0.09f, -1.0f, 1.10f, 0.20f, 2.20f); S_M_POOL(); shipBox(0, topY+0.12f, -1.0f, 0.85f, 0.12f, 1.85f);
    S_M_ORANGE(); shipBox( 0.35f, topY+0.22f, -0.4f, 0.15f, 0.06f, 0.45f); shipBox(-0.35f, topY+0.22f, -0.4f, 0.15f, 0.06f, 0.45f); shipBox( 0.35f, topY+0.22f, -1.6f, 0.15f, 0.06f, 0.45f); shipBox(-0.35f, topY+0.22f, -1.6f, 0.15f, 0.06f, 0.45f);
    S_M_BLUE(); shipBox(0, S_HT-0.12f, -1.0f, S_HW*2+0.04f, 0.18f, 6.0f);
}

static void ship_drawFunnels(void) {
    float fzArr[2] = { -0.8f, -2.6f }; float fy = S_BASE + 5*1.10f + 1.0f;
    for(int i=0;i<2;i++){
        float fz = fzArr[i];
        S_M_DARK(); shipTcyl(0, fy-0.15f, fz, 0.45f, 0.42f, 0.18f, 28);
        S_M_FUNNEL(); shipTcyl(0, fy, fz, 0.42f, 0.33f, 1.55f, 28);
        S_M_YELLOW(); glPushMatrix(); glTranslatef(0, fy+0.42f, fz); glRotatef(-90.0f,1,0,0); gluCylinder(getShipQ(), 0.44f, 0.44f, 0.14f, 28, 1); glPopMatrix();
        S_M_WHITE(); glPushMatrix(); glTranslatef(0, fy+0.34f, fz); glRotatef(-90.0f,1,0,0); gluCylinder(getShipQ(), 0.435f, 0.435f, 0.06f, 28, 1); glPopMatrix();
        S_M_BLACK(); shipTcyl(0, fy+1.55f, fz, 0.33f, 0.36f, 0.18f, 28);
    }
}

static void ship_drawLifeboats(void) {
    float ly = S_BASE + 1.10f + 0.32f, lx = S_HW + 0.52f;
    for(int i=0;i<4;i++){
        float lz = -3.2f + i*1.65f;
        S_M_CHROME(); shipCyl( S_HW+0.04f, S_BASE+1.10f, lz, 0.032f, 0.55f, 8); shipCyl(-S_HW-0.04f, S_BASE+1.10f, lz, 0.032f, 0.55f, 8);
        shipBox( lx, S_BASE+1.10f+0.55f, lz, 0.50f, 0.032f, 0.032f); shipBox(-lx, S_BASE+1.10f+0.55f, lz, 0.50f, 0.032f, 0.032f);
        shipCyl( lx, ly+0.22f, lz, 0.008f, 0.32f, 5); shipCyl(-lx, ly+0.22f, lz, 0.008f, 0.32f, 5);
        S_M_ORANGE(); glPushMatrix(); glTranslatef( lx, ly, lz); glScalef(0.40f, 0.22f, 0.75f); glutSolidSphere(1.0, 16, 10); glPopMatrix();
        glPushMatrix(); glTranslatef(-lx, ly, lz); glScalef(0.40f, 0.22f, 0.75f); glutSolidSphere(1.0, 16, 10); glPopMatrix();
        S_M_WHITE(); shipBox( lx, ly+0.20f, lz, 0.30f, 0.12f, 0.55f); shipBox(-lx, ly+0.20f, lz, 0.30f, 0.12f, 0.55f);
    }
}

static void ship_drawBow(void) {
    S_M_DARK(); shipBox(-0.45f, S_HT-0.04f, S_HL-0.6f, 0.14f, 0.10f, 0.22f); shipBox( 0.45f, S_HT-0.04f, S_HL-0.6f, 0.14f, 0.10f, 0.22f);
    S_M_CHROME(); shipBox(-0.65f, S_HT+0.05f, S_HL-0.3f, 0.10f, 0.08f, 0.14f); shipBox( 0.65f, S_HT+0.05f, S_HL-0.3f, 0.10f, 0.08f, 0.14f);
    shipCyl(0, S_HT+0.02f, S_HL-0.15f, 0.020f, 0.90f, 7); S_M_RED(); shipBox(0.12f, S_HT+0.80f, S_HL-0.15f, 0.22f, 0.12f, 0.003f);
    S_M_CHROME(); shipCyl(0, S_HT+0.02f, -S_HL+0.15f, 0.020f, 0.80f, 7); S_M_RED(); shipBox(0.10f, S_HT+0.68f, -S_HL+0.15f, 0.18f, 0.10f, 0.003f);
    S_M_DARK(); shipCyl(-0.45f, -S_HD+0.15f, -S_HL-0.05f, 0.08f, 0.38f, 12); shipCyl( 0.45f, -S_HD+0.15f, -S_HL-0.05f, 0.08f, 0.38f, 12);
    S_M_CHROME();
    for(int b=0;b<4;b++){
        float ang = b*90.0f + (float)(gTime*80.0);
        glPushMatrix(); glTranslatef(-0.45f, -S_HD+0.15f, -S_HL-0.42f); glRotatef(ang, 0,0,1); shipBox(0, 0.18f, 0, 0.06f, 0.28f, 0.04f); glPopMatrix();
        glPushMatrix(); glTranslatef( 0.45f, -S_HD+0.15f, -S_HL-0.42f); glRotatef(-ang,0,0,1); shipBox(0, 0.18f, 0, 0.06f, 0.28f, 0.04f); glPopMatrix();
    }
    S_M_DARK(); shipBox(0, -S_HD+0.30f, -S_HL-0.08f, 0.05f, 0.55f, 0.40f);
}
// =========================================================================

// cablePt: Computes cable coordinates. Algorithm: Cubic Bezier Curve.
void cablePt(float t, float*ox, float*oy, float*oz)
{
    float P[4][3]={
    {-9.5f, -0.2f, 0.0f}, // BD Side (Left)
    {-4.0f, -7.0f, 0.5f}, // Control point underwater
    { 5.0f, -7.0f, 0.5f}, // Control point underwater
    { 9.8f, -0.5f, 0.5f}  // SG Side (Right) - Exactly matching the underwater concrete wall!
};
    float u=1-t;
    float b0=u*u*u, b1=3*u*u*t, b2=3*u*t*t, b3=t*t*t;
    *ox=b0*P[0][0]+b1*P[1][0]+b2*P[2][0]+b3*P[3][0];
    *oy=b0*P[0][1]+b1*P[1][1]+b2*P[2][1]+b3*P[3][1];
    *oz=b0*P[0][2]+b1*P[1][2]+b2*P[2][2]+b3*P[3][2];
}

// spawnPkt: Initializes a new data packet for transmission.
void spawnPkt(int dir,float r,float g,float b){
    for(int i=0;i<MAX_PKT;i++) if(!pkts[i].active){
        pkts[i].active=1; pkts[i].dir=dir; pkts[i].t=dir?1.0f:0.0f;
        pkts[i].r=r; pkts[i].g=g; pkts[i].b=b; pkts[i].tlen=0;
        pktCnt++; totalSent++; return;
    }
}

void computeWaves(void){
    for(int i=0;i<=WAVE_RES;i++) {
        for(int j=0;j<=WAVE_RES;j++){

            float x = -10.0f + i*(20.0f/WAVE_RES);
            float z =  35.0f - j*(75.5f/WAVE_RES);

            // DEPTH FACTOR (shore vs deep sea)
            float depthFactor = (z + 40.5f) / 75.5f; // 0 = deep, 1 = near shore
            if(depthFactor < 0) depthFactor = 0;
            if(depthFactor > 1) depthFactor = 1;

            //  BIG SWELL (slow rolling ocean)
            float swell = 0.18f * sinf(0.25f*x + 0.35f*z + waveOff * 1.0f);

            //  CROSS SWELL (different direction → realism)
            float cross = 0.12f * cosf(0.4f*x - 0.2f*z + waveOff * 1.3f);

            //  MEDIUM WAVES (wind effect)
            float wind = 0.06f * sinf(1.2f*x + 0.9f*z + waveOff * 2.2f);

            //  SMALL RIPPLE (high frequency detail)
            float ripple = 0.025f * sinf(3.5f*x + 2.8f*z + waveOff * 3.5f);

            // CHAOTIC NOISE (important for realism)
            float noise = 0.02f * sinf(x*4.0f + z*3.0f + sinf(waveOff));

            //  SHORE EFFECT (waves become sharper near shore)
            float shoreBoost = powf(depthFactor, 2.0f);
            float shoreWave = shoreBoost * 0.12f * sinf(2.5f*z + waveOff * 2.5f);

            //  FINAL COMBINATION
            float h = swell + cross + wind + ripple + noise + shoreWave;

            //  Slight smoothing (important!)
            h *= (0.8f + 0.2f * depthFactor);

            wH[i][j] = h;
        }
    }
}

// drawCruiseShip: (Left-Right Movement)
void drawCruiseShip(float fixedZ, float speed, float scale, float timeOffset) {
    //boundary
    float minX = -8.5f, maxX = 8.5f;
    float dist = maxX - minX;

    //time
    float travelT = (dist / speed)+30.0f;
    float waitT = 13.0f;
    float halfT = travelT + waitT;
    float fullT = halfT * 2.0f;

    float t = fmodf(gTime + timeOffset, fullT);
    float currX = 0.0f;
    float rotY = 0.0f;

    if (t < travelT) {
        currX = minX + (t / travelT) * dist;
        rotY = 90.0f;
    }
    else if (t < halfT) {
        currX = maxX;
        float waitProg = (t - travelT) / waitT;
        float turn = waitProg * waitProg * (3.0f - 2.0f * waitProg);
        rotY = 90.0f + 180.0f * turn;
    }
    else if (t < halfT + travelT) {
        float leftT = t - halfT;
        currX = maxX - (leftT / travelT) * dist;
        rotY = -90.0f;
    }
    else {
        currX = minX;
        float waitProg = (t - (halfT + travelT)) / waitT;
        float turn = waitProg * waitProg * (3.0f - 2.0f * waitProg);
        rotY = -90.0f + 180.0f * turn;
    }

    //  Wave follow system
int i = (int)((currX + 10.0f) / 20.0f * WAVE_RES);
int j = (int)((35.0f - fixedZ) / 75.5f * WAVE_RES);

// clamp
if(i < 0) i = 0; if(i > WAVE_RES) i = WAVE_RES;
if(j < 0) j = 0; if(j > WAVE_RES) j = WAVE_RES;

// fractional position
float fx = ((currX + 10.0f) / 20.0f * WAVE_RES);
float fz = ((35.0f - fixedZ) / 75.5f * WAVE_RES);

int i0 = (int)fx;
int j0 = (int)fz;
int i1 = i0 + 1;
int j1 = j0 + 1;

// clamp
if(i1 > WAVE_RES) i1 = WAVE_RES;
if(j1 > WAVE_RES) j1 = WAVE_RES;

// interpolation weights
float tx = fx - i0;
float tz = fz - j0;

// bilinear interpolation
float h00 = wH[i0][j0];
float h10 = wH[i1][j0];
float h01 = wH[i0][j1];
float h11 = wH[i1][j1];

float waterY =
    (1-tx)*(1-tz)*h00 +
    tx*(1-tz)*h10 +
    (1-tx)*tz*h01 +
    tx*tz*h11;

// smooth floating
float bob = waterY + 0.05f;

// realistic tilt
float pitch = 2.0f * (wH[i][j] - wH[i][j+1]);
float roll  = 2.0f * (wH[i+1][j] - wH[i][j]);

    glPushMatrix();
    glTranslatef(currX, bob, fixedZ);

    float finalScale = scale * 0.45f;
    glScalef(finalScale, finalScale, finalScale);

    glRotatef(rotY, 0, 1, 0);
    glRotatef(pitch, 1, 0, 0);
    glRotatef(roll, 0, 0, 1);

    ship_drawHull();
    ship_drawSuperstructure();
    ship_drawFunnels();
    ship_drawLifeboats();
    ship_drawBow();

    glPopMatrix();
}

//  REWRITTEN SEAMLESS SEA FLOOR WITH DETAILED PROPS
void drawSeaFloor(void){
    initSeaFloor();

    int NX=40, NZ=50;
    glDisable(GL_LIGHTING);

    float skyR = isDay ? 0.7f : 0.05f;
    float skyG = isDay ? 0.85f : 0.05f;
    float skyB = isDay ? 1.0f : 0.2f;

    // 1. Draw Sand Terrain
    for(int i=0;i<NX-1;i++){
        glBegin(GL_QUAD_STRIP);
        for(int j=0;j<=NZ;j++) for(int k=0;k<2;k++){

            float x=-10.0f+(i+k)*(20.0f/(NX-1));
            float z= 35.0f - j*(75.5f/NZ);

            float finalY = getSeaFloorY(x, z);

            float base = isDay ? 0.65f : 0.25f;
            float ripple = 0.12f * sinf(x*3.0f + z*2.5f) + 0.05f * cosf(x*6.0f);
            float noise = 0.04f * sinf(x*5 + z*3);

            float r = (0.75f + ripple*0.5f + noise) * base;
            float g = (0.65f + ripple*0.4f + noise) * base;
            float b = (0.45f + ripple*0.3f + noise) * base;

            if (z < -5.0f) {
                float p = (-5.0f - z) / 35.5f;
                if(p>1.0f) p=1.0f;
                p = p*p*(3.0f-2.0f*p);

                r = r*(1-p) + skyR*p;
                g = g*(1-p) + skyG*p;
                b = b*(1-p) + skyB*p;
            }

            glColor3f(r,g,b);
            glVertex3f(x, finalY, z);
        }
        glEnd();
    }

    glEnable(GL_LIGHTING);

    // 2.  Setup Global Fog for Underwater Elements (Matches Sand fading)
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, -5.0f);
    glFogf(GL_FOG_END, -40.5f);
    GLfloat fogColor[] = {skyR, skyG, skyB, 1.0f};
    glFogfv(GL_FOG_COLOR, fogColor);

    // --- DRAW STONES ---
    for(int i=0;i<MAX_STONE;i++){
        float y = getSeaFloorY(sf_sx[i], sf_sz[i]);
        float shade = isDay ? 0.8f : 0.3f;
        mat(0.4f*shade,0.35f*shade,0.3f*shade,0.2f,0.2f);
        glPushMatrix();
        glTranslatef(sf_sx[i], y, sf_sz[i]);
        glScalef(1.0f, 0.6f, 1.0f);
        glutSolidSphere(sf_ss[i], 10, 10);
        glPopMatrix();
    }

    // --- DRAW CORAL REEFS ---
    for(int i=0; i<MAX_CORAL; i++){
        float y = getSeaFloorY(sf_cx[i], sf_cz[i]);
        drawCoral(sf_cx[i], y, sf_cz[i], sf_cs[i]);
    }

    // --- DRAW TRASH/DEBRIS ---
    for(int i=0; i<MAX_TRASH; i++){
        float y = getSeaFloorY(sf_tx[i], sf_tz[i]);
        drawTrash(sf_tx[i], y, sf_tz[i], sf_tt[i]);
    }

    // --- DRAW SHIPWRECKS ---
    for(int i=0; i<MAX_WRECK; i++){
        float y = getSeaFloorY(sf_wx[i], sf_wz[i]);
        drawShipwreck(sf_wx[i], y, sf_wz[i], sf_wr[i]);
    }

    // --- DRAW KELP/SEA PLANTS ---
    for(int i=0; i<MAX_KELP; i++){
        float y = getSeaFloorY(sf_kx[i], sf_kz[i]);
        drawKelpPlant(sf_kx[i], y, sf_kz[i]);
    }

    glDisable(GL_FOG); // End Underwater Fog
}

// drawOceanSurface: Draws ONE seamless, realistic colored ocean mesh
void drawOceanSurface(void){
    computeWaves();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_LIGHTING);
    float dc = isDay ? 1.0f : 0.25f;

    for(int i=0;i<WAVE_RES;i++){
        glBegin(GL_QUAD_STRIP);
        for(int j=0;j<=WAVE_RES;j++) for(int k=0;k<2;k++){
            float x=-10.0f+(i+k)*(20.0f/WAVE_RES);
            float z= 35.0f - j*(75.5f/WAVE_RES);
            float h=wH[i+k][j];

            //  REALISTIC WATER COLORING (Height-based shading)
            // Troughs are dark deep blue, crests are lighter cyan
            float heightFactor = (h + 0.3f) / 0.6f; // Normalize roughly to 0-1 range
            if(heightFactor < 0.0f) heightFactor = 0.0f;
            if(heightFactor > 1.0f) heightFactor = 1.0f;

            float r = (0.01f + 0.15f * heightFactor) * dc;
            float g = (0.15f + 0.35f * heightFactor) * dc;
            float b = (0.45f + 0.45f * heightFactor) * dc;

            // NATURAL FOAM on the highest peaks
            if (h > 0.18f) {
                float foam = (h - 0.18f) * 4.0f; // Scale foam intensity
                if(foam > 1.0f) foam = 1.0f;
                r += foam * 0.5f * dc;
                g += foam * 0.5f * dc;
                b += foam * 0.3f * dc;
            }

            float alpha = 0.85f; // Deep ocean opacity

            // Smoothstep fade at the distant horizon (-5.0 to -40.5) to hide edges
            if (z < -5.0f) {
                float p = (-5.0f - z) / 35.5f;
                if (p > 1.0f) p = 1.0f;
                p = p * p * (3.0f - 2.0f * p);
                alpha = 0.85f * (1.0f - p);
            }

            glColor4f(r, g, b, alpha);
            glVertex3f(x, h, z);
        }
        glEnd();
    }

    //  SUN GLINTS (Sparkles on the water surface, removed chunky fake spheres)
    if(isDay){
        float sx=-12.0f, sz=-16.0f; // Mock Sun direction
        for(int i=0;i<WAVE_RES;i+=2) for(int j=0;j<WAVE_RES;j+=2){
            float x=-10+i*(20.f/WAVE_RES), z = 35.0f - j*(75.5f/WAVE_RES);
            float dx=x-sx, dz=z-sz, dist=sqrtf(dx*dx+dz*dz);

            float glint = 0.0f;
            if (dist < 35.0f) {
                // Powf makes the sparkles sharp and intermittent like real water
                glint = 0.15f * (1.0f - dist/35.0f) * powf((0.5f + 0.5f*sinf(gTime*4.0f + i*1.2f + j*0.8f)), 4.0f);
            }

            if(glint > 0.01f){
                if (z < -5.0f) {
                    float p = (-5.0f - z) / 35.5f;
                    if (p > 1.0f) p = 1.0f;
                    glint *= (1.0f - p);
                }
                if (glint > 0.01f) {
                    glColor4f(1.0f, 1.0f, 0.9f, glint);
                    glPushMatrix();
                    glTranslatef(x, wH[i][j]+0.02f, z);
                    glScalef(1.0f, 0.1f, 1.0f); // Flatten the spark so it glides on the surface
                    glutSolidSphere(0.04f, 4, 4);
                    glPopMatrix();
                }
            }
        }
    }

    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
}

// drawCoastalSlopes: Continuous seamless mesh
void drawCoastalSlopes(void){
    glDisable(GL_LIGHTING);
    float dc=isDay?1:.28f;
    int NS=40;
    float skyR = isDay ? 0.7f : 0.05f;
    float skyG = isDay ? 0.85f : 0.05f;
    float skyB = isDay ? 1.0f : 0.2f;

    for(int i=0;i<20;i++){
        glBegin(GL_QUAD_STRIP);
        for(int j=0;j<=NS;j++){
            float z = 35.0f - j*(75.5f/NS);
            for(int k=0;k<2;k++){
                float x=-9.5f+(i+k)*(1.5f/20.0f), t=(x+9.5f)/1.5f, y=-7.8f+7.8f*t, bt=(.55f+.45f*t)*dc;
                float r = .58f*bt, g = .48f*bt, b = .34f*bt;
                if (z < -5.0f) {
                    float p = (-5.0f - z) / 35.5f; if(p>1.0f) p=1.0f;
                    p = p * p * (3.0f - 2.0f * p);
                    r = r*(1-p) + skyR*p; g = g*(1-p) + skyG*p; b = b*(1-p) + skyB*p;
                }
                glColor3f(r, g, b); glVertex3f(x,y,z);
            }
        }
        glEnd();
    }
    for(int i=0;i<20;i++){
        glBegin(GL_QUAD_STRIP);
        for(int j=0;j<=NS;j++){
            float z = 35.0f - j*(75.5f/NS);
            for(int k=0;k<2;k++){
                float x=8.0f+(i+k)*(1.5f/20.0f), t=1.0f-(x-8.0f)/1.5f, y=-7.8f+7.8f*t, bt=(.55f+.45f*(1-t))*dc;
                float r = .58f*bt, g = .48f*bt, b = .34f*bt;
                if (z < -5.0f) {
                    float p = (-5.0f - z) / 35.5f; if(p>1.0f) p=1.0f;
                    p = p * p * (3.0f - 2.0f * p);
                    r = r*(1-p) + skyR*p; g = g*(1-p) + skyG*p; b = b*(1-p) + skyB*p;
                }
                glColor3f(r, g, b); glVertex3f(x,y,z);
            }
        }
        glEnd();
    }
    glEnable(GL_LIGHTING);
}

void drawFish(void){
    for(int i=0;i<MAX_FISH;i++){
        float brt = isDay ? 1.0f : 0.45f;
        float bob = 0.07f * sinf(gTime * 2.2f + fish[i].phase);
        float wag = 0.04f * sinf(gTime * 6.0f + fish[i].phase);
        float wagAngle = wag * 250.0f;

        glPushMatrix();
        glTranslatef(fish[i].x, fish[i].y + bob, fish[i].z);
        glRotatef(fish[i].dir == 1 ? 0 : 180, 0, 1, 0);
        glRotatef(wagAngle * 0.4f, 0, 1, 0);

        float fr = fish[i].r * brt, fg = fish[i].g * brt, fb = fish[i].b * brt;

        mat(fr, fg, fb, 0.8f, 0.9f);
        glPushMatrix(); glScalef(.25f, .12f, .05f); glutSolidSphere(1, 16, 12); glPopMatrix();

        mat(fr*1.2f, fg*1.2f, fb*1.2f, 0.8f, 0.9f);
        glPushMatrix(); glTranslatef(0.0f, -0.04f, 0.0f); glScalef(.23f, .09f, .055f); glutSolidSphere(1, 16, 12); glPopMatrix();

        mat(fr*0.8f, fg*0.8f, fb*0.8f, 0.5f, 0.5f);
        glPushMatrix(); glTranslatef(-0.05f, 0.10f, 0.0f); glRotatef(-30, 0, 0, 1); glScalef(.12f, .08f, .01f); glutSolidSphere(1, 10, 8); glPopMatrix();
        glPushMatrix(); glTranslatef(0.08f, -0.05f, 0.05f); glRotatef(-30, 0, 1, 0); glRotatef(20, 1, 0, 0); glScalef(.08f, .02f, .05f); glutSolidSphere(1, 10, 8); glPopMatrix();
        glPushMatrix(); glTranslatef(0.08f, -0.05f, -0.05f); glRotatef(30, 0, 1, 0); glRotatef(-20, 1, 0, 0); glScalef(.08f, .02f, .05f); glutSolidSphere(1, 10, 8); glPopMatrix();

        glPushMatrix();
        glTranslatef(-0.24f, 0.0f, 0.0f);
        glRotatef(wagAngle * 1.5f, 0, 1, 0);
        glPushMatrix(); glTranslatef(-0.02f, 0.06f, 0.0f); glRotatef(-35, 0, 0, 1); glScalef(.08f, .12f, .01f); glutSolidSphere(1, 10, 8); glPopMatrix();
        glPushMatrix(); glTranslatef(-0.02f, -0.06f, 0.0f); glRotatef(35, 0, 0, 1); glScalef(.08f, .12f, .01f); glutSolidSphere(1, 10, 8); glPopMatrix();
        glPopMatrix();

        mat(0.0f, 0.0f, 0.0f, 0.8f, 0.8f);
        glPushMatrix(); glTranslatef(0.18f, 0.02f, 0.045f); glutSolidSphere(0.012f, 6, 6); glPopMatrix();
        glPushMatrix(); glTranslatef(0.18f, 0.02f, -0.045f); glutSolidSphere(0.012f, 6, 6); glPopMatrix();

        glPopMatrix();
    }
}

void drawSharks(void){
    for(int i=0;i<MAX_SHARK;i++){
        float brt = isDay ? 1.0f : 0.4f;
        float bob = 0.05f * sinf(gTime * 1.5f + sharks[i].phase);
        float wag = 0.06f * sinf(gTime * 2.5f + sharks[i].phase);
        float wagAngle = wag * 200.0f;

        glPushMatrix();
        glTranslatef(sharks[i].x, sharks[i].y + bob, sharks[i].z);
        glRotatef(sharks[i].dir == 1 ? 0 : 180, 0, 1, 0);
        glRotatef(wagAngle * 0.3f, 0, 1, 0);

        mat(.20f*brt, .24f*brt, .30f*brt, .6f, .5f);
        glPushMatrix(); glScalef(.60f, .16f, .14f); glutSolidSphere(1, 24, 16); glPopMatrix();

        mat(.85f*brt, .85f*brt, .88f*brt, .3f, .2f);
        glPushMatrix(); glTranslatef(0.05f, -0.05f, 0.0f); glScalef(.52f, .12f, .13f); glutSolidSphere(1, 24, 16); glPopMatrix();

        mat(.20f*brt, .24f*brt, .30f*brt, .5f, .4f);
        glPushMatrix(); glTranslatef(-0.05f, 0.14f, 0.0f); glRotatef(-35, 0, 0, 1); glScalef(.18f, .22f, .02f); glutSolidSphere(1, 16, 10); glPopMatrix();

        glPushMatrix(); glTranslatef(0.18f, -0.08f, 0.11f); glRotatef(-40, 0, 1, 0); glRotatef(20, 1, 0, 0); glScalef(.22f, .03f, .08f); glutSolidSphere(1, 16, 10); glPopMatrix();
        glPushMatrix(); glTranslatef(0.18f, -0.08f, -0.11f); glRotatef(40, 0, 1, 0); glRotatef(-20, 1, 0, 0); glScalef(.22f, .03f, .08f); glutSolidSphere(1, 16, 10); glPopMatrix();

        glPushMatrix(); glTranslatef(-0.25f, -0.12f, 0.05f); glRotatef(-30, 0, 1, 0); glScalef(.08f, .02f, .04f); glutSolidSphere(1, 10, 8); glPopMatrix();
        glPushMatrix(); glTranslatef(-0.25f, -0.12f, -0.05f); glRotatef(30, 0, 1, 0); glScalef(.08f, .02f, .04f); glutSolidSphere(1, 10, 8); glPopMatrix();

        glPushMatrix();
        glTranslatef(-0.55f, 0.0f, 0.0f);
        glRotatef(wagAngle * 1.2f, 0, 1, 0);
        mat(.20f*brt, .24f*brt, .30f*brt, .5f, .4f);
        glPushMatrix(); glTranslatef(-0.05f, 0.14f, 0.0f); glRotatef(-45, 0, 0, 1); glScalef(.16f, .24f, .02f); glutSolidSphere(1, 12, 8); glPopMatrix();
        mat(.60f*brt, .60f*brt, .65f*brt, .3f, .2f);
        glPushMatrix(); glTranslatef(-0.05f, -0.10f, 0.0f); glRotatef(45, 0, 0, 1); glScalef(.12f, .18f, .02f); glutSolidSphere(1, 12, 8); glPopMatrix();
        glPopMatrix();

        mat(0.0f, 0.0f, 0.0f, 0.9f, 0.9f);
        glPushMatrix(); glTranslatef(0.45f, 0.05f, 0.08f); glutSolidSphere(0.015f, 8, 8); glPopMatrix();
        glPushMatrix(); glTranslatef(0.45f, 0.05f, -0.08f); glutSolidSphere(0.015f, 8, 8); glPopMatrix();

        glPopMatrix();
    }
}

void drawBubbles(void){
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); glDisable(GL_LIGHTING);
    for(int i=0;i<MAX_BUBBLE;i++){
        float a=.35f*(1-bbl[i].life), brt=isDay?1:.45f; glColor4f(.65f*brt,.82f*brt,1*brt,a);
        glPushMatrix(); glTranslatef(bbl[i].x,bbl[i].y,bbl[i].z); glutSolidSphere(bbl[i].sz,6,4); glPopMatrix();
    }
    glEnable(GL_LIGHTING); glDisable(GL_BLEND);
}

void drawSpark(float x, float y, float z) {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    int sparks = 10 + rand()%8;

    for(int i=0; i<sparks; i++) {
        float angle = (float)i / sparks * 6.283f;
        float len = 0.1f + (rand()%100)/600.0f;

        float dx = cosf(angle) * len;
        float dy = ((rand()%100)/100.0f - 0.5f) * 0.25f;
        float dz = sinf(angle) * len;

        glColor4f(1.0f, 0.9f, 0.4f, 0.95f);
        glBegin(GL_LINES);
        glVertex3f(x, y, z);
        glVertex3f(x+dx, y+dy, z+dz);
        glEnd();

        glColor4f(1.0f, 0.4f, 0.1f, 0.8f);
        glPushMatrix();
        glTranslatef(x+dx, y+dy, z+dz);
        glutSolidSphere(0.025f, 6, 6);
        glPopMatrix();
    }

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

// REALISTIC CABLE BREAK ANIMATION (Triggers when 'B' is pressed!)
void drawCable(void){
    int seg = CABLE_SEG;
    float px,py,pz;
    cablePt(0,&px,&py,&pz);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(2.0f);

    for(int i=1;i<=seg;i++){
        float t=(float)i/seg, cx,cy,cz;
        cablePt(t,&cx,&cy,&cz);

        int bLeft = (int)(seg * 0.46f);
        int bRight = (int)(seg * 0.54f);

        if (!cableOK && i > bLeft && i <= bRight) {
            if (i == bRight) {
                float colors[6][3] = {{0.9f, 0.5f, 0.1f}, {0.1f, 0.8f, 0.9f}, {0.8f, 0.1f, 0.9f},
                                      {0.9f, 0.9f, 0.9f}, {0.9f, 0.1f, 0.1f}, {0.1f, 0.9f, 0.3f}};
                float fLenR[6] = {-.4f, -.45f, -.47f, -.48f, -.42f, -.43f};

                for(int f=0; f<6; f++) {
                    float angle = f * (6.283f / 6.0f);
                    float sy = cy + 0.08f * cosf(angle);
                    float sz = cz + 0.08f * sinf(angle);

                    float ex = cx - fLenR[f];

                    mat(colors[f][0], colors[f][1], colors[f][2], 0.6f, 0.6f);
                    cyl(cx, sy, sz, ex, sy, sz, 0.015f, 6);

                    glDisable(GL_LIGHTING);
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                    glDepthMask(GL_FALSE);

                    glColor4f(colors[f][0], colors[f][1], colors[f][2], 0.9f);
                    glPushMatrix();
                    glTranslatef(ex - .85f, sy, sz);
                    glutSolidSphere(0.06f, 10, 10);
                    glPopMatrix();

                    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                    glPushMatrix();
                    glTranslatef(ex - .85f, sy, sz);
                    glutSolidSphere(0.03f, 8, 8);
                    glPopMatrix();

                    glDepthMask(GL_TRUE);
                    glDisable(GL_BLEND);
                    glEnable(GL_LIGHTING);
                }

                mat(0.12f, 0.12f, 0.15f, 0.2f, 0.2f);
                for(int r=0; r<8; r++) {
                    float angle = r * (6.283f / 8.0f);
                    glPushMatrix();
                    glTranslatef(cx, cy + 0.16f*cosf(angle), cz + 0.16f*sinf(angle));
                    glRotatef(-90, 0, 1, 0);
                    glutSolidCone(0.05f, 0.2f, 4, 1);
                    glPopMatrix();
                }
                if(rand()%2==0)
                drawSpark(cx - 0.37f, cy, cz);
            }
        }
        else {
            mat(0.15f, 0.15f, 0.18f, 0.9f, 0.8f);
            cyl(px,py,pz,cx,cy,cz,0.16f,12);
            sph(cx,cy,cz,0.16f,8);

            int repInterval = seg / 8;
            if(i % repInterval == 0 && i != seg) {
                mat(0.8f, 0.6f, 0.0f, 0.8f, 0.6f);
                cyl(px,py,pz,cx,cy,cz, 0.26f, 16);

                if(cableOK) matE(0.0f, 0.8f, 1.0f, 0.0f, 0.6f, 1.0f);
                else matE(1.0f, 0.0f, 0.0f, 0.8f, 0.0f, 0.0f);

                cyl(px+(cx-px)*0.4f, py+(cy-py)*0.4f, pz+(cz-pz)*0.4f,
                    px+(cx-px)*0.6f, py+(cy-py)*0.6f, pz+(cz-pz)*0.6f, 0.27f, 16);
                noE();
            }

            if (!cableOK && i == bLeft) {
                float colors[6][3] = {{0.9f, 0.1f, 0.1f}, {0.1f, 0.4f, 0.9f}, {0.1f, 0.9f, 0.3f},
                                      {0.9f, 0.9f, 0.1f}, {0.9f, 0.5f, 0.1f}, {0.9f, 0.9f, 0.9f}};
                float fLenL[6] = {-.4f, -.45f, -.47f, -.48f, -.42f, -.43f};

                for(int f=0; f<6; f++) {
                    float angle = f * (6.283f / 6.0f);
                    float sy = cy + 0.08f * cosf(angle);
                    float sz = cz + 0.08f * sinf(angle);

                    float ex = cx + fLenL[f];

                    mat(colors[f][0], colors[f][1], colors[f][2], 0.6f, 0.6f);
                    cyl(cx, sy, sz, ex, sy, sz, 0.015f, 6);

                    glDisable(GL_LIGHTING);
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                    glDepthMask(GL_FALSE);

                    glColor4f(colors[f][0], colors[f][1], colors[f][2], 0.9f);
                    glPushMatrix();
                    glTranslatef(ex + .85f, sy, sz);
                    glutSolidSphere(0.06f, 10, 10);
                    glPopMatrix();

                    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                    glPushMatrix();
                    glTranslatef(ex + .85f, sy, sz);
                    glutSolidSphere(0.03f, 8, 8);
                    glPopMatrix();

                    glDepthMask(GL_TRUE);
                    glDisable(GL_BLEND);
                    glEnable(GL_LIGHTING);
                }

                mat(0.12f, 0.12f, 0.15f, 0.2f, 0.2f);
                for(int r=0; r<8; r++) {
                    float angle = r * (6.283f / 8.0f);
                    glPushMatrix();
                    glTranslatef(cx, cy + 0.16f*cosf(angle), cz + 0.16f*sinf(angle));
                    glRotatef(90, 0, 1, 0);
                    glutSolidCone(0.05f, 0.2f, 4, 1);
                    glPopMatrix();
                }
                if(rand()%2==0)
                drawSpark(cx + 0.37f, cy, cz);
            }
        }
        px=cx; py=cy; pz=cz;
    }

    float ex,ey,ez;
    mat(.2f,.2f,.2f,.8f,.8f);
    cablePt(0,&ex,&ey,&ez); sph(ex,ey,ez,.25f,16);
    cablePt(1,&ex,&ey,&ez); sph(ex,ey,ez,.25f,16);
    glLineWidth(1.0f);
}

void drawPackets(void){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    for(int i=0;i<MAX_PKT;i++){
        if(!pkts[i].active) continue;

        if(pkts[i].t < 0.02f || pkts[i].t > 0.98f) continue;

        float px,py,pz; cablePt(pkts[i].t,&px,&py,&pz);

        if(!cableOK && pkts[i].t>.44f && pkts[i].t<.56f){ pkts[i].active=0; pktCnt--; continue; }

        float pulse=.75f+.25f*sinf(gTime*12+pkts[i].t*15);

        glColor4f(pkts[i].r, pkts[i].g, pkts[i].b, 0.8f * pulse);
        glPushMatrix(); glTranslatef(px,py,pz); glutSolidSphere(.22f, 10, 10); glPopMatrix();

        glColor4f(1.0f, 1.0f, 1.0f, 0.95f);
        glPushMatrix(); glTranslatef(px,py,pz); glutSolidSphere(.14f, 8, 8); glPopMatrix();

        for(int tr=0;tr<pkts[i].tlen;tr++){
            float a=.6f*(1-(float)tr/TRAIL_LEN);
            float s=.18f*(1-(float)tr/TRAIL_LEN);
            glColor4f(pkts[i].r, pkts[i].g, pkts[i].b, a);
            glPushMatrix(); glTranslatef(pkts[i].tx[tr],pkts[i].ty[tr],pkts[i].tz[tr]); glutSolidSphere(s,8,8); glPopMatrix();
        }
    }
    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
}

// drawOcean: FINAL SEAMLESS ASSEMBLY
void drawOcean(void){
    float dc=isDay?1:.28f;
    float skyR = isDay ? 0.7f : 0.05f;
    float skyG = isDay ? 0.85f : 0.05f;
    float skyB = isDay ? 1.0f : 0.2f;

    glDisable(GL_LIGHTING);
    float wallR = 0.0f, wallG = 0.15f*dc, wallB = 0.35f*dc;

    glBegin(GL_QUADS);
    // Bottom Wall
    glColor3f(skyR, skyG, skyB);
    glVertex3f( 10.0f, -9.0f, -40.5f);
    glVertex3f(-10.0f, -9.0f, -40.5f);
    glColor3f(wallR, wallG, wallB);
    glVertex3f(-10.0f, -9.0f,  35.0f);
    glVertex3f( 10.0f, -9.0f,  35.0f);

    // Left Wall
    glColor3f(skyR, skyG, skyB);
    glVertex3f(-10.0f,  0.5f, -40.5f);
    glVertex3f(-10.0f, -9.0f, -40.5f);
    glColor3f(wallR, wallG, wallB);
    glVertex3f(-10.0f, -9.0f,  35.0f);
    glVertex3f(-10.0f,  0.5f,  35.0f);

    // Right Wall
    glColor3f(skyR, skyG, skyB);
    glVertex3f( 10.0f, -9.0f, -40.5f);
    glVertex3f( 10.0f,  0.5f, -40.5f);
    glColor3f(wallR, wallG, wallB);
    glVertex3f( 10.0f,  0.5f,  35.0f);
    glVertex3f( 10.0f, -9.0f,  35.0f);
    glEnd();

    glEnable(GL_LIGHTING);

    drawCoastalSlopes();
    drawSeaFloor(); // Now heavily upgraded!

    drawCable();
    drawPackets();

    drawFish();
    drawSharks();
    drawBubbles();

    //  CRUISE SHIPS
    drawCruiseShip(  2.0f, 2.2f, 0.35f, 15.0f);
    drawCruiseShip(-14.0f, 1.5f, 0.38f, 30.0f);
    drawCruiseShip(-28.0f, 2.0f, 0.32f, 45.0f);

    // Night stars
    if(!isDay){
        glDisable(GL_LIGHTING); glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE); glPointSize(2.5f);
        glBegin(GL_POINTS);
        unsigned r=2468135; for(int i=0;i<250;i++){
            r=r*1664525+1013904223; float bx=(float)(r&0xFFFF)/65535*20-10; r=r*1664525+1013904223;
            float by=-(float)(r&0xFFFF)/65535*6-2; r=r*1664525+1013904223;
            float bz=(float)(r&0xFFFF)/65535*75.5f-40.5f;
            float flash=.5f+.5f*sinf(gTime*3+i*.7f); glColor4f(0,.7f*flash,.9f*flash,.5f*flash); glVertex3f(bx,by,bz);
        }
        glEnd(); glPointSize(1); glDisable(GL_BLEND); glEnable(GL_LIGHTING);
    }

    drawOceanSurface();
}
