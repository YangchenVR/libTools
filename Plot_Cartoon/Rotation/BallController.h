// BallController.h: interface for the CBallController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BALLCONTROLLER_H__196CBD82_3858_11D2_80B1_A7800FACFE7F__INCLUDED_)
#define AFX_BALLCONTROLLER_H__196CBD82_3858_11D2_80B1_A7800FACFE7F__INCLUDED_

//#include "../VR_Global_Define.h"

#include <glm/glm.hpp>
#include <vector>
#include "3dmath.h"	// Added by ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


namespace VR_FEM
{

	// Auxiliary Type Definitions
	enum AxisSet
	{
		NO_AXES,
		CAMERA_AXES,
		BODY_AXES,
		OTHER_AXES
	};


	typedef glm::vec2 EPoint;
	typedef glm::vec3 MyPoint;
	typedef unsigned long myCOLORREF;
	typedef glm::vec4 ERect;
	typedef unsigned UINT;
	typedef glm::mat3 MyMatrix_3X3;

	class CBallController  
	{
	public:
		void setCuttingTrack(bool flag){m_isCuttingTrack = flag;}
		bool m_isCuttingTrack;//fault be false
		std::vector< MyPoint > m_vecTrackPoint;
		EPoint m_startPoint;
	private:
		bool bDrawConstraints;
		tmatrix bodyorientation;
		int angleKeyIncrement;
		void DrawConstraints();
		myVector* GetUsedAxisSet();
		//	myVector vcurr; //vettori iniz e finali per il rubber banding della rotaz
		//	myVector vdown;
		myVector BallColor;
		bool bProjectionMethod2;
		bool bDrawBallArea;
		int GLdisplayList;
		unitquaternion currentQuat;
		unitquaternion previousQuat;
		myReal radius;
		myReal winWidth;
		myReal winHeight;
		myReal xprev;
		myReal yprev;
		EPoint center;
		bool mouseButtonDown;
		AxisSet whichConstraints;
		int currentAxisIndex;
		myVector cameraAxes[3];
		myVector bodyAxes[3];
		myVector* otherAxes;
		int otherAxesNum;

		void InitDisplayLists();
		void initVars(void);
		void ProjectOnSphere(myVector& v) const;
		unitquaternion RotationFromMove(const myVector& vfrom,const myVector& vto);
		myVector ConstrainToAxis(const myVector& loose,const myVector& axis);
		int NearestConstraintAxis(const myVector& loose);
	public:
		bool GetDrawConstraints();
		void SetDrawConstraints(bool flag=true);
		void DrawBall();
		int GetAngleKeyIncrement();
		void SetAngleKeyIncrement(int ang);
		void UseConstraints(AxisSet constraints);
		void ToggleMethod();
		void SetAlternateMethod(bool flag=true);
		myVector GetColorV();
		myCOLORREF GetColor();
		void SetColor(myCOLORREF col);
		void SetColorV(myVector colvec);
		void DrawBallLimit();
		CBallController();
		CBallController(const myReal& rad);
		CBallController(const myReal& rad,const unitquaternion& initialOrient);
		CBallController(const CBallController& other);
		virtual ~CBallController();
		CBallController& operator=(const CBallController& other);
		void Resize(const myReal& newRadius);
		void ClientAreaResize(const ERect& newSize);
		void MouseDown(const EPoint& location);
		void MouseUp(const EPoint& location);
		void MouseMove(const EPoint& location);
		void IssueGLrotation();
		void Key(UINT nChar);

		MyMatrix_3X3 getRotation();
		void getRotationInfo(float& x,float& y,float& z,float& w);
	};

	//---------------------------------------------------------------------------
	// inlines

	inline CBallController::~CBallController()
	{
		if(otherAxes) delete[] otherAxes;
	}

	inline CBallController::CBallController(const CBallController& other):m_isCuttingTrack(false)
	{
		*this=other;
	}

	inline void CBallController::Resize(const myReal& newRadius)
	{
		radius=newRadius;
	}

	inline void CBallController::ClientAreaResize(const ERect& newSize)
	{

		winWidth=myReal(newSize[2]-newSize[0]/*newSize.Width()*/);
		winHeight=myReal(newSize[3]-newSize[1]/*newSize.Height()*/);
		//	center=EPoint( (newSize.right-newSize.left)/2 , (newSize.bottom-newSize.top)/2);
	}

	inline CBallController::CBallController():m_isCuttingTrack(false)
	{
		initVars();
	}

	/*inline myCOLORREF CBallController::GetColor()
	{
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
		return RGB(int(BallColor.x()*255),int(BallColor.y()*255),int(BallColor.z()*255));
	}*/

	inline myVector CBallController::GetColorV()
	{
		return BallColor;
	}

	inline void CBallController::SetAlternateMethod(bool flag)
	{
		bProjectionMethod2=flag;
	}

	inline void CBallController::ToggleMethod()
	{
		if(bProjectionMethod2) bProjectionMethod2=false;
		else bProjectionMethod2=true;
	}

	inline void CBallController::UseConstraints(AxisSet constraints)
	{
		whichConstraints=constraints;
	}

	inline int CBallController::GetAngleKeyIncrement()
	{
		return angleKeyIncrement;
	}

	inline void CBallController::SetAngleKeyIncrement(int ang)
	{
		angleKeyIncrement=abs(ang)%360;
	}

	inline bool CBallController::GetDrawConstraints()
	{
		return bDrawConstraints;
	}

	inline void CBallController::SetDrawConstraints(bool flag)
	{
		bDrawConstraints=flag;
	}

}

#endif // !defined(AFX_BALLCONTROLLER_H__196CBD82_3858_11D2_80B1_A7800FACFE7F__INCLUDED_)
