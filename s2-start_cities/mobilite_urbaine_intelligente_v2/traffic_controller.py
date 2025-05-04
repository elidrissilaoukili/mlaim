#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Contrôleur intelligent de trafic pour SUMO avec TraCI
Ce script simule une grève de bus et adapte les feux en conséquence
"""

import os
import sys
import traci
import time
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
from collections import deque
import logging

# Configuration du logging
logging.basicConfig(level=logging.INFO, 
                   format='%(asctime)s - %(levelname)s - %(message)s',
                   filename='traffic_controller.log')

# Vérifier si le chemin vers SUMO est disponible dans l'environnement
if 'SUMO_HOME' in os.environ:
    tools = os.path.join(os.environ['SUMO_HOME'], 'tools')
    sys.path.append(tools)
else:
    sys.exit("Veuillez définir la variable d'environnement SUMO_HOME")

# Paramètres de la simulation
CONFIG_FILE = "quartier.sumocfg"
BUS_TYPE = "bus"
CONGESTION_THRESHOLD = 10  # Seuil de congestion (nombre de véhicules)
GREVE_START_TIME = 600     # Moment où la grève de bus commence (en secondes)
SIM_DURATION = 360000000        # Durée totale de la simulation (en secondes)

# Liste des intersections avec feux
TRAFFIC_LIGHTS = ["i1", "i2", "i3", "i4"]

# Données pour le tableau de bord
data_waiting_time = deque(maxlen=100)
data_buses_passed = 0
data_congestion = deque(maxlen=100)
data_co2 = deque(maxlen=100)
time_series = deque(maxlen=100)

# Fonction pour démarrer SUMO avec l'interface TraCI
def start_simulation():
    sumo_binary = "C:/Program Files (x86)/Eclipse/Sumo/bin/sumo-gui.exe"  # Chemin complet
    config_file = "quartier.sumocfg"
    
    sumo_cmd = [
        sumo_binary,
        "-c", config_file,
        "--remote-port", "8813",
        "--no-step-log",
        "--duration-log.statistics",
        "--tripinfo-output", "tripinfo.xml"
    ]
    
    # Vérification de l'existence des fichiers
    if not os.path.exists(sumo_binary):
        raise Exception(f"Fichier SUMO introuvable: {sumo_binary}")
    if not os.path.exists(config_file):
        raise Exception(f"Fichier de config introuvable: {config_file}")
    
    try:
        print(f"Démarrage de SUMO avec: {' '.join(sumo_cmd)}")
        traci.start(sumo_cmd, port=8813, numRetries=10)
        print("Connexion TraCI établie avec succès")
    except Exception as e:
        print(f"Échec du démarrage: {str(e)}")
        print("Solutions possibles:")
        print("1. Vérifiez le chemin de SUMO")
        print("2. Lancez SUMO manuellement d'abord")
        print("3. Vérifiez les fichiers de configuration")
        sys.exit(1)

# Détecte si un bus est présent sur une route donnée
def detect_bus_on_edge(edge_id):
    vehicles = traci.edge.getLastStepVehicleIDs(edge_id)
    for veh in vehicles:
        if traci.vehicle.getTypeID(veh) == BUS_TYPE:
            return True, veh
    return False, None

# Vérifie la congestion sur une route donnée
def check_congestion(edge_id, threshold=CONGESTION_THRESHOLD):
    veh_count = traci.edge.getLastStepVehicleNumber(edge_id)
    return veh_count > threshold, veh_count

# Donne la priorité aux bus en modifiant les feux
def prioritize_bus(tl_id, phase_id=0):
    current_phase = traci.trafficlight.getPhase(tl_id)
    if current_phase != phase_id:
        traci.trafficlight.setPhase(tl_id, phase_id)
        logging.info(f"Priorité bus: feu {tl_id} changé à la phase {phase_id}")
        return True
    return False

# Adapte les feux en fonction de la congestion
def adapt_to_congestion(tl_id, phase_id=0):
    current_phase = traci.trafficlight.getPhase(tl_id)
    if current_phase != phase_id:
        traci.trafficlight.setPhase(tl_id, phase_id)
        logging.info(f"Congestion détectée: feu {tl_id} changé à la phase {phase_id}")
        return True
    return False

# Collecte les données pour le tableau de bord
def collect_dashboard_data():
    global data_buses_passed
    
    # Temps d'attente moyen
    total_waiting_time = 0
    vehicle_count = 0
    for edge_id in ["e2", "e3", "e6", "e9"]:
        vehicles = traci.edge.getLastStepVehicleIDs(edge_id)
        for veh in vehicles:
            total_waiting_time += traci.vehicle.getWaitingTime(veh)
            vehicle_count += 1
    
    avg_waiting_time = total_waiting_time / max(1, vehicle_count)
    data_waiting_time.append(avg_waiting_time)
    
    # Compter les bus qui passent
    for edge_id in ["e3", "e6", "e7", "e10"]:
        vehicles = traci.edge.getLastStepVehicleIDs(edge_id)
        for veh in vehicles:
            if traci.vehicle.getTypeID(veh) == BUS_TYPE and edge_id == "e3":
                data_buses_passed += 1
    
    # Niveau de congestion
    total_congestion = 0
    for edge_id in ["e2", "e3", "e6", "e9"]:
        _, veh_count = check_congestion(edge_id)
        total_congestion += veh_count
    
    data_congestion.append(total_congestion)
    
    # Émissions CO2
    total_co2 = 0
    for edge_id in ["e2", "e3", "e6", "e9"]:
        total_co2 += traci.edge.getCO2Emission(edge_id)
    
    data_co2.append(total_co2)
    time_series.append(traci.simulation.getTime())
    
    return {
        "waiting_time": avg_waiting_time,
        "buses_passed": data_buses_passed,
        "congestion": total_congestion,
        "co2": total_co2
    }

# Initialisation du tableau de bord en temps réel
def init_dashboard():
    plt.ion()  # Mode interactif
    fig, axs = plt.subplots(2, 2, figsize=(12, 8))
    fig.suptitle("Tableau de bord - Smart Traffic Control", fontsize=16)
    
    # Configuration des sous-graphiques
    axs[0, 0].set_title("Temps d'attente moyen")
    axs[0, 0].set_ylabel("Secondes")
    
    axs[0, 1].set_title("Nombre de bus passés")
    axs[0, 1].set_ylabel("Nombre")
    
    axs[1, 0].set_title("Niveau de congestion")
    axs[1, 0].set_ylabel("Nombre de véhicules")
    
    axs[1, 1].set_title("Émissions CO2")
    axs[1, 1].set_ylabel("g/s")
    
    plt.tight_layout(rect=[0, 0, 1, 0.95])  # Ajuster les marges
    return fig, axs

# Mise à jour du tableau de bord
def update_dashboard(fig, axs, data):
    # Temps d'attente
    axs[0, 0].clear()
    axs[0, 0].set_title("Temps d'attente moyen")
    axs[0, 0].set_ylabel("Secondes")
    axs[0, 0].plot(time_series, data_waiting_time, 'b-')
    axs[0, 0].grid(True)
    
    # Bus passés
    axs[0, 1].clear()
    axs[0, 1].set_title("Nombre de bus passés")
    axs[0, 1].set_ylabel("Nombre")
    axs[0, 1].bar(["Bus"], [data["buses_passed"]], color='r')
    
    # Congestion
    axs[1, 0].clear()
    axs[1, 0].set_title("Niveau de congestion")
    axs[1, 0].set_ylabel("Nombre de véhicules")
    axs[1, 0].plot(time_series, data_congestion, 'g-')
    axs[1, 0].axhline(y=CONGESTION_THRESHOLD, color='r', linestyle='--', label="Seuil")
    axs[1, 0].grid(True)
    axs[1, 0].legend()
    
    # CO2
    axs[1, 1].clear()
    axs[1, 1].set_title("Émissions CO2")
    axs[1, 1].set_ylabel("g/s")
    axs[1, 1].plot(time_series, data_co2, 'r-')
    axs[1, 1].grid(True)
    
    # Annotations sur l'état de la grève
    if traci.simulation.getTime() >= GREVE_START_TIME:
        plt.figtext(0.5, 0.01, "SIMULATION DE GRÈVE DE BUS ACTIVE", ha="center", 
                   bbox={"facecolor":"red", "alpha":0.5, "pad":5})
    
    plt.draw()
    plt.pause(0.01)
def start_simulation():
    sumo_cmd = [
        "sumo-gui",  # ou "sumo" pour la version non-graphique
        "-c", "quartier.sumocfg",
        "--start",  # Démarrer automatiquement
        "--quit-on-end",  # Quitter à la fin
        "--no-step-log",  # Réduire les logs
        "--no-warnings",  # Ignorer les warnings
        "--duration-log.disable"  # Désactiver les logs de durée
    ]
    traci.start(sumo_cmd)

# Fonction principale de simulation
def run_simulation():
    start_simulation()
    
    # Initialiser le tableau de bord
    fig, axs = init_dashboard()
    
    step = 0
    try:
        while step < SIM_DURATION:
            traci.simulationStep()
            current_time = traci.simulation.getTime()
            
            # Vérifier toutes les routes principales pour les bus et la congestion
            for edge_id, tl_id in [("e2", "i1"), ("e3", "i2"), ("e6", "i3"), ("e9", "i4")]:
                # Détecter les bus
                bus_present, bus_id = detect_bus_on_edge(edge_id)
                if bus_present:
                    prioritize_bus(tl_id)
                    logging.info(f"Bus {bus_id} détecté sur {edge_id}, priorité au feu {tl_id}")
                
                # Vérifier la congestion (surtout après la grève)
                if current_time >= GREVE_START_TIME:
                    is_congested, veh_count = check_congestion(edge_id)
                    if is_congested:
                        adapt_to_congestion(tl_id)
                        logging.info(f"Congestion sur {edge_id}: {veh_count} véhicules")
            
            # Collecter et afficher les données du tableau de bord tous les 5 pas
            if step % 50 == 0:
                data = collect_dashboard_data()
                update_dashboard(fig, axs, data)
            
            # Afficher des messages d'état
            if step == 0:
                print("Simulation en cours...")
            
            if current_time >= GREVE_START_TIME and step % 500 == 0:
                print(f"Simulation en mode grève de bus (t={current_time:.1f}s)")
            
            step += 1
    
    except Exception as e:
        logging.error(f"Erreur dans la simulation: {e}")
        print(f"Une erreur s'est produite: {e}")
    
    finally:
        # Terminer la simulation
        traci.close()
        print("Simulation terminée.")
        logging.info("Simulation terminée.")
        
        # Analyse finale des résultats
        show_final_results()
        
        plt.ioff()
        plt.show()  # Garder le tableau de bord ouvert jusqu'à fermeture manuelle

# Afficher les résultats finaux après la simulation
def show_final_results():
    fig, axs = plt.subplots(2, 2, figsize=(12, 8))
    fig.suptitle("Résultats de la simulation - Analyse comparative", fontsize=16)
    
    # Diviser les données en période avant/après grève
    split_point = next((i for i, t in enumerate(time_series) if t >= GREVE_START_TIME), len(time_series)//2)
    
    before_strike = list(range(split_point))
    after_strike = list(range(split_point, len(time_series)))
    
    # Temps d'attente
    axs[0, 0].set_title("Temps d'attente moyen")
    axs[0, 0].set_ylabel("Secondes")
    axs[0, 0].plot([time_series[i] for i in before_strike], 
                   [data_waiting_time[i] for i in before_strike], 
                   'b-', label="Avant grève")
    axs[0, 0].plot([time_series[i] for i in after_strike], 
                   [data_waiting_time[i] for i in after_strike], 
                   'r-', label="Pendant grève")
    axs[0, 0].legend()
    axs[0, 0].grid(True)
    
    # Congestion
    axs[0, 1].set_title("Niveau de congestion")
    axs[0, 1].set_ylabel("Nombre de véhicules")
    axs[0, 1].plot([time_series[i] for i in before_strike], 
                  [data_congestion[i] for i in before_strike], 
                  'b-', label="Avant grève")
    axs[0, 1].plot([time_series[i] for i in after_strike], 
                  [data_congestion[i] for i in after_strike], 
                  'r-', label="Pendant grève")
    axs[0, 1].axhline(y=CONGESTION_THRESHOLD, color='k', linestyle='--', label="Seuil")
    axs[0, 1].legend()
    axs[0, 1].grid(True)
    
    # CO2
    axs[1, 0].set_title("Émissions CO2")
    axs[1, 0].set_ylabel("g/s")
    axs[1, 0].plot([time_series[i] for i in before_strike], 
                  [data_co2[i] for i in before_strike], 
                  'b-', label="Avant grève")
    axs[1, 0].plot([time_series[i] for i in after_strike], 
                  [data_co2[i] for i in after_strike], 
                  'r-', label="Pendant grève")
    axs[1, 0].legend()
    axs[1, 0].grid(True)
    
    # Statistiques comparatives
    before_waiting = sum([data_waiting_time[i] for i in before_strike]) / len(before_strike) if before_strike else 0
    after_waiting = sum([data_waiting_time[i] for i in after_strike]) / len(after_strike) if after_strike else 0
    
    before_congestion = sum([data_congestion[i] for i in before_strike]) / len(before_strike) if before_strike else 0
    after_congestion = sum([data_congestion[i] for i in after_strike]) / len(after_strike) if after_strike else 0
    
    before_co2 = sum([data_co2[i] for i in before_strike]) / len(before_strike) if before_strike else 0
    after_co2 = sum([data_co2[i] for i in after_strike]) / len(after_strike) if after_strike else 0
    
    # Tableau statistique
    rows = ['Temps d\'attente (s)', 'Congestion (véh.)', 'CO2 (g/s)']
    before_data = [before_waiting, before_congestion, before_co2]
    after_data = [after_waiting, after_congestion, after_co2]
    change = [(after - before) / before * 100 if before > 0 else float('inf') 
              for before, after in zip(before_data, after_data)]
    
    axs[1, 1].axis('tight')
    axs[1, 1].axis('off')
    table = axs[1, 1].table(
        cellText=[[f"{before:.2f}", f"{after:.2f}", f"{c:+.1f}%"] 
                  for before, after, c in zip(before_data, after_data, change)],
        rowLabels=rows,
        colLabels=['Avant grève', 'Pendant grève', 'Variation'],
        cellLoc='center',
        loc='center'
    )
    table.auto_set_font_size(False)
    table.set_fontsize(9)
    table.scale(1.2, 1.5)
    
    plt.tight_layout(rect=[0, 0, 1, 0.95])
    plt.savefig("resultats_simulation.png", dpi=300, bbox_inches="tight")
    print("Résultats de la simulation enregistrés dans 'resultats_simulation.png'")

# Point d'entrée du programme
if __name__ == "__main__":
    run_simulation()

