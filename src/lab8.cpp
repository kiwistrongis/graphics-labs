#define dDOUBLE
#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#ifdef dDOUBLE
#define dsDrawSphere dsDrawSphereD
#endif

static dWorldID world;
static dSpaceID space;
static dGeomID ground;
static dJointGroupID contactgroup;
dsFunctions fn;
const dReal radius = 0.2;
const dReal mass = 1.0;

typedef struct {
	dBodyID body;
	dGeomID geom;
} MyObject;
MyObject ball;
void start(){
	static float xyz[3] = {0.0,-3.0,1.0};
	static float hpr[3] = {90.0,0.0,0.0};
	dsSetViewpoint (xyz,hpr);
}

static void nearCallback(void *data, dGeomID o1, dGeomID o2){
	const int N = 10;
	dContact contact[N];
	int isGround = ((ground == o1) || (ground == o2));
	int n =
	dCollide(o1,o2,N,&contact[0].geom,sizeof(dContact));
	if (isGround) {
		for (int i = 0; i < n; i++) {
			contact[i].surface.mode = dContactBounce;
			contact[i].surface.mu
			= dInfinity;
			contact[i].surface.bounce
			= 0.9; // (0.0~1.0) restitution parameter
			contact[i].surface.bounce_vel = 0.1; // minimum incoming velocity for bounce
			dJointID c = dJointCreateContact(world,contactgroup,&contact[i]);
			dJointAttach (c,dGeomGetBody(contact[i].geom.g1),dGeomGetBody(contact[i].geom.g2));
		}
	}
}
static void simLoop (int pause){
	const dReal *pos,*R;
	dSpaceCollide(space,0,&nearCallback);
	dWorldStep(world,0.01);
	dJointGroupEmpty(contactgroup);
	dsSetColor(1.0, 0.0, 0.0);
	pos = dBodyGetPosition(ball.body);
	R
	= dBodyGetRotation(ball.body);
	dsDrawSphere(pos,R,radius);
}
void prepDrawStuff() {
	fn.version = DS_VERSION;
	fn.start
	= &start;
	fn.step
	= &simLoop;
	fn.command = NULL;
	fn.stop
	= NULL;
	fn.path_to_textures = "C:/CSCI 4110/ode-0.13.1/drawstuff/textures";
}
int main (int argc, char *argv[]){
	dReal x0 = 0.0, y0 = 0.0, z0 = 2.0;
	dMass m1;
	prepDrawStuff();
	dInitODE();
	world = dWorldCreate();
	space = dHashSpaceCreate(0);
	contactgroup = dJointGroupCreate(0);
	dWorldSetGravity(world,0,0,-0.5);
	// Create a ground
	ground = dCreatePlane(space,0,0,1,0);
	// Create a ball
	ball.body = dBodyCreate(world);
	dMassSetZero(&m1);
	dMassSetSphereTotal(&m1,mass,radius);
	dBodySetMass(ball.body,&m1);
	dBodySetPosition(ball.body, x0, y0, z0);
	ball.geom = dCreateSphere(space,radius);
	dGeomSetBody(ball.geom,ball.body);
	dsSimulationLoop (argc,argv,352,288,&fn);
	dWorldDestroy (world);
	dCloseODE();
	return 0;
}