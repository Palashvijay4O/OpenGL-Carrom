#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include <sys/time.h> 
using namespace std;
#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)
#define n 8
double ti;
// Function Declarations
int pause=0,lost=0;
int checkCollision();
void drawScene();
void update(int value);
void initRendering();
void handleResize(int w, int h);
void handleKeypress1(unsigned char key, int x, int y);
void handleKeypress2(int key, int x, int y);
void handleMouseclick(int button, int state, int x, int y);
void checkPoketing();
void winscene();
void draw(int x,int y);
int tempo=0,foul=0;
int windowWidth ;
int windowHeight;
// Global Variables
int collisionDetector=0,score=30,win=0;
float white=4.5f,black=5.0f,queen=5.5f;
float white_hood=0.0f,black_hood=0.0f,queen_hood=0.0f;
int i=0,j=0,k=0,myiterator1=0,myiterator2=0;
float arrowhead_x=1.0f;
float arrowhead_y=1.0f;
float friction[n] ;
float ball_rad[n];
float box_len = 6.0f;
float ball_x[n],ball_y[n];
float ball_velx[n] ;
float ball_vely[n] ;

int theta = 30, theta1=0,theta2=((360)/(n-2)); 
float hole = box_len/2-0.25f-0.16f;
float hole_rad = 0.22;
float outer_circle_rad = 0.75f ;
float inner_circle_rad = 0.50f;
float hole_x[] = {-hole, hole, -hole, hole};
float hole_y[] = {hole, hole, -hole, -hole};
float power_length = 5.0f;
float power_breadth = 1.0f;
float hit_breadth = 1.0f;
float hit_length = 0.0f;
float velocity = 0.05f;
float x_component=1.0f*cos(DEG2RAD(theta));
float y_component=1.0f*sin(DEG2RAD(theta));
int count=0,globalflag=0;
void renderBitmapString(float x, float y, void *font,const char *string){
    const char *c;
    glRasterPos2f(x, y);
    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
} 
class Board {
    private:
        float board_len;
        void drawBoard(float board_len,float r , float g, float b,int flag) {
            if (flag==1)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            else
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            glBegin(GL_QUADS);
            glColor3f(r , g, b);
            glVertex2f(-board_len / 2, -board_len / 2);
            glVertex2f(board_len / 2, -board_len / 2);
            glVertex2f(board_len / 2, board_len / 2);
            glVertex2f(-board_len / 2, board_len / 2);
            glEnd();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        }
    public:
        void setParameters(float len,float r , float g, float b,int flag) {
            board_len = len;
            drawBoard(board_len,r,g,b,flag);
        }

};
class Rectangle {
    public:
        void drawRectangle(float x , float y , float r, float g, float b,int flag) {
            if (flag == 0)
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            else 
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            glBegin(GL_QUADS);
            glColor3f(r , g, b);
            glVertex2f(-x / 2, -y / 2);
            glVertex2f(x / 2, -y / 2);
            glVertex2f(x / 2, y / 2);
            glVertex2f(-x / 2, y / 2);
            glEnd();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
};
class Circle {
        private:
            float circle_rad;
            float circle_r;
            float circle_g;
            float circle_b;
            float vel_x;
            float vel_y;
            void drawCircle(float circle_rad, float circle_r,float circle_g,float circle_b , int flag)
             {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                if (flag == 1) 
                    glBegin(GL_LINE_STRIP);
                else 
                    glBegin(GL_TRIANGLE_FAN);
                glColor3f(circle_r,circle_g,circle_b);
                for(int i=0 ; i<360 ; i++) {
                    glVertex2f(circle_rad * cos(DEG2RAD(i)), circle_rad * sin(DEG2RAD(i)));
                }
                glEnd();
                //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        public:
            float mass;
            void setParameters(float rad , float r , float g, float b,float value,int flag) {
                circle_rad = rad;
                circle_r = r;
                circle_g = g;
                circle_b = b;
                mass = value;
                drawCircle(circle_rad ,circle_r,circle_g,circle_b,flag);
            }
            float get_direction()
            {
                if (abs(ball_velx[i]) == 0.0f)
                    return PI/2;
                else if(abs(ball_velx[i]-0.00) > 0.001f)
                    return atan(ball_vely[i]/ball_velx[i]);
                return PI/2;
            }
};

class Line {
public: 
    void drawLine(float x , float y , float length,float r,float g,float b,float linewidth,int flag) {
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glLineWidth((linewidth));  
    glBegin(GL_LINES);
    glColor3f(r,g,b);
    glPushMatrix();
    glTranslatef(x,y,0.0f);    
    if (flag == 0) {
        glVertex2f(length/2,length/2);
        glVertex2f(-length/2,-length/2);
    }
    else {
        glVertex2f(length/2,-length/2);
        glVertex2f(-length/2,length/2);
    }
    
    glPopMatrix();
    glEnd();
    glLineWidth(1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  
    }
};
void drawScore(int p) {
int  a,temp;  
 float space=0.1f;  
        tempo=(tempo+1)%60;
            if (tempo > 20){     
        if (p >0){
            int temp =p;
            
            for(int qq=0;temp>0;qq++)
            {
           
                glPushMatrix();
                a=temp%10;
              //  cout << a << endl;
                temp/=10;
                glColor3f(0.000, 0.000, 0.502);
                glRasterPos2f( 5.0f-qq*space,2.5f);
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 48+a);
                glPopMatrix();
            }
        
        }
        else if (p==0)
        {
            glColor3f(0.000, 0.000, 0.804);
            glRasterPos2f(5.0f-1*space, 2.5f);
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '0'); 
        }
            else {
            temp = -1*p;
            glColor3f(1.000, 0.271, 0.000);
            glRasterPos2f( 5.0f-2*space, 2.5f);
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '-');
            for(int qq=0;temp>0;qq++)
            {
            //    cout << temp << endl;
                glPushMatrix();
                a=temp%10;
              //  cout << a << endl;
                temp/=10;
                glColor3f(1.000, 0.271, 0.000);
                glRasterPos2f( 5.0f-qq*space, 2.5f);
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 48+a);
                glPopMatrix();
            }
        }
}
}
void handleCollision();
Circle ball[n];
void begin() {
    foul=0;
    pause=0;
    globalflag=0;
     int iterator;
    ball_rad[0] = 0.17f;
    for(iterator=1;iterator<n;iterator++)
        ball_rad[iterator]=0.12f;
    ball_x[0] = 0.0f;
    ball_y[0] = -(box_len-2.0f)/2 + ball_rad[0];
    ball_x[1]=0.0f;
    ball_y[1]=0.0f;
    for (iterator=2;iterator<n;iterator=iterator+2)
    {
        ball_x[iterator] = outer_circle_rad*cos(DEG2RAD(theta1));
        ball_y[iterator] = outer_circle_rad*sin(DEG2RAD(theta1));
        theta1=theta1+((360*2)/(n-2));
    }
      for (iterator=3;iterator<n;iterator=iterator+2)
    {
        ball_x[iterator] = outer_circle_rad*cos(DEG2RAD(theta2));
        ball_y[iterator] = outer_circle_rad*sin(DEG2RAD(theta2));
        theta2=theta2+((360*2)/(n-2));
    }
    hole = box_len/2-0.25f-0.16f;
    friction[0] = 0.0007f;
    for (iterator=1;iterator<n;iterator++)
        friction[iterator]=0.0009f;
    score = 30;
}
int main(int argc, char **argv) {
    begin();
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    int w = glutGet(GLUT_SCREEN_WIDTH);
    int h = glutGet(GLUT_SCREEN_HEIGHT);
    int windowWidth = w ;
    int windowHeight = h ;

    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);
    glutCreateWindow("Carrom Game");  // Setup the window
    initRendering();

    // Register callbacks
   
    glutDisplayFunc(drawScene);
    glutIdleFunc(drawScene);
   
    glutKeyboardFunc(handleKeypress1);
    glutSpecialFunc(handleKeypress2);
    glutMouseFunc(handleMouseclick);
    glutMotionFunc(draw);
    glutReshapeFunc(handleResize);
    if (pause%2==0)
    glutTimerFunc(1, update, 0);

    glutMainLoop();
    struct timeval tim;  
    gettimeofday(&tim, NULL);  
    ti=tim.tv_sec;
    return 0;
}
void winscene() {

}

