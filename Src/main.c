#include "common.h"

// initAll: Initializes entities (fish, bubbles, sharks, cars). Uses pseudo-random distribution.
void initAll(void){
    float fc[][3]={{1,.55f,0},{.2f,.85f,1},{1,1,.2f},{.5f,1,.5f},{1,.4f,.6f},{.4f,.6f,1},
                   {.9f,.7f,.1f},{.1f,.9f,.7f},{.7f,.1f,.9f},{1,.8f,.5f},{.5f,.8f,1},{.8f,1,.4f},
                   {.9f,.5f,.2f},{.2f,.5f,.9f}};

    // Setup fish coordinates and colors
    for(int i=0;i<MAX_FISH;i++){
        fish[i].x=-8.0f+(rand()%160)*.1f; fish[i].y=-2.5f-(rand()%45)*.1f;
        fish[i].z=-3+(rand()%200)*.1f; fish[i].spd=.5f+(rand()%7)*.12f;
        fish[i].dir=(rand()%2)?1:-1; fish[i].phase=(rand()%628)*.01f;
        int c=i%14; fish[i].r=fc[c][0]; fish[i].g=fc[c][1]; fish[i].b=fc[c][2];
    }

    // Setup bubble coordinates and lifespans
    for(int i=0;i<MAX_BUBBLE;i++){
        bbl[i].x=-9+(rand()%180)*.1f; bbl[i].y=-7+(rand()%6)*.1f;
        bbl[i].z=-3+(rand()%200)*.1f; bbl[i].spd=.12f+(rand()%8)*.04f;
        bbl[i].sz=.02f+(rand()%5)*.01f; bbl[i].life=(rand()%100)*.01f;
    }

    // Setup predefined shark positions
    sharks[0].x=-7.5f; sharks[0].y=-5.5f; sharks[0].z= 2.0f; sharks[0].spd=1.5f; sharks[0].dir=1; sharks[0].phase=0;
    sharks[1].x= 6.5f; sharks[1].y=-6.0f; sharks[1].z=-1.2f; sharks[1].spd=1.2f; sharks[1].dir=-1;sharks[1].phase=1.5f;
    sharks[2].x= 0.0f; sharks[2].y=-4.8f; sharks[2].z= 1.5f; sharks[2].spd=1.8f; sharks[2].dir=1; sharks[2].phase=3.0f;
    sharks[3].x=-2.0f; sharks[3].y=-4.2f; sharks[3].z=-2.5f; sharks[3].spd=1.6f; sharks[3].dir=-1; sharks[3].phase=4.2f;

    float bc[][3]={{.8f,.2f,.2f},{.2f,.5f,.9f},{.9f,.75f,.1f},{.3f,.7f,.3f},{.8f,.4f,.8f},{.9f,.6f,.2f}};

    // Setup BD and SG vehicle lanes
    for(int i=0;i<MAX_CARS;i++){
        bdCar[i].r=bc[i][0]; bdCar[i].g=bc[i][1]; bdCar[i].b=bc[i][2];
        bdCar[i].spd=1.5f+(rand()%5)*.3f;
        if (i < MAX_CARS/2) {
            bdCar[i].dir = 1; bdCar[i].z = 0.6f; bdCar[i].angle = 0.0f; bdCar[i].x = -24.0f + i*2.5f;
        } else {
            bdCar[i].dir = -1; bdCar[i].z = -0.2f; bdCar[i].angle = PI; bdCar[i].x = -24.0f + i*2.5f;
        }
        sgCar[i].r=bc[(i+3)%6][0]; sgCar[i].g=bc[(i+3)%6][1]; sgCar[i].b=bc[(i+3)%6][2];
        sgCar[i].spd=1.6f+(rand()%5)*.3f; sgCar[i].dir=(i%2)?1:-1;
        sgCar[i].x=10.5f+i*2.0f;
        sgCar[i].z=(i%3==0)?0.4f:(i%3==1)?2.5f:-1.5f;
    }
}

