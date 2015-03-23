#pragma once

#include "ofMain.h"
#include "ofxVoro.h"
#include "ofxUI.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    
    ofEasyCam       cam;
    ofLight         light;
    
    vector<ofPoint> cellCentroids;
    vector<float>   cellRadius;
    vector<ofColor>  cellColor;
    
    vector<ofVboMesh>  cellMeshes;
    vector<ofVboMesh>  cellMeshWireframes;
    int             containerSize;
    
    float width;
    float height;
    float deep;
    float numCells;
    
    //my line obj generator
    ofPolyline line;
    bool drawLine;
    
    bool subdivideMe;  // subdivides current model
    vector<ofPoint>oldSubs;
    bool saveLine;
    void makeObj();      //creates obj
    void updateObj();    // updates width, height,deep, subdivisions
   

    
    
    //GUI stuff
    ofxUISuperCanvas *gui0;
    void setUpGUI();
    void guiEvent(ofxUIEventArgs &e);
};