// Function to draw objects on the screen
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();


    // Draw Board
    glTranslatef(0.0f, 0.0f, -8.0f);
    // Board Outer Wall
    Board board_outer;
    board_outer.setParameters(box_len,0.22f,0.16f,0.05f,1);
    
    // Board Inner Wall 
    Board board_inner;
    board_inner.setParameters(box_len-0.5f,0.957, 0.643, 0.376,1);
    
    if (win ==n-1){
      glColor3f(1.0f,1.0f,0.0f);
      renderBitmapString(-1.0f,2.8f,GLUT_BITMAP_TIMES_ROMAN_24,"Congrats You Win!!");
      renderBitmapString(-1.2f,2.6f,GLUT_BITMAP_TIMES_ROMAN_24,"To Play again press 'o'!");
      pause++;
      lost=1;
    }
    else if (foul== 3) {
        glColor3f(1.0f,1.0f,0.0f);
        renderBitmapString(-1.0f,2.8f,GLUT_BITMAP_TIMES_ROMAN_24,"Game Over!! ");
       renderBitmapString(-1.2f,2.6f,GLUT_BITMAP_TIMES_ROMAN_24,"To Restart press 'o'");
        pause++;
        lost=1;
    }
    else if ( score > -10 && score < 0 ){
        glColor3f(1.0f,1.0f,0.0f);
        renderBitmapString(-1.0f,2.8f,GLUT_BITMAP_TIMES_ROMAN_24,"Score Low !!");
    }
     else if ( score > -36 && score < -15){
        glColor3f(1.0f,1.0f,0.0f);
        renderBitmapString(-1.0f,2.8f,GLUT_BITMAP_TIMES_ROMAN_24,"Are you Sleeping.....??");
    }
    else if (score == -36){
        glColor3f(1.0f,1.0f,0.0f);
        renderBitmapString(-1.0f,2.8f,GLUT_BITMAP_TIMES_ROMAN_24,"4");
    }
    else if (score == -37){
        glColor3f(1.0f,1.0f,0.0f);
        renderBitmapString(-1.0f,2.8f,GLUT_BITMAP_TIMES_ROMAN_24,"3");
    }
    else if (score == -38){
        glColor3f(1.0f,1.0f,0.0f);
        renderBitmapString(-1.0f,2.8f,GLUT_BITMAP_TIMES_ROMAN_24,"2");
    }
    else if (score == -39){
        glColor3f(1.0f,1.0f,0.0f);
        renderBitmapString(-1.0f,2.8f,GLUT_BITMAP_TIMES_ROMAN_24,"1");
    }
    else if (score <= -40) {
        glColor3f(1.0f,1.0f,0.0f);
        renderBitmapString(-1.0f,2.80f,GLUT_BITMAP_TIMES_ROMAN_24,"Low Score. Game Over!! ");
        renderBitmapString(-1.2f,2.67f,GLUT_BITMAP_TIMES_ROMAN_24,"To Restart press 'o'");
        pause++;
        lost=1;
    }

    glPushMatrix();

    glPopMatrix();
    Circle ball_center_outest;
    ball_center_outest.setParameters(outer_circle_rad+0.05f, 1.0f, 0.0f, 0.0f,0.0f,1);
    glLineWidth((2.0f));
    //Center Outer Circle
    Circle ball_center_outer;
    ball_center_outer.setParameters(outer_circle_rad, 1.0f, 0.0f, 0.0f,0.0f,1);
    glLineWidth(1.0f);
    // Center Inner Circle
    Circle ball_center_inner ;
    ball_center_inner.setParameters(inner_circle_rad, 1.0f, 0.0f, 0.0f,0.0f,1);

    // Center Center Circle
    Circle ball_center_center;
    ball_center_center.setParameters(0.13f, 1.0f, 0.0f, 0.0f,0.0f,0);

    //Board Striking Position
     glLineWidth(2.5f);
    Board striking_position;
    striking_position.setParameters(box_len-2.0f,0.0f,0.0f,0.5f,0);
     glLineWidth(1.0f);

