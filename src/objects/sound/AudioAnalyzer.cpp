#include "AudioAnalyzer.h"

//--------------------------------------------------------------
AudioAnalyzer::AudioAnalyzer() : PatchObject(){

    this->numInlets  = 1;
    this->numOutlets = 3;

    _inletParams[0] = new float();  // channel
    *(float *)&_inletParams[0] = 0;

    _outletParams[0] = new float(); // RMS
    *(float *)&_outletParams[0] = 0.0f;
    _outletParams[1] = new float(); // Pitch
    *(float *)&_outletParams[1] = 0.0f;
    _outletParams[2] = new vector<float>();  // Spectrum

    for(int i=0;i<this->numInlets;i++){
        this->inletsConnected.push_back(false);
    }

    isAudioINObject = true;


    actualChannel   = 0;
}

//--------------------------------------------------------------
void AudioAnalyzer::newObject(){
    this->setName("audio analyzer");
    this->addInlet(VP_LINK_NUMERIC,"channel");
    this->addOutlet(VP_LINK_NUMERIC);
    this->addOutlet(VP_LINK_NUMERIC);
    this->addOutlet(VP_LINK_ARRAY);
}

//--------------------------------------------------------------
void AudioAnalyzer::setupObjectContent(shared_ptr<ofAppBaseWindow> &mainWindow){
    loadAudioSettings();

    // TESTING
    for(int i=0;i<bufferSize;i++){
        static_cast<vector<float> *>(_outletParams[2])->push_back(0.0f);
    }

}

//--------------------------------------------------------------
void AudioAnalyzer::updateObjectContent(){

    unique_lock<mutex> lock(audioMutex);

    actualChannel = static_cast<int>(floor(*(float *)&_inletParams[0]));

    waveform.clear();
    for(size_t i = 0; i < lastBuffer.getNumFrames(); i++) {
        float sample = lastBuffer.getSample(i, 0);
        float x = ofMap(i, 0, lastBuffer.getNumFrames(), 0, OBJECT_WIDTH);
        float y = ofMap(sample, -1, 1, headerHeight, OBJECT_HEIGHT);
        waveform.addVertex(x, y);

        // TESTING
        static_cast<vector<float> *>(_outletParams[2])->at(i) = sample;
    }

}

//--------------------------------------------------------------
void AudioAnalyzer::drawObjectContent(ofxFontStash *font){
    ofSetColor(255);
    ofEnableAlphaBlending();
    waveform.draw();
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void AudioAnalyzer::removeObjectContent(){
    
}

//--------------------------------------------------------------
void AudioAnalyzer::audioInObject(ofSoundBuffer &inputBuffer){

    for(size_t i = 0; i < inputBuffer.getNumFrames(); i++) {

    }

    unique_lock<mutex> lock(audioMutex);
    lastBuffer = inputBuffer;

}

//--------------------------------------------------------------
bool AudioAnalyzer::loadAudioSettings(){
    ofxXmlSettings XML;
    bool loaded = false;

    if (XML.loadFile(patchFile)){
        if (XML.pushTag("settings")){
            sampleRate = XML.getValue("sample_rate",0);
            bufferSize = XML.getValue("buffer_size",0);
            XML.popTag();
        }

        loaded = true;
    }

    return loaded;
}