#include <iostream>
#include <cmath>

#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

// Store the width and height of the window
int width = 640, height = 640;

bool player1 = false;               // variable for player 1  -- bool value.
bool player2 = false;              // variable for player 2  -- bool value.
bool bot = false;                   // variable for ai    -- bool value.

int Box[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};   // box tracker set up for an array 9 spots of the 3 x 3 matrix.
int movecounter = 0;      // counter to check the counts as it moves along

//-------------------------------------------------------
// A function to draw the scene
//-------------------------------------------------------
void appDrawScene() {
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set background color to black
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// Set up the transformations stack
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    //TicTacToe Grid
    //---------------------------------------------
    glColor3f(0, 202, 231); //RGB
    glLineWidth(5);
    glBegin(GL_LINES);
    //Left Line
    glVertex2f(-0.15, 0.0);
    glVertex2f(-0.15, -0.9);
    //Right Line
    glVertex2f(0.15, 0.0);
    glVertex2f(0.15, -0.9);
    //Top Line
    glVertex2f(-0.55, -0.3);
    glVertex2f(0.55, -0.3);
    //Bottom Line
    glVertex2f(-0.55, -0.6);
    glVertex2f(0.55, -0.6);
    glEnd();
    //---------------------------------------------

    //Title
    //---------------------------------------------
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);//RGB
    
    glVertex2f(-0.75, .95);      // the T character
    glVertex2f(-.95, .95);
    glVertex2f(-.85, .95);
    glVertex2f(-.85, .75);
    
    glVertex2f(-.65, .95);    // the I character
    glVertex2f(-.65, .75);
    
    glVertex2f(-.55, .95);    // the C character
    glVertex2f(-.55, .75);
    glVertex2f(-.45, .95);
    glVertex2f(-.55, .95);
    glVertex2f(-.55, .75);
    glVertex2f(-.45, .75);
    
    
    glVertex2f(-0.15, .95);       // the T character
    glVertex2f(-.35, .95);
    glVertex2f(-.25, .95);
    glVertex2f(-.25, .75);
    
    glVertex2f(-.10, .75);   // the A character
    glVertex2f(.0, .95);
    glVertex2f(.0, .95);
    glVertex2f(.10, .75);
    glVertex2f(.05, .85);
    glVertex2f(-.05, .85);
    
    glVertex2f(.15, .95);    // the C character
    glVertex2f(.15, .75);
    glVertex2f(.25, .95);
    glVertex2f(.15, .95);
    glVertex2f(.15, .75);
    glVertex2f(.25, .75);
    
    
    glEnd();
    //---------------------------------------------

    //1 Player -- Mode
    //---------------------------------------------
    glColor3f(255, 0, 0);// polygon acting like a button for 1 player mode
    
    glBegin(GL_LINES);
    
    glVertex2f(-0.25, 0.3);   // ended up drawing 1 line to indicate that it is 1 player mode.
    glVertex2f(-0.25, 0.1);
    
    glEnd();

    //2 Player -- Mode
    //---------------------------------------------
   
    glColor3f(255, 0, 0);     // polygon acting a button for 2 player mode
    
    glBegin(GL_LINES);
    
    glVertex2f(0.35, 0.3);
    glVertex2f(0.35, 0.1);  // ended up drawing 2 lines to indicate that it is 2 player mode.
    glVertex2f(0.45, 0.3);
    glVertex2f(0.45, 0.1);
    
    glEnd();


    
	// We have been drawing everything to the back buffer
	// Swap the buffers to see the result of what we drew
	glFlush();
	glutSwapBuffers();
}

//-------------------------------------------------------
// A function to convert window coordinates to scene
// We use it when a mouse event is handled
// Arguments: 	
//	x, y - the coordinates to be updated
//-------------------------------------------------------
void windowToScene(float& x, float& y) {
	x = (2.0f*(x / float(width))) - 1.0f;
	y = 1.0f - (2.0f*(y / float(height)));
}