// setupLighting: Configures Day/Night lighting. Uses Gouraud shading model.
void setupLighting(void){
    glEnable(GL_LIGHTING); glEnable(GL_LIGHT0); glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL); glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    float li=isDay?1:.25f;
    GLfloat lp[]={-10,15,-20,1}, ld[]={li,li*.96f,li*.88f,1}, la[]={li*.28f,li*.28f,li*.32f,1};
    glLightfv(GL_LIGHT0,GL_POSITION,lp); glLightfv(GL_LIGHT0,GL_DIFFUSE,ld); glLightfv(GL_LIGHT0,GL_AMBIENT,la);
    float li2=isDay?.35f:.08f;
    GLfloat lp2[]={12,10,15,1}, ld2[]={li2,li2,li2*1.1f,1};
    glLightfv(GL_LIGHT1,GL_POSITION,lp2); glLightfv(GL_LIGHT1,GL_DIFFUSE,ld2);
    float ag=isDay?.22f:.06f; GLfloat gAmb[]={ag,ag,ag*1.1f,1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,gAmb); glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
}

// display: Main render callback. Draws sky, landmasses, ocean, cable, and packets.
void display(void){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    setupLighting();

    extern void drawBangladesh(void);
    extern void drawSingapore(void);
    extern void drawOcean(void);
    extern void drawCable(void);
    extern void drawPackets(void);

    // Draw background elements
    drawSky(); drawSunMoon();
    drawSatellite();

    // Set perspective projection matrix
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluPerspective(48,(float)winW/winH,.1f,180);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();

    // Apply lookAt view matrix (Zoom logic included here!)
    float cy=camPitch*D2R, ya=camYaw*D2R;

    // Dynamic Focus: As we zoom in, the target smoothly shifts underwater
    float zoomT = (camDist - 5.0f) / 31.0f; // Range: 0.0 to 1.0
    if(zoomT > 1.0f) zoomT = 1.0f;
    if(zoomT < 0.0f) zoomT = 0.0f;

    // Smoothstep focus: 1.0 (above water) -> -7.0 (underwater)
    float targetY = -7.0f + (8.0f * zoomT);

    float ex = camDist*sinf(ya)*cosf(cy);
    float ey = targetY + camDist*sinf(cy);
    float ez = camDist*cosf(ya)*cosf(cy);

    gluLookAt(ex, ey, ez, 0, targetY, 0, 0, 1, 0);

    // Draw foreground elements
    setupLighting(); glEnable(GL_NORMALIZE);
    drawBangladesh(); drawSingapore(); drawOcean(); drawCable(); drawPackets();
    drawHUD(); glutSwapBuffers();
}

