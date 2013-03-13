#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <iomanip>
#include <math.h>

#include <GL/glut.h>

#define _USE_MATH_DEFINES

using namespace std;

const float POS_DEFAULT[3] = {
	0.0f, //CAMERA POS X DEFAULT
	0.0f, //CAMERA POS Y DEFAULT
	10.0f  //CAMERA POS Z DEFAULT
};

const float PITCH_DEFAULT = 0.0f;
const float YAW_DEFAULT = 180.0f;

const bool INVERT_X = false;
const bool INVERT_Y = false;

const float INHERTIA_VELOCITY = 0.2f;
const float FRICTION = 0.03f;
const float MOUSE_SENSITIVITY = 0.05f; 

class Camera{
	private:
		float pitch;
		float yaw;
		
		bool captureFlag;
		
		bool wFlag;
		bool aFlag;
		bool sFlag;
		bool dFlag;
		
		int mouseX;
		int mouseY;
		
		void constrain(){
			if(this->yaw > 360.0f) this->yaw -= 360.0f;
			if(this->yaw < 0.0f) this->yaw += 360.0f;
			if(this->pitch > 89.0f) this->pitch = 89.0f;
			if(this->pitch < -89.0f) this->pitch = -89.0f;
		}
		
		float toRad(float deg){
			return ((deg*M_PI)/180.0f);
		}
		
		float toDeg(float rad){
			return ((rad*180)/M_PI);
		}
		
		void processKeyStates(){
			if(wFlag) this->update(INHERTIA_VELOCITY, 0.0, 0.0f, 0.0f);
			if(aFlag) this->update(0.0f, -INHERTIA_VELOCITY, 0.0f, 0.0f);
			if(sFlag) this->update(-INHERTIA_VELOCITY, 0.0f, 0.0f, 0.0f);
			if(dFlag) this->update(0.0f, INHERTIA_VELOCITY, 0.0f, 0.0f);	
		}
		
		void applyFriction(){
			for(int i = 0; i < 3; i++){
				if(fabs(this->velocity[i]) > 0){
					float sign = -1*this->velocity[i]/fabs(this->velocity[i]);	
					this->velocity[i] += sign*FRICTION;
				}
			}
		}
				
		void capVelocity(){
			for(int i = 0; i < 3; i++){
				if(this->velocity[i] > 0.3f){
					this->velocity[i] = 0.3f;	
				}
				else if(this->velocity[i] < -0.3f){
					this->velocity[i] = -0.3f;	
				}
				else if((this->velocity[i] > -0.05f) && (this->velocity[i] < 0.05f)){
					this->velocity[i] = 0.0f;	
				}
			}	
		}
		
	public:
		float pos[3];
		float velocity[3];
		float viewVector[3];
		float lookAt[3];

		Camera(){
			this->pos[0] = POS_DEFAULT[0];	
			this->pos[1] = POS_DEFAULT[1];
			this->pos[2] = POS_DEFAULT[2];
			
			this->velocity[0] = 0.0f;	
			this->velocity[1] = 0.0f;
			this->velocity[2] = 0.0f;
			
			float yawRad = toRad(this->yaw);
			float pitchRad = toRad(this->pitch);
			
			this->viewVector[0] = -cos(pitchRad)*sin(yawRad);
			this->viewVector[1] = sin(pitchRad);
			this->viewVector[2] = cos(pitchRad)*cos(yawRad);
			
			this->pos[1] = POS_DEFAULT[1];
			this->pos[2] = POS_DEFAULT[2];
			
			this->pitch = PITCH_DEFAULT;
			this->yaw = YAW_DEFAULT;
			
			this->captureFlag = false;
			
			this->wFlag = false;
			this->aFlag = false;
			this->sFlag = false;
			this->dFlag = false;
			
			int mouseX = 0;
			int mouseY = 0;
			
			this->update(0.0f, 0.0f, 0.0f, 0.0f);
		}
		
		Camera(float yaw, float pitch){
			this->pos[0] = POS_DEFAULT[0];	
			this->pos[1] = POS_DEFAULT[1];
			this->pos[2] = POS_DEFAULT[2];
			
			this->velocity[0] = 0.0f;	
			this->velocity[1] = 0.0f;
			this->velocity[2] = 0.0f;
			
			float yawRad = toRad(this->yaw);
			float pitchRad = toRad(this->pitch);
			
			this->viewVector[0] = -cos(pitchRad)*sin(yawRad);
			this->viewVector[1] = sin(pitchRad);
			this->viewVector[2] = cos(pitchRad)*cos(yawRad);
			
			this->pitch = pitch;
			this->yaw = yaw;
			
			this->captureFlag = false;
			
			this->wFlag = false;
			this->aFlag = false;
			this->sFlag = false;
			this->dFlag = false;
			
			int mouseX = 0;
			int mouseY = 0;
			
			this->update(0.0f, 0.0f, 0.0f, 0.0f);
		}
		