//-------------------------------------------------------
// A function to handle window resizing
// Called every time the window is resized
// Arguments: 	
//	b    - mouse button that was clicked, left or right
//	s 	 - state, either mouse-up or mouse-down
//	x, y - coordinates of the mouse when click occured
//-------------------------------------------------------
void appReshapeFunc(int w, int h) {
	// Window size has changed
	width = w;
	height = h;

	double scale, center;
	double winXmin, winXmax, winYmin, winYmax;

	// Define x-axis and y-axis range
	const double appXmin = -1.0;
	const double appXmax = 1.0;
	const double appYmin = -1.0;
	const double appYmax = 1.0;

	// Define that OpenGL should use the whole window for rendering
	glViewport(0, 0, w, h);

	// Set up the projection matrix using a orthographic projection that will
	// maintain the aspect ratio of the scene no matter the aspect ratio of
	// the window, and also set the min/max coordinates to be the disered ones
	w = (w == 0) ? 1 : w;
	h = (h == 0) ? 1 : h;

	if ((appXmax - appXmin) / w < (appYmax - appYmin) / h) {
		scale = ((appYmax - appYmin) / h) / ((appXmax - appXmin) / w);
		center = (appXmax + appXmin) / 2;
		winXmin = center - (center - appXmin)*scale;
		winXmax = center + (appXmax - center)*scale;
		winYmin = appYmin;
		winYmax = appYmax;
	}
	else {
		scale = ((appXmax - appXmin) / w) / ((appYmax - appYmin) / h);
		center = (appYmax + appYmin) / 2;
		winYmin = center - (center - appYmin)*scale;
		winYmax = center + (appYmax - center)*scale;
		winXmin = appXmin;
		winXmax = appXmax;
	}

	// Now we use glOrtho to set up our viewing frustum
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(winXmin, winXmax, winYmin, winYmax, -1, 1);
}

//-------------------------------------------------------
// A function to handle mouse clicks
// Called every time the mouse button goes up or down
// Arguments: 	
//	b    - mouse button that was clicked, left or right
//	s 	 - state, either mouse-up or mouse-down
//	x, y - coordinates of the mouse when click occured
//-------------------------------------------------------

