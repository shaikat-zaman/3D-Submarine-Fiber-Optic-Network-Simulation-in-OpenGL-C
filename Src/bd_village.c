#include "common.h"

// drawLineBresenham: Draws house antenna lines. Algorithm: Bresenham's Line.
void drawLineBresenham(float x1, float y1, float z1, float x2, float y2, float z2) {
    float density = 40.0f;
    int ix1 = (int)(x1 * density), iy1 = (int)(y1 * density);
    int ix2 = (int)(x2 * density), iy2 = (int)(y2 * density);

    int dx = abs(ix2 - ix1), dy = abs(iy2 - iy1);
    int sx = ix1 < ix2 ? 1 : -1;
    int sy = iy1 < iy2 ? 1 : -1;
    int err = dx - dy;

    glBegin(GL_POINTS);
    while (1) {
        glVertex3f((float)ix1 / density, (float)iy1 / density, z1);
        if (ix1 == ix2 && iy1 == iy2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; ix1 += sx; }
        if (e2 < dx) { err += dx; iy1 += sy; }
    }
    glEnd();
}

// drawBDTerrain: Draws the main ground, the vanishing extension, and the right-side platform with distant light fog.
void drawBDTerrain(void){
    int NX=40,NZ=30;
    float x0=-24.5f,x1=-8.5f,z0=-18.0f,z1=35.0f; // x0 -24.5 is far left edge
    glDisable(GL_LIGHTING);

    // Sky colors for blending at the edge (Fog Color)
    float skyR = isDay ? 0.7f : 0.05f;
    float skyG = isDay ? 0.85f : 0.05f;
    float skyB = isDay ? 1.0f : 0.2f;

    // 1. MAIN TERRAIN
    for(int i=0;i<NX-1;i++){
        glBegin(GL_QUAD_STRIP);
        for(int j=0;j<NZ;j++) for(int k=0;k<2;k++){
            float fx=x0+(i+k)*(x1-x0)/(NX-1);
            float fz=z0+j*(z1-z0)/(NZ-1);

            float hy=.35f*sinf(fx*.42f+.3f)*cosf(fz*.37f)
                     +.18f*sinf(fx*1.1f+fz*.8f+1)
                     +.08f*cosf(fx*2-fz*1.3f);
            if(hy<0) hy=0;

            float rdist=0;
            if(fz > -0.8f && fz < 1.2f) rdist = 0.0f;
            else if(fz <= -0.8f) rdist = fabsf(fz - -0.8f);
            else rdist = fabsf(fz - 1.2f);
            float rb = rdist / 0.8f; if(rb > 1) rb = 1;

            float ddx=fx-(-9.5f), ddz=fz-3.5f;
            float dc=sqrtf(ddx*ddx+ddz*ddz);
            float db=(dc<3.5f)?0:(dc-3.5f)/1.0f; if(db>1) db=1;

            float dtX = fx - (-20.5f);
            float dtZ = fz - (-6.0f);
            float dTow = sqrtf(dtX*dtX + dtZ*dtZ);
            float bTow = (dTow < 2.0f) ? 0.0f : (dTow - 2.0f) / 0.8f;
            if(bTow > 1.0f) bTow = 1.0f;

            float finalBlend = (rb < db) ? rb : db;
            finalBlend = (finalBlend < bTow) ? finalBlend : bTow;
            hy *= finalBlend;

            float brt=isDay?1:.28f, gr=.48f+hy*.4f, re=.08f+hy*.12f;
            float r = re*brt;
            float g = gr*brt;
            float b = .08f*brt;

            // Left edge fade
            if (fx < -22.0f) {
                float fadeStart = -22.0f;
                float fadeWidth = 3.0f;
                float edgeBlend = (fadeStart - fx) / fadeWidth;
                if(edgeBlend > 1.0f) edgeBlend = 1.0f;

                edgeBlend = edgeBlend * edgeBlend * (3 - 2 * edgeBlend);

                r = r * (1.0f - edgeBlend) + skyR * edgeBlend;
                g = g * (1.0f - edgeBlend) + skyG * edgeBlend;
                b = b * (1.0f - edgeBlend) + skyB * edgeBlend;
            }

            glColor3f(r, g, b);
            glVertex3f(fx,hy,fz);
        }
        glEnd();
    }

    // 2. VANISHING EXTENSION (Deep Z distance)
    glBegin(GL_QUAD_STRIP);
    for(int st = 0; st <= 21; st++) {
        float p = st / 20.0f;
        float curZ = -7.0f - (30.5f * p);

        float curX0 = -24.5f * (1.0f - p) + (-13.0f) * p;
        float curX1 = -10.5f * (1.0f - p) + (-9.3f) * p;

        float brt = isDay ? 1 : .28f;
        float r = 0.08f * brt;
        float g = 0.48f * brt;
        float b = 0.08f * brt;

        float colorNoise = 0.05f * sinf(p * 8.0f);
        g += colorNoise;

        //   FOG:
        float fogFade = 0.0f;
        if (p > 0.9f) {
            fogFade = (p - 0.7f) / 0.35f; // Starts at 70% distance
            if (fogFade > 1.0f) fogFade = 1.0f;
            fogFade *= 0.8f; // Maximum 80% opacity, so it stays subtle
        }

        r = r * (1.0f - fogFade) + skyR * fogFade;
        g = g * (1.0f - fogFade) + skyG * fogFade;
        b = b * (1.0f - fogFade) + skyB * fogFade;

        glColor3f(r, g, b);

        float h = 0.08f * sinf(p * 2.5f) * (1.0f - p) - (0.02f * p);

        glVertex3f(curX0, h, curZ);
        glVertex3f(curX1, h, curZ);
    }
    glEnd();

    // 3. RIGHT SIDE WHITE PLATFORM EXTENSION
    glBegin(GL_QUAD_STRIP);
    for(int st = 0; st <= 21; st++) {
        float p = st / 20.0f;
        float curZ = -7.0f - (30.5f * p);

        float curX1 = -10.5f * (1.0f - p) + (-9.3f) * p;
        float pWidth = 1.6f * (1.0f - p) + 0.3f * p;
        float curX2 = curX1 + pWidth;

        float h = 0.08f * sinf(p * 2.5f) * (1.0f - p) - (0.02f * p);

        float r = isDay ? 0.9f : 0.15f;
        float g = isDay ? 0.9f : 0.2f;
        float b = isDay ? 0.9f : 0.25f;

        // 🌫️ FOG for platform too
        float fogFade = 0.0f;
        if (p > 0.7f) {
            fogFade = (p - 0.7f) / 0.35f;
            if (fogFade > 1.0f) fogFade = 1.0f;
            fogFade *= 0.8f;
        }

        r = r * (1.0f - fogFade) + skyR * fogFade;
        g = g * (1.0f - fogFade) + skyG * fogFade;
        b = b * (1.0f - fogFade) + skyB * fogFade;

        glColor3f(r, g, b);

        glVertex3f(curX1, h + 0.01f, curZ);
        glVertex3f(curX2, h + 0.01f, curZ);
    }
    glEnd();

    // 4. PLATFORM SIDE WALL
    glBegin(GL_QUAD_STRIP);
    for(int st = 0; st <= 21; st++) {
        float p = st / 20.0f;
        float curZ = -7.0f - (30.5f * p);
        float curX1 = -10.5f * (1.0f - p) + (-9.3f) * p;
        float pWidth = 1.6f * (1.0f - p) + 0.3f * p;
        float curX2 = curX1 + pWidth;

        float h = 0.08f * sinf(p * 2.5f) * (1.0f - p) - (0.02f * p);

        float r = isDay ? 0.65f : 0.1f;
        float g = isDay ? 0.65f : 0.15f;
        float b = isDay ? 0.65f : 0.2f;

        // 🌫️  FOG for side wall
        float fogFade = 0.0f;
        if (p > 0.7f) {
            fogFade = (p - 0.7f) / 0.35f;
            if (fogFade > 1.0f) fogFade = 1.0f;
            fogFade *= 0.8f;
        }

        r = r * (1.0f - fogFade) + skyR * fogFade;
        g = g * (1.0f - fogFade) + skyG * fogFade;
        b = b * (1.0f - fogFade) + skyB * fogFade;

        glColor3f(r, g, b);

        glVertex3f(curX2, h + 0.01f, curZ);
        glVertex3f(curX2, h - 0.9f, curZ);
    }
    glEnd();

    glEnable(GL_LIGHTING);
}

