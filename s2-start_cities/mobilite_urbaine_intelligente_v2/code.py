#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Tableau de bord en temps réel pour la simulation SUMO
Permet de visualiser les métriques de trafic pendant une simulation de grève de bus
"""

import os
import sys
import traci
import time
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
from collections import deque
import argparse

# Vérifier si le chemin vers SUMO est disponible dans l'environnement
if 'SUMO_HOME' in os.environ:
    tools = os.path.join(os.environ['SUMO_HOME'], 'tools')
    sys.path.append(tools)
else:
    sys.exit("Veuillez définir la variable d'environnement SUMO_HOME")

# Paramètres
PORT = 8813  # Port pour la connexion à SUMO (doit être le même que dans le script principal)
UPDATE_INTERVAL = 1  # Intervalle de mise à jour du tableau de bord en secondes
GREVE_START_TIME = 600  # Début de la grève de bus en secondes
BUS_TYPE = "bus"

# Structures pour stocker les données temporelles
max_data_points = 100
time_points = deque(maxlen=max_data_points)
waiting_times = deque(maxlen=max_data_points)
congestion_levels = deque(maxlen=max_data_points)
co2_emissions = deque(maxlen=max_data_points)
buses_passed = 0

# Paramètres de configuration de la visualisation
plt.style.use('ggplot')

def connect_to_sumo(port=PORT):
    """Établit la connexion à une simulation SUMO en cours via TraCI"""
    try:
        traci.connect(port)
        print(f"Connecté à SUMO sur le port {port}")
        return True
    except Exception as e:
        print(f"Erreur de connexion à SUMO: {e}")
        return False

def collect_data():
    """Collecte les données de la simulation SUMO en cours"""
    global buses_passed
    
    # Temps de simulation actuel
    current_time = traci.simulation.getTime()
    
    # Temps d'attente moyen
    total_waiting_time = 0
    vehicle_count = 0
    for edge_id in ["e2", "e3", "e6", "e9"]:
        vehicles = traci.edge.getLastStepVehicleIDs(edge_id)
        for veh in vehicles:
            total_waiting_time += traci.vehicle.getWaitingTime(veh)
            vehicle_count += 1
    
    avg_waiting = total_waiting_time / max(1, vehicle_count)
    
    # Niveau de congestion (nombre total de véhicules sur les routes principales)
    congestion = 0
    for edge_id in ["e2", "e3", "e6", "e9"]:
        congestion += traci.edge.getLastStepVehicleNumber(edge_id)
    
    # Émissions CO2
    total_co2 = 0
    for edge_id in ["e2", "e3", "e6", "e9"]:
        total_co2 += traci.edge.getCO2Emission(edge_id)
    
    # Comptage des bus
    for edge_id in ["e3", "e6", "e7", "e10"]:
        vehicles = traci.edge.getLastStepVehicleIDs(edge_id)
        for veh in vehicles:
            if traci.vehicle.getTypeID(veh) == BUS_TYPE:
                if edge_id == "e3":  # Compter uniquement à un point de passage
                    buses_passed += 1
    
    # Stocker les données
    time_points.append(current_time)
    waiting_times.append(avg_waiting)
    congestion_levels.append(congestion)
    co2_emissions.append(total_co2)
    
    return {
        "time": current_time,
        "waiting": avg_waiting,
        "congestion": congestion,
        "co2": total_co2,
        "buses": buses_passed
    }

def create_dashboard():
    """Crée l'interface graphique du tableau de bord"""
    fig, axs = plt.subplots(2, 2, figsize=(10, 8))
    fig.suptitle('Tableau de bord - Smart Cities - Gestion de Trafic', fontsize=16)
    
    # Configurer les sous-graphiques
    waiting_line, = axs[0, 0].plot([], [], 'b-', lw=2)
    axs[0, 0].set_title('Temps d\'attente moyen')
    axs[0, 0].set_xlabel('Temps (s)')
    axs[0, 0].set_ylabel('Attente (s)')
    axs[0, 0].grid(True)
    
    bus_bar = axs[0, 1].bar([''], [0], color='red')
    axs[0, 1].set_title('Nombre de bus passés')
    axs[0, 1].set_ylim(0, 20)
    
    congestion_line, = axs[1, 0].plot([], [], 'g-', lw=2)
    axs[1, 0].set_title('Niveau de congestion')
    axs[1, 0].set_xlabel('Temps (s)')
    axs[1, 0].set_ylabel('Nombre de véhicules')
    axs[1, 0].grid(True)
    
    co2_line, = axs[1, 1].plot([], [], 'r-', lw=2)
    axs[1, 1].set_title('Émissions CO2')
    axs[1, 1].set_xlabel('Temps (s)')
    axs[1, 1].set_ylabel('g/s')
    axs[1, 1].grid(True)
    
    plt.tight_layout(rect=[0, 0, 1, 0.95])
    
    return fig, (waiting_line, bus_bar, congestion_line, co2_line)