void XsandOs() {
    if (player1 == true) {
        glColor3f(255,20,147);//RGB
        
        glLineWidth(4);
        glBegin(GL_LINES);
        glVertex2f(-0.9, -0.61);        // basically saying if it is player 1 turn we see a vertical line on left
        glVertex2f(-0.6, -0.61);
        glEnd();
    } else if (player1 == false) {
        glColor3f(0.0, 0.0, 0.0); //RGB
        
        glLineWidth(4);
        glBegin(GL_LINES);
        glVertex2f(-0.9, -0.61);            // basically saying if it is player 1 turn we see a vertical line on left
        glVertex2f(-0.6, -0.61);
        glEnd();
    }
    
    if (player2 == true) {
        glColor3f(255, 255, 0.0); //RGB
        
        glLineWidth(4);                         // If it is player 2 turn we see a vertical line on the right
        glBegin(GL_LINES);
        glVertex2f(0.9, -0.61);
        glVertex2f(0.6, -0.61);
        glEnd();
    } else if (player2 == false) {
        glColor3f(0.0, 0.0, 0.0); //RGB
        
        glLineWidth(4);
        glBegin(GL_LINES);
        glVertex2f(0.9, -0.61);          // If it is player 2 turn we see a vertical line on the right
        glVertex2f(0.6, -0.61);
        glEnd();
    }
    
    if (Box[0] == 1) {
        glColor3f(0.0, 1.0, 0.0); //RGB
        
        glLineWidth(5);                                 // tracker on array 0 for our 3 by 3 matrix
        glBegin(GL_LINES);                              // for player 1
        glVertex2f(-0.45, -0.05);
        glVertex2f(-0.35, -0.25);
        glVertex2f(-0.45, -0.25);
        glVertex2f(-0.35, -0.05);
        glEnd();
    } else if (Box[0] == 2) {
        glColor3f(1.0, 0.0, 0.0); //RGB
        
        glBegin(GL_LINES);
        glVertex2f(-0.45, -0.05);                   //// tracker on array 0 for our 3 by 3 matrix
        glVertex2f(-0.45, -0.25);                    // for player 2
        glVertex2f(-0.35, -0.05);
        glVertex2f(-0.35, -0.25);
        glVertex2f(-0.45, -0.05);
        glVertex2f(-0.35, -0.05);
        glVertex2f(-0.45, -0.25);
        glVertex2f(-0.35, -0.25);
        glEnd();
    }
    
    if (Box[1] == 1) {
        glColor3f(0.0, 1.0, 0.0); //RGB
       
        glLineWidth(5);
        glBegin(GL_LINES);
        glVertex2f(-0.1, -0.05);
        glVertex2f(0, -0.25);
        glVertex2f(-0.1, -0.25);
        glVertex2f(0, -0.05);
        glEnd();
    } else if (Box[1] == 2) {
        glColor3f(1.0, 0.0, 0.0); //RGB
        
        glBegin(GL_LINES);
        glVertex2f(-0.1, -0.05);                   //// tracker on array 0 for our 3 by 3 matrix
        glVertex2f(-0.1, -0.25);                    // for player 2
        glVertex2f(0, -0.05);
        glVertex2f(0, -0.25);
        glVertex2f(-0.1, -0.05);
        glVertex2f(0, -0.05);
        glVertex2f(-0.1, -0.25);
        glVertex2f(0, -0.25);
        glEnd();
    }
    
    if (Box[2] == 1) {
        glColor3f(0.0, 1.0, 0.0); //RGB
        
        glLineWidth(5);
        glBegin(GL_LINES);
        glVertex2f(0.45, -0.05);
        glVertex2f(0.35, -0.25);
        glVertex2f(0.45, -0.25);
        glVertex2f(0.35, -0.05);
        glEnd();
    } else if (Box[2] == 2) {
        glColor3f(1.0, 0.0, 0.0); //RGB
       
        glBegin(GL_LINES);
        glVertex2f(0.35, -0.05);                   //// tracker on array 0 for our 3 by 3 matrix
        glVertex2f(0.35, -0.25);                    // for player 2
        glVertex2f(0.25, -0.05);
        glVertex2f(0.25, -0.25);
        glVertex2f(0.35, -0.05);
        glVertex2f(0.25, -0.05);
        glVertex2f(0.35, -0.25);
        glVertex2f(0.25, -0.25);
    }
    
    if (Box[3] == 1) {
        glColor3f(0.0, 1.0, 0.0); //RGB
        
        glLineWidth(5);
        glBegin(GL_LINES);
        glVertex2f(-0.45, -0.35);
        glVertex2f(-0.35, -0.55);
        glVertex2f(-0.45, -0.55);
        glVertex2f(-0.35, -0.35);
        glEnd();
    } else if (Box[3] == 2) {
        glColor3f(1.0, 0.0, 0.0); //RGB
        
        glBegin(GL_LINES);
        glVertex2f(-0.45, -0.35);                   //// tracker on array 0 for our 3 by 3 matrix
        glVertex2f(-0.45, -0.55);                    // for player 2
        glVertex2f(-0.35, -0.35);
        glVertex2f(-0.35, -0.55);
        glVertex2f(-0.45, -0.35);
        glVertex2f(-0.35, -0.35);
        glVertex2f(-0.45, -0.55);
        glVertex2f(-0.35, -0.55);
    }
    
    if (Box[4] == 1) {
        glColor3f(0.0, 1.0, 0.0); //RGB
        
        glLineWidth(5);
        glBegin(GL_LINES);
        glVertex2f(-0.1, -0.35);
        glVertex2f(0, -0.55);
        glVertex2f(-0.1, -0.55);
        glVertex2f(0, -0.35);
        glEnd();
    } else if (Box[4] == 2) {
        glColor3f(1.0, 0.0, 0.0); //RGB
        
        glBegin(GL_LINES);
        glVertex2f(-0.1, -0.35);                   //// tracker on array 0 for our 3 by 3 matrix
        glVertex2f(-0.1, -0.55);                    // for player 2
        glVertex2f(0, -0.35);
        glVertex2f(0, -0.55);
        glVertex2f(-0.1, -0.35);
        glVertex2f(0, -0.35);
        glVertex2f(-0.1, -0.55);
        glVertex2f(0, -0.55);
        glEnd();    }
    
    if (Box[5] == 1) {
        glColor3f(0.0, 1.0, 0.0); //RGB
        
        glLineWidth(5);
        glBegin(GL_LINES);
        glVertex2f(0.35, -0.35);
        glVertex2f(0.45, -0.55);
        glVertex2f(0.35, -0.55);
        glVertex2f(0.45, -0.35);
        glEnd();
    } else if (Box[5] == 2) {
        glColor3f(1.0, 0.0, 0.0);
        
        glBegin(GL_LINES);//RGB
        glVertex2f(0.35, -0.35);                   //// tracker on array 0 for our 3 by 3 matrix
        glVertex2f(0.35, -0.55);                    // for player 2
        glVertex2f(0.25, -0.35);
        glVertex2f(0.25, -0.55);
        glVertex2f(0.35, -0.35);
        glVertex2f(0.25, -0.35);
        glVertex2f(0.35, -0.55);
        glVertex2f(0.25, -0.55);
    }
    
    if (Box[6] == 1) {
        glColor3f(0.0, 1.0, 0.0); //RGB
        
        glLineWidth(5);
        glBegin(GL_LINES);
        glVertex2f(-0.45, -0.65);
        glVertex2f(-0.35, -0.85);
        glVertex2f(-0.45, -0.85);
        glVertex2f(-0.35, -0.65);
        glEnd();
    } else if (Box[6] == 2) {
        glColor3f(1.0, 0.0, 0.0); //RGB
        
        glBegin(GL_LINES);
        glVertex2f(-0.45, -0.65);                   //// tracker on array 0 for our 3 by 3 matrix
        glVertex2f(-0.45, -0.85);                    // for player 2
        glVertex2f(-0.35, -0.65);
        glVertex2f(-0.35, -0.85);
        glVertex2f(-0.45, -0.65);
        glVertex2f(-0.35, -0.65);
        glVertex2f(-0.45, -0.85);
        glVertex2f(-0.35, -0.85);
        glEnd();
    }
    
    if (Box[7] == 1) {
        glColor3f(0.0, 1.0, 0.0); //RGB
        
        glLineWidth(5);
        glBegin(GL_LINES);
        glVertex2f(-0.1, -0.65);
        glVertex2f(0, -0.85);
        glVertex2f(-0.1, -0.85);
        glVertex2f(0, -0.65);
        glEnd();
    } else if (Box[7] == 2) {
        glColor3f(1.0, 0.0, 0.0); //RGB
        
        glBegin(GL_LINES);
        glVertex2f(-0.1, -0.65);                   //// tracker on array 0 for our 3 by 3 matrix
        glVertex2f(-0.1, -0.85);                    // for player 2
        glVertex2f(0, -0.65);
        glVertex2f(0, -0.85);
        glVertex2f(-0.1, -0.65);
        glVertex2f(0, -0.65);
        glVertex2f(-0.1, -0.85);
        glVertex2f(0, -0.85);
        glEnd();
    }
    
    if (Box[8] == 1) {
        glColor3f(0.0, 1.0, 0.0); //RGB
        
        glLineWidth(5);
        glBegin(GL_LINES);
        glVertex2f(0.35, -0.65);
        glVertex2f(0.45, -0.85);
        glVertex2f(0.35, -0.85);
        glVertex2f(0.45, -0.65);
        glEnd();
    } else if (Box[8] == 2) {
        glColor3f(1.0, 0.0, 0.0); //RGB
        
        glBegin(GL_LINES);
        glVertex2f(.35, -0.65);                   //// tracker on array 0 for our 3 by 3 matrix
        glVertex2f(.35, -0.85);                    // for player 2
        glVertex2f(.25, -0.65);
        glVertex2f(.25, -0.85);
        glVertex2f(.35, -0.65);
        glVertex2f(.25, -0.65);
        glVertex2f(.35, -0.85);
        glVertex2f(.25, -0.85);
        glEnd();

    }
}

