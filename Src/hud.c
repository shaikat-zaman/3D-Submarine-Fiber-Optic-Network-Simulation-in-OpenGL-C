#include "common.h"
#include <stdlib.h>

// Draw Unified Admin Panel (BSCCL + ISP01) - Beautiful & User Friendly
void drawAdminPanels() {
    int cx = winW / 2;
    int cy = winH / 2;

    // Dim background completely to focus on the panel
    glColor4f(0, 0, 0, 0.85f);
    glBegin(GL_QUADS); glVertex2f(0,0); glVertex2f(winW,0); glVertex2f(winW,winH); glVertex2f(0,winH); glEnd();

    // Made the panel slightly wider and taller for better spacing
    int w = 940, h = 440;
    int x = cx - w/2, y = cy - h/2;

    // Main Box Background
    glColor4f(0, .05f, .10f, 0.95f);
    glBegin(GL_QUADS); glVertex2f(x,y); glVertex2f(x+w,y); glVertex2f(x+w,y+h); glVertex2f(x,y+h); glEnd();

    // Glowing Neon Border
    glColor4f(0, 1.0f, 0.8f, 1.0f); glLineWidth(2);
    glBegin(GL_LINE_LOOP); glVertex2f(x,y); glVertex2f(x+w,y); glVertex2f(x+w,y+h); glVertex2f(x,y+h); glEnd(); glLineWidth(1);

    // Center Divider Line
    glColor4f(0, 1.0f, 0.8f, 0.3f);
    glBegin(GL_LINES); glVertex2f(x + 320, y + 30); glVertex2f(x + 320, y + h - 30); glEnd();


    // ==========================================
    // LEFT PANEL: BSCCL ADMIN
    // ==========================================
    glColor3f(0, 1.0f, 0.8f); glRasterPos2i(x + 30, y + h - 45);
    const char* ttl1 = "BSCCL ADMIN PANEL";
    for(;*ttl1;ttl1++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *ttl1);

    // Subtitle
    glColor3f(0.5f, 0.8f, 1.0f); glRasterPos2i(x + 30, y + h - 65);
    const char* sub1 = "Bangladesh Submarine Cable Co. Ltd.";
    for(;*sub1;sub1++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *sub1);

    // Polite Instruction Text
    glColor3f(0.9f, 0.9f, 1.0f); glRasterPos2i(x + 30, y + h - 115);
    const char* pr1 = "Please enter the desired bandwidth";
    for(;*pr1;pr1++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *pr1);

    glColor3f(0.9f, 0.9f, 1.0f); glRasterPos2i(x + 30, y + h - 135);
    const char* pr2 = "from Singapore (in Gbps):";
    for(;*pr2;pr2++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *pr2);

    // Input Box for Gbps
    if (menuEditMode == 3) glColor3f(0, 1.0f, 0.5f); else glColor3f(0.2f, 0.3f, 0.4f);
    glBegin(GL_LINE_LOOP); glVertex2f(x+30, y+h-190); glVertex2f(x+200, y+h-190); glVertex2f(x+200, y+h-150); glVertex2f(x+30, y+h-150); glEnd();

    char inputStr[30]; sprintf(inputStr, "%s %s", bscclGbps[0] ? bscclGbps : "0", (menuEditMode==3)?"_":"");
    if (menuEditMode == 3) glColor3f(0, 1.0f, 0.5f); else glColor3f(0.7f, 0.7f, 0.7f);
    glRasterPos2i(x + 40, y + h - 175);
    for(char* s = inputStr; *s; s++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *s);

    // Contextual Hint
    glColor3f(1.0f, 1.0f, 0.2f); glRasterPos2i(x + 30, y + h - 220);
    const char* gHint = "-> Press 'G' to edit Gbps input";
    for(;*gHint;gHint++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *gHint);


    // ==========================================
    // RIGHT PANEL: ISP01 ADMIN
    // ==========================================
    glColor3f(0, 1.0f, 0.8f); glRasterPos2i(x + 350, y + h - 45);
    const char* ttl2 = "ISP01 USER CONNECTION ROUTING PANEL";
    for(;*ttl2;ttl2++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *ttl2);

    int activeCount = 0; for(int i=0; i<8; i++) if(housesData[i].active) activeCount++;
    char st[50]; sprintf(st, "Currently Active Connections: %d / 8 Houses", activeCount);
    glColor3f(0.7f, 0.9f, 1.0f); glRasterPos2i(x + 350, y + h - 75);
    for(char* s = st; *s; s++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *s);

    const char* ptypNames[]={"HTTP (H)","Data (F)","Video (V)","VoIP (O)","Encrpt (E)"};

    // Render House List
    for(int i = 0; i < 8; i++) {
        int hy = y + h - 120 - (i * 30); // Row height spacing

        // Highlight selection row
        if (menuSelectedHouse == i) {
            glColor4f(0, 0.5f, 0.5f, 0.35f);
            glBegin(GL_QUADS); glVertex2f(x+340, hy-8); glVertex2f(x+910, hy-8); glVertex2f(x+910, hy+18); glVertex2f(x+340, hy+18); glEnd();
            glColor4f(0, 1.0f, 0.8f, 0.8f);
            glBegin(GL_LINES); glVertex2f(x+340, hy-8); glVertex2f(x+340, hy+18); glEnd(); // Left accent line
        }

        // Build display strings
        char ipDisp[30]; sprintf(ipDisp, "%s%s", housesData[i].ip[0] ? housesData[i].ip : "---.---.---.---", (menuSelectedHouse==i && menuEditMode==1)?"_":"");
        char bwDisp[30]; sprintf(bwDisp, "%s%s", housesData[i].bw[0] ? housesData[i].bw : "---", (menuSelectedHouse==i && menuEditMode==2)?"_":"");

        char typeDisp[30];
        if (menuSelectedHouse == i && menuEditMode == 4) {
            sprintf(typeDisp, "[Press H/F/V/O/E]");
        } else {
            sprintf(typeDisp, "%s", ptypNames[housesData[i].pktType]);
        }

        char hStr[150];
        sprintf(hStr, "HN%02d | IP: %-15s | BW: %-5s Mbps | Type: %-10s | %s", i+1, ipDisp, bwDisp, typeDisp, housesData[i].active ? "CONNECTED" : "OFFLINE");

        if(housesData[i].active) glColor3f(0.0f, 1.0f, 0.5f); else glColor3f(0.5f, 0.6f, 0.7f);
        glRasterPos2i(x + 355, hy);
        for(char* s = hStr; *s; s++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *s);
    }

    // Controls Legend at the bottom of the ISP panel
    glColor3f(1.0f, 0.9f, 0.2f); glRasterPos2i(x + 350, y + 65);
    const char* ctrl1 = "Nav: [Up/Down]  |  [I] Set IP  |  [B] Set BW  |  [T] Set Type  |  [C] Connect  |  [D] Disconnect";
    for(;*ctrl1;ctrl1++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *ctrl1);

    // Master Action Hint at the very bottom center
    glColor3f(0.0f, 1.0f, 0.5f);
    const char* hint = "[ Press ENTER to Apply Changes & Start Simulation ]      |      [ Press TAB to Open/Close Menu ]";
    int hintWidth = 500;
    glRasterPos2i(cx - hintWidth/2 + 30, y + 25);
    for(;*hint;hint++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *hint);
}