def update_plot(frame, waiting_line, bus_bar, congestion_line, co2_line):
    """Met à jour le tableau de bord avec les nouvelles données"""
    try:
        # Récupérer les données de la simulation
        data = collect_data()
        
        # Mettre à jour le graphique temps d'attente
        waiting_line.set_data(list(time_points), list(waiting_times))
        waiting_line.axes.set_xlim(min(time_points) if time_points else 0, 
                                  max(time_points) if time_points else 100)
        waiting_line.axes.set_ylim(0, max(waiting_times) * 1.1 if waiting_times else 10)
        
        # Mettre à jour le graphique des bus
        bus_bar[0].set_height(data["buses"])
        
        # Mettre à jour le graphique de congestion
        congestion_line.set_data(list(time_points), list(congestion_levels))
        congestion_line.axes.set_xlim(min(time_points) if time_points else 0, 
                                     max(time_points) if time_points else 100)
        congestion_line.axes.set_ylim(0, max(congestion_levels) * 1.1 if congestion_levels else 10)
        
        # Mettre à jour le graphique CO2
        co2_line.set_data(list(time_points), list(co2_emissions))
        co2_line.axes.set_xlim(min(time_points) if time_points else 0, 
                              max(time_points) if time_points else 100)
        co2_line.axes.set_ylim(0, max(co2_emissions) * 1.1 if co2_emissions else 10)
        
        # Ajouter une ligne verticale pour indiquer le début de la grève
        for ax in [waiting_line.axes, congestion_line.axes, co2_line.axes]:
            if not hasattr(ax, 'greve_line') and min(time_points) < GREVE_START_TIME < max(time_points):
                ax.greve_line = ax.axvline(x=GREVE_START_TIME, color='k', linestyle='--', label='Début grève')
                ax.legend()
        
        # Indiquer le statut de la grève
        if data["time"] >= GREVE_START_TIME:
            if not hasattr(plt, 'greve_text'):
                plt.figtext(0.5, 0.01, "GRÈVE DE BUS EN COURS", ha="center", 
                           bbox={"facecolor":"red", "alpha":0.5, "pad":5})
                plt.greve_text = True
    
    except Exception as e:
        print(f"Erreur lors de la mise à jour du tableau de bord: {e}")
        return None
    
    return waiting_line, bus_bar[0], congestion_line, co2_line

def run_dashboard(port=PORT):
    """Lance le tableau de bord et met à jour les données en temps réel"""
    if not connect_to_sumo(port):
        return
    
    try:
        # Création du tableau de bord
        fig, (waiting_line, bus_bar, congestion_line, co2_line) = create_dashboard()
        
        # Animation pour mise à jour en temps réel
        ani = FuncAnimation(fig, update_plot, fargs=(waiting_line, bus_bar, congestion_line, co2_line),
                           interval=UPDATE_INTERVAL*1000, blit=False)
        
        plt.show()
    
    except KeyboardInterrupt:
        print("Tableau de bord fermé par l'utilisateur")
    except Exception as e:
        print(f"Erreur dans le tableau de bord: {e}")
    finally:
        if traci.isConnected():
            traci.close()
        print("Connexion à SUMO fermée")

def main():
    parser = argparse.ArgumentParser(description='Tableau de bord pour la simulation SUMO')
    parser.add_argument('-p', '--port', type=int, default=PORT,
                       help=f'Port pour la connexion à SUMO (défaut: {PORT})')
    args = parser.parse_args()
    
    run_dashboard(args.port)

if __name__ == "__main__":
    main()