// update: Physics and animation step. Uses Euler integration for movement.
void update(int v){
    float dt=.016f; gTime+=dt; windAngle+=110*dt; if(windAngle>360) windAngle-=360;
    waveOff+=.72f*dt;

    extern void spawnPkt(int dir,float r,float g,float b);
    extern void cablePt(float t, float*ox, float*oy, float*oz);

   // Randomly spawn data packets if cable is active, sim is running, AND Gbps is provided
    if(cableOK && simState == 2 && atof(bscclGbps) > 0.0f && (rand() % 10 == 0)) {
        float pc[][3]={{0,.88f,1},{0,1,.45f},{1,.85f,0},{1,.45f,0},{.85f,0,1}};
        int ci=rand()%5;
        int dir=rand()%2;
        spawnPkt(dir, pc[ci][0], pc[ci][1], pc[ci][2]);
    }

    // Update packet positions along the parametric cable curve
    for(int i=0;i<MAX_PKT;i++){
        if(!pkts[i].active) continue;
        float px,py,pz; cablePt(pkts[i].t,&px,&py,&pz);
        if(pkts[i].tlen<TRAIL_LEN){
            pkts[i].tx[pkts[i].tlen]=px; pkts[i].ty[pkts[i].tlen]=py; pkts[i].tz[pkts[i].tlen]=pz; pkts[i].tlen++;
        } else {
            for(int tr=TRAIL_LEN-1;tr>0;tr--){ pkts[i].tx[tr]=pkts[i].tx[tr-1]; pkts[i].ty[tr]=pkts[i].ty[tr-1]; pkts[i].tz[tr]=pkts[i].tz[tr-1]; }
            pkts[i].tx[0]=px; pkts[i].ty[0]=py; pkts[i].tz[0]=pz;
        }
        pkts[i].t+=(pkts[i].dir?-1:1)*dataspd*dt;
        if(pkts[i].t>1.05f||pkts[i].t<-.05f){ pkts[i].active=0; pktCnt--; }
    }

    // Update fish translation and wrap boundaries
    for(int i=0;i<MAX_FISH;i++){
        fish[i].x += fish[i].dir * fish[i].spd * dt;
        if(fish[i].x > 8.0f) { fish[i].dir = -1; }
        if(fish[i].x < -8.0f) { fish[i].dir = 1; }
    }

    // Update shark translation and wrap boundaries
    for(int i=0;i<MAX_SHARK;i++){
        sharks[i].x += sharks[i].dir * sharks[i].spd * dt;
        if(sharks[i].x > 7.5f) { sharks[i].dir = -1; }
        if(sharks[i].x < -7.5f) { sharks[i].dir = 1; }
    }

    // Update bubble buoyancy and recycle expired ones
    for(int i=0;i<MAX_BUBBLE;i++){
        bbl[i].y+=bbl[i].spd*dt; bbl[i].life+=dt*.28f;
        if(bbl[i].y>.3f||bbl[i].life>1){
            bbl[i].x=-9+(rand()%180)*.1f; bbl[i].y=-7+(rand()%6)*.1f;
            bbl[i].z=-3+(rand()%200)*.1f; bbl[i].life=0;
        }
    }

    // Update Bangladesh car positions
    for(int i=0;i<MAX_CARS;i++){
        bdCar[i].x += bdCar[i].dir * bdCar[i].spd * dt;
        if(bdCar[i].dir == 1) {
            bdCar[i].z = 0.6f;
            bdCar[i].angle = 0.0f;
            if(bdCar[i].x > -9.5f) {
                bdCar[i].dir = -1;
                bdCar[i].x = -9.5f;
            }
        } else {
            bdCar[i].z = -0.2f;
            bdCar[i].angle = PI;
            if(bdCar[i].x < -24.5f) {
                bdCar[i].dir = 1;
                bdCar[i].x = -24.5f;
            }
        }
    }

    // Update Singapore car positions
    for(int i=0;i<MAX_CARS;i++){
        sgCar[i].x += sgCar[i].dir * sgCar[i].spd * dt;
        if(sgCar[i].x > 23.5f) { sgCar[i].dir = -1; }
        if(sgCar[i].x < 8.8f) { sgCar[i].dir = 1; }
    }
    glutPostRedisplay(); glutTimerFunc(16,update,0);
}