// drawHUD: Draws 2D UI overlay (Controls, Status Monitor, Packet Legend).
void drawHUD(void){
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0,winW,0,winH);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    // Call Admin panels if not running
    if (simState < 2) {
        drawAdminPanels();
        glDisable(GL_BLEND); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW); glPopMatrix();
        glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
        return;
    }

    // ==========================================
    // 1. CONTROLS MENU (Top Right)
    // ==========================================
    int pw=225, ph=205, px2=winW-pw-10, py2=10;
    glColor4f(0,.04f,.14f,.80f); glBegin(GL_QUADS); glVertex2f(px2,py2); glVertex2f(px2+pw,py2); glVertex2f(px2+pw,py2+ph); glVertex2f(px2,py2+ph); glEnd();
    glColor4f(0,.85f,1,.9f); glBegin(GL_LINE_LOOP); glVertex2f(px2,py2); glVertex2f(px2+pw,py2); glVertex2f(px2+pw,py2+ph); glVertex2f(px2,py2+ph); glEnd();

    const char*ctrl[]={"  [ CONTROLS ]", "  TAB - Admin Panel", "  S  - Send Packet","  M  - Multi Burst","  B  - Break Cable",
                       "  R  - Repair Cable","  D  - Day/Night","  ","  ESC  Exit"};
    for(int i=0;i<9;i++){
        glColor3f(i==0?0:.78f,i==0?.92f:.92f,i==0?1:.85f); glRasterPos2i(px2+8,py2+ph-22-i*20);
        for(const char*s=ctrl[i];*s;s++) glutBitmapCharacter(i==0?GLUT_BITMAP_HELVETICA_18:GLUT_BITMAP_HELVETICA_12,*s);
    }

    // ==========================================
    // 2. BSCCL LIVE MONITOR
    // ==========================================
    int statW=320, statH=210;
    int statX=px2-statW-10, statY=10;

    glColor4f(0,.04f,.14f,.80f); glBegin(GL_QUADS); glVertex2f(statX,statY); glVertex2f(statX+statW,statY); glVertex2f(statX+statW,statY+statH); glVertex2f(statX,statY+statH); glEnd();
    glColor4f(0,.85f,1,.90f); glLineWidth(2); glBegin(GL_LINE_LOOP); glVertex2f(statX,statY); glVertex2f(statX+statW,statY); glVertex2f(statX+statW,statY+statH); glVertex2f(statX,statY+statH); glEnd(); glLineWidth(1);

    glColor3f(0,.92f,1); glRasterPos2i(statX+15, statY+statH-22);
    const char*ttl="  BSCCL LIVE MONITOR  "; for(;*ttl;ttl++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ttl);
    glColor4f(0,.6f,.8f,.6f); glBegin(GL_LINES); glVertex2f(statX+10,statY+statH-32); glVertex2f(statX+statW-10,statY+statH-32); glEnd();

    // --- LOGICAL BANDWIDTH CALCULATION ---
    float totalCapGbps = atof(bscclGbps);
    float totalCapMbps = totalCapGbps * 1000.0f; // Convert Gbps to Mbps
    float usedMbps = 0.0f;

    if (cableOK) { // If cable is broken, usage drops to 0
        for(int i=0; i<8; i++) {
            if(housesData[i].active && strlen(housesData[i].bw) > 0) {
                usedMbps += atof(housesData[i].bw);
            }
        }
    }

    float remainMbps = totalCapMbps - usedMbps;
    if(remainMbps < 0) remainMbps = 0; // Prevent negative remaining
    float loadPct = (totalCapMbps > 0) ? (usedMbps / totalCapMbps) * 100.0f : 0.0f;

    // Monitor Data
    const char* labels[]={"Cable Status:", "Total Supply:", "ISP01 Usage:", "Available Cap:", "Network Load:"};
    float clrs[5][3]={{cableOK?0:1, cableOK?1:.2f, .2f}, {0.0f, 1.0f, 0.8f}, {1.0f, 0.8f, 0.0f}, {0.0f, 1.0f, 0.4f}, {loadPct>80?1:0, loadPct>80?0.2f:1.0f, 0.2f}};
    const char* vals[5]; char bufs[5][32];

    vals[0] = cableOK ? "CONNECTED & SECURE" : "WARNING: CABLE BROKEN!";
    sprintf(bufs[1], "%.1f Gbps", totalCapGbps); vals[1]=bufs[1];
    sprintf(bufs[2], "%.1f Mbps", usedMbps); vals[2]=bufs[2];
    sprintf(bufs[3], "%.1f Mbps", remainMbps); vals[3]=bufs[3];
    sprintf(bufs[4], "%.1f %%", loadPct); vals[4]=bufs[4];

    // Draw Monitor Text
    for(int i=0; i<5; i++){
        glColor3f(.65f,.78f,.88f); glRasterPos2i(statX+15, statY+statH-55-i*22);
        for(const char*s=labels[i];*s;s++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*s);
        glColor3f(clrs[i][0],clrs[i][1],clrs[i][2]); glRasterPos2i(statX+130, statY+statH-55-i*22);
        for(const char*s=vals[i];*s;s++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*s);
    }

    // ==========================================
    // 3. ISP01 LIVE MONITOR TABLE (Bottom Left Corner)
    // ==========================================
    int tbW = 340, tbH = 180, tbX = 10, tbY = 10; // Placed at the bottom
    glColor4f(0,.04f,.14f,.80f); glBegin(GL_QUADS); glVertex2f(tbX,tbY); glVertex2f(tbX+tbW,tbY); glVertex2f(tbX+tbW,tbY+tbH); glVertex2f(tbX,tbY+tbH); glEnd();
    glColor4f(0, 1.0f, 0.8f, 0.9f); glBegin(GL_LINE_LOOP); glVertex2f(tbX,tbY); glVertex2f(tbX+tbW,tbY); glVertex2f(tbX+tbW,tbY+tbH); glVertex2f(tbX,tbY+tbH); glEnd();

    glColor3f(0, 1.0f, 0.8f); glRasterPos2i(tbX + 10, tbY + tbH - 20);
    const char* tbTtl = "ISP01 LIVE MONITOR"; for(;*tbTtl;tbTtl++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *tbTtl);

    // Headers
    glColor3f(0.8f, 0.9f, 1.0f); glRasterPos2i(tbX + 10, tbY + tbH - 45);
    const char* hds = "House No  |  IP Address       |  Bandwidth  |  Packet";
    for(;*hds;hds++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *hds);

    // Dynamic Data Rows
    for(int i = 0; i < 8; i++) {
        if (!housesData[i].active) continue; // Only show active houses

        int ry = tbY + tbH - 65 - (i * 14);

        // Wire Color Indicator Box (Exactly matches the house packet color)
        glColor3f(housesData[i].currR, housesData[i].currG, housesData[i].currB);
        glBegin(GL_QUADS); glVertex2f(tbX+270, ry-2); glVertex2f(tbX+290, ry-2); glVertex2f(tbX+290, ry+8); glVertex2f(tbX+270, ry+8); glEnd();

        // Data text
        glColor3f(0.6f, 1.0f, 0.6f);
        char row[100];
        sprintf(row, "HN%02d        %-15s    %-5s Mbps", i+1, housesData[i].ip, housesData[i].bw);
        glRasterPos2i(tbX + 10, ry);
        for(char* s = row; *s; s++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *s);
    }


    // ==========================================
    // 4. PACKET LEGEND (Placed right next to ISP01 Monitor)
    // ==========================================
    float pclrs[][3]={{0,.85f,1},{0,1,.4f},{1,.85f,0},{1,.45f,0},{.85f,0,1}};
    const char*ptyp[]={"HTTP/Web","Data File","Video","VoIP","Encrypted"};

    // Position setup: Right next to ISP01 Monitor with 15px gap
    int legX = tbX + tbW + 15;
    int legY = tbY;
    int legW = 160;
    int legH = 138;

    glColor4f(0,.04f,.14f,.78f); glBegin(GL_QUADS); glVertex2f(legX, legY); glVertex2f(legX+legW, legY); glVertex2f(legX+legW, legY+legH); glVertex2f(legX, legY+legH); glEnd();
    glColor4f(0,.85f,1,.9f); glBegin(GL_LINE_LOOP); glVertex2f(legX, legY); glVertex2f(legX+legW, legY); glVertex2f(legX+legW, legY+legH); glVertex2f(legX, legY+legH); glEnd();

    glColor3f(0,.92f,1); glRasterPos2i(legX+10, legY+118); const char*leg="Packet Types:"; for(;*leg;leg++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*leg);
    for(int i=0;i<5;i++){
        glColor3f(pclrs[i][0],pclrs[i][1],pclrs[i][2]);
        glBegin(GL_QUADS); glVertex2f(legX+10, legY+98-i*21); glVertex2f(legX+24, legY+98-i*21); glVertex2f(legX+24, legY+110-i*21); glVertex2f(legX+10, legY+110-i*21); glEnd();
        glColor3f(.8f,.9f,1); glRasterPos2i(legX+32, legY+101-i*21); for(const char*s=ptyp[i];*s;s++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*s);
    }

    glDisable(GL_BLEND); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW); glPopMatrix();
    glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
}