//Board Striking Position
     glLineWidth(1.5f);
    Board striking_position1;
    striking_position1.setParameters(box_len-2.0f-2.9*ball_rad[0],0.0f,0.0f,0.5f,0);
     glLineWidth(1.0f);

     Circle b1,b2,b3,b4;
     glPushMatrix();
     glTranslatef(-1.844f,-1.844f,0.0f);
     Line l1;
     l1.drawLine(0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,3.0f,0);
     b1.setParameters(0.16f,1.0f,1.0f,1.0f,0.0f,1);
     b1.setParameters(0.15f,0.502f,0.0f,0.0f,0.0f,0);  
     glPopMatrix();
     glPushMatrix();
     glTranslatef(1.844f,-1.844f,0.0f);
     Line l2;
     l2.drawLine(0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,3.0f,1);
     b2.setParameters(0.16f,1.0f,1.0f,1.0f,0.0f,1);
     b2.setParameters(0.15f,0.502f,0.0f,0.0f,0.0f,0);  
     glPopMatrix();
     glPushMatrix();
     glTranslatef(-1.844f,1.844f,0.0f);
     Line l3;
     l3.drawLine(0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,3.0f,1);
     b3.setParameters(0.16f,1.0f,1.0f,1.0f,0.0f,1);
     b3.setParameters(0.15f,0.502f,0.0f,0.0f,0.0f,0);
     glPopMatrix();
     glPushMatrix();
     glTranslatef(1.844f,1.844f,0.0f);
     Line l4;
     l4.drawLine(0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,3.0f,0);
     b4.setParameters(0.16f,1.0f,1.0f,1.0f,0.0f,1);
     b4.setParameters(0.15f,0.502f,0.0f,0.0f,0.0f,0);  
     glPopMatrix();


    //Draw Arrow
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glLineWidth((3.0f));  
    glBegin(GL_LINES);
    glColor3f(0.0f ,0.0f,0.0f);
    glPushMatrix();
    
    glVertex2f(4.5f,-2.0f);    
    glVertex2f(x_component+4.5f,y_component-2.0f);
    glPopMatrix();
    glEnd();
    glLineWidth(1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  
    x_component=1.0f*cos(DEG2RAD(theta));
    y_component=1.0f*sin(DEG2RAD(theta));
    
    
    int holeiterator;
    for (holeiterator=0;holeiterator<4;holeiterator++) {   
        glPushMatrix();
        glTranslatef(hole_x[holeiterator],hole_y[holeiterator],0.0f);
        Circle holeTopLeft;
        holeTopLeft.setParameters(hole_rad,0.0f,0.0f,0.0f,0.0f,0);
        glPopMatrix();
    }

    // Implementing Power meter. 
    glPushMatrix();
    glTranslatef(-5.0f,0.0f,0.0f);
    Rectangle Power;
    Power.drawRectangle(power_breadth,power_length,1.0f,1.0f,1.0f,0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-5.0f,(hit_length-power_length)/2,0.0f);
    Rectangle Hit;
    if(hit_length < 0.6f)
    Hit.drawRectangle(hit_breadth,hit_length,0.0f,1.0f,0.0f,0);
    else if (hit_length < 1.2)
         Hit.drawRectangle(hit_breadth,hit_length,0.1f,0.9f,0.0f,0);
    else if (hit_length <2.0f)
         Hit.drawRectangle(hit_breadth,hit_length,0.2f,0.8f,0.0f,0);
     else if (hit_length <3.0f)
          Hit.drawRectangle(hit_breadth,hit_length,0.3f,0.7f,0.0f,0);
      else if (hit_length<4.0f)
         Hit.drawRectangle(hit_breadth,hit_length,0.5f,0.6f,0.0f,0);
     else if(hit_length < 4.5f)
         Hit.drawRectangle(hit_breadth,hit_length,0.6f,0.5f,0.0f,0);
     else 
         Hit.drawRectangle(hit_breadth,hit_length,1.0f,0.0f,0.0f,0);
    glPopMatrix();
    glPushMatrix();
    glLineWidth(2.0f);
    glTranslatef(white,0.8f,0.0f);
    Rectangle black_hood_outer;
    black_hood_outer.drawRectangle(0.32f,2.0f,0.0f,0.0f,0.0f,1);
    glPopMatrix();
     glPushMatrix();
    glTranslatef(black,0.8f,0.0f);
    Rectangle white_hood_outer;
    white_hood_outer.drawRectangle(0.32f,2.0f,1.0f,1.0f,1.0f,1);
    glPopMatrix();
     glPushMatrix();
    glTranslatef(queen,0.8f,0.0f);
    Rectangle queen_hood_outer;
    queen_hood_outer.drawRectangle(0.32f,2.0f, 0.80f, 0.0f, 0.6f,1);
    glPopMatrix();
    glLineWidth(1.0f);

    glPushMatrix();
    glTranslatef(ball_x[1] ,ball_y[1], 0.0f);
    ball[1].setParameters(ball_rad[1], 0.80f, 0.0f, 0.6f,1.0f,0);
    glPopMatrix();
    // Black Balls
    for (holeiterator=2;holeiterator<n;holeiterator=holeiterator+2) {
    glPushMatrix();
    glTranslatef(ball_x[holeiterator] ,ball_y[holeiterator], 0.0f);
    ball[holeiterator].setParameters(ball_rad[holeiterator], 0.0f, 0.0f, 0.0f,1.0f,0);
    glPopMatrix();
    }

    for (holeiterator=3;holeiterator<n;holeiterator=holeiterator+2) {
    glPushMatrix();
    glTranslatef(ball_x[holeiterator] ,ball_y[holeiterator], 0.0f);
    ball[holeiterator].setParameters(ball_rad[holeiterator], 1.0f, 1.0f, 1.0f,1.0f,0);
    glPopMatrix();
    }


    // Draw Striker
    glPushMatrix();
    glTranslatef(ball_x[0] ,ball_y[0], 0.0f);
    ball[0].setParameters(ball_rad[0], 0.0f, 0.8f, 1.0f,2.0f,0);
    glPopMatrix();

    drawScore(score);
    glColor3f(0.863, 0.078, 0.235);
    renderBitmapString(4.2,2.5,GLUT_BITMAP_TIMES_ROMAN_24,"Score:");
    glColor3f(0.545, 0.000, 0.000);
    renderBitmapString(4.3,-0.45,GLUT_BITMAP_TIMES_ROMAN_24,"Coins Earned");
    glColor3f(1.000, 0.412, 0.706);
    renderBitmapString(-1.0,-2.93,GLUT_BITMAP_TIMES_ROMAN_24,"OpenGL Carrom Board");
    glColor3f(1.000, 0.412, 0.706);
    renderBitmapString(4.0,-2.5,GLUT_BITMAP_TIMES_ROMAN_24,"Fouls:");
    if (foul == 0)
       renderBitmapString(4.7,-2.5,GLUT_BITMAP_TIMES_ROMAN_24,"0"); 
   if (foul == 1)
       renderBitmapString(4.7,-2.5,GLUT_BITMAP_TIMES_ROMAN_24,"1");
    if (foul == 2)
       renderBitmapString(4.7,-2.5,GLUT_BITMAP_TIMES_ROMAN_24,"2");
       if (foul == 3)
       renderBitmapString(4.7,-2.5,GLUT_BITMAP_TIMES_ROMAN_24,"3");   
    glPopMatrix();
    glutSwapBuffers();
}

