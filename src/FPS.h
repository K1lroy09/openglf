#ifndef FPS_H
#define FPS_H

#include <iostream>
#include <iomanip>
#include <math.h>

#define _USE_MATH_DEFINES

#include <GL/glut.h>

using namespace std;

const unsigned int MILLISECONDS_PER_SECOND = 1000;
const unsigned int FPS_CAP = 60;
const long TARGET_DELTA_T = floor((double)MILLISECONDS_PER_SECOND/ (double)FPS_CAP);

class FPS{
	private:
		float avgFPS;
		float instantaneousFPS;
		int lastFrame;
		int lastPrint;
		bool firstMeasurement;	
	public:

		FPS(){
			this->firstMeasurement = true;
		}
		
		~FPS(){
		}
		
		
		void start(){
			this->avgFPS = 0;
			this->instantaneousFPS = 0;
			this->lastFrame = 0;
			this->lastPrint = 0;
		}
		
		void measure(){
			int now = glutGet(GLUT_ELAPSED_TIME);
			int deltaTime = now - lastFrame;
			double seconds = (double)deltaTime/MILLISECONDS_PER_SECOND;
			
			this->instantaneousFPS = 1.0/seconds;
			
			if(firstMeasurement){
				this->avgFPS = instantaneousFPS;
				this->firstMeasurement = false;
			}
			else{
				this->avgFPS += instantaneousFPS;
				this->avgFPS /= 2;
			}	
		}
		
		bool capFPS(){
			int now = glutGet(GLUT_ELAPSED_TIME);
			int deltaTime = now - lastFrame;

			if (deltaTime >= TARGET_DELTA_T)	return true;
			else								return false;
		}
		
		void setLastFrameNow(){
			int now = glutGet(GLUT_ELAPSED_TIME);
			this->lastFrame = now;	
		}
		
		void setLastPrintNow(){
			int now = glutGet(GLUT_ELAPSED_TIME);
			this->lastPrint = now;	
		}
		
		bool timeToPrintFPS(){
			int now = glutGet(GLUT_ELAPSED_TIME);
			int deltaTime = now - lastPrint;

			if (deltaTime >= MILLISECONDS_PER_SECOND)	return true;
			else										return false;			
		}
		
		friend ostream& operator << (std::ostream& out, const FPS& fps){
			out << "=====================================================\n";
			out << "FPS:\n";
			out << "=====================================================\n";
			out << "FPS CAP=" << FPS_CAP << "\n";
			out << "TARGET DELTA T=" << TARGET_DELTA_T << "\n";
			out << "FPS=" << fps.instantaneousFPS << "\n";
			out << "AVG FPS=" << fps.avgFPS << "\n";
			out << "=====================================================\n";
			
			return out;
		}
};

#endif