bool WinnerChecker() {
    for (int i = 1; i <= 2; i++) {
        if (Box[0] == i && Box[1] == i && Box[2] == i) {   // horizontal winner
            std::cout << "player " << i << " winner" << std::endl;
            return true;
            
        } else if (Box[3] == i && Box[4] == i && Box[5] == i) {  // horizonatal winner
            std::cout << "player " << i << " winner" << std::endl;
            return true;
            
        } else if (Box[6] == i && Box[7] == i && Box[8] == i) {  // horizonatal winner
            std::cout << "player " << i << " winner" << std::endl;
            return true;
            
        } else if (Box[0] == i && Box[3] == i && Box[6] == i) {
            std::cout << "player " << i << " winner" << std::endl;
            return true;
            
        } else if (Box[1] == i && Box[4] == i && Box[7] == i) {
            std::cout << "player " << i << " winner" << std::endl;
            return true;
            
        } else if (Box[2] == i && Box[5] == i && Box[8] == i) {
            std::cout << "player" << i << " winner" << std::endl;
            return true;
            
        } else if (Box[0] == i && Box[4] == i && Box[8] == i) {
            std::cout << "player " << i << " winner" << std::endl;
            return true;
            
        } else if (Box[2] == i && Box[4] == i && Box[6] == i) {
            std::cout << "player" << i << " winner" << std::endl;
            return true;
            
        } else if (movecounter >= 18) {
            std::cout << "DRAW" << std::endl;
        }
    }
    return false;
}