// Function to handle all calculations in the scene
// updated evry 10 milliseconds
void update(int value) {
    
  for (i=0;i<n;i++){
    if(abs(ball_vely[i])<0.001f ){
        ball_vely[i]=0.0f;
    }
    if(abs(ball_velx[i])<0.001f ){       
            ball_velx[i]=0.0f;
    }

    if(abs(ball_velx[i]) > 0.001f)
        ball_velx[i] = ball_velx[i] - (ball_velx[i]/abs(ball_velx[i]))*friction[i]*abs(cos(ball[i].get_direction()));
    if(abs(ball_vely[i]) > 0.001f)
        ball_vely[i] = ball_vely[i] - (ball_vely[i]/abs(ball_vely[i]))*friction[i]*abs(sin(ball[i].get_direction())); 

    // Handle ball collisions with box
    if(ball_x[i] + ball_rad[i] > box_len/2 - 0.3f || ball_x[i] - ball_rad[i] < -box_len/2+ 0.3f)
        ball_velx[i] *= -1.0;
    
    if(ball_y[i] + ball_rad[i] > box_len/2 -0.3f || ball_y[i] - ball_rad[i] < -box_len/2+0.3f)
        ball_vely[i] *= -1.0;
    
    // Update position of ball 
    if (abs(ball_velx[i]  ) > 0.0001f) 
        ball_x[i] += ball_velx[i];
    
    if(abs(ball_vely[i] ) > 0.0001f)
        ball_y[i] += ball_vely[i];
    // Implemented Power meter.    
}

    if(hit_length -power_length> 0.01f)
        velocity *=-1;
    if(hit_length-0.0f < 0.01f)
        velocity*=1;
    hit_length+=velocity; 
    if(hit_length+velocity < 0.01f) {
        hit_length=0.0f;
        velocity*=-1;
    }
    for (j=0;j<n;j++) {
        for(k=j+1;k<n;k++) {
        collisionDetector=checkCollision();
        if (collisionDetector==1 ) {
            handleCollision();
        }
    }
}
    checkPoketing();
    int z;
    count=0;
    for (z=0;z<n;z++) {
        if (abs(ball_velx[z]) == 0.0f && abs(ball_vely[z]) == 0.0f) {
            count++;
            if (count == n && globalflag == 1) {
            if (abs(ball_x[0]) > (box_len-2.0f)/2)
            ball_x[0]=0.0f;
            ball_y[0]=-(box_len-2.0f)/2 + ball_rad[0];
            globalflag = 0;
            }
        }
    }

    struct timeval tim;  
    gettimeofday(&tim, NULL);  
    double tf=tim.tv_sec;
    if(tf-ti>=1)
    {
        ti=tim.tv_sec;
        score--;
    }  
    if (pause%2==0)
    glutTimerFunc(1, update, 0);
}
void handleCollision() {
        float k_x,k_y,k_mod,u_x,u_y,a_x,a_y,a,m1,m2;
        m1=ball[j].mass;
        m2=ball[k].mass;
        float t1=ball_x[j] - ball_x[k],t3=ball_y[j] - ball_y[k];
        k_mod = sqrt(t1*t1+t3*t3);
        k_x = ball_x[j]-ball_x[k];   //x
        k_y = ball_y[j]-ball_y[k];  //y
        u_x = ball_velx[j]-ball_velx[k];            
        u_y = ball_vely[j]-ball_vely[k];        
        a_x = (2*k_x*u_x)/(k_mod * (1/m1+1/m2));
        a_y = (2*k_y*u_y)/(k_mod * (1/m1+1/m2));
        a=a_x+a_y;
        ball_velx[j] -= ((a/m1)*k_x)/k_mod;
        ball_vely[j] -= ((a/m1)*k_y)/k_mod;
        ball_velx[k] += ((a/m2)*k_x)/k_mod;
        ball_vely[k] += ((a/m2)*k_y)/k_mod;
        ball_x[j] += ball_velx[j];
        ball_y[j] += ball_vely[j];
        ball_x[k] += ball_velx[k];
        ball_y[k] += ball_vely[k];
}
int checkCollision() {
    if (((ball_x[j] - ball_x[k])*(ball_x[j] - ball_x[k])+(ball_y[j] - ball_y[k])*(ball_y[j] - ball_y[k]) < 
        (ball_rad[j]+ball_rad[k])*(ball_rad[j]+ball_rad[k]) )) 
        return 1;
    return 0;
}
// Initializing some openGL 3D rendering options
void initRendering() {
    // Enable objects to be drawn ahead/behind one another
    glEnable(GL_DEPTH_TEST); 
    // Enable coloring       
    glEnable(GL_COLOR_MATERIAL);    
    glClearColor(0.863, 0.863, 0.863, 1.0f);   // Setting a background color
}

