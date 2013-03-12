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
	0.0f  //CAMERA POS Z DEFAULT
};

const float PITCH_DEFAULT = 0.0f;
const float YAW_DEFAULT = 180.0f;

const bool INVERT_X = false;
const bool INVERT_Y = false;

class Camera{
	private:
		float pitch;
		float yaw;
		
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
		
	public:
		float pos[3];
		float lookAt[3];

		Camera(){
			this->pos[0] = POS_DEFAULT[0];	
			this->pos[1] = POS_DEFAULT[1];
			this->pos[2] = POS_DEFAULT[2];
			this->pitch = PITCH_DEFAULT;
			this->yaw = YAW_DEFAULT;
			this->update(0.0f, 0.0f, 0.0f, 0.0f);
		}
		
		Camera(float yaw, float pitch){
			this->pos[0] = POS_DEFAULT[0];	
			this->pos[1] = POS_DEFAULT[1];
			this->pos[2] = POS_DEFAULT[2];
			this->pitch = pitch;
			this->yaw = yaw;	
			this->update(0.0f, 0.0f, 0.0f, 0.0f);
		}
		
		Camera(float posX, float posY, float posZ, float yaw, float pitch){
			this->pos[0] = posX;
			this->pos[1] = posY;
			this->pos[2] = posZ;
			this->pitch = pitch;
			this->yaw = yaw;
			this->update(0.0f, 0.0f, 0.0f, 0.0f);
		}
		
		~Camera(){
		}
		
		void update(float progressionAdj, float strafeAdj, float pitchAdj, float yawAdj){
			if(INVERT_Y) 	this->pitch -= pitchAdj;
			else			this->pitch += pitchAdj;	
			
			if(INVERT_X) 	this->yaw -= yawAdj;
			else 			this->yaw += yawAdj;
			
			this->constrain();
			
			float yawRad = toRad(this->yaw);
			float pitchRad = toRad(this->pitch);
			
			float viewVector[3];	
			
			viewVector[0] = -cos(pitchRad)*sin(yawRad);
			viewVector[1] = sin(pitchRad);
			viewVector[2] = cos(pitchRad)*cos(yawRad);
			
			float magnitude = sqrt(pow(-viewVector[2],2) + pow(viewVector[0],2));
			this->pos[0] += strafeAdj*(-viewVector[2]/magnitude);
			this->pos[2] += strafeAdj*(viewVector[0]/magnitude);
			
			this->pos[0] += progressionAdj*viewVector[0];
			this->pos[1] += progressionAdj*viewVector[1];
			this->pos[2] += progressionAdj*viewVector[2];
			
			this->lookAt[0] = this->pos[0] + viewVector[0];
			this->lookAt[1] = this->pos[1] + viewVector[1];
			this->lookAt[2] = this->pos[2] + viewVector[2];
		}
		
		void processKeys(int key, int x, int y){
				switch(key){
					case 'w' : this->update(0.1f, 0.0, 0.0f, 0.0f); break;
					case 'a' : this->update(0.0f, -0.1f, 0.0f, 0.0f); break;
					case 's' : this->update(-0.1f, 0.0f, 0.0f, 0.0f); break;
					case 'd' : this->update(0.0f, 0.1f, 0.0f, 0.0f); break;
				}		
		}
		
		void processSpecialKeys(unsigned char key, int x, int y){
				switch(key){
					case GLUT_KEY_UP : this->update(0.0f, 0.0f, 5.0f, 0.0f); break;
					case GLUT_KEY_LEFT : this->update(0.0f, 0.0f, 0.0f, -5.0f); break;
					case GLUT_KEY_DOWN : this->update(0.0f, 0.0f, -5.0f, 0.0f); break;
					case GLUT_KEY_RIGHT : this->update(0.0f, 0.0f, 0.0f, 5.0f); break;
				}	
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
};

#endif