// drawBDDataCenter: Draws the main server building and rooftop elements.
void drawBDDataCenter(void){
    float cx=-9.5f, cz=3.5f;
    mat(.22f,.24f,.22f,.2f,.2f); box(cx,-.01f,cz,3.5f,.05f,3.0f);
    mat(.18f,.22f,.18f,.3f,.3f); box(cx-.8f,0,cz-0.6f,2.2f,1.8f,1.4f);

    if(!isDay) matE(.05f,.45f,.2f,.0f,.25f,.1f); else mat(.25f,.55f,.35f,.7f,.8f);
    box(cx-.8f,.5f,cz+.12f,.55f,1.4f,.04f);
    box(cx-.8f,.5f,cz+.12f-1.4f,.55f,1.4f,.04f); noE();

    mat(.14f,.18f,.14f,.3f,.3f); box(cx+1.0f,0,cz-.2f,1.0f,2.6f,1.0f);
    if(!isDay) matE(.0f,.8f,.3f,.0f,.4f,.15f); else mat(.2f,.65f,.3f,.8f,.9f);
    for(int i=0;i<3;i++) box(cx+1.0f,.8f+i*.6f,cz+.51f,.7f,.45f,.03f); noE();

    mat(.5f,.55f,.5f,.4f,.4f);
    cyl(cx+.7f,2.6f,cz-.6f,cx+.7f,3.5f,cz-.6f,.02f,5);
    cyl(cx+1.0f,2.6f,cz-.2f,cx+1.0f,3.8f,cz-.2f,.02f,5);
    cyl(cx+1.3f,2.6f,cz+.3f,cx+1.3f,3.3f,cz+.3f,.02f,5);

    mat(.05f,.07f,.25f,.5f,.6f);
    for(int r=0;r<3;r++) for(int c=0;c<5;c++) box(cx-1.9f+c*.35f,.1f+1.8f,cz-0.5f+r*.38f,.28f,.03f,.3f);

    matE(.0f,.9f,.4f,.0f,.45f,.2f);
    box(cx+.0f,2.0f,cz+.52f,1.8f,.25f,.03f); noE();

    float bl=sinf(gTime*4)>.0f?1:.2f;
    matE(.1f,.1f,.1f,bl*.9f,.0f,.0f);
    sph(cx+1.0f,3.85f,cz-.2f,.06f,8); noE();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    if(isDay) glColor3f(1.0f, 1.0f, 1.0f);
    else glColor3f(0.2f, 0.6f, 1.0f);
    drawStr3(cx - 0.7f, 1.4f, cz + 1.55f, "BSCCL", GLUT_BITMAP_HELVETICA_18);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

// drawBDISPCenter: ISP Center right beside the Data Center
void drawBDISPCenter(void) {
    float cx = -11.5f, cz = 6.0f;

    // 1. MAIN BASE (Dark, sleek alloy structure)
    mat(0.18f, 0.20f, 0.22f, 0.5f, 0.5f);
    box(cx, 0.0f, cz, 2.0f, 1.0f, 1.8f);

    // 2. SECOND TIER (Slightly smaller, lighter grey metal)
    mat(0.25f, 0.28f, 0.32f, 0.6f, 0.6f);
    box(cx, 1.0f, cz - 0.1f, 1.6f, 0.7f, 1.4f);

    // 3. GLOWING SERVER WINDOWS (High-tech glass panel)
    if (!isDay) matE(0.0f, 0.1f, 0.2f, 0.0f, 0.3f, 0.8f);
    else mat(0.1f, 0.2f, 0.3f, 0.8f, 0.9f);

    // Ground floor large glass panel
    box(cx, 0.5f, cz + 0.91f, 1.6f, 0.7f, 0.04f);

    // Upper floor wide control room window
    box(cx, 1.35f, cz + 0.61f, 1.2f, 0.4f, 0.04f);
    noE();

    // 4. NEON ACCENTS & SERVER RACKS INSIDE (Visible through glass)
    if (!isDay) matE(0.0f, 0.8f, 1.0f, 0.0f, 1.0f, 1.0f); // Bright Cyan Neon
    else mat(0.0f, 0.5f, 0.8f, 0.5f, 0.5f);

    // Vertical glowing server strips inside ground glass
    for(float px = cx - 0.6f; px <= cx + 0.6f; px += 0.3f) {
        box(px, 0.5f, cz + 0.92f, 0.08f, 0.5f, 0.02f);
    }

    // Horizontal glowing tech trim wrapping the building border
    box(cx, 1.0f, cz + 0.92f, 2.02f, 0.04f, 0.04f);
    noE();

    // 5. HIGH-TECH COMM TOWER (Replaces old Bresenham lines)
    float roofY = 1.7f;
    float antX = cx + 0.4f;
    float antZ = cz - 0.3f;

    // Tower Base
    mat(0.15f, 0.15f, 0.15f, 0.4f, 0.4f);
    cyl(antX, roofY, antZ, antX, roofY + 0.6f, antZ, 0.15f, 8);

    // Tall Antenna Spire
    mat(0.4f, 0.4f, 0.45f, 0.8f, 0.8f);
    cyl(antX, roofY + 0.6f, antZ, antX, roofY + 2.5f, antZ, 0.03f, 6);

    // Glowing transmission rings on the tower
    if (!isDay) matE(0.0f, 0.9f, 0.3f, 0.0f, 0.8f, 0.3f); // Neon Green Signal
    else mat(0.0f, 0.8f, 0.3f, 0.5f, 0.5f);

    glPushMatrix(); glTranslatef(antX, roofY + 1.2f, antZ); glRotatef(90, 1, 0, 0); glutSolidTorus(0.02f, 0.12f, 6, 12); glPopMatrix();
    glPushMatrix(); glTranslatef(antX, roofY + 1.8f, antZ); glRotatef(90, 1, 0, 0); glutSolidTorus(0.02f, 0.10f, 6, 12); glPopMatrix();

    // Glowing Orb Transmitter at the very top
    sph(antX, roofY + 2.5f, antZ, 0.06f, 8);
    noE();

    // 6. MINI SATELLITE DISH (Left side of the roof)
    float dishX = cx - 0.25f;
    float dishZ = cz - 0.3f;
    mat(1.0f, 0.5f, 0.5f, 0.6f, 0.6f);
    cyl(dishX, roofY, dishZ, dishX, roofY + 0.3f, dishZ, 0.05f, 6); // Heavy mount

    glPushMatrix();
    glTranslatef(dishX, roofY + 0.3f, dishZ + 0.05f);
    glRotatef(-25, 1, 0, 0); // Tilt up towards sky
    glScalef(1.0f, 1.0f, 0.2f);
    glutSolidSphere(0.2f, 12, 12); // Dish bowl
    glPopMatrix();

    // Dish center receiver (Blinking red light style)
    if(!isDay) matE(1.0f, 0.1f, 0.1f, 0.8f, 0.0f, 0.0f); else mat(0.8f, 0.1f, 0.1f, 0.5f, 0.5f);
    sph(dishX, roofY + 0.35f, dishZ + 0.15f, 0.03f, 6);
    noE();

    // 7. SLEEK SIGNBOARD ("ISP01")
    // Dark backplate for the sign
    mat(0.1f, 0.1f, 0.1f, 0.2f, 0.2f);
    box(cx - 0.4f, 1.35f, cz + 0.62f, 0.7f, 0.25f, 0.02f);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    if(isDay) glColor3f(1.0f, 1.0f, 1.0f);
    else glColor3f(0.0f, 1.0f, 1.0f); // Cyber-Cyan Neon Text

    // Position text perfectly inside the backplate
    drawStr3(cx - 0.62f, 1.3f, cz + 0.65f, "ISP01", GLUT_BITMAP_HELVETICA_12);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}
// drawBDHouseLinks: Draws power poles and animated packet links to houses.
void drawBDHouseLinks(void){
    float dcX=-8.5f, dcY=2.6f, dcZ=3.3f;
    float ispX=-11.5f, ispY=2.2f, ispZ=6.0f;

    float houses[8][2]={
        {-23.5f, 0.8f, 2.0f}, {-17.0f, 3.5f}, {-12.0f, 3.5f}, {-22.0f,-3.0f}, {-17.0f,-3.0f}, {-12.0f,-3.0f},
        {-18.0f, 9.5f}, {-13.0f, 9.5f}
    };
    float poles[8][2] = {
        {-22.0f, 1.2f}, {-17.0f, 1.2f}, {-12.0f, 1.2f}, {-22.0f,-0.8f}, {-17.0f,-0.8f}, {-12.0f,-0.8f},
        {-18.0f, 7.5f}, {-13.0f, 7.5f}
    };

    glDisable(GL_LIGHTING); glLineWidth(1.2f);

    glColor3f(0.05f, 0.05f, 0.05f);
    glLineWidth(2.5f);
    glBegin(GL_LINES);
    glVertex3f(dcX, dcY, dcZ);
    glVertex3f(ispX, ispY, ispZ);
    glEnd();
    glLineWidth(1.2f);

    if (cableOK && simState == 2 && atof(bscclGbps) > 0.0f) {
        float trunkColors[5][3] = {{0,.85f,1}, {0,1,.4f}, {1,.85f,0}, {1,.45f,0}, {.85f,0,1}};
        glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        for(int p = 0; p < 5; p++) {
            float t_trunk = fmodf(gTime * dataspd * 1.5f + (p * 0.2f), 1.0f);
            float px = dcX + (ispX - dcX) * t_trunk;
            float py = dcY + (ispY - dcY) * t_trunk;
            float pz = dcZ + (ispZ - dcZ) * t_trunk;

            glColor4f(trunkColors[p][0], trunkColors[p][1], trunkColors[p][2], 0.95f);
            sph(px, py, pz, 0.07f, 8);
            glColor4f(trunkColors[p][0], trunkColors[p][1], trunkColors[p][2], 0.3f);
            sph(px, py, pz, 0.18f, 8);
        }
        glDisable(GL_BLEND);
    }

    for(int i=0;i<8;i++){
        glColor3f(.2f,.2f,.2f);
        glBegin(GL_LINES);
        glVertex3f(poles[i][0], 0.0f, poles[i][1]);
        glVertex3f(poles[i][0], 1.8f, poles[i][1]);
        glEnd();

        glColor3f(.1f,.1f,.1f);
        glBegin(GL_LINES);
        glVertex3f(ispX, ispY, ispZ); glVertex3f(poles[i][0], 1.8f, poles[i][1]);
        glVertex3f(poles[i][0], 1.8f, poles[i][1]); glVertex3f(houses[i][0], 0.9f, houses[i][1]);
        glEnd();

        if (cableOK && housesData[i].active && simState == 2 && atof(bscclGbps) > 0.0f && strlen(housesData[i].ip) > 0 && strlen(housesData[i].bw) > 0) {
            float t_anim = fmodf(gTime*(dataspd*1.5f+i*.15f)+i*.4f, 1.f);
            float px, py, pz;
            if (t_anim < 0.6f) {
                float nt = t_anim / 0.6f;
                px = ispX + (poles[i][0] - ispX) * nt; py = ispY + (1.8f - ispY) * nt; pz = ispZ + (poles[i][1] - ispZ) * nt;
            } else {
                float nt = (t_anim - 0.6f) / 0.4f;
                px = poles[i][0] + (houses[i][0] - poles[i][0]) * nt; py = 1.8f + (0.9f - 1.8f) * nt; pz = poles[i][1] + (houses[i][1] - poles[i][1]) * nt;
            }

            float stdCols[5][3]={{0,.85f,1},{0,1,.4f},{1,.85f,0},{1,.45f,0},{.85f,0,1}};
            int pType = housesData[i].pktType;

            housesData[i].currR = stdCols[pType][0];
            housesData[i].currG = stdCols[pType][1];
            housesData[i].currB = stdCols[pType][2];

            glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE);
            glColor4f(stdCols[pType][0], stdCols[pType][1], stdCols[pType][2], .95f); sph(px,py,pz,.05f,8);
            glColor4f(stdCols[pType][0], stdCols[pType][1], stdCols[pType][2], .3f);  sph(px,py,pz,.13f,8);
            glDisable(GL_BLEND);
        }
    }
    glLineWidth(1); glEnable(GL_LIGHTING);

    mat(.3f,.3f,.3f,.1f,.1f);
    for(int i=0;i<8;i++){
        cyl(poles[i][0], 0, poles[i][1], poles[i][0], 1.8f, poles[i][1], 0.03f, 6);
        box(poles[i][0], 1.7f, poles[i][1], 0.4f, 0.05f, 0.05f);
    }
}

