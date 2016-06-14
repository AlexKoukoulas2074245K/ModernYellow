/* ======================
   Date: 30/5/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

class GState
{
public:

    GState();

    virtual ~GState() = 0;
        
    virtual void update() = 0;

    virtual void render() = 0;

    bool isFinished() const;

protected:

    bool m_finished;
};