		Camera(float posX, float posY, float posZ, float yaw, float pitch){
			this->pos[0] = posX;
			this->pos[1] = posY;
			this->pos[2] = posZ;
			
			this->velocity[0] = 0.0f;	
			this->velocity[1] = 0.0f;
			this->velocity[2] = 0.0f;
			
			float yawRad = toRad(this->yaw);
			float pitchRad = toRad(this->pitch);
			
			this->viewVector[0] = -cos(pitchRad)*sin(yawRad);
			this->viewVector[1] = sin(pitchRad);
			this->viewVector[2] = cos(pitchRad)*cos(yawRad);
			
			this->pitch = pitch;
			this->yaw = yaw;
			
			this->captureFlag = false;
			
			this->wFlag = false;
			this->aFlag = false;
			this->sFlag = false;
			this->dFlag = false;
			
			int mouseX = 0;
			int mouseY = 0;
			
			this->update(0.0f, 0.0f, 0.0f, 0.0f);
		}
		
		~Camera(){
		}
		
		void recalculate(){	
			if(this->isCaptured()){
				int midWidth = glutGet(GLUT_WINDOW_WIDTH)/2;
				int midHeight = glutGet(GLUT_WINDOW_HEIGHT)/2;	
			
				int deltaX = this->mouseX - midWidth;
				int deltaY = this->mouseY - midHeight;
			
				if(!INVERT_Y) 	deltaY = -deltaY;
			
				if(INVERT_X) 	deltaX = -deltaX;
			
				this->update(0.0f, 0.0f, MOUSE_SENSITIVITY*deltaY, MOUSE_SENSITIVITY*deltaX);
				
				this->mouseX = midWidth;
				this->mouseY = midHeight;
			
				glutWarpPointer(this->mouseX, this->mouseY);
			}
						
			this->pos[0] += this->velocity[0];
			this->pos[1] += this->velocity[1];
			this->pos[2] += this->velocity[2];
			
			this->lookAt[0] = this->pos[0] + this->viewVector[0];
			this->lookAt[1] = this->pos[1] + this->viewVector[1];
			this->lookAt[2] = this->pos[2] + this->viewVector[2];	
			
			this->processKeyStates();
			this->applyFriction();
			this->capVelocity();
		}
		
		void update(float progressionAdj, float strafeAdj, float pitchAdj, float yawAdj){
			if(INVERT_Y) 	this->pitch -= pitchAdj;
			else			this->pitch += pitchAdj;	
			
			if(INVERT_X) 	this->yaw -= yawAdj;
			else 			this->yaw += yawAdj;
			
			this->constrain();
			
			float yawRad = toRad(this->yaw);
			float pitchRad = toRad(this->pitch);
			
			this->viewVector[0] = -cos(pitchRad)*sin(yawRad);
			this->viewVector[1] = sin(pitchRad);
			this->viewVector[2] = cos(pitchRad)*cos(yawRad);
			
			float magnitude = sqrt(pow(-this->viewVector[2],2) + pow(this->viewVector[0],2));
			this->velocity[0] += strafeAdj*(-this->viewVector[2]/magnitude) + progressionAdj*this->viewVector[0];
			this->velocity[1] += progressionAdj*this->viewVector[1];
			this->velocity[2] += strafeAdj*(this->viewVector[0]/magnitude) + progressionAdj*this->viewVector[2];
		}
		
		void processKeysPressed(int key, int x, int y){
				switch(key){
					case 'w' : this->wFlag = true; break;
					case 'a' : this->aFlag = true; break;
					case 's' : this->sFlag = true; break;
					case 'd' : this->dFlag = true; break;
				}		
		}
		
		void processKeysReleased(int key, int x, int y){
				switch(key){
					case 'w' : this->wFlag = false; break;
					case 'a' : this->aFlag = false; break;
					case 's' : this->sFlag = false; break;
					case 'd' : this->dFlag = false; break;
				}
		}
		
		void capture(){
			if(!captureFlag){
				captureFlag = true;	
				
				glutSetCursor(GLUT_CURSOR_NONE); 
				
				int midWidth = glutGet(GLUT_WINDOW_WIDTH)/2;
				int midHeight = glutGet(GLUT_WINDOW_HEIGHT)/2;
				
				this->mouseX = midWidth;
				this->mouseY = midHeight;
			
				glutWarpPointer(this->mouseX, this->mouseY);
			}
		}
		
		void uncapture(){
			if(captureFlag){
				captureFlag = false;	
				
				glutSetCursor(GLUT_CURSOR_INHERIT); 
			}
		}
		
		void grabMousePosition(int x, int y){
			this->mouseX = x;
			this->mouseY = y;
		}
		
		friend ostream& operator << (std::ostream& out, const Camera& camera){
			out << "=====================================================\n";
			out << "CAMERA:\n";
			out << "=====================================================\n";
			out << "Yaw=" << camera.yaw << " degrees\n";
			out << "Pitch=" << camera.pitch << " degrees\n";
			out << "Position=(" << camera.pos[0] << ", " << camera.pos[1] << ", " << camera.pos[2] << ")\n";
			out << "Look at=(" << camera.lookAt[0] << ", " << camera.lookAt[1] << ", " << camera.lookAt[2] << ")\n";
			out << "=====================================================\n";
			
			return out;
		}
		
		bool isCaptured(){
			return this->captureFlag;
		}
};

#endif