void appMouseFunc(int b, int s, int x, int y) {
    // Convert from Window to Scene coordinates
    float mx = (float)x;
    float my = (float)y;

    windowToScene(mx, my);

    //if 1 player mode is clicked on
    if (mx >= -0.75 && mx <= -0.15 && my <= 0.5 && my >= 0.05){
        std::cout << "Gamemode: 1P" << std::endl;
        player1 = true;
        bot = true;
        Box[0] = 0;
        Box[1] = 0;
        Box[2] = 0;
        Box[3] = 0;
        Box[4] = 0;
        Box[5] = 0;
        Box[6] = 0;
        Box[7] = 0;
        Box[8] = 0;
        Box[9] = 0;
        movecounter = 0;
    }

    //if 2 player mode is clicked on
    if (mx >= 0.15 && mx <= 0.75 && my <= 0.5 && my >= 0.05){
        std::cout << "Gamemode: 2P" << std::endl;
        player1 = true;
        bot = false;
        Box[0] = 0;
        Box[1] = 0;
        Box[2] = 0;
        Box[3] = 0;
        Box[4] = 0;
        Box[5] = 0;
        Box[6] = 0;
        Box[7] = 0;
        Box[8] = 0;
        Box[9] = 0;
        movecounter = 0;
    }
    
    if (bot == true && player2 == true) {
        for (int i = 0; i <= 8; i++){
            int r = rand() % 9;
            if (Box[r] == 0){
                Box[r] = 2;
                break;
            }
        }
        player1 = true;
        player2 = false;
    }

    // the 1 box
    if (mx >= -0.45 && mx <= -0.2 && my <= -0.05 && my >= -0.25){
        if (Box[0] == 0){
            if (player1 == true){
                Box[0] = 1;
                player1 = false;
                player2 = true;
            } else {
                Box[0] = 2;
                player1 = true;
                player2 = false;
            }
        } else {
            std::cout << "choose a diffrent spot" << std::endl;
        }
        movecounter++;
    } // the 2 box
    if (mx >= -0.1 && mx <= 0.1 && my <= 0.05 && my >= -0.25){
        if (Box[1] == 0){
            if (player1 == true){
                Box[1] = 1;
                player1 = false;
                player2 = true;
            } else {
                Box[1] = 2;
                player1 = true;
                player2 = false;
            }
        }  else {
            std::cout << "choose a diffrent spot" << std::endl;
        }
        movecounter++;
    } // the 3 box
    if (mx >= 0.2 && mx <= 0.45 && my <= -0.05 && my >= -0.25){
        if (Box[2] == 0){
            if (player1 == true){
                Box[2] = 1;
                player1 = false;
                player2 = true;
            } else {
                Box[2] = 2;
                player1 = true;
                player2 = false;
            }
        }  else {
            std::cout << "choose a diffrent spot" << std::endl;
        }
        movecounter++;
    } // the 4 box
    if (mx >= -0.45 && mx <= -0.2 && my <= -0.35 && my >= -0.55){
        if (Box[3] == 0){
            if (player1 == true){
                Box[3] = 1;
                player1 = false;
                player2 = true;
            } else {
                Box[3] = 2;
                player1 = true;
                player2 = false;
            }
        }  else {
            std::cout << "choose a diffrent spot" << std::endl;
        }
        movecounter++;
    } // // the 5 box
    if (mx >= -0.1 && mx <= 0.1 && my <= -0.35 && my >= -0.55){
        if (Box[4] == 0){
            if (player1 == true){
                Box[4] = 1;
                player1 = false;
                player2 = true;
            } else {
                Box[4] = 2;
                player1 = true;
                player2 = false;
            }
        }  else {
            std::cout << "choose a diffrent spot" << std::endl;
        }
        movecounter++;
    } // the 6 box
    if (mx >= 0.2 && mx <= 0.45 && my <= -0.35 && my >= -0.55){
        if (Box[5] == 0){
            if (player1 == true){
                Box[5] = 1;
                player1 = false;
                player2 = true;
            } else {
                Box[5] = 2;
                player1 = true;
                player2 = false;
            }
        }  else {
            std::cout << "choose a diffrent spot" << std::endl;
        }
        movecounter++;
    } // the 7 box
    if (mx >= -0.45 && mx <= -0.2 && my <= -0.65 && my >= -0.85){
        if (Box[6] == 0){
            if (player1 == true){
                Box[6] = 1;
                player1 = false;
                player2 = true;
            } else {
                Box[6] = 2;
                player1 = true;
                player2 = false;
            }
        }  else {
            std::cout << "choose a diffrent spot" << std::endl;
        }
        movecounter++;
    } // the 8 box
    if (mx >= -0.1 && mx <= 0.1 && my <= -0.65 && my >= -0.85){
        if (Box[7] == 0){
            if (player1 == true){
                Box[7] = 1;
                player1 = false;
                player2 = true;
            } else {
                Box[7] = 2;
                player1 = true;
                player2 = false;
            }
        }  else {
            std::cout << "choose a diffrent spot" << std::endl;
        }
        movecounter++;
    } // the 9 box
    if (mx >= 0.2 && mx <= 0.45 && my <= -0.65 && my >= -0.85){
        if (Box[8] == 0){
            if (player1 == true){
                Box[8] = 1;
                player1 = false;
                player2 = true;
            } else {
                Box[8] = 2;
                player1 = true;
                player2 = false;
            }
        }  else {
            std::cout << "choose a diffrent spot" << std::endl;
        }
        movecounter++;
    }

    // Redraw the scene by calling appDrawScene above
    // so that the point we added above will get painted
    glutPostRedisplay();
}

void idle() {
    XsandOs();
    WinnerChecker();
    
    glFlush();
    glutSwapBuffers();
}


int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    
    // Setup window position, size, and title
    glutInitWindowPosition(20, 60);
    glutInitWindowSize(width, height);
    glutCreateWindow("CSE165 OpenGL Demo");
    
    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    
    
    // Set callback for drawing the scene
    glutDisplayFunc(appDrawScene);
    
    // Set callback for resizing th window
    glutReshapeFunc(appReshapeFunc);
    
    // Set callback to handle mouse clicks
    glutMouseFunc(appMouseFunc);
    
    glutIdleFunc(idle);
    
    // Start the main loop
    glutMainLoop();
}

