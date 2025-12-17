#include "vehicle/Engine.h"

#include "config/EngineConstants.h"
#include "config/PhysicsConstants.h"


double Engine::getVolumetricEfficiency()
{
    return 0.8; //placeholder- should be a function of rpm
}


double Engine::getAirFlowRate()
{
    double airMassPerCycle = getVolumetricEfficiency() * EngineConstants::INTAKE_MANIFOLD_PRESSURE * EngineConstants::CYLINDER_VOLUME;
    return airMassPerCycle * (rpm / 120.0);
}

double Engine::getAirFuelRatio()
{
    return 0.8;
}

double Engine::getPowerGenerated()
{
    double fuelMass = getAirFlowRate() / getAirFuelRatio();
    double powerGenerated = fuelMass * EngineConstants::LATENT_HEAT * EngineConstants::ENGINE_EFFICIENCY;
    return powerGenerated;
}

double Engine::updateRPM(double throttle)
{

}



double Engine::calculateTorque(double throttle)
{
    double angularSpeed = (2.0 * M_PI * rpm) / 60.0;
    double driveshaftTorque = getPowerGenerated() / angularSpeed;
}


