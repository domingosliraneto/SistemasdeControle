#include "SistemasdeControle/headers/controlLibs/pid.h"

template<typename Type>
ControlHandler::PID<Type>::PID()
{
    this->kd = 0;
    this->ki = 0;
    this->kp = 0;

    this->Error = 0;
    this->derivativeError = 0;
    this->pastError = 0;
    this->integralError = 0;

    this->Step = 0.1;
    this->upperLimit = 200;
    this->lowerLimit = -200;
}

template<typename Type>
void ControlHandler::PID<Type>::antReset()
{
    if(this->PIDout >= this->upperLimit)
    {
        this->checkUpLim = true;
        this->PIDout = this->upperLimit;
    }
    else
        this->checkUpLim = false;

    if(this->PIDout <= this->lowerLimit)
    {
        this->checkLowLim = true;
        this->PIDout = this->lowerLimit;
    }
    else
        this->checkLowLim = false;
}

template<typename Type>
void ControlHandler::PID<Type>::intError()
{
    antReset();

    if(!this->checkUpLim && !this->checkLowLim)
        this->integralError += this->Error*this->Step;
}

template<typename Type>
void ControlHandler::PID<Type>::difError()
{
    this->derivativeError = ((this->Error - this->pastError)/this->Step);
    this->pastError = this->Error;
}

template<typename Type>
void ControlHandler::PID<Type>::setLimits(Type upperLimit, Type lowerLimit)
{
    this->upperLimit = upperLimit;
    this->lowerLimit = lowerLimit;
}

template<typename Type>
void ControlHandler::PID<Type>::setSampleTime(Type Time)
{
    this->Step = Time;
}

template<typename Type>
void ControlHandler::PID<Type>::setParams(Type kp, Type ki, Type kd)
{
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

template<typename Type>
void ControlHandler::PID<Type>::setParams(const LinAlg::Matrix<Type> &PIDsParameters)
{
    this->kp = PIDsParameters(1,1);
    this->ki = PIDsParameters(1,2);
    this->kd = PIDsParameters(1,3);
}

template<typename Type>
Type ControlHandler::PID<Type>::OutputControl(Type Reference, Type SignalInput)
{
    this->Error = Reference - SignalInput;

    difError();
    intError();
    this->PIDout = (this->kp*this->Error + this->ki*this->integralError + this->kd*this->derivativeError);
    antReset();
    
    return this->PIDout;
}
