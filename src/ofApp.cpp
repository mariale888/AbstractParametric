#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    light.setPosition(100,500, 100);
    cam.setAutoDistance(true);
    
    numCells = 5;
    deep    = 10;
    width   = 150;
    height  = width;
    containerSize = 200;
    makeObj();
    
    saveLine = false;
    
    //setting gui
    setUpGUI();
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255);
    
    ofSetColor(0);
    line.draw();
    
    ofPushMatrix();
    cam.begin();
    light.enable();
    ofEnableLighting();
    glEnable(GL_DEPTH_TEST);
   
    for (int i = 0; i < cellCentroids.size(); i++){
       
        ofSetColor(cellColor[i]);
        ofDrawSphere(cellCentroids[i], cellRadius[i]*0.15);
    }
    for(int i = 0; i < cellMeshes.size(); i++){
        ofSetColor(0,200,200, 30);
        cellMeshes[i].drawFaces();
        
        ofPushStyle();
        
        ofSetLineWidth(3);
        ofSetColor(0,255,255);
        cellMeshWireframes[i].draw();
        ofPopStyle();
    }
    
    glDisable(GL_DEPTH_TEST);
    ofDisableLighting();
    light.disable();
    cam.end();
    ofPopMatrix();

}

//--------------------------------------------------------------
void ofApp::makeObj()
{
    float w = width * 0.5;
    float h = height * 0.5;
    
    bool press = true;
    voro::container con(-w, w,
                        -h, h,
                        -10,   10,
                        1,1,1, press,press,press, 8);
    
    
    for(int i = 0;i< line.size();i++)
    {
        ofPoint l = line[i];
        float x = ofMap(l.x, 0, ofGetWidth(), -w, w);
        float y = ofMap(l.y, 0, ofGetHeight(), -h, h);
        float z = i*3;
        if(i%2 == 0)z *=-1;
        ofPoint newC = ofPoint(x,y, z);

        addCellSeed(con, newC, i, true);
    }
    
    cellMeshes.clear();

    cellMeshes = getCellsFromContainer(con,0.0);
    cellMeshWireframes = getCellsFromContainer(con,0.0,true);
    cellRadius.clear();
    cellRadius = getCellsRadius(con);
    cellCentroids.clear();
    
    cellCentroids = getCellsCentroids(con);
    
    for (int i = 0; i < cellCentroids.size(); i++){
        
        ofColor c = ofColor(ofRandom(255),ofRandom(255),ofRandom(255));
       cellColor.push_back(c);
    }

}
//--------------------------------------------------------------
void ofApp::updateObj()
{
   
    float w = width * 0.5;
    float h = height * 0.5;
    float d = deep * 0.5;
    bool press = true;
    voro::container con(-w, w,
                        -h, h,
                        -deep,   deep,
                        1,1,1, press,press,press, 8);
    
    
    int i = 0;
    float minZ = 0;
    float maxZ = 0;
    for(i = 0;i< line.size();i++)
    {
        ofPoint l = line[i];
        float x = ofMap(l.x, 0, ofGetWidth(), -w, w);
        float y = ofMap(l.y, 0, ofGetHeight(), -h, h);
        
        float z = i*5;
        if(i%2 == 0)z *=-1;
        ofPoint newC = ofPoint(x,y, d);
       if(z > maxZ)
           maxZ = z;
        if(z < minZ) minZ = z;
        addCellSeed(con, newC, i, true);
    }
    cout<<minZ<<" "<<maxZ<<endl;
    
    if(subdivideMe)
    {
        subdivideMe = false;
        oldSubs.clear(); // clear old subs.
        for(int j = 0; j < numCells ;j++){
            
            
            ofPoint newCell = ofPoint(ofRandom(-w,w),
                                      ofRandom(-h,h), d);
                                     // ofRandom(minZ, maxZ));
            
            
            if(insideContainer(con, newCell)){
               addCellSeed(con, newCell, i, true);
                i++;
               oldSubs.push_back(newCell);
            }
            
        }
    }
    else
    {
        // add any old subdivision
        for(vector<ofPoint>::iterator it = oldSubs.begin(); it != oldSubs.end(); ++it)
        {
            addCellSeed(con, *it, i, true);
            i++;
        }
    }
    
    cellMeshes.clear();
    
    cellMeshes = getCellsFromContainer(con,0.0);
    cellMeshWireframes = getCellsFromContainer(con,0.0,true);
    cellRadius.clear();
    cellRadius = getCellsRadius(con);
    cellCentroids.clear();
    
    cellCentroids = getCellsCentroids(con);
    for (int i = cellColor.size(); i < cellCentroids.size(); i++){
        
        ofColor c = ofColor(ofRandom(255),ofRandom(255),ofRandom(255));
        cellColor.push_back(c);
    }

}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == ' ')
    {
        subdivideMe = true;
        updateObj();
    }
    if(key == 'n')
    {
        updateObj();
    }
    if(key == 'c')
        line.clear();
}

//--------------------------------------------------------------
void ofApp::setUpGUI()
{
    gui0 = new ofxUISuperCanvas("LINE CONTROL");
    //gui0->setPosition(image.width + 20, 0);
    //gui0->addSpacer();
    gui0->addLabel("CREATE LINE");
    gui0->addToggle("draw line", &saveLine, 33, 33);
    gui0->addSpacer();
    
    gui0->addLabel("WIDTH");
    gui0->addSlider("width", 0, 1000, &width);
    gui0->addSpacer();
    
    gui0->addLabel("HEIGHT");
    gui0->addSlider("height", 0, 1000, &height);
    gui0->addSpacer();
    
    gui0->addLabel("NUM CELLS");
    gui0->addSlider("numCells", 0, 1000, &numCells);
    gui0->addSpacer();
    
    gui0->autoSizeToFitWidgets();
    ofAddListener(gui0->newGUIEvent,this,&ofApp::guiEvent);
}
//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e)
{
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

    if(saveLine)
        line.addVertex(ofPoint(x,y));
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

    if(saveLine)
    {
        line.addVertex(ofPoint(x,y));
        saveLine = false;
        makeObj();
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