// keyboard: Input handler for user interaction and Admin Panel text input.
void keyboard(unsigned char k, int xx, int yy) {
    (void)xx; (void)yy;

    // --- TOGGLE ADMIN PANEL ---
    if (k == 9) { // 9 is the ASCII code for the TAB key
        simState = (simState == 2) ? 0 : 2; // Toggle between Admin Panel and Simulation
        menuEditMode = 0;
        glutPostRedisplay();
        return;
    }

    // --- UNIFIED ADMIN PANEL STATE ---
    if (simState == 0) {
        if (k == 13) { // ENTER Key -> Start/Resume Simulation
            simState = 2;
        }
        else if (k == 'g' || k == 'G') { menuEditMode = 3; } // Edit BSCCL Gbps
        else if (k == 'i' || k == 'I') { menuEditMode = 1; } // Edit IP
        else if (k == 'b' || k == 'B') { menuEditMode = 2; } // Edit BW
        else if (k == 't' || k == 'T') { menuEditMode = 4; } // NEW: Edit Packet Type
        else if (k == 'c' || k == 'C') {
            // Validation: IP and BW must not be empty to connect
            if (strlen(housesData[menuSelectedHouse].ip) > 0 && strlen(housesData[menuSelectedHouse].bw) > 0) {
                housesData[menuSelectedHouse].active = 1;
            }
            menuEditMode = 0;
        } // Connect
        else if (k == 'd' || k == 'D') { housesData[menuSelectedHouse].active = 0; menuEditMode = 0;} // Disconnect

        else if (menuEditMode == 3) { // Typing BSCCL Gbps
            if (k == 8 || k == 127) {
                int len = strlen(bscclGbps);
                if (len > 0) bscclGbps[len - 1] = '\0';
            } else if ((k >= '0' && k <= '9') && strlen(bscclGbps) < 10) {
                int len = strlen(bscclGbps);
                bscclGbps[len] = k; bscclGbps[len + 1] = '\0';
            }
        }
        else if (menuEditMode == 1) { // Typing IP
            if (k == 8 || k == 127) {
                int len = strlen(housesData[menuSelectedHouse].ip);
                if (len > 0) housesData[menuSelectedHouse].ip[len - 1] = '\0';
            } else if (((k >= '0' && k <= '9') || k == '.') && strlen(housesData[menuSelectedHouse].ip) < 15) {
                int len = strlen(housesData[menuSelectedHouse].ip);
                housesData[menuSelectedHouse].ip[len] = k; housesData[menuSelectedHouse].ip[len + 1] = '\0';
            }
        }
        else if (menuEditMode == 2) { // Typing BW
            if (k == 8 || k == 127) {
                int len = strlen(housesData[menuSelectedHouse].bw);
                if (len > 0) housesData[menuSelectedHouse].bw[len - 1] = '\0';
            } else if ((k >= '0' && k <= '9') && strlen(housesData[menuSelectedHouse].bw) < 6) {
                int len = strlen(housesData[menuSelectedHouse].bw);
                housesData[menuSelectedHouse].bw[len] = k; housesData[menuSelectedHouse].bw[len + 1] = '\0';
            }
        }
        else if (menuEditMode == 4) { // Typing Packet Type
            if (k == 'h' || k == 'H') { housesData[menuSelectedHouse].pktType = 0; menuEditMode = 0; }
            else if (k == 'f'|| k == 'F' ) { housesData[menuSelectedHouse].pktType = 1; menuEditMode = 0; }
            else if (k == 'v' || k == 'V') { housesData[menuSelectedHouse].pktType = 2; menuEditMode = 0; }
            else if (k == 'o' || k == 'O') { housesData[menuSelectedHouse].pktType = 3; menuEditMode = 0; }
            else if (k == 'e' || k == 'E') { housesData[menuSelectedHouse].pktType = 4; menuEditMode = 0; }
        }
        glutPostRedisplay();
        return;
    }

    // --- NORMAL SIMULATION CONTROLS ---
    float pc[][3]={{0,.88f,1},{0,1,.45f},{1,.85f,0},{1,.45f,0},{.85f,0,1}};
    int ci=rand()%5;
    extern void spawnPkt(int dir,float r,float g,float b);

    switch(k){
    case 's': case 'S': spawnPkt(0,pc[ci][0],pc[ci][1],pc[ci][2]); spawnPkt(1,pc[(ci+2)%5][0],pc[(ci+2)%5][1],pc[(ci+2)%5][2]); break;
    case 'm': case 'M':
        for(int i=0;i<5;i++){
            int c=rand()%5;
            spawnPkt(0,pc[c][0],pc[c][1],pc[c][2]); spawnPkt(1,pc[(c+1)%5][0],pc[(c+1)%5][1],pc[(c+1)%5][2]);
        } break;
    case 'b': case 'B': cableOK=0; break;
    case 'r': case 'R': cableOK=1; break;
    case 'd': case 'D': isDay=!isDay; break;
    case '+': case '=': dataspd*=1.5f; if(dataspd>3)dataspd=3; break;
    case '-': case '_': dataspd/=1.5f; if(dataspd<.05f)dataspd=.05f; break;

    // ======== ZOOM LOGIC WITH Z/X ========
    case 'z': case 'Z': camDist -= 2.0f; if (camDist < 5.0f) camDist = 5.0f; break; // Zoom In
    case 'x': case 'X': camDist += 2.0f; if (camDist > 33.2f) camDist = 33.2f; break; // Zoom Out
    // =====================================

    case 27: exit(0); // ESC Key
    }
}