// Function called when the window is resized
void handleResize(int w, int h) {

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void handleKeypress1(unsigned char key, int x, int y) {

    if (key == 32 && ball_vely[0]-0.0f <0.001f  && abs(ball_y[0] +(box_len-2.0f)/2 -ball_rad[0] < 0.1f)) {
        ball_velx[0]=hit_length/38 *cos(DEG2RAD(theta));   
        ball_vely[0]=hit_length/38 *sin(DEG2RAD(theta));
        globalflag=1;
    }
    if(key == 'a') {
        if ( theta < 180)
        theta+=2;
    }
    if(key == 'c'){
        if(theta >0)
       theta-=2;
    }
    if (key == 'p') {
        pause++;
        if(pause%2==0)
            update(pause);
    }
    if (key == 'o' && lost == 1) {
        begin();
        lost=0;
        if(pause%2==0)
            update(pause);
        
    }
    
}
void handleKeypress2(int key, int x, int y) {

    if (key == GLUT_KEY_LEFT ) {
        if((ball_x[0]-0.1f > -box_len/2 +1.0f && ball_vely[0] -0.0f < 0.1f && 
            abs(ball_y[0] +(box_len-2.0f)/2 -ball_rad[0] <0.1f) && (box_len-2.0f)/2-ball_y[0]> -0.00001f) )
        ball_x[0] -= 0.1f; 
    }
    if (key == GLUT_KEY_RIGHT ) { 
       if((ball_x[0] + 0.1f  < box_len/2 - 1.0f && ball_vely[0] -0.0f < 0.1f && 
            abs(ball_y[0] +(box_len-2.0f)/2 -ball_rad[0] <0.1f && (box_len-2.0f)/2-ball_y[0]> -0.00001f) ))
        ball_x[0] += 0.1f;
    }
}

int xDrag=0,yDrag=0;
int middleMouseState=0;
void handleMouseclick(int button, int state, int x, int y) {
    if (state == GLUT_DOWN)
    {
        if (button == GLUT_LEFT_BUTTON)
            theta += 4;
        else if (button == GLUT_RIGHT_BUTTON)
            theta -= 4;

        else if(button == GLUT_MIDDLE_BUTTON)
            middleMouseState=1;
    }
      if(button==GLUT_MIDDLE_BUTTON && state == GLUT_DOWN && abs(ball_y[0]+(box_len-2.0f)/2 - ball_rad[0]) < 0.001f ){
      xDrag=x;yDrag=y;
  }
}

void checkPoketing() {
    for(myiterator1=0;myiterator1<n;myiterator1++) {
        for(myiterator2=0;myiterator2<4;myiterator2++) {
            if ((sqrt((ball_x[myiterator1]-hole_x[myiterator2])*(ball_x[myiterator1]-hole_x[myiterator2])+
                (ball_y[myiterator1]-hole_y[myiterator2])*(ball_y[myiterator1]-hole_y[myiterator2])) < hole_rad + ball_rad[myiterator1]-0.1f) &&
                abs(ball_velx[myiterator1]) < 1.0f && abs(ball_vely[myiterator1]) < 1.0f ) {
                if (myiterator1 == 0) {   
                ball_x[myiterator1] = 0.0f;
                ball_y[myiterator1] = -(box_len-2.0f)/2 + ball_rad[0];
                ball_rad[myiterator1] = ball_rad[0];
                foul++;
                }
                else if (myiterator1 ==1) {
                ball_x[myiterator1] = queen;
                ball_y[myiterator1] = queen_hood;
                queen_hood+=0.3f;
                win++;
                }
                else if(myiterator1%2) {
                    ball_x[myiterator1] = black;
                    ball_y[myiterator1] = black_hood;
                    black_hood+=0.3f;
                    win++;
                }
                else {
                    ball_x[myiterator1] = white;
                    ball_y[myiterator1] = white_hood;
                    white_hood+=0.3f;
                    win++;
                }
                ball_velx[myiterator1] = 0.0f;
                ball_vely[myiterator1] = 0.0f;    
                if (myiterator1==0)
                    score -= 5;
                else if (myiterator1 == 1)
                    score += 50;
                else if (myiterator1 % 2 ==0)
                    score -=10;
                else 
                    score +=10;
            }
        }
    }    
}
void draw(int x,int y) {
    if(middleMouseState==1 && abs(ball_y[0]+(box_len-2.0f)/2 - ball_rad[0]) < 0.001f  ){
        float check=ball_x[0]+(-xDrag+x)*0.01;
        if (abs(check) <= (box_len-2.0f)/2-0.001f)
         ball_x[0]+=(-xDrag+x)*0.01;
        xDrag=x;yDrag=y;
    }
  }