// drawBDHouse: Draws houses. Algorithms: Midpoint Circle (window), DDA Line & Bresenham (antenna).
void drawBDHouse(float cx, float cz, float rot, float wr, float wg, float wb, float rr, float rg, float rb, int type, int houseId) {
    glPushMatrix();
    glTranslatef(cx, 0, cz);
    glRotatef(rot, 0, 1, 0);

    mat(wr, wg, wb, 0.1f, 0.05f);
    box(0, 0, 0, 1.2f, 0.85f, 1.0f);
    mat(0.4f, 0.4f, 0.4f, 0.1f, 0.1f);
    box(0, 0, 0, 1.3f, 0.1f, 1.1f);
    mat(rr, rg, rb, 0.3f, 0.2f);
    if (type == 0) {
        glPushMatrix(); glTranslatef(0, 0.85f, 0); glRotatef(15, 1, 0, 0); glScalef(1.4f, 0.06f, 1.3f); glutSolidCube(1); glPopMatrix();
    } else if (type == 1) {
        glPushMatrix(); glTranslatef(0, 0.85f, 0); glRotatef(45, 0, 1, 0); glRotatef(-90, 1, 0, 0); glutSolidCone(0.9f, 0.6f, 4, 1); glPopMatrix();
    } else if (type == 2) {
        box(0, 0.85f, 0, 1.3f, 0.1f, 1.1f);
        box(0, 0.95f, 0.5f, 1.3f, 0.15f, 0.1f);
        box(0, 0.95f, -0.5f, 1.3f, 0.15f, 0.1f);
    }

    mat(0.38f, 0.22f, 0.08f, 0.1f, 0.05f); box(0.2f, 0.1f, 0.5f, 0.3f, 0.55f, 0.02f);
    if(!isDay) matE(0.9f, 0.8f, 0.3f, 0.6f, 0.5f, 0.1f); else mat(0.3f, 0.42f, 0.55f, 0.8f, 0.9f);
    box(-0.35f, 0.35f, 0.5f, 0.3f, 0.28f, 0.02f);
    box(0.6f, 0.35f, 0.0f, 0.02f, 0.28f, 0.4f);
    noE();

    // REMOVE FLOATING LABELS for houses beyond index 8
    if (houseId <= 8) {
        glDisable(GL_LIGHTING);
        if(isDay) glColor3f(0.0f, 0.0f, 0.0f); else glColor3f(0.0f, 1.0f, 0.8f);
        char hnLabel[10];
        sprintf(hnLabel, "HN%02d", houseId);
        drawStr3(-0.3f, 2.0f, 0.0f, hnLabel, GLUT_BITMAP_HELVETICA_12);
        glEnable(GL_LIGHTING);
    }

    glPopMatrix();
}