// special: Input handler for special keys (up/down arrows)
void special(int k, int xx, int yy) {
    (void)xx; (void)yy;
    if (simState == 0) { // ISP Menu te thakle select change hobe
        if (k == GLUT_KEY_UP) {
            menuSelectedHouse = (menuSelectedHouse > 0) ? menuSelectedHouse - 1 : 7;
        }
        if (k == GLUT_KEY_DOWN) {
            menuSelectedHouse = (menuSelectedHouse < 7) ? menuSelectedHouse + 1 : 0;
        }
        menuEditMode = 0; // Select change korle edit mode off hoye jabe
    }
    glutPostRedisplay();
}

// Window and input event stubs/handlers
void mouse(int btn,int state,int xx,int yy){
    (void)xx; (void)yy;
    // ======== MOUSE WHEEL ZOOM LOGIC ========
    if (state == GLUT_DOWN) {
        if (btn == 3) { // Scroll Up (Zoom In)
            camDist -= 2.0f;
            if (camDist < 5.0f) camDist = 5.0f;
        } else if (btn == 4) { // Scroll Down (Zoom Out)
            camDist += 2.0f;
            if (camDist > 33.2f) camDist = 33.2f;
        }
    }
    // ========================================
}
void motion(int xx,int yy){ (void)xx;(void)yy; }
void reshape(int w,int h){winW=w;winH=h;glViewport(0,0,w,h);}

// main: Program entry. Initializes GLUT and OpenGL state machine.
int main(int argc,char**argv){
    srand((unsigned)time(NULL)); memset(pkts,0,sizeof(pkts)); initAll();
    glutInit(&argc,argv); glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH|GLUT_MULTISAMPLE);
    glutInitWindowSize(winW,winH);

    glutCreateWindow("OptiFlux:Cross-Border Network Simulation");

    glEnable(GL_DEPTH_TEST); glEnable(GL_NORMALIZE); glShadeModel(GL_SMOOTH);
    glClearColor(.04f,.07f,.16f,1); glHint(GL_LINE_SMOOTH_HINT,GL_NICEST); glEnable(GL_LINE_SMOOTH);
    glutDisplayFunc(display); glutReshapeFunc(reshape); glutKeyboardFunc(keyboard);

    glutSpecialFunc(special); glutMouseFunc(mouse); glutMotionFunc(motion); glutTimerFunc(16,update,0);

    printf("\n+--------------------------------------------------------+\n");
    printf("|  OptiFlux:Cross-Border Network Simulation                    |\n");
    printf("|  Bangladesh (Green Village) <----> Singapore (City)    |\n");
    printf("+--------------------------------------------------------+\n");
    printf("|  S=Send  M=Burst  B=Break  R=Repair  D=Day/Night       |\n");
    printf("|  Z=Zoom In  X=Zoom Out (Or Mouse Wheel)                |\n");
    printf("|  +/- Speed  |  ESC=Exit                                |\n");
    printf("+--------------------------------------------------------+\n\n");
    glutMainLoop(); return 0;
}
