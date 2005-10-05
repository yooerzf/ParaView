/*
 * Copyright 2004 Sandia Corporation.
 * Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
 * license for use of this work by or on behalf of the
 * U.S. Government. Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that this Notice and any
 * statement of authorship are reproduced on all copies.
 */

#ifndef _pqRenderViewProxy_h
#define _pqRenderViewProxy_h

#include "vtkPVRenderViewProxy.h"

class QVTKWidget;

class pqRenderViewProxy : public vtkPVRenderViewProxy
{
  public:
    static pqRenderViewProxy* New();
    vtkTypeMacro(pqRenderViewProxy,vtkPVRenderViewProxy)
    
    virtual void Render();
    virtual void EventuallyRender();
    
    virtual vtkRenderWindow* GetRenderWindow();
    void SetRenderWindow(QVTKWidget* win);

  private:
    QVTKWidget* mRenWin;
};

#endif

