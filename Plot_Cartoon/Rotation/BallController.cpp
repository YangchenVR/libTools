// BallController.cpp: implementation of the CBallController class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "TestGL.h"
#include "BallController.h"
 #include <GL/freeglut.h>

namespace VR_FEM
{
	
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBallController::CBallController(const myReal& rad):m_isCuttingTrack(false)
{
	initVars();
	radius=clamped(rad,0.1,1);
}


CBallController::CBallController(const myReal& rad,const unitquaternion& initialOrient):m_isCuttingTrack(false)
{
	initVars();
	radius=clamped(rad,0.1,1);
	currentQuat=initialOrient;
}

CBallController& CBallController::operator=(const CBallController& other)
{
	if(this==&other) return *this;
	initVars();
	currentQuat=other.currentQuat;
	previousQuat=other.previousQuat;
	radius=other.radius;
	winWidth=other.winWidth;
	winHeight=other.winHeight;
	otherAxesNum=other.otherAxesNum;
	otherAxes=new myVector[otherAxesNum];
	for(int c=0;c<otherAxesNum;c++) otherAxes[c]=other.otherAxes[c];
	BallColor=other.BallColor;
	return *this;
}

void CBallController::MouseDown(const EPoint& location)
{
	xprev=(2*location[0]-winWidth)/winWidth;
	yprev=(winHeight-2*location[1])/winHeight;

	previousQuat=currentQuat;
	mouseButtonDown=true;
	if (m_isCuttingTrack)
	{
		m_vecTrackPoint.clear();
		m_startPoint = location;
	}
	bDrawBallArea=bProjectionMethod2;// draw circle only if method 2 active

/*	codice per memorizzare il vettore iniziale dove uno clicca per rubberbanding

	myReal winWidth=clientArea.Width();
	myReal winHeight=clientArea.Height();
	vdown=myVector((2*location.x-winWidth)/winWidth,(winHeight-2*location.y)/winHeight,0);
	ProjectOnSphere2(vdown);
	vcurr=vdown;
*/
}


void CBallController::MouseUp(const EPoint& location)
{
	mouseButtonDown=false;
	xprev=yprev=0.0;
	bDrawBallArea=false;
	// save current rotation axes for bodyAxes constraint at next rotation
	bodyorientation=currentQuat.getRotMatrix();
	bodyAxes[0]=myVector(bodyorientation(0,0),bodyorientation(1,0),bodyorientation(2,0));
	bodyAxes[1]=myVector(bodyorientation(0,1),bodyorientation(1,1),bodyorientation(2,1));
	bodyAxes[2]=myVector(bodyorientation(0,2),bodyorientation(1,2),bodyorientation(2,2));

//	vdown=vcurr=ORIGIN;// per rubberbanding
}


void CBallController::MouseMove(const EPoint& location)
{
	myReal xcurr=(2*location[0]-winWidth)/winWidth;
	myReal ycurr=(winHeight-2*location[1])/winHeight;
	myVector vfrom(xprev,yprev,0);
	myVector vto(xcurr,ycurr,0);
	/*printf("MouseMove[%f,%f,%f,%f]\n",xprev,yprev,xcurr,ycurr);
	system("pause");*/
	if(mouseButtonDown)
	{
// find the two points on sphere according to the projection method
		ProjectOnSphere(vfrom);
		ProjectOnSphere(vto);
		if (m_isCuttingTrack)
		{
			m_vecTrackPoint.push_back(MyPoint(vfrom.x(),vfrom.y(),vfrom.z()));
		}
// modify the vectors according to the active constraint
		if(whichConstraints != NO_AXES)
		{
			myVector* axisSet=GetUsedAxisSet();
			vfrom=ConstrainToAxis(vfrom,axisSet[currentAxisIndex]);
			vto=ConstrainToAxis(vto,axisSet[currentAxisIndex]);
		};
// get the corresponding unitquaternion
		unitquaternion lastQuat=RotationFromMove(vfrom,vto);
		currentQuat*=lastQuat;
		xprev=xcurr;
		yprev=ycurr;
	}
	else if(whichConstraints != NO_AXES)
		{
			ProjectOnSphere(vto);
			currentAxisIndex=NearestConstraintAxis(vto);
		};
}

void CBallController::getRotationInfo(float& x,float& y,float& z,float& w)
{
	x = currentQuat.x();
	y = currentQuat.y();
	z = currentQuat.z();
	w = currentQuat.w();
}

MyMatrix_3X3 CBallController::getRotation()
{
	tmatrix& R = currentQuat.getRotMatrix();
	MyMatrix_3X3 rt;
	rt[0][0] = R(0,0);
	rt[0][1] = R(0,1);
	rt[0][2]  = R(0,2);

	rt[1][0]  = R(1,0);
	rt[1][1] = R(1,1);
	rt[1][2] = R(1,2);

	rt[2][0] = R(2,0);
	rt[2][1] = R(2,1);
	rt[2][2] = R(2,2);
	return rt;
}

void CBallController::IssueGLrotation()
{
#ifdef REAL_IS_DOUBLE
	glMultMatrixd(currentQuat.getRotMatrix());
#else
	glMultMatrixf(currentQuat.getRotMatrix());
#endif
}

void CBallController::ProjectOnSphere(myVector& v) const
{
	myReal rsqr=radius*radius;
	myReal dsqr=v.x()*v.x()+v.y()*v.y();
	if(bProjectionMethod2)
	{
		// if inside sphere project to sphere else on plane
		if(dsqr>rsqr)
		{
			register myReal scale=(radius-.05)/sqrt(dsqr);
			v.x()*=scale;
			v.y()*=scale;
			v.z()=0;
		}
		else
		{
			v.z()=sqrt(rsqr-dsqr);
		}
	}
	else
	{
		// if relatively "inside" sphere project to sphere else on hyperbolic sheet
		if(dsqr<(rsqr*0.5))	v.z()=sqrt(rsqr-dsqr);
		else v.z()=rsqr/(2*sqrt(dsqr));
	};
}

unitquaternion CBallController::RotationFromMove(const myVector& vfrom,const myVector& vto)
{
//	vcurr=vto;// per rubberbanding
	if(bProjectionMethod2)
	{
		quaternion q;
		q.x()=vfrom.z()*vto.y()-vfrom.y()*vto.z();
		q.y()=vfrom.x()*vto.z()-vfrom.z()*vto.x();
		q.z()=vfrom.y()*vto.x()-vfrom.x()*vto.y();
		q.w()=vfrom*vto;
		return unitquaternion(q);
	}
	else
	{
// calculate axis of rotation and correct it to avoid "near zero length" rot axis
		myVector rotaxis=(vto^vfrom);
		rotaxis.EpsilonCorrect(X_AXIS);
// find the amount of rotation
		myVector d(vfrom-vto);
		myReal t=d.length()/(2.0*radius);
		clamp(t,-1.0,1.0);
		myReal phi=2.0*asin(t);
		return unitquaternion(phi,rotaxis);
	}
}

void CBallController::Key(UINT nChar)
{
	switch(nChar)
	{
	case VK_UP:
	case VK_NUMPAD8:
		currentQuat*=unitquaternion(DegToRad(angleKeyIncrement),X_AXIS);
		break;
	case VK_DOWN:
	case VK_NUMPAD2:
		currentQuat*=unitquaternion(DegToRad(-angleKeyIncrement),X_AXIS);
		break;
	case VK_RIGHT:
	case VK_NUMPAD6:
		currentQuat*=unitquaternion(DegToRad(angleKeyIncrement),Y_AXIS);
		break;
	case VK_LEFT:
	case VK_NUMPAD4:
		currentQuat*=unitquaternion(DegToRad(-angleKeyIncrement),Y_AXIS);
		break;
	case VK_PRIOR:
	case VK_NUMPAD9:
		currentQuat*=unitquaternion(DegToRad(angleKeyIncrement),Z_AXIS);
		break;
	case VK_HOME:
	case VK_NUMPAD7:
		currentQuat*=unitquaternion(DegToRad(-angleKeyIncrement),Z_AXIS);
		break;
	case VK_DELETE:
	case VK_NUMPAD5:
		currentQuat=unitquaternion(0,X_AXIS);
		break;
	case VK_ESCAPE:
		currentQuat=previousQuat;
		MouseUp(EPoint(-1,-1));
		break;
	case VK_TAB:
		if(mouseButtonDown && whichConstraints!=NO_AXES)
		{
			currentAxisIndex=(currentAxisIndex+1)%3;
			currentQuat=previousQuat;
		}
		break;
	};
}

void CBallController::DrawBallLimit()
{
	// "spherical zone" of controller
#ifdef REAL_IS_DOUBLE
		glColor3dv(BallColor);
#else
		glColor3fv(BallColor);
#endif
		glCallList(GLdisplayList);
/* rotation rubber banding (it works only if radius = 1)
	if(vdown!=vcurr)
	{
		const divs=3;
		const np=1<<divs;
		myVector pts[np+1];
		pts[0]=vdown;
		pts[1]=pts[np]=vcurr;
		for(int i=0;i<divs;i++)
			pts[1]=Bisect(pts[0],pts[1]);
		//pts[1]*=radius;
		myReal scale=2.0*(pts[0]*pts[1]);
		for(i=2;i<np;i++)
		{
			pts[i]=(pts[i-1]*scale)-pts[i-2];
//			afxDump<<"v["<<i<<"]="<<pts[i]<<"\n";
		};
		glBegin(GL_LINE_STRIP);
		glVertex3fv(pts[0]);
		for(i=1;i<np;i++)
#ifdef REAL_IS_DOUBLE
			glVertex3dv(radius*pts[i]);
#else
			glVertex3fv(radius*pts[i]);
#endif
		glVertex3fv(pts[np]);
		glEnd();
	};
*/
}

void CBallController::InitDisplayLists()
{
	GLdisplayList=glGenLists(1);
	if(GLdisplayList!=0)
	{
		GLUquadricObj* qobj=gluNewQuadric();
		gluQuadricDrawStyle(qobj,GLU_LINE);
		gluQuadricNormals(qobj,GLU_NONE);
		glNewList(GLdisplayList,GL_COMPILE);
			gluDisk(qobj,radius,radius,24,1);
		glEndList();
		gluDeleteQuadric(qobj);
	}
}

void CBallController::initVars()
{
	winWidth=winHeight=0;
	previousQuat=currentQuat=unitquaternion(0,myVector(1,0,0));
	mouseButtonDown=bDrawBallArea=bProjectionMethod2=bDrawConstraints=false;
	xprev=yprev=0.0;
	center=EPoint(0,0),
	radius=0.6;
	GLdisplayList=currentAxisIndex=otherAxesNum=0;
	BallColor=myVector(0.0,0.5,1.0);
	otherAxes=NULL;
	whichConstraints=NO_AXES;
	cameraAxes[0]=bodyAxes[0]=X_AXIS;
	cameraAxes[1]=bodyAxes[1]=Y_AXIS;
	cameraAxes[2]=bodyAxes[2]=Z_AXIS;
	bodyorientation.loadIdentity();
	angleKeyIncrement=5;
}

void CBallController::SetColor(myCOLORREF col)
{
	BallColor.x()=GetRValue(col)/255.0;
	BallColor.y()=GetGValue(col)/255.0;
	BallColor.z()=GetBValue(col)/255.0;
}

void CBallController::SetColorV(myVector colvec)
{
	clamp(colvec,0,1);
	BallColor=colvec;
}

myVector CBallController::ConstrainToAxis(const myVector& loose,const myVector& axis)
{
	myVector onPlane;
    register myReal norm;
    onPlane = loose-axis*(axis*loose);
    norm = onPlane.length();
    if (norm > 0)
	{
		if (onPlane.z() < 0.0) onPlane = -onPlane;
		return ( onPlane/=sqrt(norm) );
    };
    if (axis.z() == 1) onPlane = X_AXIS;
	else
	{
		onPlane = myVector(-axis.y(), axis.x(), 0);
		onPlane.normalize();
    }
    return (onPlane);
}

int CBallController::NearestConstraintAxis(const myVector& loose)
{
	myVector* axisSet=GetUsedAxisSet();
	myVector onPlane;
	register float max, dot;
	register int i, nearest;
	max = -1; 
	nearest = 0;
	if(whichConstraints == OTHER_AXES)
	{
		for (i=0; i<otherAxesNum; i++)
		{
			onPlane = ConstrainToAxis(loose, axisSet[i]);
			dot = onPlane*loose;
			if (dot>max) max = dot; nearest = i;
		}
	}
	else
	{
		for (i=0; i<3; i++)
		{
			onPlane = ConstrainToAxis(loose, axisSet[i]);
			dot = onPlane*loose;
			if (dot>max)
			{
				max = dot;
				nearest = i;
			};
		}
	};
	return (nearest);
}

myVector* CBallController::GetUsedAxisSet()
{
    myVector* axes=NULL;
	switch(whichConstraints)
	{
	case CAMERA_AXES: axes=cameraAxes;
		break;
	case BODY_AXES: axes=bodyAxes;
		break;
	case OTHER_AXES: axes=otherAxes;
		break;
	};
	return axes;
}

void CBallController::DrawConstraints()
{
	glColor3f(0,.75f,0);
	if(whichConstraints==CAMERA_AXES)
	{
		glCallList(GLdisplayList);
		glBegin(GL_LINES);
			glVertex3f(-radius,0,0);
			glVertex3f(radius,0,0);
			glVertex3f(0,-radius,0);
			glVertex3f(0,radius,0);
		glEnd();
	};
	if(whichConstraints==BODY_AXES)
	{
		glPushMatrix();
#ifdef REAL_IS_DOUBLE
		glMultMatrixd(bodyorientation);
#else
		glMultMatrixf(bodyorientation);
#endif
		glCallList(GLdisplayList);
		glRotated(90,1.0,0.0,0.0);
		glCallList(GLdisplayList);
		glRotated(90,0.0,1.0,0.0);
		glCallList(GLdisplayList);

		glPopMatrix();
	};
}

void CBallController::DrawBall()
{
// change the projection matrix to identity (no view transformation )
	glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
// reset the transformations
	glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
// prepare the circle display list the first time
	if(GLdisplayList == 0) InitDisplayLists();
// disable lighting and depth testing
	glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
// draw the constraints or the ball limit if appropriate
		if(bDrawConstraints && whichConstraints!=NO_AXES) DrawConstraints();
		else if(bDrawBallArea) DrawBallLimit();
	glPopAttrib();
// restore the modelview and projection matrices
		glPopMatrix();
	glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
}