void drawBDSatelliteBase(void){
    float tx = -20.5f, tz = -6.0f;
    float rot = 30.0f * sinf(gTime * 0.6f);
    float tilt = 35.0f + 15.0f * sinf(gTime * 0.4f);

    mat(0.2f, 0.25f, 0.3f, 0.5f, 0.5f);
    box(tx - 1.0f, 0.0f, tz - 1.0f, 2.0f, 0.15f, 2.0f);
    mat(0.3f, 0.35f, 0.4f, 0.6f, 0.6f);
    box(tx - 0.7f, 0.15f, tz - 0.7f, 1.4f, 0.2f, 1.4f);

    mat(0.7f, 0.7f, 0.75f, 0.4f, 0.4f);
    box(tx - 0.4f, 0.35f, tz + 0.3f, 0.8f, 0.6f, 0.5f);

    if(!isDay) matE(0.0f, 0.8f, 1.0f, 0.2f, 0.6f, 1.0f); else mat(0.0f, 0.5f, 0.8f, 0.8f, 1.0f);
    box(tx - 0.2f, 0.6f, tz + 0.78f, 0.4f, 0.1f, 0.05f);
    noE();

    mat(0.5f, 0.5f, 0.55f, 0.7f, 0.7f);
    cyl(tx, 0.35f, tz, tx, 1.8f, tz, 0.25f, 16);

    glPushMatrix();
    glTranslatef(tx, 1.8f, tz);
    glRotatef(rot, 0, 1, 0);

    mat(0.3f, 0.3f, 0.35f, 0.6f, 0.6f);
    glPushMatrix();
    glRotatef(90, 0, 0, 1);
    cyl(0, -0.4f, 0, 0, 0.4f, 0, 0.15f, 12);
    glPopMatrix();

    glRotatef(tilt, 1, 0, 0);

    mat(0.4f, 0.4f, 0.4f, 0.5f, 0.5f);
    box(-0.25f, -0.1f, -0.3f, 0.5f, 0.5f, 0.3f);

    mat(0.9f, 0.9f, 0.95f, 0.8f, 0.8f);
    glPushMatrix();
    glTranslatef(0.0f, 0.4f, 0.0f);
    glRotatef(-90, 1, 0, 0);

    GLUquadric* q = gluNewQuadric();
    gluCylinder(q, 0.0f, 1.4f, 0.4f, 32, 1);

    mat(0.7f, 0.7f, 0.75f, 0.5f, 0.5f);
    gluCylinder(q, 1.4f, 1.45f, 0.4f, 32, 1);

    mat(0.8f, 0.8f, 0.85f, 0.5f, 0.5f);
    gluDisk(q, 0.0f, 1.45f, 32, 1);
    gluDeleteQuadric(q);
    glPopMatrix();

    mat(0.8f, 0.8f, 0.85f, 0.6f, 0.6f);
    cyl(0.0f, 0.4f, 0.0f, 0.0f, 1.5f, 0.0f, 0.04f, 8);

    mat(0.7f, 0.7f, 0.75f, 0.8f, 0.8f);
    sph(0.0f, 1.5f, 0.0f, 0.1f, 12);

    if(cableOK) {
        if(!isDay) matE(0.0f, 1.0f, 0.2f, 0.0f, 1.0f, 0.2f); else mat(0.0f, 1.0f, 0.2f, 0.8f, 0.8f);
    } else {
        if(!isDay) matE(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f); else mat(1.0f, 0.0f, 0.0f, 0.8f, 0.8f);
    }
    sph(0.0f, 1.6f, 0.0f, 0.05f, 8);
    noE();

    glPopMatrix();
}
void drawLeftSilhouette(void){
    glDisable(GL_LIGHTING);

    if(isDay) glColor4f(0.4f, 0.5f, 0.4f, 0.4f);
    else glColor4f(0.1f, 0.2f, 0.3f, 0.5f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // distant fake hills / trees
    for(int i=0;i<6;i++){
        float x = -24.0f + i * 0.6f;
        float h = 1.5f + (i%2)*0.5f;

        glBegin(GL_TRIANGLES);
        glVertex3f(x, 0.0f, -5.0f);
        glVertex3f(x + 0.4f, h, -5.0f);
        glVertex3f(x + 0.8f, 0.0f, -5.0f);
        glEnd();
    }

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

// drawBangladesh: Assembles terrain, houses, windmills, trees, roads, and data center.
void drawBangladesh(void){
    drawBDTerrain();

    // Original Houses (HN01 to HN08)
    drawBDHouse(-22.9f, 2.0f, 180,  0.95f, 0.95f, 0.9f,   0.85f, 0.25f, 0.2f, 1, 1);
    drawBDHouse(-17.0f,  3.5f, 180,  0.9f,0.9f,0.9f,    0.7f,0.2f,0.2f, 1, 2);
    drawBDHouse(-12.0f,  3.5f, 180,  0.6f,0.75f,0.6f,   0.4f,0.4f,0.4f, 2, 3);
    drawBDHouse(-22.0f, -3.0f,   0,  0.9f,0.85f,0.7f,   0.2f,0.6f,0.3f, 1, 4);
    drawBDHouse(-17.0f, -3.0f,   0,  0.6f,0.75f,0.85f,  0.5f,0.3f,0.2f, 0, 5);
    drawBDHouse(-12.0f, -3.0f,   0,  0.8f,0.7f,0.8f,    0.85f,0.85f,0.85f, 2, 6);
    drawBDHouse(-18.0f,  9.5f,   0,  0.9f,0.9f,0.8f,    0.8f,0.3f,0.3f, 2, 7);
    drawBDHouse(-13.0f,  9.5f,   0,  0.8f,0.8f,0.9f,    0.2f,0.5f,0.7f, 1, 8);

    // Original Windmills & Trees
    drawWindmill(-19.5f,  6.5f); drawWindmill( -14.5f,  6.5f);
    drawWindmill(-18.0f, -6.5f); drawWindmill( -13.0f, -6.5f);
    drawWindmill(-15.5f, 12.0f);

    float tp[][2]={ {-23.0f, 6.0f}, {-18.5f,-7.5f}, {-15.0f, 7.5f}, {-11.5f, -6.5f}, {-10.0f, 6.0f}, {-9.5f, -4.0f}, {-21.0f, -1.8f} };
    for(int i=0;i<7;i++) drawRealTree(tp[i][0], tp[i][1], 0.8f + 0.3f*sinf(i*1.3f));

    float tp_new[][2] = {{-19.5f, 10.0f}, {-11.5f, 10.0f}, {-15.5f, 8.0f}};
    for(int i=0; i<3; i++) drawRealTree(tp_new[i][0], tp_new[i][1], 0.8f + 0.3f*sinf(i*2.1f));

    drawPalm(-9.5f,-.5f); drawPalm(-9.1f,.8f);


    // ==========================================
   // LEFT SIDE DISTANCE FOG (illusion of more world)
glEnable(GL_FOG);
glFogi(GL_FOG_MODE, GL_LINEAR);


glFogf(GL_FOG_START, -5.0f);
glFogf(GL_FOG_END, -25.0f);

if (isDay) {
    GLfloat fogColor[] = {0.7f, 0.85f, 1.0f, 1.0f};
    glFogfv(GL_FOG_COLOR, fogColor);
} else {
    GLfloat fogColor[] = {0.05f, 0.05f, 0.2f, 1.0f};
    glFogfv(GL_FOG_COLOR, fogColor);
}

    // New Distant Houses
    drawBDHouse(-16.0f, -14.0f, 0,  0.8f,0.75f,0.7f,   0.2f,0.3f,0.6f, 0, 9);
    drawBDHouse(-20.0f, -15.0f, 0,  0.9f,0.75f,0.7f,   0.7f,0.3f,0.6f, 0, 9);
    drawBDHouse(-14.0f, -22.0f, 0,  0.6f,0.75f,0.85f,  0.5f,0.3f,0.2f, 1, 10);
    //drawBDHouse(-11.5f, -28.0f, 0,  0.9f,0.9f,0.8f,    0.8f,0.3f,0.3f, 2, 11);

    // New Distant Windmills
    drawWindmill(-19.0f, -19.0f);
    drawWindmill(-14.0f, -26.0f);

    // NEW DISTANT BIG TREES TO FILL THE SPACE
    drawRealTree(-13.0f, -13.0f, 1.2f);
    drawRealTree(-19.0f, -22.0f, 1.5f);
    drawRealTree(-20.0f, -17.0f, 1.5f);
    drawRealTree(-11.5f, -20.0f, 1.3f);
    drawRealTree(-18.0f, -27.0f, 1.4f);
    drawRealTree(-12.0f, -27.0f, 1.6f);
    //drawRealTree(-12.5f, -32.0f, 1.2f);

    glDisable(GL_FOG); // Turn off fog immediately to keep main elements clear!
    // ==========================================


    mat(.18f,.18f,.18f,.1f,.1f); box(-16.0f, .01f, 0.2f, 15.0f, .02f, 1.2f);

    glDisable(GL_LIGHTING);
    float brt=isDay?1:.6f; glColor3f(.9f*brt,.9f*brt,.88f*brt);
    for(int i=0;i<10;i++){
        float rx=-23.0f+i*1.5f;
        glBegin(GL_QUADS);
        glVertex3f(rx,      .03f, 0.18f); glVertex3f(rx+.6f,  .03f, 0.18f);
        glVertex3f(rx+.6f,  .03f, 0.22f); glVertex3f(rx,      .03f, 0.22f);
        glEnd();
    }
    glEnable(GL_LIGHTING);

    for(int i=0;i<MAX_CARS;i++){
        glPushMatrix();
        glTranslatef(bdCar[i].x,.10f,bdCar[i].z); glRotatef(-bdCar[i].angle*180/PI,0,1,0);
        mat(bdCar[i].r,bdCar[i].g,bdCar[i].b,.6f,.7f); box(0,0,0,.65f,.18f,.32f);
        mat(bdCar[i].r*.9f,bdCar[i].g*.9f,bdCar[i].b*.9f,.8f,.8f); box(-.05f,.18f,0,.35f,.15f,.28f);
        mat(.2f,.3f,.4f,.9f,.9f); box(.12f,.18f,0,.05f,.12f,.26f);
        if(!isDay){
            matE(1,1,.8f,1,1,.5f); sph(.33f,.05f,-.1f,.05f,6); sph(.33f,.05f,.1f,.05f,6);
            matE(1,.1f,.1f,.8f,0,0); sph(-.33f,.05f,-.1f,.04f,6); sph(-.33f,.05f,.1f,.04f,6); noE();
        }
        mat(.1f,.1f,.1f,.2f,.1f);
        for(int w=0;w<4;w++){ float wx=(w<2)?-.2f:.2f,wz=(w%2==0)?-.16f:.16f; sph(wx,-.02f,wz,.08f,8); }
        glPopMatrix();
    }

    drawBDSatelliteBase();
    drawBDDataCenter();
    drawLeftSilhouette();

        // Floating text for TX/RX
    glDisable(GL_LIGHTING);
    if(!isDay) glColor3f(0.0f, 1.0f, 0.8f); else glColor3f(0.1f, 0.2f, 0.8f);
    char txLabel[50];
    drawStr3(-10.5f, 2.8f, 3.5f, txLabel, GLUT_BITMAP_HELVETICA_18);
    glEnable(GL_LIGHTING);

    drawBDISPCenter();
    drawBDHouseLinks();
    drawBeach(-9.15f,12.5f,.7f,45.0f,1);

    float fc=isDay?1:.5f;
    glDisable(GL_LIGHTING);
    glVertex3f(-24.2f,3.0f,-1.5f); glVertex3f(-22.3f,3.0f,-1.5f);
    glVertex3f(-22.3f,2.45f,-1.5f); glVertex3f(-24.2f,2.45f,-1.5f); glEnd();
    glEnable(GL_LIGHTING);

    glDisable(GL_LIGHTING);
    float lc=isDay?1:.45f;
    glColor3f(.1f,.6f*lc,.15f*lc); drawStr3(-24.0f,5.8f,0," BANGLADESH",GLUT_BITMAP_TIMES_ROMAN_24);
    glEnable(GL_LIGHTING);
}
