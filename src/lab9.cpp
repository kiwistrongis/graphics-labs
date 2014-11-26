#include <ode/ode.h>
// ODE
#include <drawstuff/drawstuff.h> // The drawing library for ODE
// No. of links
#define NUM 4
// a dynamic world
dWorldID world;
// links, link[0] is the base
dBodyID link[NUM];
// joints, joint[0] is afixed joint the other joints are hinge joints
dJointID joint[NUM];

// target angle of joints
static double THETA[NUM] = { 0.0, 0.0, 0.0, 0.0};
// length of links
static double l[NUM] = { 0.10, 0.90, 1.00, 1.00};
// radius of links
static double r[NUM] = { 0.20, 0.04, 0.04, 0.04};
//
static double angle = 0.0;

void command(int cmd){
	switch (cmd){
		case 'j':
			THETA[1] += 0.05;
			break;
		case 'f':
			THETA[1] -= 0.05;
			break;
		case 'k':
			THETA[2] += 0.05;
			break;
		case 'd':
			THETA[2] -= 0.05;
			break;
		case 'l':
			THETA[3] += 0.05;
			break;
		case 's':
			THETA[3] -= 0.05;
			break;
	}

	// limit target angles not to destroy a robot
	if(THETA[1] < - M_PI) THETA[1] = - M_PI;
	if(THETA[1] > M_PI) THETA[1] = M_PI;
	if(THETA[2] < -2*M_PI/3) THETA[2] = - 2*M_PI/3;
	if(THETA[2] > 2*M_PI/3) THETA[2] = 2*M_PI/3;
	if(THETA[3] < -2*M_PI/3) THETA[3] = - 2*M_PI/3;
	if(THETA[3] > 2*M_PI/3) THETA[3] = 2*M_PI/3;
}

void control(){
	double k1 = 10.0;
	// gain for computing velocity
	double fMax = 100.0;
	// maximum torque value
	for (int j = 1; j <NUM; j++){
		double tmpAngle = dJointGetHingeAngle(joint[j]); // current joint angle
		double z = THETA[j] - tmpAngle;
		dJointSetHingeParam(joint[j], dParamVel, k1*z); // angular velocity
		dJointSetHingeParam(joint[j], dParamFMax, fMax); // max torque
		dJointAddHingeTorque( joint[j], 80 * sin( angle += 0.2));
	}
}

void start(){
	float xyz[3] = { 3.04, 1.28, 0.76};
	float hpr[3] = { -160.0, 4.50, 0.00};
	dsSetViewpoint(xyz,hpr);
}

void simLoop(int pause){
	control();
	dWorldStep(world, 0.02);
	dsSetColor(1.0,1.0,1.0);
	for (int i = 0; i <NUM; i++ )
	dsDrawCapsuleD(
		dBodyGetPosition(link[i]),
		dBodyGetRotation(link[i]),
		l[i], r[i]);
}

int main(int argc, char *argv[]){
	dsFunctions fn;
	double x[NUM] = { 0.00}, y[NUM] = {0.00}; // link position
	double z[NUM] = { 0.05, 0.50, 1.50, 2.55};
	double m[NUM] = { 10.00, 2.00, 2.00, 2.00}; // mass
	double anchor_x[NUM] = { 0.00}, anchor_y[NUM] = {0.00}; // anchor point for hinge
	double anchor_z[NUM] = { 0.00, 0.10, 1.00, 2.00};
	double axis_x[NUM] = { 0.00, 0.00, 0.00, 0.00}; // hinge rotation axis
	double axis_y[NUM] = { 0.00, 0.00, 1.00, 1.00};
	double axis_z[NUM] = { 1.00, 1.00, 0.00, 0.00};

	fn.version = DS_VERSION;
	fn.start = &start;
	fn.step = &simLoop;
	fn.command = &command;
	fn.path_to_textures = "/mnt/data/p/libs/ode-0.13/drawstuff/textures";
	dInitODE();
	world = dWorldCreate();
	dWorldSetGravity(world, 0, 0, -9.8);
	// create the links
	for( int i = 0; i <NUM; i++){
		dMass mass;
		link[i] = dBodyCreate(world);
		dBodySetPosition(link[i], x[i], y[i], z[i]);
		dMassSetZero(&mass);
		dMassSetCapsuleTotal(&mass,m[i],3,r[i],l[i]);
		dBodySetMass(link[i], &mass);
	}
	joint[0] = dJointCreateFixed(world, 0);
	dJointAttach(joint[0], link[0], 0);
	dJointSetFixed(joint[0]);
	for (int j = 1; j <NUM; j++){
		joint[j] = dJointCreateHinge(world, 0);
		dJointAttach(joint[j], link[j-1], link[j]);
		dJointSetHingeAnchor(joint[j], anchor_x[j], anchor_y[j],anchor_z[j]);
		dJointSetHingeAxis(joint[j], axis_x[j], axis_y[j], axis_z[j]);
		//dJointAddHingeTorqueFunction( joint[j]
	}

	//
	dsSimulationLoop(argc, argv, 640, 570, &fn);
	dCloseODE();
	return 0;
}
