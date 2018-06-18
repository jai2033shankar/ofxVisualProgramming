#include "moSlider.h"

//--------------------------------------------------------------
moSlider::moSlider() : PatchObject(){

    this->numInlets  = 3;
    this->numOutlets = 1;

    _inletParams[0] = new float();  // min
    _inletParams[1] = new float();  // max
    _inletParams[2] = new float();  // value
    *(float *)&_inletParams[0] = 0.0f;
    *(float *)&_inletParams[1] = 1.0f;
    *(float *)&_inletParams[2] = 0.0f;

    _outletParams[0] = new float(); // output
    *(float *)&_outletParams[0] = 0.0f;

    for(int i=0;i<this->numInlets;i++){
        this->inletsConnected.push_back(false);
    }

    isGUIObject     = true;
    isOverGui       = true;

}

//--------------------------------------------------------------
void moSlider::newObject(){
    this->setName("slider");
    this->addInlet(VP_LINK_NUMERIC,"min");
    this->addInlet(VP_LINK_NUMERIC,"max");
    this->addInlet(VP_LINK_NUMERIC,"value");
    this->addOutlet(VP_LINK_NUMERIC);
}

//--------------------------------------------------------------
void moSlider::setupObjectContent(shared_ptr<ofAppBaseWindow> &mainWindow){
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT );
    gui->setAutoDraw(false);
    gui->setUseCustomMouse(true);
    gui->setWidth(this->width);
    gui->onSliderEvent(this, &moSlider::onSliderEvent);

    slider = gui->addSlider("", *(float *)&_inletParams[0], *(float *)&_inletParams[1], *(float *)&_inletParams[2]);
    slider->setUseCustomMouse(true);

    gui->setPosition(0,this->height/3 + slider->getHeight()/2);
}

//--------------------------------------------------------------
void moSlider::updateObjectContent(){
    gui->update();
    slider->update();

    slider->setMin(*(float *)&_inletParams[0]);
    slider->setMax(*(float *)&_inletParams[1]);
    if(this->inletsConnected[2]){
        slider->setValue(*(float *)&_inletParams[2]);
    }

    *(float *)&_outletParams[0] = static_cast<float>(slider->getValue());
}

//--------------------------------------------------------------
void moSlider::drawObjectContent(ofxFontStash *font){
    ofSetColor(255);
    ofEnableAlphaBlending();
    gui->draw();
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void moSlider::removeObjectContent(){
    
}

//--------------------------------------------------------------
void moSlider::mouseMovedObjectContent(ofVec3f _m){
    if(!this->inletsConnected[2]){
        gui->setCustomMousePos(static_cast<int>(_m.x - this->getPos().x),static_cast<int>(_m.y - this->getPos().y));
        slider->setCustomMousePos(static_cast<int>(_m.x - this->getPos().x),static_cast<int>(_m.y - this->getPos().y));
    }

    isOverGui = slider->hitTest(_m-this->getPos());
}

//--------------------------------------------------------------
void moSlider::dragGUIObject(ofVec3f _m){
    if(isOverGui && !this->inletsConnected[2]){
        gui->setCustomMousePos(static_cast<int>(_m.x - this->getPos().x),static_cast<int>(_m.y - this->getPos().y));
        slider->setCustomMousePos(static_cast<int>(_m.x - this->getPos().x),static_cast<int>(_m.y - this->getPos().y));
    }else{
        ofNotifyEvent(dragEvent, nId);

        box->setFromCenter(_m.x, _m.y,box->getWidth(),box->getHeight());
        headerBox->set(box->getPosition().x,box->getPosition().y,box->getWidth(),headerHeight);

        x = box->getPosition().x;
        y = box->getPosition().y;

        for(int j=0;j<outPut.size();j++){
            outPut[j]->linkVertices[0].move(outPut[j]->posFrom.x,outPut[j]->posFrom.y);
            outPut[j]->linkVertices[1].move(outPut[j]->posFrom.x+20,outPut[j]->posFrom.y);
        }
    }
}

//--------------------------------------------------------------
void moSlider::onSliderEvent(ofxDatGuiSliderEvent e){
    *(float *)&_outletParams[0] = static_cast<float>(e.value);
}