
#ifndef SIMPLETRAFFICGAME_ENGINE_H
#define SIMPLETRAFFICGAME_ENGINE_H

#endif


class Engine
{
private:
    double rpm;
    double getVolumetricEfficiency();
    double getAirFlowRate();
    double getAirFuelRatio();
    double getPowerGenerated();


public:
    double updateRPM(double throttle);
    double calculateTorque(double throttle);
};