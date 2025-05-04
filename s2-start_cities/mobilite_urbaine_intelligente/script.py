import traci
import time

BUS_TYPE = "bus"
THRESHOLD_CARS = 10

sumoCmd = ["sumo-gui", "-c", "region.sumocfg", "--remote-port", "8813"]
traci.start(sumoCmd)
print("Agent connecté à SUMO.")

try:
    for step in range(500):
        traci.simulationStep()
        veh_count = traci.edge.getLastStepVehicleNumber("edge1")
        veh_list = traci.edge.getLastStepVehicleIDs("edge1")
        bus_present = any(traci.vehicle.getTypeID(v) == BUS_TYPE for v in veh_list)

        traffic_light_id = "n2"
        current_phase = traci.trafficlight.getPhase(traffic_light_id)

        if bus_present and current_phase != 0:
            print("🚌 Bus détecté, passage au vert.")
            traci.trafficlight.setPhase(traffic_light_id, 0)
        elif veh_count > THRESHOLD_CARS and current_phase != 0:
            print(f"🚗 {veh_count} voitures détectées, passage au vert.")
            traci.trafficlight.setPhase(traffic_light_id, 0)

        time.sleep(0.05)
finally:
    traci.close()
    print("✅ Simulation terminée.")
