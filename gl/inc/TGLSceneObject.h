// @(#)root/gl:$Name:  $:$Id: TGLSceneObject.h,v 1.15 2004/11/29 12:43:35 brun Exp $
// Author:  Timur Pocheptsov  03/08/2004

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TGLSceneObject
#define ROOT_TGLSceneObject

#include <utility>

#ifndef ROOT_TObject
#include "TObject.h"
#endif
#ifndef ROOT_Gtypes
#include "Gtypes.h"
#endif

class TGLFrustum;
class TBuffer3D;

/////////////////////////////////////////////////////////////
class TGLSelection {
private:
   Double_t fBBox[6];

public:
   TGLSelection();
   TGLSelection(const Double_t *bbox);
   TGLSelection(Double_t xmin, Double_t xmax, Double_t ymin,
                Double_t ymax, Double_t zmin, Double_t zmax);

   void DrawBox()const;

   void SetBBox(const Double_t *newBbox);
   void SetBBox(Double_t xmin, Double_t xmax, Double_t ymin,
               Double_t ymax, Double_t zmin, Double_t zmax);
   const Double_t *GetData()const{return fBBox;}
   
               
   void Shift(Double_t x, Double_t y, Double_t z);
   void Stretch(Double_t xs, Double_t ys, Double_t zs);

   const Double_t *GetRangeX()const{return fBBox;}
   const Double_t *GetRangeY()const{return fBBox + 2;}
   const Double_t *GetRangeZ()const{return fBBox + 4;}
};

/////////////////////////////////////////////////////////////
class TGLSceneObject : public TObject {
protected:
   std::vector<Double_t> fVertices;
   Float_t               fColor[17];
   TGLSelection          fSelectionBox;
   Bool_t                fIsSelected;

private:
   UInt_t                fGLName;
   TGLSceneObject        *fNextT;
   TObject               *fRealObject;

public:
   TGLSceneObject(const Double_t *vertStart, const Double_t *vertEnd, 
                  const Float_t *color = 0, UInt_t glName = 0, TObject *realObj = 0);

   virtual Bool_t IsTransparent()const;

   virtual void GLDraw(const TGLFrustum *fr)const = 0;

   virtual void Shift(Double_t x, Double_t y, Double_t z);
   virtual void Stretch(Double_t xs, Double_t ys, Double_t zs);

   TGLSelection *GetBBox(){return &fSelectionBox;}
   const TGLSelection *GetBBox()const{return &fSelectionBox;}
   
   void SetNextT(TGLSceneObject *next){fNextT = next;}
   TGLSceneObject *GetNextT()const{return fNextT;}
   
   UInt_t GetGLName()const{return fGLName;}
   TObject *GetRealObject()const{return fRealObject;}
   
   const Float_t *GetColor()const{return fColor;}
   void SetColor(const Float_t *newColor);
   
   void Select(Bool_t select = kTRUE){fIsSelected = select;}

private:
   TGLSceneObject(const TGLSceneObject &);
   TGLSceneObject & operator = (const TGLSceneObject &);

   ClassDef(TGLSceneObject,0)
};

///////////////////////////////////////////////////////////////////////
class TGLFaceSet : public TGLSceneObject {
private:
   std::vector<Double_t> fNormals;
   std::vector<Int_t>    fPolyDesc;
   UInt_t                fNbPols;

public:
   TGLFaceSet(const TBuffer3D &buff, const Float_t *color,
              UInt_t glName, TObject *realObj);

   void GLDraw(const TGLFrustum *fr)const;
   void Stretch(Double_t xs, Double_t ys, Double_t zs);

private:
   Int_t CheckPoints(const Int_t *source, Int_t *dest)const;
   static Bool_t Eq(const Double_t *p1, const Double_t *p2);
   void CalculateNormals();
};

////////////////////////////////////////////////////////////////////////
class TGLPolyMarker : public TGLSceneObject {
private:
   UInt_t fStyle;

public:
   TGLPolyMarker(const TBuffer3D &buff, const Float_t *color, UInt_t glName, TObject *realObject);
   void GLDraw(const TGLFrustum *fr)const;

private:
   void DrawStars()const;
};


class TGLPolyLine : public TGLSceneObject {
public:
   TGLPolyLine(const TBuffer3D &buff, const Float_t *color, UInt_t glName, TObject *realObject);
   void GLDraw(const TGLFrustum *fr)const;
};

// Utility class to draw a Sphere using OpenGL Sphere primitive
class TGLSphere : public TGLSceneObject {
private:
   Double_t fX;      // Sphere X center position
   Double_t fY;      // Sphere Y center position
   Double_t fZ;      // Sphere Z center position
   Double_t fRadius; // Sphere radius
   Int_t    fNdiv;   // Number of divisions

public:
   TGLSphere(const TBuffer3D &buff, const Float_t *color, UInt_t glName, TObject *realObject);

   void GLDraw(const TGLFrustum *fr)const;

   void Shift(Double_t x, Double_t y, Double_t z);
   void Stretch(Double_t xs, Double_t ys, Double_t zs);   
};


class TGLTube : public TGLSceneObject {
private:
   Double_t fX;
   Double_t fY;
   Double_t fZ;
   Double_t fRmin1;
   Double_t fRmax1;
   Double_t fRmin2;
   Double_t fRmax2;
   Double_t fDz;
   Int_t    fNdiv;
   Double_t fRotM[16];
   Bool_t   fInv;

public:
   TGLTube(const TBuffer3D &buff, const Float_t *color, UInt_t glName, TObject *realObject);
   void GLDraw(const TGLFrustum *fr)const;
   void Shift(Double_t x, Double_t y, Double_t z);
};

class TGLSimpleLight : public TGLSceneObject {
private:
   Float_t  fBulbRad;
   UInt_t   fLightName;

public:
   TGLSimpleLight(UInt_t glName, UInt_t lightName, const Float_t *color, const Double_t *position);



   void GLDraw(const TGLFrustum *fr)const;

   Bool_t IsTransparent()const;

   void Shift(Double_t x, Double_t y, Double_t z);
   void Stretch(Double_t xs, Double_t ys, Double_t zs);
   void SetBulbRad(Float_t newRad);
};

#endif
