#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofEnableDepthTest();

	float R = 120;
	float r = 13;	
	int v_span = 8;
	int u_span = 10;

	for (int v = 0; v < 360; v += v_span) {

		for (auto u = 0; u < 360; u += u_span) {

			vector<glm::vec3> vertices;
			vertices.push_back(this->make_point(R, r, u, v - v_span * 0.5));
			vertices.push_back(this->make_point(R, r, u + u_span, v - v_span * 0.5));
			vertices.push_back(this->make_point(R, r, u + u_span, v + v_span * 0.5));
			vertices.push_back(this->make_point(R, r, u, v + v_span * 0.5));

			int index = face.getNumVertices();
			this->face.addVertices(vertices);

			this->face.addIndex(index + 0); face.addIndex(index + 1); face.addIndex(index + 2);
			this->face.addIndex(index + 0); face.addIndex(index + 2); face.addIndex(index + 3);

		}
	}

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	float R = 120;
	float r = 15;
	for (int deg = 0; deg < 360; deg += 30) {

		float v = ofGetFrameNum() * 0.25 + deg;
		float u = (ofGetFrameNum() + deg * 2) * 10;
		float next_v = (ofGetFrameNum() + 1) * 0.25 + deg;
		float next_u = (ofGetFrameNum() + 1 + deg * 2) * 10;

		auto location = this->make_point(R, r, u, v);
		auto next = this->make_point(R, r, next_u, next_v);
		auto distance = location - next;
		
		auto future = glm::vec3(location) + distance * 80;
		auto random_deg_1 = ofRandom(360);
		auto random_deg_2 = ofRandom(360);
		future += glm::vec3(
			30 * cos(random_deg_1 * DEG_TO_RAD) * sin(random_deg_2 * DEG_TO_RAD),
			30 * sin(random_deg_1 * DEG_TO_RAD) * sin(random_deg_2 * DEG_TO_RAD),
			30 * cos(random_deg_2 * DEG_TO_RAD)
		);
		auto future_distance = future - location;
		
		this->frame.addVertex(location);
		this->velocity_list.push_back(glm::normalize(future_distance) * glm::length(distance));
		this->life_list.push_back(100);
	}

	for (int i = 0; i < this->frame.getNumVertices(); i++) {

		this->frame.setVertex(i, this->frame.getVertex(i) + this->velocity_list[i]);
		this->life_list[i] += -3;
	}

	for (int i = this->velocity_list.size() - 1; i >= 0; i--) {

		if(this->life_list[i] < 0) {

			this->life_list.erase(this->life_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->frame.removeVertex(i);
		}
	}

	this->frame.clearIndices();
	this->frame.clearColors();
	for (int i = 0; i < this->frame.getNumVertices(); i++) {

		this->frame.addColor(ofColor(39, ofMap(this->life_list[i], 0, 100, 0, 255)));
		for (int k = i + 1; k < this->frame.getNumVertices(); k += 1) {

			if (glm::distance(this->frame.getVertex(i), this->frame.getVertex(k)) < 15) {

				this->frame.addIndex(i); this->frame.addIndex(k);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(290);

	ofSetColor(239);
	this->face.draw();

	ofSetColor(39);
	this->face.drawWireframe();

	for (int i = 0; i < this->frame.getNumVertices(); i++) {

		ofSetColor(ofColor(39, ofMap(this->life_list[i], 0, 100, 0, 255)));
		ofDrawSphere(this->frame.getVertex(i), 1);
	}

	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);

	return glm::vec3(x, y, z);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}